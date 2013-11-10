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

LCUI_API LCUI_RGB RGB ( uchar_t red, uchar_t green, uchar_t blue )
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

LCUI_API void Graph_Lock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Lock( &src->mutex );
}

LCUI_API void Graph_Unlock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Unlock( &src->mutex );
}

LCUI_API LCUI_BOOL Graph_GetPixel( LCUI_Graph *graph, LCUI_Pos pos, LCUI_RGBA *pixel )
{
	int i;
	LCUI_Rect rect;
	
	if( pos.x < 0 || pos.y < 0 ) {
		return FALSE;
	}
	rect = Graph_GetValidRect( graph );
	/* 若坐标超出范围 */
	if( pos.x >= rect.width || pos.y >= rect.height ) {
		return FALSE;
	}
	graph = Graph_GetQuote( graph );
	if( !Graph_IsValid(graph) ) {
		return FALSE;
	}
	i = graph->w*(pos.y+rect.y) + pos.x + rect.x;
	pixel->red = graph->rgba[0][i];
	pixel->green = graph->rgba[1][i];
	pixel->blue = graph->rgba[2][i];
	
	if(graph->color_type == COLOR_TYPE_RGBA) {
		pixel->alpha = graph->rgba[3][i];
	} else {
		pixel->alpha = 255;
	}
	return TRUE;
}

LCUI_API LCUI_Size Graph_GetSize( LCUI_Graph *graph )
{
	if(NULL == graph) {
		return Size(0,0);
	}
	return Size(graph->w, graph->h);
}

LCUI_API int Graph_GetWidth( LCUI_Graph *graph )
{
	if(NULL == graph) {
		return 0;
	}
	return graph->w;
}

LCUI_API int Graph_GetHeight( LCUI_Graph *graph )
{
	if(NULL == graph) {
		return 0;
	}
	return graph->h;
}

LCUI_API LCUI_BOOL Graph_HaveAlpha( LCUI_Graph *graph )
{
	graph = Graph_GetQuote(graph);
	if( graph && graph->color_type == COLOR_TYPE_RGBA ) {
		return TRUE;
	}
	return FALSE; 
}

LCUI_API LCUI_BOOL Graph_IsValid( LCUI_Graph *graph )
{
	LCUI_Graph *p;
	p = Graph_GetQuote(graph);
	if(p != NULL && p->rgba != NULL
	  && p->h > 0 && p->w > 0 ) {
		return TRUE;
	}
	return FALSE;
}

LCUI_API void Graph_PrintInfo( LCUI_Graph *graph )
{
	printf("address:%p\n", graph);
	if( !graph ) {
		return;
	}
	
	printf("width:%d, ", graph->w);
	printf("height:%d, ", graph->h);
	printf("alpha:%u, ", graph->alpha);
	printf("%s\n", graph->color_type == COLOR_TYPE_RGBA ? "RGBA":"RGB");
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
	
	graph->quote		= FALSE; 
	graph->src		= NULL;
	graph->color_type	= COLOR_TYPE_RGB;
	graph->rgba		= NULL;
	graph->alpha		= 255;
	graph->mem_size		= 0;
	graph->x		= 0;
	graph->y		= 0;
	graph->w		= 0;
	graph->h		= 0;
	LCUIMutex_Init( &graph->mutex );
}

