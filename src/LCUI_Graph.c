/** ******************************************************************************
 * @file	LCUI_Graph.c
 * @brief	Graphics processing.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	LCUI_Graph.c
 * @brief	图形处理
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_MISC_H 
#include LC_WIDGET_H
#include LC_CURSOR_H

LCUI_API void Graph_PrintInfo( LCUI_Graph *graph )
{
	printf("address:%p\n", graph);
	if( !graph ) {
		return;
	}
	
	printf("width:%d, ", graph->w);
	printf("height:%d, ", graph->h);
	printf("alpha:%u, ", graph->alpha);
	printf("%s\n", graph->color_type == COLOR_TYPE_ARGB ? "RGBA":"RGB");
	if( graph->quote ) {
		printf("graph src:");
		Graph_PrintInfo(Graph_GetQuote(graph));
	}
}

LCUI_API void Graph_Init( LCUI_Graph *graph )
{
	if( !graph ) {
		return;
	}
	
	graph->quote = FALSE; 
	graph->src = NULL;
	graph->color_type = COLOR_TYPE_RGB;
	graph->bytes = NULL;
	graph->alpha = 255;
	graph->mem_size	 = 0;
	graph->x = 0;
	graph->y = 0;
	graph->w = 0;
	graph->h = 0;
}

LCUI_API int Graph_Create( LCUI_Graph *graph, int w, int h )
{
	size_t size;
	if(w > 10000 || h > 10000) {
		_DEBUG_MSG("graph size is too large!");
		abort();
	}
	if(h <= 0 || w <= 0) {
		Graph_Free( graph );
		return -1; 
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		size = sizeof(LCUI_ARGB)*w*h;
	} else {
		size = sizeof(uchar_t)*w*h*3;
	}
	if( Graph_IsValid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if( graph->mem_size >= size ) {
			if( (w != graph->w || h != graph->h)
			 && graph->color_type == COLOR_TYPE_ARGB ) {
				Graph_FillAlpha( graph, 0 );
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

LCUI_API void Graph_Copy( LCUI_Graph *des, LCUI_Graph *src )
{
	LCUI_Graph *graph;
	if( !des || !Graph_IsValid(src) ) {
		return;
	}
	graph = Graph_GetQuote( src );
	des->color_type = graph->color_type;
	/* 创建合适尺寸的Graph */
	Graph_Create( des, src->w, src->h );
	Graph_Replace( des, src, Pos(0,0) );
	des->alpha = src->alpha;
}

