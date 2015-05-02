/* ***************************************************************************
 * graph.c -- LCUI base graphics processing module.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************/

/* ****************************************************************************
 * graph.c -- LCUI 的基础图形处理模块
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

void Graph_PrintInfo( LCUI_Graph *graph )
{
	printf("address:%p\n", graph);
	if( !graph ) {
		return;
	}

	printf("width:%d, ", graph->w);
	printf("height:%d, ", graph->h);
	printf("opacity:%.2f, ", graph->opacity);
	printf("%s\n", graph->color_type == COLOR_TYPE_ARGB ? "RGBA":"RGB");
	if( graph->quote.is_valid ) {
		printf("graph src:");
		Graph_PrintInfo(Graph_GetQuote(graph));
	}
}

void Graph_Init( LCUI_Graph *graph )
{
	graph->quote.is_valid = FALSE;
	graph->quote.source = NULL;
	graph->quote.top = 0;
	graph->quote.left = 0;
	graph->palette = NULL;
	graph->color_type = COLOR_TYPE_RGB;
	graph->bytes = NULL;
	graph->opacity = 1.0;
	graph->mem_size	 = 0;
	graph->width = 0;
	graph->height = 0;
	graph->bytes_per_pixel = 3;
	graph->bytes_per_row = 0;
}

static size_t get_pixel_size( int color_type )
{
	switch( color_type ) {
	case COLOR_TYPE_INDEX8:
	case COLOR_TYPE_GRAY8:
	case COLOR_TYPE_RGB323:
	case COLOR_TYPE_ARGB2222:
		 return 1;
	case COLOR_TYPE_RGB555:
	case COLOR_TYPE_RGB565:
		return 2;
	case COLOR_TYPE_RGB888:
		return 3;
	case COLOR_TYPE_ARGB8888:
	default:break;
	}
	return 4;
}

/*----------------------------------- RGB ----------------------------------*/

static void Pixels_ARGBFormatToRGB( const uchar_t *in_pixels, 
				    uchar_t *out_pixels, 
				    size_t pixel_count )
{
	const LCUI_ARGB8888 *p_px, *p_end_px;
	LCUI_ARGB8888 *p_out_px;
	uchar_t *p_out_byte;

	p_out_byte = out_pixels;
	p_out_px = (LCUI_ARGB8888*)out_pixels;
	p_px = (const LCUI_ARGB8888*)in_pixels;
	/* 遍历到倒数第二个像素为止 */
	p_end_px = p_px + pixel_count - 1;
	for( ; p_px < p_end_px; ++p_px ) {
		*p_out_px = *p_px;
		/* 将后3字节的数据当成4字节（LCUI_ARGB8888）的像素点访问 */
		p_out_px = (LCUI_ARGB8888*)(((uchar_t*)p_out_px) + 3);
	}
	/* 最后一个像素，以逐个字节的形式写数据 */
	p_out_byte = (uchar_t*)(p_out_px + 1);
	*p_out_byte++ = p_px->blue;
	*p_out_byte++ = p_px->green;
	*p_out_byte++ = p_px->red;
}

static void Pixels_RGBFormatToARGB( const uchar_t *in_pixels, 
				    uchar_t *out_pixels, 
				    size_t pixel_count )
{
	LCUI_ARGB8888 *p_px, *p_end_px;
	const uchar_t *p_in_byte;

	p_in_byte = in_pixels;
	p_px = (LCUI_ARGB8888*)out_pixels;
	p_end_px = p_px + pixel_count;
	while( p_px < p_end_px ) {
		p_px->blue = *p_in_byte++;
		p_px->green = *p_in_byte++;
		p_px->red = *p_in_byte++;
		p_px->alpha = 255;
		++p_px;
	}
}

static void Pixels_Format( const uchar_t *in_pixels, int in_color_type,
			   uchar_t *out_pixels, int out_color_type,
			   size_t pixel_count )
{
	switch( in_color_type ) {
	case COLOR_TYPE_ARGB8888:
		if( out_color_type == COLOR_TYPE_ARGB8888 ) {
			return;
		}
		Pixels_ARGBFormatToRGB( in_pixels, out_pixels, pixel_count );
		break;
	case COLOR_TYPE_RGB888:
		if( out_color_type == COLOR_TYPE_RGB888 ) {
			return;
		}
		Pixels_RGBFormatToARGB( in_pixels, out_pixels, pixel_count );
		break;
	default: break;
	}
}

static int Graph_RGBToARGB( LCUI_Graph *graph )
{
	int x, y;
	LCUI_ARGB *px_des, *px_row_des, *buffer;
	uchar_t *byte_row_src, *byte_src;

	buffer = (LCUI_ARGB*)malloc( sizeof(LCUI_ARGB)*graph->w*graph->h );
	if( !buffer ) {
		return -1;
	}
	px_row_des = buffer;
	byte_row_src = graph->bytes;
	for( y=0; y<graph->h; ++y ) {
		px_des = px_row_des;
		byte_src = byte_row_src;
		for( x=0; x<graph->w; ++x ) {
			px_des->b = *byte_src++;
			px_des->g = *byte_src++;
			px_des->r = *byte_src++;
			px_des->a = 255;
			++px_des;
		}
		byte_row_src += graph->bytes_per_row;
		px_row_des += graph->w;
	}
	free( graph->argb );
	graph->argb = buffer;
	graph->mem_size = sizeof(LCUI_ARGB)*graph->w*graph->h;
	graph->color_type = COLOR_TYPE_ARGB8888;
	return 0;
}