LCUI_API int Graph_Create( LCUI_Graph *graph, int w, int h )
{
	size_t size;

	if(w > 10000 || h > 10000) {
		printf("error: can not allocate too much memory!\n");
		abort();
	}
	if(h <= 0 || w <= 0) {
		Graph_Free( graph );
		return -1; 
	}
	
	size = sizeof(uchar_t)*w*h;
	if( Graph_IsValid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if( graph->mem_size >= size ) {
			if( (w != graph->w || h != graph->h)
			 && graph->color_type == COLOR_TYPE_RGBA ) {
				memset( graph->rgba[3], 0, graph->mem_size );
			}
			graph->w = w;
			graph->h = h;
			return 0;
		}
		Graph_Free( graph );
		LCUIMutex_Init( &graph->mutex );
	}
	graph->mem_size = size;
	if( graph->color_type == COLOR_TYPE_RGBA ) {
		graph->rgba = (uchar_t**)malloc(sizeof(uchar_t*)*4);
	} else {
		graph->rgba = (uchar_t**)malloc(sizeof(uchar_t*)*3);
	}
	if( graph->rgba == NULL ) {
		goto error_exit;
	}
	graph->rgba[0] = (uchar_t*)malloc( graph->mem_size );
	if( graph->rgba[0] == NULL ) {
		free( graph->rgba );
		goto error_exit;
	}
	graph->rgba[1] = (uchar_t*)malloc( graph->mem_size );
	if( graph->rgba[1] == NULL ) {
		free( graph->rgba[0] );
		free( graph->rgba );
		goto error_exit;
	}
	graph->rgba[2] = (uchar_t*)malloc( graph->mem_size );
	if( graph->rgba[2] == NULL ) {
		free( graph->rgba[0] );
		free( graph->rgba[1] );
		free( graph->rgba );
		goto error_exit;
	}
	if( graph->color_type == COLOR_TYPE_RGBA ) {
		graph->rgba[3] = (uchar_t*)malloc( graph->mem_size );
		if( graph->rgba[3] == NULL ) {
			free( graph->rgba[0] );
			free( graph->rgba[1] );
			free( graph->rgba[2] );
			free( graph->rgba );
			goto error_exit;
		}
		/* 默认将alpha通道用0填充 */
		memset( graph->rgba[3], 0, graph->mem_size );
	}
	graph->w = w;
	graph->h = h;
	return 0;
error_exit:
	graph->w = 0;
	graph->h = 0;
	return -1;
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

LCUI_API void Graph_Free( LCUI_Graph *pic )
{
	LCUI_Graph *p;

	if( pic && pic->quote ) {
		pic->src = NULL; 
		pic->quote = FALSE;
		return;
	}
	p = Graph_GetQuote( pic );
	if( !Graph_IsValid(p)) {
		return;
	}
	LCUIMutex_Lock( &p->mutex );
	free( p->rgba[0] );
	free( p->rgba[1] );
	free( p->rgba[2] );
	if( p->color_type == COLOR_TYPE_RGBA ) {
		free( p->rgba[3] );
	}
	free( p->rgba );
	p->rgba = NULL;
	p->w = 0;
	p->h = 0;
	LCUIMutex_Unlock( &p->mutex );
	LCUIMutex_Destroy( &pic->mutex );
}

LCUI_API int Graph_Quote( LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area )
{
	if( !src || !des ) {
		return -1;
	}
	//printf("Graph_Quote(), before, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	area = LCUIRect_ValidArea(Size(src->w, src->h), area);
	//printf("Graph_Quote(), after, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	if(!LCUIRect_IsValid( area )) { 
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

LCUI_API LCUI_Rect Graph_GetValidRect( LCUI_Graph *graph )
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
	rect = Graph_GetValidRect(graph->src);
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
	if( !graph ) {
		return NULL;
	}
	if( !graph->quote ) {
		return graph; 
	}
	return Graph_GetQuote(graph->src);
}

LCUI_API int Graph_Zoom(	LCUI_Graph *in_graph,
				LCUI_Graph *out_graph, 
				LCUI_BOOL keep_scale,
				LCUI_Size size )
{
	LCUI_Graph *src;
	LCUI_Rect rect;
	LCUI_Pos pos; 
	int des_n, src_n, x, y, k, m;
	double scale_x,scale_y;

	if(!Graph_IsValid(in_graph)) {
		return -1;
	}
	if(size.w <=0 || size.h <= 0) { 
		Graph_Free(out_graph);
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	rect = Graph_GetValidRect(in_graph);
	src = Graph_GetQuote(in_graph);

	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	/* 如果缩放方式为缺省，图像的宽和高的缩放比例将会一样 */
	if( keep_scale ) {
		if (scale_x<scale_y) {
			scale_y = scale_x; 
		} else {
			scale_x = scale_y;
		}
	}
	out_graph->color_type = in_graph->color_type;
	if( Graph_Create(out_graph, size.w, size.h) < 0) {
		return -2;
	}
	for (y=0; y < size.h; ++y)  {
		pos.y = y*scale_y;
		k = y*size.w;
		m = (pos.y+rect.y)*src->w + rect.x;
		for (x = 0; x < size.w; ++x) {
			pos.x = x*scale_x; 
			src_n  = k + x;
			des_n = m + pos.x;
			out_graph->rgba[0][src_n] = src->rgba[0][des_n];
			out_graph->rgba[1][src_n] = src->rgba[1][des_n];
			out_graph->rgba[2][src_n] = src->rgba[2][des_n];
			if( in_graph->color_type == COLOR_TYPE_RGBA) {
				out_graph->rgba[3][src_n] = src->rgba[3][des_n];
			}
		}
	} 
	return 0;
}

LCUI_API int Graph_Cut( LCUI_Graph *src_graph, LCUI_Rect rect, LCUI_Graph *out_graph )
{
	int k, x, y, des_n, src_n;
	
	out_graph->alpha = src_graph->alpha;
	rect = LCUIRect_ValidArea(Size(src_graph->w, src_graph->h), rect); 
	
	if( !Graph_IsValid(src_graph) || rect.width <= 0 || rect.height <= 0) {
		return -1;
	}
	
	if( 0 != Graph_Create(out_graph, rect.width, rect.height) ) {
		return -1; 
	}

	des_n = 0;
	/* 开始读取图片中的图形数组并写入窗口 */ 
	for (y=0;y< rect.height; ++y) {
		k = (rect.y+y)*src_graph->w + rect.x;
		for (x=0;x<rect.width; ++x) {
			src_n = k + x; /* 计算图片内需要读取的区域的各点坐标 */
			out_graph->rgba[0][des_n] = src_graph->rgba[0][src_n];
			out_graph->rgba[1][des_n] = src_graph->rgba[1][src_n];
			out_graph->rgba[2][des_n] = src_graph->rgba[2][src_n];
			if(out_graph->color_type == COLOR_TYPE_RGBA) {
				if(src_graph->color_type == COLOR_TYPE_RGBA) {
					out_graph->rgba[3][des_n] = src_graph->rgba[3][src_n];
				} else {
					out_graph->rgba[3][des_n] = 255;
				}
			}
			++des_n;
		}
	}
	return 0; 
}

LCUI_API int Graph_HorizFlip( LCUI_Graph *src_graph, LCUI_Graph *out_graph )
{
	int x, y, center;
	int src_left_pos, src_right_pos;
	int des_left_pos, des_right_pos;
	uchar_t buff;
	LCUI_Graph *src;
	LCUI_Rect rect;

	if(!Graph_IsValid(src_graph)) {
		return -1;
	}
	rect = Graph_GetValidRect( src_graph );
	src = Graph_GetQuote( src_graph );
	out_graph->color_type = src->color_type;
	if( 0 != Graph_Create( out_graph, rect.width, rect.height ) ) {
		return -2;	
	}
	/* 水平翻转其实也就是交换两边的数据，需要计算中点像素的位置 */
	if( rect.width > 1 ) {
		center = rect.width-1;
		center = center/2;
	} else {
		center = 0;
	}
	for (y = 0; y < rect.height; ++y) {
		des_left_pos = y * rect.width;
		des_right_pos = des_left_pos + rect.width -1;
		src_left_pos = (y + rect.y)*src->w + rect.x;
		src_right_pos = src_left_pos + rect.width -1;
		for (x = 0; x <= center; ++x)  {
			buff = src->rgba[0][src_left_pos]; 
			out_graph->rgba[0][des_left_pos] = src->rgba[0][src_right_pos];  
			out_graph->rgba[0][des_right_pos] = buff;

			buff = src->rgba[1][src_left_pos]; 
			out_graph->rgba[1][des_left_pos] = src->rgba[1][src_right_pos];  
			out_graph->rgba[1][des_right_pos] = buff;

			buff = src->rgba[2][src_left_pos]; 
			out_graph->rgba[2][des_left_pos] = src->rgba[2][src_right_pos];  
			out_graph->rgba[2][des_right_pos] = buff;

			if(src->color_type == COLOR_TYPE_RGBA) {
				buff = src->rgba[3][src_left_pos]; 
				out_graph->rgba[3][des_left_pos] = src->rgba[3][src_right_pos];  
				out_graph->rgba[3][des_right_pos] = buff;
			}
			++src_left_pos;
			++des_left_pos;
			--src_right_pos;
			--des_right_pos;
		}
	} 
	return 0;
}

LCUI_API int Graph_VertiFlip( LCUI_Graph *src_graph, LCUI_Graph *out_graph )
{
	uchar_t buff;
	LCUI_Rect rect;
	int x, y, center; 
	int src_top_pos, src_bottom_pos;
	int des_top_pos, des_bottom_pos;
	int src_start_top_pos, src_start_bottom_pos;
	int des_start_top_pos, des_start_bottom_pos;

	if(!Graph_IsValid(src_graph)) {
		return -1;
	}
	src_graph = Graph_GetQuote(src_graph );
	rect = Graph_GetValidRect( src_graph );
	out_graph->color_type = src_graph->color_type;
	if( 0 != Graph_Create(out_graph, rect.width, rect.height ) ) {
		return -2;
	}

	center = (int)(rect.height / 2.0);
	/* 记录基坐标 */
	des_start_top_pos = 0;
	des_start_bottom_pos = (rect.height-1)*rect.width;
	src_start_top_pos = rect.y * src_graph->w + rect.x;
	src_start_bottom_pos = (rect.y + rect.height-1)*src_graph->w + rect.x;

	for (x=0; x < rect.width; ++x) {
		/* 当前坐标=基坐标+x */
		des_top_pos = des_start_top_pos + x;
		des_bottom_pos = des_start_bottom_pos + x;
		src_top_pos = src_start_top_pos + x;
		src_bottom_pos = src_start_bottom_pos + x;
		for (y = 0; y <= center; ++y) {
			buff = src_graph->rgba[0][src_top_pos]; 
			out_graph->rgba[0][des_top_pos] = src_graph->rgba[0][src_bottom_pos];  
			out_graph->rgba[0][des_bottom_pos] = buff;

			buff = src_graph->rgba[1][src_top_pos]; 
			out_graph->rgba[1][des_top_pos] = src_graph->rgba[1][src_bottom_pos];  
			out_graph->rgba[1][des_bottom_pos] = buff;

			buff = src_graph->rgba[2][src_top_pos]; 
			out_graph->rgba[2][des_top_pos] = src_graph->rgba[2][src_bottom_pos];  
			out_graph->rgba[2][des_bottom_pos] = buff;

			if(src_graph->color_type == COLOR_TYPE_RGBA) {
				buff = src_graph->rgba[3][src_top_pos]; 
				out_graph->rgba[3][des_top_pos] = src_graph->rgba[3][src_bottom_pos];  
				out_graph->rgba[3][des_bottom_pos] = buff;
			}
			src_top_pos += src_graph->w;
			des_top_pos += rect.width;
			src_bottom_pos -= src_graph->w;
			des_bottom_pos -= rect.width;
		}
	}
	return 0;
}

LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_RGB color, LCUI_Rect rect )
{
	size_t size;
	int pos, y;
	LCUI_Rect src_rect;
	uchar_t *r_ptr, *g_ptr, *b_ptr;
	
	src_rect = Graph_GetValidRect( graph ); 
	graph = Graph_GetQuote( graph );
	
	if(! Graph_IsValid(graph) ) {
		return -1;
	}
	rect.x = rect.x + src_rect.x;
	rect.y = rect.y + src_rect.y;
	/* 计算出该填充区域实际的有效区域范围 */
	if( rect.x < src_rect.x ) {
		rect.width -= (src_rect.x-rect.x);
		rect.x = src_rect.x;
	}
	if( rect.x + rect.width > src_rect.x + src_rect.width ) {
		rect.width = src_rect.x + src_rect.width - rect.x;
	}
	
	if( rect.y < src_rect.y ) {
		rect.height -= (src_rect.y-rect.y);
		rect.y = src_rect.y;
	}
	if( rect.y + rect.height > src_rect.x + src_rect.height ) {
		rect.height = src_rect.y + src_rect.height - rect.y;
	}
	size = sizeof(uchar_t) * rect.width;
	pos = rect.x + rect.y * graph->w;
	r_ptr = graph->rgba[0] + pos;
	g_ptr = graph->rgba[1] + pos;
	b_ptr = graph->rgba[2] + pos;
	for(y=0; y<rect.height; ++y) {
		memset( r_ptr, color.red, size );
		memset( g_ptr, color.green, size );
		memset( b_ptr, color.blue, size );
		r_ptr += graph->w;
		g_ptr += graph->w;
		b_ptr += graph->w;
	}
	return 0; 
}

LCUI_API int Graph_FillColor( LCUI_Graph *graph, LCUI_RGB color )
{
	return Graph_FillRect( graph, color, Rect(0,0,graph->w, graph->h) );
}

LCUI_API int Graph_Tile( LCUI_Graph *src, LCUI_Graph *des, LCUI_BOOL replace )
{
	int ret = 0;
	LCUI_Pos pos;
	
	if(!Graph_IsValid(src) || !Graph_IsValid(des)) {
		return -1;
	}
	for(pos.y=0; pos.y<des->h; pos.y+=src->h) {
		for(pos.x=0; pos.x<des->w; pos.x+=src->w) {
			if( replace ) {
				ret += Graph_Replace( des, src, pos );
			} else {
				ret += Graph_Mix( des, src, pos );
			}
		}
	}
	return ret;
}

/**
 * 将色彩类型为RGBA的前景图像，合成至目标背景图像中
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Mix函
 *	数调用，Graph_Mix函数会预先进行参数有效性判断
 */
static void Graph_RGBAMix(	LCUI_Graph *des,
				LCUI_Rect des_rect,
				LCUI_Graph *src,
				LCUI_Pos src_pos )
{
	int x, y;
	int src_n, des_n;
	int des_row_start, src_row_start;

	/* 计算并保存第一行的首个像素的位置 */
	src_row_start = src_pos.y * src->w + src_pos.x;
	des_row_start = des_rect.y * des->w + des_rect.x;
	for(y=0; y<des_rect.height; ++y) { 
		des_n = des_row_start;
		src_n = src_row_start;
		for(x=0; x<des_rect.width; ++x) {
			/* 将R、G、B三个色彩值进行alpha混合 */
			ALPHA_BLEND( src->rgba[0][src_n], des->rgba[0][des_n], src->rgba[3][src_n] );
			ALPHA_BLEND( src->rgba[1][src_n], des->rgba[1][des_n], src->rgba[3][src_n] );
			ALPHA_BLEND( src->rgba[2][src_n], des->rgba[2][des_n], src->rgba[3][src_n] );
			/* 切换到下个像素的数据 */
			++src_n;
			++des_n;
		}
		/* 切换到下一行像素 */
		des_row_start += des->w;
		src_row_start += src->w;
	}
}

/**
 * 将色彩类型为RGBA的前景图像，合成至目标背景图像中
 * 在进行alpha混合时，会将源图像的全局透明度计算在内进行混合
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Mix函
 *	数调用，Graph_Mix函数会预先进行参数有效性判断
 */
static void Graph_RGBAMixWithGlobalAlpha(	LCUI_Graph *des,
						LCUI_Rect des_rect,
						LCUI_Graph *src,
						LCUI_Pos src_pos )
{
	double k;
	int x, y;
	uchar_t alpha;
	int src_n, des_n;
	int des_row_start, src_row_start;

	src_row_start = src_pos.y * src->w + src_pos.x;
	des_row_start = des_rect.y * des->w + des_rect.x;
	/* 先得出透明度比例，避免在循环中进行除法运算 */
	k = src->alpha / 255.0;

	for (y=0; y < des_rect.height; ++y) {
		des_n = des_row_start;
		src_n = src_row_start;
		for(x=0; x<des_rect.width; ++x) { 
			alpha = src->rgba[3][src_n] * k;
			ALPHA_BLEND(	src->rgba[0][src_n],
					des->rgba[0][des_n],
					alpha );
			ALPHA_BLEND(	src->rgba[1][src_n],
					des->rgba[1][des_n],
					alpha );
			ALPHA_BLEND(	src->rgba[2][src_n],
					des->rgba[2][des_n],
					alpha );
			++des_n;
			++src_n;
		} 
		src_row_start += src->w;
		des_row_start += des->w; 
	}
}

/**
 * 将色彩类型为RGB的前景图像，合成至目标背景图像中
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Mix函
 *	数调用，Graph_Mix函数会预先进行参数有效性判断
 */
static void Graph_RGBMix(	LCUI_Graph *des,
				LCUI_Rect des_rect,
				LCUI_Graph *src,
				LCUI_Pos src_pos )
{
	int y;
	int des_n, src_n;

	src_n = src_pos.y * src->w + src_pos.x;
	des_n = des_rect.y * des->w + des_rect.x;

	for(y=0; y<des_rect.height; ++y) { 
		/* 直接拷贝覆盖每一行像素数据 */
		memcpy( &des->rgba[0][des_n], &src->rgba[0][src_n], des_rect.width );
		memcpy( &des->rgba[1][des_n], &src->rgba[1][src_n], des_rect.width );
		memcpy( &des->rgba[2][des_n], &src->rgba[2][src_n], des_rect.width );
		des_n += des->w;
		src_n += src->w;
	}
}

/**
 * 将色彩类型为RGB的前景图像，合成至目标背景图像中
 * 在进行alpha混合时，会将源图像的全局透明度计算在内进行混合
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Mix函
 *	数调用，Graph_Mix函数会预先进行参数有效性判断
 */
static void Graph_RGBMixWithGlobalAlpha(	LCUI_Graph *des,
						LCUI_Rect des_rect,
						LCUI_Graph *src,
						LCUI_Pos src_pos )
{
	int x, y;
	int src_n, des_n;
	int des_row_start, src_row_start;
	
	src_row_start = src_pos.y * src->w + src_pos.x;
	des_row_start = des_rect.y * src->w + des_rect.x;
	for(y=0; y<des_rect.height; ++y) { 
		des_n = des_row_start;
		src_n = src_row_start;
		for(x=0; x<des_rect.width; ++x) {
			ALPHA_BLEND(	src->rgba[0][src_n],
					des->rgba[0][des_n],
					src->alpha );
			ALPHA_BLEND(	src->rgba[1][src_n],
					des->rgba[1][des_n],
					src->alpha );
			ALPHA_BLEND(	src->rgba[2][src_n],
					des->rgba[2][des_n],
					src->alpha );
			++des_n;
			++src_n;
		}
	}
}

LCUI_API int Graph_Mix(	LCUI_Graph *back_graph, 
			LCUI_Graph *fore_graph,
			LCUI_Pos des_pos )
{
	LCUI_Graph *src, *des;
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
	src = Graph_GetQuote(fore_graph);
	des = Graph_GetQuote(back_graph);  
	/* 判断引用的源图像的有效性 */
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	} 
	/* 判断坐标是否在背景图的范围内 */
	if(des_pos.x > des->w || des_pos.y > des->h) {
		return -1;
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
	
	/* 如果前景图像有alpha通道 */
	if( src->color_type == COLOR_TYPE_RGBA ) {
		/* 如果全局透明度为255，则说明混合时不需要考虑全局透明度 */
		if( src->alpha == 255 ) {
			Graph_RGBAMix( des, des_rect, src, src_pos );
			return 0;
		}
		Graph_RGBAMixWithGlobalAlpha( des, des_rect, src, src_pos );
		return 0;
	}
	/* 否则，前景图像没有Alpha通道 */

	/* 如果全局透明度为255，说明前景图形没有透明效果 */
	if( src->alpha == 255 ) { 
		Graph_RGBMix( des, des_rect, src, src_pos );
		return 0;
	}
	/* 否则，将全局透明度计算在内，进行alpha混合 */
	Graph_RGBMixWithGlobalAlpha( des, des_rect, des, src_pos );
	return 0;
}

/**
 * 将色彩类型为RGBA的前景图像，覆盖至色彩类型为RGB的目标背景图像中
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Replace函
 *	数调用，Graph_Replace函数会预先进行参数有效性判断
 */
static int Graph_RGBAReplaceRGB(	LCUI_Graph *back,
					LCUI_Rect des_rect,
					LCUI_Graph *fore,
					LCUI_Pos src_pos )
{
	int x, y;
	int src_n, des_n;
	int des_row_start, src_row_start;
	LCUI_Graph *src, *des;

	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	
	src_row_start = src_pos.y * src->w + src_pos.x;
	des_row_start = des_rect.y * des->w +  des_rect.x;
	
	for(y=0; y<des_rect.height; ++y) { 
		des_n = des_row_start;
		src_n = src_row_start;
		for(x=0; x<des_rect.width; ++x) {
			des->rgba[0][des_n] = _ALPHA_BLEND( src->rgba[0][src_n], 255, src->rgba[3][src_n] );
			des->rgba[1][des_n] = _ALPHA_BLEND( src->rgba[1][src_n], 255, src->rgba[3][src_n] );
			des->rgba[2][des_n] = _ALPHA_BLEND( src->rgba[2][src_n], 255, src->rgba[3][src_n] );
			++src_n;
			++des_n;
		}
		des_row_start += des->w;
		src_row_start += src->w;
	}
	return 0;
}


/**
 * 将色彩类型为RGBA的前景图像，覆盖至色彩类型为RGB的目标背景图像中
 * 当背景图像没有alpha通道时，该函数使用白色背景与前景图像进行混合，并将源图像
 * 的全局透明度计算在内
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Replace函
 *	数调用，Graph_Replace函数会预先进行参数有效性判断
 */
static int Graph_RGBAReplaceRGBWithGlobalAlpha(	LCUI_Graph *back,
						LCUI_Rect des_rect,
						LCUI_Graph *fore,
						LCUI_Pos src_pos )
{
	int x, y;
	double k;
	uchar_t alpha;
	int src_n, des_n;
	int des_row_start, src_row_start;
	LCUI_Graph *src, *des;
	
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	k = 1.0 / src->alpha;

	src_row_start = src_pos.y * src->w + src_pos.x;
	des_row_start = des_rect.y * des->w + des_rect.x;
	
	for(y=0; y<des_rect.height; ++y) { 
		des_n = des_row_start;
		src_n = src_row_start;
		for(x=0; x<des_rect.width; ++x) {
			alpha = src->alpha * k;
			des->rgba[0][des_n] = _ALPHA_BLEND( src->rgba[0][src_n], 255, alpha );
			des->rgba[1][des_n] = _ALPHA_BLEND( src->rgba[1][src_n], 255, alpha );
			des->rgba[2][des_n] = _ALPHA_BLEND( src->rgba[2][src_n], 255, alpha );
			++src->w;
			++des->w;
		}
		des_row_start += des->w;
		src_row_start += src->w;
	}
	return 0;
}

/**
 * 将前景图像，直接覆盖目标背景图像中(两个图像的色彩类型相同)
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Replace函
 *	数调用，Graph_Replace函数会预先进行参数有效性判断
 */
static int Graph_DirectReplace(	LCUI_Graph *back,
				LCUI_Rect des_rect,
				LCUI_Graph *fore,
				LCUI_Pos src_pos )
{
	int y;
	int des_n, src_n;
	LCUI_Graph *src, *des;

	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	src_n = src_pos.y * src->w + src_pos.x;
	des_n = des_rect.y * des->w + des_rect.x;

	for(y=0; y<des_rect.height; ++y) {
		memcpy( &des->rgba[0][des_n], &src->rgba[0][src_n], des_rect.width );
		memcpy( &des->rgba[1][des_n], &src->rgba[1][src_n], des_rect.width );
		memcpy( &des->rgba[2][des_n], &src->rgba[2][src_n], des_rect.width );
		if( des->color_type == src->color_type
		 && des->color_type == COLOR_TYPE_RGBA ) {
			memcpy( &des->rgba[3][des_n], &src->rgba[3][src_n], des_rect.width );
		}
		des_n += des->w;
		src_n += src->w;
	}

	return 0;
}
 /**
 * 将色彩类型为RGB的前景图像，覆盖至色彩类型为RGBA的目标背景图像中
 * param back
 *	背景图像
 * param des_rect
 *	背景图像中的写入区域
 * param fore
 *	前景图像
 * param src_pos
 *	前景图像中的读取区域的坐标
 * warning
 *	此函数不会对传入的参数进行有效性判断，因为此函数主要被Graph_Replace函
 *	数调用，Graph_Replace函数会预先进行参数有效性判断
 */
 static int Graph_RGBReplaceRGBA(	LCUI_Graph *back,
					LCUI_Rect des_rect,
					LCUI_Graph *fore,
					LCUI_Pos src_pos )
 {
	int y;
	int des_n, src_n;
	LCUI_Graph *src, *des;

	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	src_n = src_pos.y * src->w + src_pos.x;
	des_n = des_rect.y * des->w + des_rect.x;

	for(y=0; y<des_rect.height; ++y) {
		memcpy( &des->rgba[0][des_n], &src->rgba[0][src_n], des_rect.width );
		memcpy( &des->rgba[1][des_n], &src->rgba[1][src_n], des_rect.width );
		memcpy( &des->rgba[2][des_n], &src->rgba[2][src_n], des_rect.width );
		memset( &des->rgba[3][des_n], src->alpha, des_rect.width );
		des_n += des->w;
		src_n += src->w;
	}
	return 0;
}

LCUI_API int Graph_Replace(	LCUI_Graph *back_graph,
				LCUI_Graph *fore_graph,
				LCUI_Pos des_pos )
{
	LCUI_Graph *src, *des;
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
	src = Graph_GetQuote(fore_graph);
	des = Graph_GetQuote(back_graph);  
	/* 判断引用的源图像的有效性 */
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	} 
	/* 判断坐标是否在背景图的范围内 */
	if(des_pos.x > des->w || des_pos.y > des->h) {
		return -1;
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
	/* 如果前景图像有透明度 */
	if( src->color_type == COLOR_TYPE_RGBA ) {
		 /* 若背景图有透明度 */
		if( des->color_type == COLOR_TYPE_RGBA ) {
			return Graph_DirectReplace( des, des_rect, src, src_pos );
		}
		if( src->alpha == 255 ) {
			return Graph_RGBAReplaceRGB( des, des_rect, src, src_pos );
		}
		return Graph_RGBAReplaceRGBWithGlobalAlpha( des, des_rect, src, src_pos );
	}
	if( des->color_type == COLOR_TYPE_RGBA ) {
		return Graph_RGBReplaceRGBA( des, des_rect, src, src_pos );
	}
	return Graph_DirectReplace( des, des_rect, src, src_pos );
}

LCUI_API int Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int flag )
{
	LCUI_Pos pos;
	
	if(!Graph_IsValid(graph) || ! Graph_IsValid(image)) {
		return -1; 
	}
	pos.x = pos.y = 0;
	if((flag & ALIGN_TOP_LEFT) == ALIGN_TOP_LEFT); /* 左上角对齐 */
	else if((flag & ALIGN_TOP_CENTER) == ALIGN_TOP_CENTER) {
		/* 向上中间对齐 */
		pos.x = (graph->w - image->w) / 2;  
	}/* 向右上角对齐 */
	else if((flag & ALIGN_TOP_RIGHT) == ALIGN_TOP_RIGHT) {
		pos.x = graph->w - image->w;
	}/* 向中央偏左对齐 */ 
	else if((flag & ALIGN_MIDDLE_LEFT) == ALIGN_MIDDLE_LEFT) {
		pos.y = (graph->h - image->h) / 2; 
	}/* 向正中央对齐 */
	else if((flag & ALIGN_MIDDLE_CENTER) == ALIGN_MIDDLE_CENTER) { 
		pos.x = (graph->w - image->w) / 2;
		pos.y = (graph->h - image->h) / 2;
	}/* 向中央偏右对齐 */
	else if((flag & ALIGN_MIDDLE_RIGHT) == ALIGN_MIDDLE_RIGHT) { 
		pos.x = graph->w - image->w;
		pos.y = (graph->h - image->h) / 2;
	}/* 向底部偏左对齐 */ 
	else if((flag & ALIGN_BOTTOM_LEFT) == ALIGN_BOTTOM_LEFT) {
		pos.y = graph->h - image->h; 
	}/* 向底部居中对齐 */
	else if((flag & ALIGN_BOTTOM_CENTER) == ALIGN_BOTTOM_CENTER) { 
		pos.x = (graph->w - image->w) / 2;
		pos.y = graph->h - image->h;
	}/* 向底部偏右对齐 */
	else if((flag & ALIGN_BOTTOM_RIGHT) == ALIGN_BOTTOM_RIGHT) { 
		pos.x = graph->w - image->w;
		pos.y = graph->h - image->h;
	}
	
	if( Check_Option(flag, GRAPH_MIX_FLAG_OVERLAY) ) {
	/* 如果包含GRAPH_MIX_FLAG_OVERLAY选项 */
		Graph_Mix(graph, image, pos); 
	}
	else if( Check_Option(flag, GRAPH_MIX_FLAG_REPLACE) ) {
	/* 如果包含GRAPH_MIX_FLAG_REPLACE选项 */
		Graph_Replace(graph, image, pos); 
	} else {
		Graph_Mix(graph, image, pos);
	}
	
	return 0;
}