LCUI_API void Graph_Free( LCUI_Graph *graph )
{
	/* 解除引用 */
	if( graph && graph->quote ) {
		graph->src = NULL;
		graph->quote = FALSE;
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

LCUI_API int Graph_Quote( LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area )
{
	if( !src || !des ) {
		return -1;
	}
	//printf("Graph_Quote(), before, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	LCUIRect_ValidateArea( &area, Size(src->w, src->h) );
	//printf("Graph_Quote(), after, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	if( area.w <= 0 || area.h <= 0 ) { 
		des->src = NULL;
		des->x = 0;
		des->y = 0;
		des->w = 0;
		des->h= 0;
		des->alpha = 255;
		des->quote = FALSE;
		return -1;
	} 
	des->src = src;
	des->x = area.x;
	des->y = area.y;
	des->w = area.width;
	des->h = area.height;
	des->alpha = 255;
	des->quote = TRUE;
	return 0;
}

LCUI_API LCUI_BOOL Graph_HaveAlpha( const LCUI_Graph *graph )
{
	graph = Graph_GetQuoteConst(graph);
	if( graph && graph->color_type == COLOR_TYPE_ARGB ) {
		return TRUE;
	}
	return FALSE; 
}

LCUI_API LCUI_BOOL Graph_IsValid( const LCUI_Graph *graph )
{
	while( graph && graph->quote ) {
		graph = graph->src;
	}
	if( graph && graph->bytes && graph->h > 0 && graph->w > 0 ) {
		return TRUE;
	}
	return FALSE;
}

LCUI_API LCUI_Rect Graph_GetValidRect( const LCUI_Graph *graph )
{
	int x, y, w, h, temp; 
	LCUI_Rect rect, cut_rect;
	
	x = cut_rect.x = graph->x;
	y = cut_rect.y = graph->y;
	cut_rect.width = graph->w;
	cut_rect.height = graph->h;
	
	if( !graph->quote ) {
		return cut_rect; 
	} else {
		w = graph->src->w;
		h = graph->src->h;
	} 
	/* 获取需裁剪的区域 */
	if(x < 0) {
		cut_rect.width += x;
		cut_rect.x = 0 - x; 
	}
	if(x + graph->w > w) {
		cut_rect.width -= (x +  graph->w - w); 
	}
	
	if(y < 0) {
		cut_rect.height += y;
		cut_rect.y = 0 - y; 
	}
	if(y + graph->h > h) {
		cut_rect.height -= (y +  graph->h - h); 
	}
	
	/* 获取引用的图像的有效显示范围 */
	rect = Graph_GetValidRect( graph->src );
	/* 如果引用的图像需要裁剪，那么，该图像根据情况，也需要进行裁剪 */
	if(rect.x > 0) { 
		temp = x + cut_rect.x;
		if(temp < rect.x) { 
			temp = rect.x - x; 
			cut_rect.width -= (temp - cut_rect.x); 
			cut_rect.x = temp; 
		}
	}
	if(rect.y > 0) {
		temp = y + cut_rect.y;
		if(y < rect.y) {
			temp = rect.y - y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.width < w) { 
		temp = x + cut_rect.x + cut_rect.width;
		if(temp > rect.x + rect.width) {
			cut_rect.width -= (temp-(rect.x+rect.width));
		}
	}
	if(rect.height < h) {
		temp = y + cut_rect.y+cut_rect.height;
		if(temp > rect.y+rect.height) {
			cut_rect.height -= (temp-(rect.y+rect.height));
		}
	} 
	
	return cut_rect;
}

/* 
 * 功能：获取指向被引用的图形的指针 
 * 说明：如果当前图形引用了另一个图形，并且，该图形处于一条引用链中，那么，本函数会返
 * 回指向被引用的最终图形的指针。
 * */
LCUI_API LCUI_Graph* Graph_GetQuote( LCUI_Graph *graph )
{
	while( graph && graph->quote ) {
		graph = graph->src;
	}
	return graph;
}

LCUI_API const LCUI_Graph* Graph_GetQuoteConst( const LCUI_Graph *graph )
{
	while( graph && graph->quote ) {
		graph = graph->src;
	}
	return graph;
}

LCUI_API int Graph_Zoom( const LCUI_Graph *graph, LCUI_Graph *buff, 
			 LCUI_BOOL keep_scale, LCUI_Size size )
{
	LCUI_Rect rect;
	LCUI_ARGB *pixel_src, *pixel_des;
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
	rect = Graph_GetValidRect( graph );
	graph = Graph_GetQuoteConst( graph );
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
	if( graph->color_type == COLOR_TYPE_ARGB ) {
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

LCUI_API int Graph_Cut( const LCUI_Graph *graph, LCUI_Rect rect, LCUI_Graph *buff )
{
	int x, y;
	LCUI_ARGB *pixel_src, *pixel_des;
	uchar_t *byte_src, *byte_des;
	
	buff->alpha = graph->alpha;
	LCUIRect_ValidateArea( &rect, Size(graph->w, graph->h) );
	
	if( !Graph_IsValid(graph) || rect.width <= 0 || rect.height <= 0) {
		return -1;
	}
	if( 0 != Graph_Create(buff, rect.width, rect.height) ) {
		return -2; 
	}

	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( y=0; y<rect.h; ++y ) {
			pixel_des = buff->argb;
			pixel_des += y*buff->w;
			pixel_src = graph->argb;
			pixel_src += (rect.y+y)*graph->w + rect.x;
			for( x=0; x<rect.w; ++x ) {
				*pixel_des++ = *pixel_src++;
			}
		}
		return 0;
	}
	
	for( y=0; y<rect.h; ++y ) {
		byte_des = buff->bytes;
		byte_des += y*buff->w*3;
		byte_src = graph->bytes;
		byte_src += ((rect.y+y)*graph->w + rect.x)*3;
		for( x=0; x<rect.w; ++x ) {
			*byte_des++ = *byte_src++;
			*byte_des++ = *byte_src++;
			*byte_des++ = *byte_src++;
		}
	}
	return 0; 
}

LCUI_API int Graph_HorizFlip( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_ARGB *pixel_src, *pixel_des;
	uchar_t *byte_src, *byte_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	rect = Graph_GetValidRect( graph );
	graph = Graph_GetQuoteConst( graph );
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;	
	}

	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( y=0; y<rect.h; ++y ) {
			pixel_des = buff->argb + y*buff->w;
			pixel_src = graph->argb + (rect.y+y)*graph->w;
			pixel_src += rect.x + rect.w - 1;
			for( x=0; x<rect.w; ++x ) {
				*pixel_des++ = *pixel_src--;
			}
		}
	} else {
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
	}
	return 0;
}

LCUI_API int Graph_VertiFlip( const LCUI_Graph *graph, LCUI_Graph *buff )
{
	int y;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	rect = Graph_GetValidRect( graph );
	graph = Graph_GetQuoteConst( graph );
	buff->color_type = graph->color_type;
	if( 0 != Graph_Create( buff, rect.width, rect.height ) ) {
		return -2;	
	}
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		byte_src = graph->bytes;
		byte_src += ((rect.y+rect.h-1)*graph->w + rect.x)*4;
		byte_des = buff->bytes;
		for( y=0; y<rect.h; ++y ) {
			memcpy( byte_des, byte_src, buff->w*4 );
			byte_src -= graph->w*4;
			byte_des += buff->w*4;
		} 
	} else {
		byte_src = graph->bytes + (rect.y*graph->w + rect.x)*3;
		byte_src += ((rect.y+rect.h-1)*graph->w + rect.x)*3;
		byte_des = buff->bytes;
		for( y=0; y<rect.h; ++y ) {
			memcpy( byte_des, byte_src, buff->w*3 );
			byte_src -= graph->w*3;
			byte_des += buff->w*3;
		} 
	}
	return 0;
}

LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_Color color, LCUI_Rect rect )
{
	int x, y, n;
	LCUI_Rect rect_src;
	uchar_t *byte;
	LCUI_ARGB *pixel;

	if(!Graph_IsValid(graph)) {
		return -1;
	}
	rect_src = Graph_GetValidRect( graph );
	graph = Graph_GetQuote( graph );
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		for( y=0; y<rect.h; ++y ) {
			pixel = graph->argb;
			pixel += (rect_src.y + rect.y + y)*graph->w;
			pixel += rect.x + rect_src.x;
			for( x=0; x<rect.w; ++x ) {
				*pixel++ = color;
			}
		}
	} else {
		for( y=0; y<rect.h; ++y ) {
			n = (rect_src.y + rect.y + y)*graph->w;
			n += rect.x + rect_src.x;
			n *= 3;
			byte = graph->bytes + n;
			for( x=0; x<rect.w; ++x ) {
				*byte++ = color.blue;
				*byte++ = color.green;
				*byte++ = color.red;
			}
		}
	}
	return 0;
}

LCUI_API int Graph_FillColor( LCUI_Graph *graph, LCUI_Color color )
{
	return Graph_FillRect( graph, color, Rect(0,0,graph->w, graph->h) );
}

LCUI_API int Graph_FillAlpha( LCUI_Graph *graph, uchar_t alpha )
{
	int x, y;
	LCUI_Rect rect;
	LCUI_ARGB *pixel, *pixel_row;
	
	rect = Graph_GetValidRect( graph );
	graph = Graph_GetQuote( graph );
	if(! Graph_IsValid(graph) ) {
		return -1;
	}
	if( !Graph_HaveAlpha(graph) ) {
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

LCUI_API int Graph_Tile( LCUI_Graph *buff,  const LCUI_Graph *graph, 
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

/** ARGB & ARGB */
static void Graph_ARGBMixARGB( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_src, *px_des;
	LCUI_ARGB *px_row_src, *px_row_des;
	
	/* 计算并保存第一行的首个像素的位置 */
	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		px_des = px_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			/* 将R、G、B三个色彩值进行alpha混合 */
			ALPHA_BLEND( px_des->r, px_src->r, px_src->a );
			ALPHA_BLEND( px_des->g, px_src->g, px_src->a );
			ALPHA_BLEND( px_des->b, px_src->b, px_src->a );
			/* 切换到下个像素的数据 */
			++px_src;
			++px_des;
		}
		/* 切换到下一行像素 */
		px_row_des += des->w;
		px_row_src += src->w;
	}
}

/** ARGB & RGB */
static void Graph_ARGBMixRGB( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *byte_row_des, *byte_des;
	
	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			/* B -> G -> R */
			ALPHA_BLEND( *byte_des++, px_src->b, px_src->a );
			ALPHA_BLEND( *byte_des++, px_src->g, px_src->a );
			ALPHA_BLEND( *byte_des++, px_src->r, px_src->a );
			++px_src;
		}
		byte_row_des += des->w*3;
		px_row_src += src->w;
	}
}

