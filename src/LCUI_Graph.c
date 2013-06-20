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
 * 版权所有 (C) 2012-2013 归属于							\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)					\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。		\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定用途	\n
 * 的隐含担保，详情请参照GPLv2许可协议。						\n
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

LCUI_API LCUI_RGB
RGB ( uchar_t red, uchar_t green, uchar_t blue )
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

LCUI_API void
Graph_Lock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Lock( &src->mutex );
}

LCUI_API void
Graph_Unlock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Unlock( &src->mutex );
}

LCUI_API LCUI_BOOL
Graph_GetPixel( LCUI_Graph *graph, LCUI_Pos pos, LCUI_RGBA *pixel )
{
	int i;
	LCUI_Rect rect;
	
	if( pos.x < 0 || pos.y < 0 ) {
		return FALSE;
	}
	rect = Graph_GetValidRect( graph );
	graph = Graph_GetQuote( graph );
	if( !Graph_IsValid(graph) ) {
		return FALSE;
	}
	i = graph->w*(pos.y+rect.y) + pos.x + rect.x;
	pixel->red = graph->rgba[i++];
	pixel->green = graph->rgba[i++];
	pixel->blue = graph->rgba[i++];
	
	if(graph->color_type == COLOR_TYPE_RGBA) {
		pixel->alpha = graph->rgba[i];
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
	printf("alpha:%u", graph->alpha);
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
	if(h < 0 || w < 0) {
		Graph_Free( graph );
		return -1; 
	}
	
	switch( graph->color_type ) {
	case COLOR_TYPE_RGBA:
		size = sizeof(uchar_t)*w*h*4;
		break;
	case COLOR_TYPE_RGB:
	default:
		size = sizeof(uchar_t)*w*h*3;
		break;
	}

	if( Graph_IsValid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if( graph->mem_size >= size ) {
			graph->w = w;
			graph->h = h;
			return 0;
		}
		Graph_Free( graph );
		LCUIMutex_Init( &graph->mutex );
	}
	graph->mem_size = size;
	graph->rgba = (uchar_t*)malloc( size ); 

	if( NULL == graph->rgba ) {
		graph->w = 0;
		graph->h = 0;
		return -1;
	}
	
	graph->w = w;
	graph->h = h;
	return 0;
}

LCUI_API void Graph_Copy( LCUI_Graph *des, LCUI_Graph *src )
{
	if( !des ) {
		return;
	}
	
	if( Graph_IsValid(src) ) { 
		des->color_type = src->color_type;
		/* 创建合适尺寸的Graph */
		Graph_Create( des, src->w, src->h );
		Graph_Replace( des, src, Pos(0,0) );
		des->alpha = src->alpha;
	}
}

LCUI_API void Graph_Free( LCUI_Graph *pic )
{
	LCUI_Graph *p;
	p = Graph_GetQuote(pic);
	if( !Graph_IsValid(p)) {
		return;
	}
	if( pic->quote ) {
		pic->src = NULL; 
		pic->quote = FALSE;
	} else {
		LCUIMutex_Lock( &p->mutex );
		free(p->rgba);
		p->rgba = NULL;
		p->w = 0;
		p->h = 0;
		LCUIMutex_Unlock( &p->mutex );
	}
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

LCUI_API void Graph_Zoom(	LCUI_Graph *in,
				LCUI_Graph *out, 
				LCUI_BOOL keep_scale,
				LCUI_Size size )
{
	LCUI_Graph *src;
	LCUI_Rect rect;
	int src_n, des_n, x, y;
	double scale_x,scale_y;
	
	if(!Graph_IsValid(in)) {
		return; 
	}
	if(size.w <=0 || size.h <= 0) { 
		Graph_Free(out);
		return;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	rect = Graph_GetValidRect(in);
	src = Graph_GetQuote(in);
	
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
	out->color_type = in->color_type;
	if( Graph_Create(out, size.w, size.h) < 0) {
		return;
	}
	switch( in->color_type ) {
	case COLOR_TYPE_RGBA:
		des_n = 0;
		for (y=0; y < size.h; ++y)  {
			src_n = (y*scale_y+rect.y) * src->w + rect.x;
			src_n *= 4;
			for (x = 0; x < size.w; ++x) {
				out->rgba[des_n++] = src->rgba[src_n];
				out->rgba[des_n++] = src->rgba[src_n+1];
				out->rgba[des_n++] = src->rgba[src_n+2];
				out->rgba[des_n++] = src->rgba[src_n+3];
				src_n += (x*scale_x*4);
			}
		}
		break;
	case COLOR_TYPE_RGB:
	default:
		des_n = 0;
		for (y=0; y < size.h; ++y)  {
			src_n = (y*scale_y+rect.y) * src->w + rect.x;
			src_n *= 3;
			for (x = 0; x < size.w; ++x) {
				out->rgba[des_n++] = src->rgba[src_n];
				out->rgba[des_n++] = src->rgba[src_n+1];
				out->rgba[des_n++] = src->rgba[src_n+2];
				src_n += (x*scale_x*3);
			}
		}
		break;
	}
}

LCUI_API int Graph_Cut( LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out )
{
	int x, y;
	int count = 0; 
	int src_n, src_start_n, des_n;
	out->alpha = src->alpha;
	rect = LCUIRect_ValidArea(Size(src->w, src->h), rect); 
	
	if( !Graph_IsValid(src) || rect.width <= 0 || rect.height <= 0) {
		return -1;
	}
	if( Graph_Create( out, rect.width, rect.height ) != 0 ) {
		return -2;
	}
	
	switch( src->color_type ) {
	case COLOR_TYPE_RGBA:
		des_n = 0;
		src_start_n = rect.y * src->w + rect.x;
		for (y=0; y < rect.height; ++y)  {
			src_n = src_start_n * 4;
			for (x = 0; x < rect.width; ++x) {
				out->rgba[des_n++] = src->rgba[src_n++];
				out->rgba[des_n++] = src->rgba[src_n++];
				out->rgba[des_n++] = src->rgba[src_n++];
				out->rgba[des_n++] = src->rgba[src_n++];
			}
			src_start_n += src->w;
		}
		break;
	case COLOR_TYPE_RGB:
	default:
		des_n = 0;
		src_start_n = rect.y * src->w + rect.x;
		for (y=0; y < rect.height; ++y)  {
			src_n = src_start_n * 3;
			for (x = 0; x < rect.width; ++x) {
				out->rgba[des_n++] = src->rgba[src_n++];
				out->rgba[des_n++] = src->rgba[src_n++];
				out->rgba[des_n++] = src->rgba[src_n++];
			}
			src_start_n += src->w;
		}
		break;
	}

	return 0; 
}

LCUI_API int Graph_HorizFlip( LCUI_Graph *src, LCUI_Graph *out )
{
	uchar_t buff;
	LCUI_Rect rect;
	int x, y, center;
	int src_left_pos, src_right_pos;
	int des_left_pos, des_right_pos;  
	
	if(!Graph_IsValid(src)) {
		return -1;
	}
	rect = Graph_GetValidRect( src );
	src = Graph_GetQuote( src );
	out->color_type = src->color_type;
	if( Graph_Create( out, rect.width, rect.height ) != 0 ) {
		return -2;	
	}
	/* 水平翻转其实也就是交换两边的数据 */  
	center = (int)(rect.width / 2.0);
	switch( src->color_type ) {
	case COLOR_TYPE_RGBA:
		for (y = 0; y < rect.height; ++y) {
			des_left_pos = y * rect.width;
			des_right_pos = des_left_pos + rect.width -1;
			src_left_pos = (y + rect.y) * src->w + rect.x;
			src_right_pos = src_left_pos + rect.width -1;
			des_left_pos *= 4;
			des_right_pos *= 4;
			src_left_pos *= 4;
			src_right_pos *= 4;
			for (x = 0; x <= center; ++x)  {
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos];
				out->rgba[des_right_pos] = buff;
				
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos+1];
				out->rgba[des_right_pos+1] = buff;
				
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos+2];
				out->rgba[des_right_pos+2] = buff;
				
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos+3];
				out->rgba[des_right_pos+3] = buff;

				des_right_pos -= 4;
				src_right_pos -= 4;
			}
		}
		break;
	case COLOR_TYPE_RGB:
		for (y = 0; y < rect.height; ++y) {
			des_left_pos = y * rect.width;
			des_right_pos = des_left_pos + rect.width -1;
			src_left_pos = (y + rect.y) * src->w + rect.x;
			src_right_pos = src_left_pos + rect.width -1;
			des_left_pos *= 3;
			des_right_pos *= 3;
			src_left_pos *= 3;
			src_right_pos *= 3;
			for (x = 0; x <= center; ++x)  {
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos];
				out->rgba[des_right_pos] = buff;
				
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos+1];
				out->rgba[des_right_pos+1] = buff;
				
				buff = src->rgba[src_left_pos++];
				out->rgba[des_left_pos++] = src->rgba[src_right_pos+2];
				out->rgba[des_right_pos+2] = buff;
				
				des_right_pos -= 3;
				src_right_pos -= 3;
			}
		}
	default:
		break;
	}
	return 0;
}