static int Graph_CutRGB( const LCUI_Graph *graph, LCUI_Rect rect,
			 LCUI_Graph *buff )
{
	int y;
	uchar_t *byte_src_row, *byte_des_row;

	buff->color_type = graph->color_type;
	if( 0 != Graph_Create(buff, rect.width, rect.height) ) {
		return -1;
	}
	buff->opacity = graph->opacity;
	byte_des_row = buff->bytes;
	byte_src_row = graph->bytes + rect.y*graph->bytes_per_row;
	byte_src_row += rect.x * graph->bytes_per_pixel;
	for( y=0; y<rect.h; ++y ) {
		memcpy( byte_des_row, byte_src_row, buff->bytes_per_row );
		byte_des_row += buff->bytes_per_row;
		byte_src_row += graph->bytes_per_row;
	}
	return 0;
}

static int Graph_ReplaceRGB( LCUI_Graph *des, LCUI_Rect des_rect,
			     const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int y, row_size;
	uchar_t *byte_row_des, *byte_row_src;

	byte_row_src = src->bytes + src_pos.y * src->bytes_per_row;
	byte_row_src += src_pos.x * src->bytes_per_pixel;
	if( des->color_type == COLOR_TYPE_RGB888 ) {
		row_size = 3 * des_rect.w;
		byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x) * 3;
		for( y = 0; y < des_rect.h; ++y ) {
			memcpy( byte_row_des, byte_row_src, row_size );
			byte_row_src += src->bytes_per_row;
			byte_row_des += des->bytes_per_row;
		}
		return 0;
	}

	if( des->color_type != COLOR_TYPE_ARGB8888 ) {
		return 0;
	}
	byte_row_des = des->bytes + des_rect.y * des->bytes_per_row;
	byte_row_des += des_rect.x * des->bytes_per_pixel;
	for( y = 0; y < des_rect.h; ++y ) {
		Pixels_Format( byte_row_src, src->color_type,
			       byte_row_des, des->color_type,
			       des_rect.w );
		byte_row_src += src->bytes_per_row;
		byte_row_des += des->bytes_per_row;
	}
	return 0;
}

#define Graph_MixRGB Graph_ReplaceRGB

static int Graph_HorizFlipRGB( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int x, y, n;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;
	}

	for( y=0; y<rect.h; ++y ) {
		byte_des = buff->bytes + y*buff->w*3;
		n = ((rect.y+y)*graph->w + rect.x + rect.w - 1)*3;
		byte_src = buff->bytes + n;
		for( x=0; x<rect.w; ++x ) {
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
		}
	}
	return 0;
}