/** RGB & ARGB */
static int Graph_RGBMixARGB( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *byte_row_src, *byte_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	for( y=0; y<des_rect.h; ++y ) { 
		px_des = px_row_des;
		byte_src = byte_row_src;
		for( x=0; x<des_rect.w; ++x ) {
			px_des->b = *byte_src++;
			px_des->g = *byte_src++;
			px_des->r = *byte_src++;
			px_des->a = 255;
			++px_des;
		}
		byte_row_src += src->w*3;
		px_row_des += des->w;
	}
}

/** (ARGB + GlobalAlpha) & ARGB */
static void Graph_ARGBMixARGB_( LCUI_Graph *des, 
		LCUI_Rect des_rect, const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	double k;
	uchar_t alpha;
	LCUI_ARGB *px_src, *px_des;
	LCUI_ARGB *px_row_src, *px_row_des;

	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	/* 先得出透明度比例，避免在循环中进行除法运算 */
	k = src->alpha / 255.0;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		px_des = px_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			alpha = px_src->a * k;
			ALPHA_BLEND( px_des->r, px_src->r, alpha );
			ALPHA_BLEND( px_des->g, px_src->g, alpha );
			ALPHA_BLEND( px_des->b, px_src->b, alpha );
			++px_src;
			++px_des;
		}
		px_row_des += des->w;
		px_row_src += src->w;
	}
}

/** (ARGB + GlobalAlpha) & RGB */
static void Graph_ARGBMixRGB_( LCUI_Graph *des, LCUI_Rect des_rect, 
			      const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	double k;
	uchar_t alpha;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *byte_row_des, *byte_des;
	
	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	k = src->alpha / 255.0;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			alpha = px_src->a * k;
			ALPHA_BLEND( *byte_des++, px_src->b, alpha );
			ALPHA_BLEND( *byte_des++, px_src->g, alpha );
			ALPHA_BLEND( *byte_des++, px_src->r, alpha );
			++px_src;
		}
		byte_row_des += des->w*3;
		px_row_src += src->w;
	}
}

/** (RGB + GlobalAlpha) & ARGB */
static void Graph_RGBMixARGB_( LCUI_Graph *des, LCUI_Rect des_rect, 
			      const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *byte_row_src, *byte_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	for( y=0; y<des_rect.h; ++y ) { 
		byte_src = byte_row_src;
		px_des = px_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			ALPHA_BLEND( px_des->b, *byte_src++, src->alpha );
			ALPHA_BLEND( px_des->g, *byte_src++, src->alpha );
			ALPHA_BLEND( px_des->r, *byte_src++, src->alpha );
			++px_des;
		}
		px_row_des += des->w;
		byte_row_src += src->w*3;
	}
}