LCUI_API int Graph_VertiFlip( LCUI_Graph *src, LCUI_Graph *out )
{
	uchar_t buff;
	LCUI_Rect rect;
	int x, y, center;
	int src_top_pos, src_bottom_pos;
	int des_top_pos, des_bottom_pos;
	int src_start_top_pos, src_start_bottom_pos;
	int des_start_top_pos, des_start_bottom_pos;
	
	if(!Graph_IsValid(src)) {
		return -1;
	}
	rect = Graph_GetValidRect( src );
	src = Graph_GetQuote( src );
	out->color_type = src->color_type;
	if( Graph_Create(out, rect.width, rect.height ) != 0 ) {
		return -2;
	}

	center = (int)(rect.height / 2.0);
	/* 计算基坐标 */
	des_start_top_pos = 0;
	des_start_bottom_pos = (rect.height-1)*rect.width;
	src_start_top_pos = rect.y * src->w + rect.x;
	src_start_bottom_pos = (rect.y + rect.height-1)*src->w + rect.x;
	switch( src->color_type ) {
	case COLOR_TYPE_RGBA:
		for (x=0; x < rect.width; ++x) {
			/* 当前坐标=基坐标+x */
			des_top_pos = des_start_top_pos + x;
			des_bottom_pos = des_start_bottom_pos + x;
			src_top_pos = src_start_top_pos + x;
			src_bottom_pos = src_start_bottom_pos + x;
			des_top_pos *= 4;
			des_bottom_pos *= 4;
			src_top_pos *= 4;
			src_bottom_pos *= 4;
			for (y = 0; y <= center; ++y) {
				buff = src->rgba[src_top_pos];
				out->rgba[des_top_pos] = src->rgba[src_bottom_pos];  
				out->rgba[des_bottom_pos] = buff;
			
				buff = src->rgba[src_top_pos+1];
				out->rgba[des_top_pos+1] = src->rgba[src_bottom_pos+1];
				out->rgba[des_bottom_pos+1] = buff;
			
				buff = src->rgba[src_top_pos+2];
				out->rgba[des_top_pos+2] = src->rgba[src_bottom_pos+2];
				out->rgba[des_bottom_pos+2] = buff;
			
				buff = src->rgba[src_top_pos+3];
				out->rgba[des_top_pos+3] = src->rgba[src_bottom_pos+3];
				out->rgba[des_bottom_pos+3] = buff;
			
				src_top_pos += (src->w*4);
				des_top_pos += (rect.width*4);
				src_bottom_pos -= (src->w*4);
				des_bottom_pos -= (rect.width*4);
			}
		}
		break;
	case COLOR_TYPE_RGB:
	default:
		for (x=0; x < rect.width; ++x) {
			/* 当前坐标=基坐标+x */
			des_top_pos = des_start_top_pos + x;
			des_bottom_pos = des_start_bottom_pos + x;
			src_top_pos = src_start_top_pos + x;
			src_bottom_pos = src_start_bottom_pos + x;
			des_top_pos *= 3;
			des_bottom_pos *= 3;
			src_top_pos *= 3;
			src_bottom_pos *= 3;
			for (y = 0; y <= center; ++y) {
				buff = src->rgba[src_top_pos];
				out->rgba[des_top_pos] = src->rgba[src_bottom_pos];  
				out->rgba[des_bottom_pos] = buff;
			
				buff = src->rgba[src_top_pos+1];
				out->rgba[des_top_pos+1] = src->rgba[src_bottom_pos+1];
				out->rgba[des_bottom_pos+1] = buff;
			
				buff = src->rgba[src_top_pos+2];
				out->rgba[des_top_pos+2] = src->rgba[src_bottom_pos+2];
				out->rgba[des_bottom_pos+2] = buff;

				src_top_pos += (src->w*3);
				des_top_pos += (rect.width*3);
				src_bottom_pos -= (src->w*3);
				des_bottom_pos -= (rect.width*3);
			}
		}
		break;
	}
	return 0;  
}

LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_RGB color, LCUI_Rect rect )
{
	int pos, y, x;
	LCUI_Rect src_rect;
	
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
	/* 根据不同的色彩类型，进行像素颜色填充 */
	switch( graph->color_type ) {
	case COLOR_TYPE_RGBA:
		for(y=0; y<rect.height; ++y) {
			pos = (rect.y+y)*graph->w+rect.x;
			pos *= 4;
			for(x=0; x<rect.width; ++x) {
				graph->rgba[pos++] = color.red;
				graph->rgba[pos++] = color.green;
				graph->rgba[pos++] = color.blue;
				pos++;
			}
		}
		break;
	case COLOR_TYPE_RGB:
		for(y=0; y<rect.height; ++y) {
			pos = (rect.y+y)*graph->w+rect.x;
			pos *= 3;
			for(x=0; x<rect.width; ++x) {
				graph->rgba[pos++] = color.red;
				graph->rgba[pos++] = color.green;
				graph->rgba[pos++] = color.blue;
			}
		}
	default:
		break;
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


static int Graph_RGBAMix( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	int x, y;
	int des_pixel_bytes;
	int n, des_row_size, src_row_size;
	uchar_t *src_alpha_ptr, *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	
	/* 判断坐标是否在背景图的范围内 */
	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	/* 如果前景图尺寸超出背景图的范围，需要更改前景图的像素读取范围 */ 
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	/* 根据左上角点的二维坐标，计算出一维坐标 */
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*4;
	src_row_start = src->rgba + n;
	/* 计算出源图像每行像素占的字节数 */
	src_row_size = 4 * src->w;
	/* 根据目标图像的色彩类型，计算出每个像素和每行像素占的字节数 */
	if( des->color_type == COLOR_TYPE_RGBA ) {
		des_pixel_bytes = 4;
		des_row_size = 4 * des->w;
	} else {
		des_pixel_bytes = 3;
		des_row_size = 3 * des->w;
	}
	/* 计算并保存第一行的首个像素的位置 */
	n = (pos.y + des_rect.y) * des->w + pos.x + des_rect.x;
	n = n * des_pixel_bytes;
	des_row_start = des->rgba + n;
	for(y=0; y<cut.height; ++y) { 
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		src_alpha_ptr = src_row_start + 3;
		for(x=0; x<cut.width; ++x) {
			/* 将R、G、B三个色彩值进行alpha混合 */
			ALPHA_BLEND( *src_ptr++, *des_ptr, *src_alpha_ptr );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+1), *src_alpha_ptr );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+2), *src_alpha_ptr );
			/* 切换到下个像素的数据 */
			src_ptr++;
			des_ptr += des_pixel_bytes;
			/* 切换到下一个像素的alpha值 */
			src_alpha_ptr += 4;
		}
		/* 切换到下一行像素 */
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