static int Graph_VertiFlipRGB( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int y;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;
	}
	/* 引用最后一行像素 */
	byte_src = graph->bytes + (rect.y + rect.h - 1)*graph->bytes_per_row;
	byte_src += rect.x * graph->bytes_per_pixel;
	byte_des = buff->bytes;
	/* 交换上下每行像素 */
	for( y=0; y<rect.h; ++y ) {
		memcpy( byte_des, byte_src, buff->w*3 );
		byte_src -= graph->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

int Graph_FillRectRGB( LCUI_Graph *graph, LCUI_Color color, LCUI_Rect rect )
{
	int x, y;
	LCUI_Graph canvas;
	uchar_t *rowbytep, *bytep;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_Quote( &canvas, graph, &rect );
	Graph_GetValidRect( &canvas, &rect );
	graph = Graph_GetQuote( &canvas );
	rowbytep = graph->bytes + rect.y*graph->bytes_per_row;
	rowbytep += rect.x*graph->bytes_per_pixel;
	for( y=0; y<rect.h; ++y ) {
		bytep = rowbytep;
		for( x=0; x<rect.w; ++x ) {
			*bytep++ = color.blue;
			*bytep++ = color.green;
			*bytep++ = color.red;
		}
		rowbytep += graph->bytes_per_row;
	}
	return 0;
}


static int Graph_ZoomRGB( const LCUI_Graph *graph,
			  LCUI_Graph *buff,
			  LCUI_BOOL keep_scale,
			  LCUI_Size size )
{
	LCUI_Rect rect;
	int x, y, src_x, src_y, tmp;
	double scale_x, scale_y;
	uchar_t *byte_src, *byte_des;

	if( !Graph_IsValid(graph) ) {
		return -1;
	}
	if( size.w <= 0 || size.h <= 0 ) {
		Graph_Free( buff );
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	/* 如果保持宽高比 */
	if( keep_scale ) {
		if (scale_x<scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = graph->color_type;
	if( Graph_Create(buff, size.w, size.h) < 0) {
		return -2;
	}

	for( y=0; y < size.h; ++y )  {
		src_y = y * scale_y;
		tmp = ((src_y + rect.y) * graph->w + rect.x) * 3;
		byte_des = buff->bytes + y * size.w * 3;
		for( x=0; x < size.w; ++x ) {
			src_x = x * scale_x * 3;
			byte_src = graph->bytes + tmp + src_x;
			*byte_des++ = *byte_src++;
			*byte_des++ = *byte_src++;
			*byte_des++ = *byte_src++;
		}
	}
	return 0;
}

/*--------------------------------- End RGB --------------------------------*/

/*---------------------------------- ARGB ----------------------------------*/

static int Graph_ARGBToRGB( LCUI_Graph *graph )
{
	int x, y;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *buffer, *byte_row_des, *byte_des;

	buffer = (uchar_t*)malloc( sizeof(uchar_t)*graph->w*graph->h*3 );
	if( !buffer ) {
		return -1;
	}
	byte_row_des = buffer;
	px_row_src = graph->argb;
	for( y=0; y<graph->h; ++y ) {
		px_src = px_row_src;
		byte_des = byte_row_des;
		for( x=0; x<graph->w; ++x ) {
			px_src->b = *byte_des++;
			px_src->g = *byte_des++;
			px_src->r = *byte_des++;
			px_src->a = 255;
			++px_src;
		}
		byte_row_des += graph->bytes_per_row;
		px_row_src += graph->w;
	}
	free( graph->argb );
	graph->bytes = buffer;
	graph->mem_size = sizeof(uchar_t)*graph->w*graph->h*3;
	graph->color_type = COLOR_TYPE_RGB888;
	graph->bytes_per_pixel = 3;
	return 0;
}

static int Graph_CutARGB( const LCUI_Graph *graph, LCUI_Rect rect,
			  LCUI_Graph *buff )
{
	int x, y;
	LCUI_ARGB *pixel_src, *pixel_des;

	buff->color_type = graph->color_type;
	if( 0 != Graph_Create(buff, rect.width, rect.height) ) {
		return -1;
	}

	buff->opacity = graph->opacity;
	for( y=0; y<rect.h; ++y ) {
		pixel_des = buff->argb + y*buff->w;
		pixel_src = graph->argb;
		pixel_src += (rect.y+y)*graph->w + rect.x;
		for( x=0; x<rect.w; ++x ) {
			*pixel_des++ = *pixel_src++;
		}
	}
	return 0;
}

/**
 * 将前景图形混合到背景图形上，两个图形都是ARGB像素格式
 */
static int Graph_MixARGB( LCUI_Graph *des, LCUI_Rect des_rect,
			  const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y, val;
	uchar_t alpha;
	LCUI_ARGB *px_src, *px_des;
	LCUI_ARGB *px_row_src, *px_row_des;

	/* 计算并保存第一行的首个像素的位置 */
	px_row_src = src->argb + src_pos.y*src->w + src_pos.x;
	px_row_des = des->argb + des_rect.y*des->w + des_rect.x;
	if( src->opacity < 1.0 ) {
		for( y=0; y<des_rect.h; ++y ) {
			px_src = px_row_src;
			px_des = px_row_des;
			for( x=0; x<des_rect.w; ++x ) {
				alpha = px_src->a * src->opacity;
				/* 将R、G、B三个色彩值进行alpha混合 */
				ALPHA_BLEND( px_des->r, px_src->r, alpha );
				ALPHA_BLEND( px_des->g, px_src->g, alpha );
				ALPHA_BLEND( px_des->b, px_src->b, alpha );
				if( px_des->alpha == 255 || px_src->alpha == 255 ) {
					px_des->alpha = 255;
				} else {
					/* alpha = 1.0 - (1.0 - a1)*(1.0 - a2) */
					val = (255 - px_src->alpha)*(255 - px_des->alpha);
					px_des->alpha = (uchar_t)(255 - val / 65025);
				}
				/* 切换到下个像素的数据 */
				++px_src;
				++px_des;
			}
			/* 切换到下一行像素 */
			px_row_des += des->w;
			px_row_src += src->w;
		}
		return 0;
	}

	for( y=0; y<des_rect.h; ++y ) {
		px_src = px_row_src;
		px_des = px_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			ALPHA_BLEND( px_des->r, px_src->r, px_src->a );
			ALPHA_BLEND( px_des->g, px_src->g, px_src->a );
			ALPHA_BLEND( px_des->b, px_src->b, px_src->a );
			if( px_des->alpha == 255 || px_src->alpha == 255 ) {
				px_des->alpha = 255;
			} else {
				val = (255 - px_src->alpha)*(255 - px_des->alpha);
				px_des->alpha = (uchar_t)(255 - val / 65025);
			}
			++px_src;
			++px_des;
		}
		px_row_des += des->w;
		px_row_src += src->w;
	}
	return 0;
}

static int Graph_ReplaceARGB( LCUI_Graph *des, LCUI_Rect des_rect,
			      const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y, row_size;
	LCUI_ARGB *px_row_src, *px_row_des, *px_src, *px_des;

	px_row_src = src->argb + src_pos.y*src->w + src_pos.x;
	px_row_des = des->argb + des_rect.y*des->w + des_rect.x;
	if( src->opacity < 0 ) {
		for( y=0; y<des_rect.h; ++y ) {
			px_src = px_row_src;
			px_des = px_row_des;
			for( x=0; x<des_rect.w; ++x ) {
				px_des->b = px_src->b;
				px_des->g = px_src->g;
				px_des->r = px_src->r;
				px_des->a = src->opacity*px_src->a;
			}
			px_row_src += src->w;
			px_row_des += des->w;
		}
		return 0;
	}
	row_size = sizeof(LCUI_ARGB)*des_rect.w;
	for( y=0; y<des_rect.h; ++y ) {
		memcpy( px_row_des, px_row_src, row_size );
		px_row_src += src->w;
		px_row_des += des->w;
	}
	return 0;
}

static int Graph_HorizFlipARGB( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int x, y;
	LCUI_Rect rect;
	LCUI_ARGB *pixel_src, *pixel_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;
	}

	for( y=0; y<rect.h; ++y ) {
		pixel_des = buff->argb + y*buff->w;
		pixel_src = graph->argb + (rect.y+y)*graph->w;
		pixel_src += rect.x + rect.w - 1;
		for( x=0; x<rect.w; ++x ) {
			*pixel_des++ = *pixel_src--;
		}
	}
	return 0;
}

static int Graph_VertiFlipARGB( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int y;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;
	}
	byte_src = graph->bytes + (rect.y + rect.h - 1)*graph->bytes_per_row;
	byte_src += rect.x * graph->bytes_per_pixel;
	byte_des = buff->bytes;
	for( y=0; y<rect.h; ++y ) {
		memcpy( byte_des, byte_src, buff->bytes_per_row );
		byte_src -= graph->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

static int Graph_FillRectARGB( LCUI_Graph *graph, LCUI_Color color,
			       LCUI_Rect rect )
{
	int x, y;
	LCUI_Rect rect_src;
	LCUI_ARGB *px_p, *px_row_p;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect( graph, &rect_src );
	graph = Graph_GetQuote( graph );
	px_row_p = graph->argb + (rect_src.y+rect.y)*graph->w;
	px_row_p += rect.x + rect_src.x;
	for( y=0; y<rect.h; ++y ) {
		px_p = px_row_p;
		for( x=0; x<rect.w; ++x ) {
			px_p->b = color.b;
			px_p->g = color.g;
			px_p->r = color.r;
			++px_p;
		}
		px_row_p += graph->w;
	}
	return 0;
}

static int Graph_ZoomARGB( const LCUI_Graph *graph, LCUI_Graph *buff,
			   LCUI_BOOL keep_scale, LCUI_Size size )
{
	LCUI_Rect rect;
	LCUI_ARGB *pixel_src, *pixel_des;
	int x, y, src_x, src_y, tmp;
	double scale_x, scale_y;

	if( !Graph_IsValid(graph) ) {
		return -1;
	}
	if( size.w <= 0 || size.h <= 0 ) {
		Graph_Free( buff );
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	/* 如果保持宽高比 */
	if( keep_scale ) {
		if (scale_x<scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = graph->color_type;
	if( Graph_Create(buff, size.w, size.h) < 0) {
		return -2;
	}
	for( y=0; y < size.h; ++y )  {
		src_y = y * scale_y;
		tmp = (src_y + rect.y) * graph->w + rect.x;
		pixel_des = buff->argb + y * size.w;
		for( x=0; x < size.w; ++x ) {
			src_x = x * scale_x;
			pixel_src = graph->argb + tmp + src_x;
			*pixel_des++ = *pixel_src++;
		}
	}
	return 0;
}

/*-------------------------------- End ARGB --------------------------------*/

/** 改变色彩类型 */
int Graph_ChangeColorType( LCUI_Graph *graph, int color_type )
{
	if( graph->color_type == color_type ) {
		return -1;
	}
	switch( graph->color_type ) {
	case COLOR_TYPE_ARGB8888:
		switch( color_type ) {
		case COLOR_TYPE_RGB888:
			return Graph_ARGBToRGB( graph );
		default:break;
		}
		break;
	case COLOR_TYPE_RGB888:
		switch( color_type ) {
		case COLOR_TYPE_ARGB8888:
			return Graph_RGBToARGB( graph );
		default:break;
		}
		break;
	default:
		break;
	}
	return -2;
}

int Graph_Create( LCUI_Graph *graph, int w, int h )
{
	size_t size;
	if( w > 10000 || h > 10000 ) {
		_DEBUG_MSG("graph size is too large!");
		abort();
	}
	if( h <= 0 || w <= 0 ) {
		Graph_Free( graph );
		return -1;
	}
	graph->bytes_per_pixel = get_pixel_size( graph->color_type );
	graph->bytes_per_row = graph->bytes_per_pixel * w;
	size = graph->bytes_per_row * h;
	if( Graph_IsValid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if( graph->mem_size >= size ) {
			if( (w != graph->w || h != graph->h)
			 && graph->color_type == COLOR_TYPE_ARGB ) {
				Graph_FillAlpha( graph, 0 );
				Graph_FillColor(graph, RGB(255,255,255));
			}
			graph->w = w;
			graph->h = h;
			return 0;
		}
		Graph_Free( graph );
	}
	graph->mem_size = size;
	graph->bytes = (uchar_t*)malloc( size );
	if( !graph->bytes ) {
		graph->w = 0;
		graph->h = 0;
		return -2;
	}
	/* 默认全透明 */
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		Graph_FillAlpha( graph, 0 );
	}
	graph->w = w;
	graph->h = h;
	return 0;
}

void Graph_Copy( LCUI_Graph *des, const LCUI_Graph *src )
{
	const LCUI_Graph *graph;
	if( !des || !Graph_IsValid(src) ) {
		return;
	}
	graph = Graph_GetQuote( src );
	des->color_type = graph->color_type;
	/* 创建合适尺寸的Graph */
	Graph_Create( des, src->w, src->h );
	Graph_Replace( des, src, Pos(0,0) );
	des->opacity = src->opacity;
}

void Graph_Free( LCUI_Graph *graph )
{
	/* 解除引用 */
	if( graph && graph->quote.is_valid ) {
		graph->quote.source = NULL;
		graph->quote.is_valid = FALSE;
		return;
	}
	if( graph->bytes ) {
		free( graph->bytes );
		graph->bytes = NULL;
	}
	graph->w = 0;
	graph->h = 0;
	graph->mem_size = 0;
}

/**
* 为图像创建一个引用
* @param self	用于存放图像引用的缓存区
* @param source	引用的源图像
* &param rect	引用的区域，若为NULL，则引用整个图像
*/
int Graph_Quote( LCUI_Graph *self, LCUI_Graph *source, const LCUI_Rect *rect )
{
	LCUI_Size box_size;
	LCUI_Rect quote_rect;

	if( rect == NULL ) {
		quote_rect.x = 0;
		quote_rect.y = 0;
		quote_rect.width = source->width;
		quote_rect.height = source->height;
	} else {
		quote_rect = *rect;
	}

	box_size.w = source->width;
	box_size.h = source->height;
	LCUIRect_ValidateArea( &quote_rect, box_size );
	/* 如果引用源本身已经引用了另一个源 */
	if( source->quote.is_valid ) {
		quote_rect.x += source->quote.left;
		quote_rect.y += source->quote.top;
		source = source->quote.source;
	}
	if( quote_rect.w <= 0 || quote_rect.h <= 0 ) {
		self->width = 0;
		self->height = 0;
		self->opacity = 1.0;
		self->quote.left = 0;
		self->quote.top = 0;
		self->bytes = NULL;
		self->quote.source = NULL;
		self->quote.is_valid = FALSE;
		return -2;
	}
	self->width = quote_rect.width;
	self->height = quote_rect.height;
	self->opacity = 1.0;
	self->bytes = NULL;
	self->mem_size = 0;
	self->color_type = source->color_type;
	self->bytes_per_pixel = source->bytes_per_pixel;
	self->bytes_per_row = source->bytes_per_row;
	self->quote.is_valid = TRUE;
	self->quote.source = source;
	self->quote.left = quote_rect.x;
	self->quote.top = quote_rect.y;
	return 0;
}

void Graph_GetValidRect( const LCUI_Graph *graph, LCUI_Rect *rect )
{
	if( graph->quote.is_valid ) {
		rect->x = graph->quote.left;
		rect->y = graph->quote.top;
	} else {
		rect->x = 0;
		rect->y = 0;
	}
	rect->width = graph->width;
	rect->height = graph->height;
}

int Graph_SetAlphaBits( LCUI_Graph *graph, uchar_t *a, size_t size )
{
	size_t i;
	if( size > (size_t)(graph->w*graph->h) ) {
		size = (size_t)(graph->w*graph->h);
	}
	if( graph->color_type != COLOR_TYPE_ARGB ) {
		return -2;
	}
	for( i=0; i<size; ++i ) {
		graph->argb[i].a = a[i];
	}
	return 0;
}

int Graph_SetRedBits( LCUI_Graph *graph, uchar_t *r, size_t size )
{
	size_t i;
	uchar_t *pByte;

	if( size > (size_t)(graph->w*graph->h) ) {
		size = (size_t)(graph->w*graph->h);
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( i=0; i<size; ++i ) {
			graph->argb[i].r = r[i];
		}
	} else {
		pByte = graph->bytes + 2;
		for( i=0; i<size; ++i ) {
			*pByte = r[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_SetGreenBits( LCUI_Graph *graph, uchar_t *g, size_t size )
{
	size_t i;
	uchar_t *pByte;

	if( size > (size_t)(graph->w*graph->h) ) {
		size = (size_t)(graph->w*graph->h);
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( i=0; i<size; ++i ) {
			graph->argb[i].g = g[i];
		}
	} else {
		pByte = graph->bytes + 1;
		for( i=0; i<size; ++i ) {
			*pByte = g[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_SetBlueBits( LCUI_Graph *graph, uchar_t *b, size_t size )
{
	size_t i;
	uchar_t *pByte;

	if( size > (size_t)(graph->w*graph->h) ) {
		size = (size_t)(graph->w*graph->h);
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( i=0; i<size; ++i ) {
			graph->argb[i].b = b[i];
		}
	} else {
		pByte = graph->bytes;
		for( i=0; i<size; ++i ) {
			*pByte = b[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_Zoom( const LCUI_Graph *graph, LCUI_Graph *buff,
		LCUI_BOOL keep_scale, LCUI_Size size )
{
	LCUI_Rect rect;
	int x, y, src_x, src_y;
	double scale_x, scale_y;

	if( !Graph_IsValid(graph) ) {
		return -1;
	}
	if( size.w <= 0 || size.h <= 0 ) {
		Graph_Free( buff );
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	/* 如果保持宽高比 */
	if( keep_scale ) {
		if (scale_x<scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = graph->color_type;
	if( Graph_Create(buff, size.w, size.h) < 0 ) {
		return -2;
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		LCUI_ARGB *px_src, *px_des, *px_row_src;
		for( y=0; y < size.h; ++y )  {
			src_y = y * scale_y;
			px_row_src = graph->argb;
			px_row_src += (src_y + rect.y) * graph->w + rect.x;
			px_des = buff->argb + y * size.w;
			for( x=0; x < size.w; ++x ) {
				src_x = x * scale_x;
				px_src = px_row_src + src_x;
				*px_des++ = *px_src;
			}
		}
	} else {
		uchar_t *byte_src, *byte_des, *byte_row_src;
		for( y = 0; y < size.h; ++y ) {
			src_y = y * scale_y;
			byte_row_src = graph->bytes;
			byte_row_src += (src_y + rect.y) * graph->bytes_per_row;
			byte_row_src += rect.x * graph->bytes_per_pixel;
			byte_des = buff->bytes + y * buff->bytes_per_row;
			for( x = 0; x < size.w; ++x ) {
				src_x = x * scale_x;
				src_x *= graph->bytes_per_pixel;
				byte_src = byte_row_src + src_x;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src;
			}
		}
	}
	return 0;
}

int Graph_Cut( const LCUI_Graph *graph, LCUI_Rect rect,
		        LCUI_Graph *buff )
{
	if( !Graph_IsValid( graph ) ) {
		return -2;
	}
	LCUIRect_ValidateArea( &rect, Size(graph->w, graph->h) );
	if( rect.width <= 0 || rect.height <= 0) {
		return -3;
	}
	switch( graph->color_type ) {
	case COLOR_TYPE_ARGB8888:
		return Graph_CutARGB( graph, rect, buff );
	case COLOR_TYPE_RGB888:
		return Graph_CutRGB( graph, rect, buff );
	default:break;
	}
	return -4;
}

int Graph_HorizFlip( const LCUI_Graph *graph, LCUI_Graph *buff )
{

	switch( graph->color_type ) {
	case COLOR_TYPE_RGB888:
		return Graph_HorizFlipRGB( graph, buff );
	case COLOR_TYPE_ARGB8888:
		return Graph_HorizFlipARGB( graph, buff );
	default:break;
	}
	return -1;
}

int Graph_VertiFlip( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	switch( graph->color_type ) {
	case COLOR_TYPE_RGB888:
		return Graph_VertiFlipRGB( graph, buff );
	case COLOR_TYPE_ARGB8888:
		return Graph_VertiFlipARGB( graph, buff );
	default:break;
	}
	return -1;
}

int Graph_FillRect( LCUI_Graph *graph, LCUI_Color color,
				LCUI_Rect rect )
{
	switch( graph->color_type ) {
	case COLOR_TYPE_RGB888:
		return Graph_FillRectRGB( graph, color, rect );
	case COLOR_TYPE_ARGB8888:
		return Graph_FillRectARGB( graph, color, rect );
	default:break;
	}
	return -1;
}

int Graph_FillColor( LCUI_Graph *graph, LCUI_Color color )
{
	return Graph_FillRect( graph, color, Rect(0,0,graph->w, graph->h) );
}

int Graph_FillAlpha( LCUI_Graph *graph, uchar_t alpha )
{
	int x, y;
	LCUI_Rect rect;
	LCUI_ARGB *pixel, *pixel_row;

	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	if( !Graph_IsValid(graph) ) {
		return -1;
	}
	if( !Graph_HasAlpha(graph) ) {
		return -2;
	}

	pixel_row = graph->argb + rect.y*graph->w + rect.x;
	for(y=0; y<rect.h; ++y) {
		pixel = pixel_row;
		for( x=0; x<rect.w; ++x ) {
			pixel->alpha = alpha;
			++pixel;
		}
		pixel_row += graph->w;
	}
	return 0;
}

int Graph_Tile( LCUI_Graph *buff,  const LCUI_Graph *graph,
		LCUI_BOOL replace )
{
	int ret = 0;
	LCUI_Pos pos;

	if(!Graph_IsValid(graph) || !Graph_IsValid(buff)) {
		return -1;
	}
	for(pos.y=0; pos.y<buff->h; pos.y+=graph->h) {
		for(pos.x=0; pos.x<buff->w; pos.x+=graph->w) {
			if( replace ) {
				ret += Graph_Replace( buff, graph, pos );
			} else {
				ret += Graph_Mix( buff, graph, pos );
			}
		}
	}
	return ret;
}

int Graph_Mix( LCUI_Graph *back, const LCUI_Graph *fore, LCUI_Pos pos )
{
	const LCUI_Graph *src;
	LCUI_Graph*des;
	LCUI_Rect cut, src_rect, area, des_rect;
	LCUI_Size box_size;
	LCUI_Pos src_pos;

	/* 预先进行有效性判断 */
	if( !Graph_IsValid(back) || !Graph_IsValid(fore) ) {
		return -1;
	}
	/* 获取引用的源图像的最终区域 */
	Graph_GetValidRect( fore, &src_rect );
	Graph_GetValidRect( back, &des_rect );
	/* 获取引用的源图像 */
	src = Graph_GetQuote( fore );
	des = Graph_GetQuote( back );
	/* 判断坐标是否在背景图的范围内 */
	if( pos.x > des->w || pos.y > des->h ) {
		return -3;
	}
	/* 记录容器尺寸 */
	box_size.w = des_rect.width;
	box_size.h = des_rect.height;
	/* 记录前景图像在容器中的区域 */
	area.x = pos.x;
	area.y = pos.y;
	area.width = src_rect.width;
	area.height = src_rect.height;
	/* 获取前景图像区域中的需要裁减的区域 */
	LCUIRect_GetCutArea( box_size, area, &cut );
	/* 移动前景图像区域的坐标 */
	pos.x += cut.x;
	pos.y += cut.y;
	/* 得出源图像的读取区域的坐标 */
	src_pos.x = cut.x + src_rect.x;
	src_pos.y = cut.y + src_rect.y;
	/* 得出目标图像的写入区域 */
	des_rect.x = pos.x + des_rect.x;
	des_rect.y = pos.y + des_rect.y;
	des_rect.width = cut.width;
	des_rect.height = cut.height;

	switch( src->color_type ) {
	case COLOR_TYPE_RGB888:
		return Graph_MixRGB( des, des_rect, src, src_pos );
	case COLOR_TYPE_ARGB8888:
		return Graph_MixARGB( des, des_rect, src, src_pos );
	default:break;
	}

	return -1;
}

int Graph_Replace( LCUI_Graph *back,  const LCUI_Graph *fore, LCUI_Pos pos )
{
	const LCUI_Graph *src;
	LCUI_Graph *des;
	LCUI_Rect cut, src_rect, area, des_rect;
	LCUI_Size box_size;
	LCUI_Pos src_pos;

	if( !Graph_IsValid(back) || !Graph_IsValid(fore) ) {
		return -1;
	}

	Graph_GetValidRect( fore, &src_rect );
	Graph_GetValidRect( back, &des_rect );
	src = Graph_GetQuote( fore );
	des = Graph_GetQuote( back );

	if(pos.x > des->w || pos.y > des->h) {
		return -1;
	}

	box_size.w = des_rect.width;
	box_size.h = des_rect.height;
	area.x = pos.x;
	area.y = pos.y;
	area.width = src_rect.width;
	area.height = src_rect.height;
	LCUIRect_GetCutArea( box_size, area, &cut );
	pos.x += cut.x;
	pos.y += cut.y;
	src_pos.x = cut.x + src_rect.x;
	src_pos.y = cut.y + src_rect.y;
	des_rect.x = pos.x + des_rect.x;
	des_rect.y = pos.y + des_rect.y;
	des_rect.width = cut.width;
	des_rect.height = cut.height;

	switch( src->color_type ) {
	case COLOR_TYPE_RGB888:
		return Graph_ReplaceRGB( des, des_rect, src, src_pos );
	case COLOR_TYPE_ARGB8888:
		return Graph_ReplaceARGB( des, des_rect, src, src_pos );
	default:break;
	}

	return -1;
}

int Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int align, LCUI_BOOL replace )
{
	LCUI_Pos pos;
	if(!Graph_IsValid(graph) || ! Graph_IsValid(image)) {
		return -1;
	}
	pos.x = pos.y = 0;
	switch( align ) {
	case ALIGN_TOP_CENTER:
		pos.x = (graph->w - image->w) / 2;
		break;
	case ALIGN_TOP_RIGHT:
		pos.x = graph->w - image->w;
		break;
	case ALIGN_MIDDLE_LEFT:
		pos.y = (graph->h - image->h) / 2;
		break;
	case ALIGN_MIDDLE_CENTER:
		pos.x = (graph->w - image->w) / 2;
		pos.y = (graph->h - image->h) / 2;
		break;
	case ALIGN_MIDDLE_RIGHT:
		pos.x = graph->w - image->w;
		pos.y = (graph->h - image->h) / 2;
		break;
	case ALIGN_BOTTOM_LEFT:
		pos.y = graph->h - image->h;
		break;
	case ALIGN_BOTTOM_CENTER:
		pos.x = (graph->w - image->w) / 2;
		pos.y = graph->h - image->h;
		break;
	case ALIGN_BOTTOM_RIGHT:
		pos.x = graph->w - image->w;
		pos.y = graph->h - image->h;
		break;
	case ALIGN_TOP_LEFT:
	default: break;
	}

	if( replace ) {
		Graph_Replace(graph, image, pos);
	} else {
		Graph_Mix(graph, image, pos);
	}

	return 0;
}

/** 填充图像
 * @param graph		目标图像
 * @param backimg	要填充的背景图
 * @param layout	背景图的布局
 * @param area		需要绘制的区域
 */
int Graph_FillImageEx( LCUI_Graph *graph, const LCUI_Graph *backimg,
				int layout, LCUI_Rect area )
{
	LCUI_Pos pos;
	LCUI_Graph box, tmp_img;
	int x, y;

	if( Graph_Quote( &box, graph, &area ) != 0 ) {
		return -1;
	}
	/* 转换成相同的色彩类型 */
	if( backimg->color_type != graph->color_type ) {
		Graph_Init( &tmp_img );
		Graph_Copy( &tmp_img, backimg );
		Graph_ChangeColorType( &tmp_img, graph->color_type );
		backimg = &tmp_img;
	}
	Graph_FillAlpha( &box, 0 );
	switch( layout ) {
	case LAYOUT_CENTER:
		pos.x = (graph->w - backimg->w) / 2.0;
		pos.y = (graph->h - backimg->h) / 2.0;
		pos.x -= area.x;
		pos.y -= area.y;
		Graph_Replace( &box, backimg, pos );
		break;
	case LAYOUT_TILE:
		for( y=0; y<area.y; y+=backimg->h );
		for( ; y<area.y+area.h; y+=backimg->h ) {
			for( x=0; x<area.x; x+=backimg->w );
			for( ; x<area.x+area.w; x+=backimg->w ) {
				pos.x = x - area.x;
				pos.y = y - area.y;
				Graph_Mix( &box, backimg, pos );
			}
		}
		break;
	case LAYOUT_STRETCH:
		break;
	case LAYOUT_ZOOM:
		break;
	case LAYOUT_NORMAL:
	default:
		pos.x = -area.x;
		pos.y = -area.y;
		Graph_Replace( &box, backimg, pos );
		break;
	}
	if( backimg->color_type != graph->color_type ) {
		Graph_Free( &tmp_img );
	}
	return 0;
}

/** 填充图像和背景色
 * @param graph		目标图像
 * @param backimg	背景图
 * @param layout	背景图的布局
 * @param color		背景色
 * @param area		需要绘制的区域
 */
int Graph_FillImageWithColorEx( LCUI_Graph *graph,
				const LCUI_Graph *backimg,
				int layout,
				LCUI_Color color,
				LCUI_Rect area )
{
	LCUI_Pos pos;
	LCUI_Graph box, tmp_img;
	int x, y;

	if( Graph_Quote( &box, graph, &area ) != 0 ) {
		return -1;
	}
	if( backimg->color_type != graph->color_type ) {
		Graph_Init( &tmp_img );
		Graph_Copy( &tmp_img, backimg );
		Graph_ChangeColorType( &tmp_img, graph->color_type );
		backimg = &tmp_img;
	}
	Graph_FillAlpha( &box, 255 );
	Graph_FillColor( &box, color );
	switch( layout ) {
	case LAYOUT_CENTER:
		pos.x = (graph->w - backimg->w) / 2.0;
		pos.y = (graph->h - backimg->h) / 2.0;
		pos.x -= area.x;
		pos.y -= area.y;
		Graph_Mix( &box, backimg, pos );
		break;
	case LAYOUT_TILE:
		for( y=0; y<area.y; y+=backimg->h );
		for( ; y<area.y+area.h; y+=backimg->h ) {
			for( x=0; x<area.x; x+=backimg->w );
			for( ; x<area.x+area.w; x+=backimg->w ) {
				pos.x = x - area.x;
				pos.y = y - area.y;
				Graph_Mix( &box, backimg, pos );
			}
		}
		break;
	case LAYOUT_STRETCH:
		break;
	case LAYOUT_ZOOM:
		break;
	case LAYOUT_NORMAL:
	default:
		pos.x = -area.x;
		pos.y = -area.y;
		Graph_Mix( &box, backimg, pos );
		break;
	}
	if( backimg->color_type != graph->color_type ) {
		Graph_Free( &tmp_img );
	}
	return 0;
}

int Graph_FillImage( LCUI_Graph *graph, const LCUI_Graph *backimg,
				int layout )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = graph->w;
	area.h = graph->h;
	return Graph_FillImageEx( graph, backimg, layout, area );
}

int Graph_FillImageWithColor( LCUI_Graph *graph,
		const LCUI_Graph *backimg, int layout, LCUI_Color color )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = graph->w;
	area.h = graph->h;
	return Graph_FillImageWithColorEx( graph, backimg, layout,
							color, area );
}