/** (RGB + GlobalAlpha) & RGB */
static void Graph_RGBMixRGB_( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	uchar_t *byte_src, *byte_row_src;
	uchar_t *byte_row_des, *byte_des;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	for( y=0; y<des_rect.h; ++y ) { 
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			ALPHA_BLEND( *byte_des++, *byte_src++, src->alpha );
			ALPHA_BLEND( *byte_des++, *byte_src++, src->alpha );
			ALPHA_BLEND( *byte_des++, *byte_src++, src->alpha );
		}
		byte_row_des += des->w;
		byte_row_src += src->w;
	}
}

/** ARGB => RGB */
static void Graph_ARGBReplaceRGB( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *byte_row_des, *byte_des;
	
	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			*byte_des++ = _ALPHA_BLEND( 255, px_src->b, px_src->a );
			*byte_des++ = _ALPHA_BLEND( 255, px_src->g, px_src->a );
			*byte_des++ = _ALPHA_BLEND( 255, px_src->r, px_src->a );
			++px_src;
		}
		byte_row_des += des->w*3;
		px_row_src += src->w;
	}
}

/** (ARGB + GolbalAlpha) => RGB */
static void Graph_ARGBReplaceRGB_( LCUI_Graph *des, LCUI_Rect des_rect,
				 const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	double k;
	uchar_t alpha;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *byte_row_des, *byte_des;
	
	px_row_src = src->argb + src_pos.y * src->w + src_pos.x;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	k = src->alpha / 255.0;
	for( y=0; y<des_rect.h; ++y ) { 
		px_src = px_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			alpha = px_src->a * k;
			*byte_des++ = _ALPHA_BLEND( 255, px_src->b, alpha );
			*byte_des++ = _ALPHA_BLEND( 255, px_src->g, alpha );
			*byte_des++ = _ALPHA_BLEND( 255, px_src->r, alpha );
			++px_src;
		}
		byte_row_des += des->w*3;
		px_row_src += src->w;
	}
}

/** RGB => ARGB */
static void Graph_RGBReplaceARGB( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *byte_row_src, *byte_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	for( y=0; y<des_rect.h; ++y ) { 
		px_des = px_row_des;
		byte_src = byte_row_src;
		for( x=0; x<des_rect.w; ++x ) {
			px_des->b = *byte_src++;
			px_des->g = *byte_src++;
			px_des->r = *byte_src++;
			++px_des;
		}
		byte_row_src += src->w*3;
		px_row_des += des->w;
	}
}

/** (RGB + GolbalAlpha) => ARGB */
static void Graph_RGBReplaceARGB_( LCUI_Graph *des, LCUI_Rect des_rect, 
				 const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *byte_row_src, *byte_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	px_row_des = des->argb + des_rect.y * des->w + des_rect.x;
	for( y=0; y<des_rect.h; ++y ) { 
		px_des = px_row_des;
		byte_src = byte_row_src;
		for( x=0; x<des_rect.w; ++x ) {
			px_des->b = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
			px_des->g = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
			px_des->r = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
			++px_des;
		}
		byte_row_src += src->w*3;
		px_row_des += des->w;
	}
}

static void Graph_DirectReplace( LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int y, row_bytes;
	uchar_t *byte_row_des, *byte_row_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	if( des->color_type == COLOR_TYPE_ARGB ) {
		row_bytes = sizeof(uchar_t)*4*des_rect.w;
		for( y=0; y<des_rect.h; ++y ) { 
			memcpy( byte_row_des, byte_row_src, row_bytes );
			byte_row_src += src->w*4;
			byte_row_des += des->w*4;
		}
	} else {
		row_bytes = sizeof(uchar_t)*3*des_rect.w;
		for( y=0; y<des_rect.h; ++y ) { 
			memcpy( byte_row_des, byte_row_src, row_bytes );
			byte_row_src += src->w*3;
			byte_row_des += des->w*3;
		}
	}
}