static int Graph_RGBAMixWithGlobalAlpha( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	double k;
	int x, y;
	uchar_t alpha;
	int des_pixel_bytes;
	int n, des_row_size, src_row_size;
	uchar_t *src_alpha_ptr, *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*4;
	src_row_start = src->rgba + n;
	src_row_size = 4 * src->w;
	if( des->color_type == COLOR_TYPE_RGBA ) {
		des_pixel_bytes = 4;
		des_row_size = 4 * des->w;
	} else {
		des_pixel_bytes = 3;
		des_row_size = 3 * des->w;
	}

	n = (pos.y + des_rect.y) * des->w + pos.x + des_rect.x;
	n = n * des_pixel_bytes;
	des_row_start = des->rgba + n;

	k = src->alpha / 255.0;

	for(y=0; y<cut.height; ++y) {
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		src_alpha_ptr = src_row_start + 3;
		for(x=0; x<cut.width; ++x) {
			alpha = *src_alpha_ptr * k;
			ALPHA_BLEND( *src_ptr++, *des_ptr, alpha );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+1), alpha );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+2), alpha );
			src_ptr++;
			des_ptr += des_pixel_bytes;
			src_alpha_ptr += 4;
		}
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

static int Graph_RGBMix( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	int y, des_pixel_bytes;
	int n, des_row_size, src_row_size;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*3;
	src_row_start = src->rgba + n;
	src_row_size = 3 * src->w;
	if( des->color_type == COLOR_TYPE_RGBA ) {
		des_pixel_bytes = 4;
		des_row_size = 4 * des->w;
	} else {
		des_pixel_bytes = 3;
		des_row_size = 3 * des->w;
	}

	n = (pos.y + des_rect.y) * des->w + pos.x + des_rect.x;
	n = n * des_pixel_bytes;
	des_row_start = des->rgba + n;

	for(y=0; y<cut.height; ++y) { 
		/* 直接拷贝覆盖每一行像素数据 */
		memcpy( des_row_start, src_row_start, cut.width*des_pixel_bytes );
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}

	return 0;
}