LCUI_API int Graph_FillImage(	LCUI_Graph *graph,
				LCUI_Graph *bg, 
				int mode,
				LCUI_RGB color )
{
	LCUI_Size size;
	LCUI_Pos pos;
	LCUI_Graph temp_bg;
	LCUI_BOOL replace_mix;

	if( Check_Option( mode, GRAPH_MIX_FLAG_REPLACE ) ) {
		/* 将alpha通道置为0 */
		Graph_FillAlpha( graph, 0 );
		replace_mix = TRUE;
	} else {
		/* 填充背景色，将alpha通道置为255 */
		Graph_FillColor( graph, color );
		Graph_FillAlpha( graph, 255 );
		replace_mix = FALSE;
	}
	if(!Graph_IsValid(bg) || !Graph_IsValid(graph)) {
		return -1; 
	}
	size.w = graph->w;
	size.h = graph->h;
	Graph_Init(&temp_bg);
	pos.x = pos.y = 0;
	/* 平铺 */
	if( Check_Option( mode, LAYOUT_TILE ) ) {
		return Graph_Tile( bg, graph, replace_mix );
	}
	/* 缩放 */
	if( Check_Option( mode, LAYOUT_ZOOM ) ) {
		Graph_Zoom( bg, &temp_bg, TRUE, size );
		pos.x = (size.w - temp_bg.w) / 2.0;
		pos.y = (size.h - temp_bg.h) / 2.0;
		bg = &temp_bg;
	}
	/* 拉伸 */
	else if( Check_Option( mode, LAYOUT_STRETCH ) ) {
		Graph_Zoom( bg, &temp_bg, FALSE, size );
		bg = &temp_bg;
	}
	/* 居中 */
	else if( Check_Option( mode, LAYOUT_CENTER ) ) {
		pos.x = (size.w - bg->w) / 2.0;
		pos.y = (size.h - bg->h) / 2.0;
	}
	if( replace_mix ) {
		Graph_Replace( graph, bg, pos );
	} else {
		Graph_Mix( graph, bg, pos );
	}
	Graph_Free( &temp_bg );
	return 0; 
}

LCUI_API int Graph_FillAlpha( LCUI_Graph *src, uchar_t alpha )
{
	int y, row_start;
	LCUI_Rect src_rect;
	
	/* 获取引用的区域在源图形中的有效区域 */
	src_rect = Graph_GetValidRect( src );
	/* 获取引用的源图指针 */
	src = Graph_GetQuote( src );
	
	if(! Graph_IsValid(src) ) {
		return -1;
	}
	if( !Graph_HaveAlpha(src) ) {
		return -2;
	}
	
	row_start = src_rect.x + src_rect.y * src->w;
	for(y=0; y<src_rect.height; ++y) {
		memset( &src->rgba[3][row_start], 
			alpha, src_rect.width*sizeof(uchar_t) );
		row_start += src->w;
	}
	return 0; 
}