static void Graph_DirectReplace_( LCUI_Graph *des, LCUI_Rect des_rect, 
				 const LCUI_Graph *src, LCUI_Pos src_pos )
{
	int x, y;
	double k;
	uchar_t *byte_row_des, *byte_row_src;
	uchar_t *byte_des, *byte_src;
	
	byte_row_src = src->bytes + (src_pos.y * src->w + src_pos.x)*3;
	byte_row_des = des->bytes + (des_rect.y * des->w + des_rect.x)*3;
	k = src->alpha / 255.0;
	if( des->color_type == COLOR_TYPE_ARGB ) {
		for( y=0; y<des_rect.h; ++y ) { 
			byte_src = byte_row_src;
			byte_des = byte_row_des;
			for( x=0; x<des_rect.w; ++x ) {
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src++;
				*byte_des++ = k*(*byte_src++);
			}
			byte_row_src += src->w*4;
			byte_row_des += des->w*4;
		}
		return;
	} 

	for( y=0; y<des_rect.h; ++y ) { 
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for( x=0; x<des_rect.w; ++x ) {
			*byte_des++ = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
			*byte_des++ = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
			*byte_des++ = _ALPHA_BLEND( 255, *byte_src++, src->alpha );
		}
		byte_row_src += src->w*3;
		byte_row_des += des->w*3;
	}
}

LCUI_API int Graph_Mix(	LCUI_Graph *back_graph, const LCUI_Graph *fore_graph,
			LCUI_Pos des_pos )
{
	const LCUI_Graph *src;
	LCUI_Graph*des;
	LCUI_Rect cut, src_rect, area, des_rect;
	LCUI_Size box_size;
	LCUI_Pos src_pos;
	
	/* 预先进行有效性判断 */
	if( !Graph_IsValid(back_graph)
	 || !Graph_IsValid(fore_graph) ) {
		return -1;
	}
	/* 获取引用的源图像的最终区域 */
	src_rect = Graph_GetValidRect(fore_graph);
	des_rect = Graph_GetValidRect(back_graph);
	/* 获取引用的源图像 */
	src = Graph_GetQuoteConst(fore_graph);
	des = Graph_GetQuote(back_graph);  
	/* 判断引用的源图像的有效性 */
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -2;
	} 
	/* 判断坐标是否在背景图的范围内 */
	if(des_pos.x > des->w || des_pos.y > des->h) {
		return -3;
	}
	/* 记录容器尺寸 */
	box_size.w = des_rect.width;
	box_size.h = des_rect.height;
	/* 记录前景图像在容器中的区域 */
	area.x = des_pos.x;
	area.y = des_pos.y;
	area.width = src_rect.width;
	area.height = src_rect.height;
	/* 获取前景图像区域中的需要裁减的区域 */ 
	LCUIRect_GetCutArea( box_size, area, &cut );
	/* 移动前景图像区域的坐标 */
	des_pos.x += cut.x;
	des_pos.y += cut.y;
	/* 得出源图像的读取区域的坐标 */
	src_pos.x = cut.x + src_rect.x;
	src_pos.y = cut.y + src_rect.y;
	/* 得出目标图像的写入区域 */
	des_rect.x = des_pos.x + des_rect.x;
	des_rect.y = des_pos.y + des_rect.y;
	des_rect.width = cut.width;
	des_rect.height = cut.height;

	if( src->color_type == COLOR_TYPE_ARGB ) {
		if( src->alpha == 255 ) {
			if( des->color_type == COLOR_TYPE_ARGB ) {
				Graph_ARGBMixARGB_( des, des_rect, src, src_pos );
				return 0;
			}
			Graph_ARGBMixRGB_( des, des_rect, src, src_pos );
			return 0;
		}
		if( des->color_type == COLOR_TYPE_ARGB ) {
			Graph_ARGBMixARGB( des, des_rect, src, src_pos );
			return 0;
		}
		Graph_ARGBMixRGB( des, des_rect, src, src_pos );
		return 0;
	}
	if( src->alpha == 255 ) {
		if( des->color_type == COLOR_TYPE_ARGB ) {
			Graph_RGBMixARGB_( des, des_rect, src, src_pos );
			return 0;
		}
		Graph_RGBMixRGB_( des, des_rect, src, src_pos );
		return 0;
	}
	if( des->color_type == COLOR_TYPE_ARGB ) {
		Graph_RGBReplaceARGB( des, des_rect, src, src_pos );
		return 0;
	}
	Graph_DirectReplace( des, des_rect, src, src_pos );
	return 0;
}