static int Graph_RGBMixWithGlobalAlpha( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	int x, y;
	int des_pixel_bytes;
	int n, des_row_size, src_row_size;
	uchar_t *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*4;
	src_row_start = src->rgba + n;
	src_row_size = 3 * src->w;
	if( des->color_type == COLOR_TYPE_RGBA ) {
		des_pixel_bytes = 4;
		des_row_size = 4 * des->w;
	} else {
		des_pixel_bytes = 3;
		des_row_size = 3 * des->w;
	}
	n = (pos.y + des_rect.y) * des->w + pos.x + des_rect.x;
	n = n * des_pixel_bytes;
	des_row_start = des->rgba + n;
	for(y=0; y<cut.height; ++y) { 
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		for(x=0; x<cut.width; ++x) {
			ALPHA_BLEND( *src_ptr++, *des_ptr, src->alpha );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+1), src->alpha );
			ALPHA_BLEND( *src_ptr++, *(des_ptr+2), src->alpha );
			des_ptr += des_pixel_bytes;
		}
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

LCUI_API int Graph_Mix( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	LCUI_Graph *src, *des;

	/* 预先进行有效性判断 */
	if(!Graph_IsValid(back) || !Graph_IsValid(fore)) {
		return -1;
	}
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	/* 判断引用的源图像的有效性 */
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	}

	/* 如果前景图像有alpha通道 */
	if( src->color_type == COLOR_TYPE_RGBA ) {
		/* 如果全局透明度为255，则说明混合时不需要考虑全局透明度 */
		if( src->alpha == 255 ) {
			return Graph_RGBAMix( back, fore, pos );
		}
		/* 否则，在进行alpha混合时，需要将全局透明度计算在内 */
		return Graph_RGBAMixWithGlobalAlpha( back, fore, pos );
	}
	/* 否则，前景图像没有Alpha通道 */

	if( src->alpha == 255 ) {
		return Graph_RGBMix( back, fore, pos );
	}
	return Graph_RGBMixWithGlobalAlpha( back, fore, pos );
}

static int Graph_RGBAReplaceRGB( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	int x, y;
	int n, des_row_size, src_row_size;
	uchar_t *src_alpha_ptr, *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	
	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*4;
	src_row_start = src->rgba + n;
	src_row_size = 4 * src->w;
	des_row_size = 3 * des->w;
	n = ((pos.y + des_rect.y) * des->w + pos.x + des_rect.x)*3;
	des_row_start = des->rgba + n;
	
	for(y=0; y<cut.height; ++y) { 
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		src_alpha_ptr = src_row_start + 3;
		for(x=0; x<cut.width; ++x) {
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, *src_alpha_ptr );
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, *src_alpha_ptr );
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, *src_alpha_ptr );
			src_ptr++;
			src_alpha_ptr += 4;
		}
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

static int Graph_RGBAReplaceRGBWithGlobalAlpha(	LCUI_Graph *back,
						LCUI_Graph *fore,
						LCUI_Pos pos )
{
	int x, y;
	double k;
	int n, des_row_size, src_row_size;
	uchar_t alpha, *src_alpha_ptr, *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	k = 1.0 / src->alpha;
	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*4;
	src_row_start = src->rgba + n;
	src_row_size = 4 * src->w;
	des_row_size = 3 * des->w;
	n = ((pos.y + des_rect.y) * des->w + pos.x + des_rect.x)*3;
	des_row_start = des->rgba + n;
	
	for(y=0; y<cut.height; ++y) { 
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		src_alpha_ptr = src_row_start + 3;
		for(x=0; x<cut.width; ++x) {
			alpha = src->alpha * k;
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, alpha );
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, alpha );
			*des_ptr++ = _ALPHA_BLEND( *src_ptr++, 255, alpha );
			src_ptr++;
			src_alpha_ptr += 4;
		}
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

static int __Graph_Replace( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	int y, pixel_bytes;
	int n, des_row_size, src_row_size;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);

	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( Graph_HaveAlpha(src) && Graph_HaveAlpha(des) ) {
		pixel_bytes = 4;
	} else {
		pixel_bytes = 3;
	}

	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x);
	n = n * pixel_bytes;
	src_row_start = src->rgba + n;
	src_row_size = pixel_bytes * src->w;
	n = ((pos.y + des_rect.y) * des->w + pos.x + des_rect.x);
	n = n * pixel_bytes;
	des_row_start = des->rgba + n;
	des_row_size = pixel_bytes * des->w;

	for(y=0; y<cut.height; ++y) {
		memcpy( des_row_start, src_row_start, cut.width*pixel_bytes );
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}

	return 0;
}
 
 static int Graph_RGBReplaceRGBA(	LCUI_Graph *back, 
					LCUI_Graph *fore,
					LCUI_Pos pos )
 {
	int x, y;
	int n, des_row_size, src_row_size;
	uchar_t *src_ptr, *des_ptr;
	uchar_t *des_row_start, *src_row_start;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	src_rect = Graph_GetValidRect(fore);
	des_rect = Graph_GetValidRect(back);
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	
	if(pos.x >= des_rect.width || pos.y >= des_rect.height) {
		return -1;
	}
	
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( pos.x, pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		pos.x += cut.x;
		pos.y += cut.y;
	}
	
	n = ((cut.y + src_rect.y) * src->w + cut.x + src_rect.x)*3;
	src_row_start = src->rgba + n;
	src_row_size = 3 * src->w;
	des_row_size = 4 * des->w;
	n = ((pos.y + des_rect.y) * des->w + pos.x + des_rect.x)*4;
	des_row_start = des->rgba + n;
	
	for(y=0; y<cut.height; ++y) { 
		des_ptr = des_row_start;
		src_ptr = src_row_start;
		for(x=0; x<cut.width; ++x) {
			*des_ptr++ = *src_ptr++;
			*des_ptr++ = *src_ptr++;
			*des_ptr++ = *src_ptr++;
			*des_ptr++ = src->alpha;
		}
		des_row_start += des_row_size;
		src_row_start += src_row_size;
	}
	return 0;
}

LCUI_API int Graph_Replace( LCUI_Graph *back, LCUI_Graph *fore, LCUI_Pos pos )
{
	LCUI_Graph *src, *des;

	/* 预先进行有效性判断 */
	if(!Graph_IsValid(back) || !Graph_IsValid(fore)) {
		return -1;
	}
	src = Graph_GetQuote(fore);
	des = Graph_GetQuote(back);
	/* 判断引用的源图像的有效性 */
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	}

	/* 如果前景图像有透明度 */
	if(Graph_HaveAlpha(src)) {
		if( Graph_HaveAlpha(des)) { /* 弱背景图有透明度 */
			return __Graph_Replace( back, fore, pos );
		}
		if( src->alpha == 255 ) {
			return Graph_RGBAReplaceRGB( back, fore, pos );
		}
		return Graph_RGBAReplaceRGBWithGlobalAlpha( back, fore, pos );
	}
	if( Graph_HaveAlpha(des) ) {
		return Graph_RGBReplaceRGBA( back, fore, pos );
	}
	return __Graph_Replace( back, fore, pos );
}

LCUI_API int Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int flag )
{
	LCUI_Pos pos;
	pos = Pos(0,0);
	/* 如果图片无效 */
	if(!Graph_IsValid(graph) || ! Graph_IsValid(image)) {
		return -1; 
	}
		
	if((flag & ALIGN_TOP_LEFT) == ALIGN_TOP_LEFT); /* 向左上角对齐 */
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
	int x, y, n, row_size;
	uchar_t *ptr, *row_start_ptr;
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
	
	row_size = 4 * src_rect.width;
	n = (src_rect.x + src_rect.y * src->w) + 4;
	row_start_ptr = src->rgba + n;
	for(y=0; y<src_rect.height; ++y) {
		ptr = row_start_ptr + 3;
		for(x=0; x<src_rect.width; ++x) {
			*ptr = alpha;
			ptr += 4;
		}
	}
	return 0; 
}