LCUI_API int Graph_Replace( LCUI_Graph *back_graph, 
			   const LCUI_Graph *fore_graph, LCUI_Pos des_pos )
{
	const LCUI_Graph *src;
	LCUI_Graph *des;
	LCUI_Rect cut, src_rect, area, des_rect;
	LCUI_Size box_size;
	LCUI_Pos src_pos;
	
	if( !Graph_IsValid(back_graph)
	 || !Graph_IsValid(fore_graph) ) {
		return -1;
	}

	src_rect = Graph_GetValidRect(fore_graph);
	des_rect = Graph_GetValidRect(back_graph);
	src = Graph_GetQuoteConst(fore_graph);
	des = Graph_GetQuote(back_graph);

	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	} 
	if(des_pos.x > des->w || des_pos.y > des->h) {
		return -1;
	}

	box_size.w = des_rect.width;
	box_size.h = des_rect.height;
	area.x = des_pos.x;
	area.y = des_pos.y;
	area.width = src_rect.width;
	area.height = src_rect.height;
	LCUIRect_GetCutArea( box_size, area, &cut );
	des_pos.x += cut.x;
	des_pos.y += cut.y;
	src_pos.x = cut.x + src_rect.x;
	src_pos.y = cut.y + src_rect.y;
	des_rect.x = des_pos.x + des_rect.x;
	des_rect.y = des_pos.y + des_rect.y;
	des_rect.width = cut.width;
	des_rect.height = cut.height;

	if( src->color_type == des->color_type ) {
		if( src->alpha == 255 ) {
			Graph_DirectReplace( des, des_rect, src, src_pos );
			return 0;
		}
		Graph_DirectReplace_( des, des_rect, src, src_pos );
		return 0;
	}
	
	if( src->color_type == COLOR_TYPE_ARGB ) {
		if( src->alpha == 255 ) {
			Graph_ARGBReplaceRGB( des, des_rect, src, src_pos );
			return 0;
		}
		Graph_ARGBReplaceRGB_( des, des_rect, src, src_pos );
		return 0;
	}
	if( src->alpha == 255 ) {
		Graph_RGBReplaceARGB( des, des_rect, src, src_pos );
		return 0;
	}
	Graph_RGBReplaceARGB_( des, des_rect, src, src_pos );
	return 0;
}

LCUI_API int Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, 
				    AlignType align, LCUI_BOOL replace )
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
LCUI_API int Graph_FillImageEx( LCUI_Graph *graph, LCUI_Graph *backimg,
					     int layout, LCUI_Rect area )
{
	LCUI_Pos pos;
	LCUI_Graph box;

	if( Graph_Quote( &box, graph, area ) != 0 ) {
		return -1;
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
	return 0;
}

/** 填充图像和背景色
 * @param graph		目标图像
 * @param backimg	背景图
 * @param layout	背景图的布局
 * @param color		背景色
 * @param area		需要绘制的区域
 */
LCUI_API int Graph_FillImageWithColorEx( LCUI_Graph *graph, 
	LCUI_Graph *backimg, int layout, LCUI_Color color, LCUI_Rect area )
{
	LCUI_Pos pos;
	LCUI_Graph box;

	if( Graph_Quote( &box, graph, area ) != 0 ) {
		return -1;
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
	return 0;
}

LCUI_API int Graph_FillImage( LCUI_Graph *graph, LCUI_Graph *backimg,
				int layout )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = graph->w;
	area.h = graph->h;
	return Graph_FillImageEx( graph, backimg, layout, area );
}

LCUI_API int Graph_FillImageWithColor( LCUI_Graph *graph, LCUI_Graph *backimg,
						int layout, LCUI_Color color )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = graph->w;
	area.h = graph->h;
	return Graph_FillImageWithColorEx( graph, backimg, layout,
							color, area );
}
