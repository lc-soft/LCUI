/* ***************************************************************************
 * LCUI_Graph.c -- graphics handle
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Graph.c -- 图形处理
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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
 * ****************************************************************************/ 
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_MEM_H
#include LC_MISC_H 
#include LC_WIDGET_H
#include LC_CURSOR_H

LCUI_API LCUI_RGB
RGB ( uchar_t red, uchar_t green, uchar_t blue )
/* 将三个颜色值转换成LCUI_RGB型数据 */
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

/* 为图形数据设定互斥锁 */
LCUI_API void
Graph_Lock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Lock( &src->mutex );
}

/* 更新图层的属性 */
LCUI_API void
Graph_UpdateAttr( LCUI_Graph *buff )
{
	LCUI_Graph *src;
	LCUI_Rect src_rect;
	int flag;
	int x=0, y=0, pos, start_pos;
	
	src = Graph_GetQuote( buff );
	src_rect = Graph_GetValidRect( buff );
	/* 根据图形数据内容来设定各个标志 */
	if( !Graph_IsValid(src) ){ 
		buff->not_visible = TRUE;
		buff->is_opaque = FALSE;
		return;
	}
	/* 如果该图层引用的源图层有alpha通道 */
	if( src->have_alpha ) {
		start_pos = src_rect.y * src->width + src_rect.x;
		/* 遍历alpha通道中的每个元素的值 */
		for(flag=0,y=0; y<src_rect.height; ++y) { 
			pos = start_pos;
			for(x=0; x<src_rect.width; ++x,++pos) {
				if(src->rgba[3][pos] == 0) {
					/* 全透明 */
					if(flag>0 && flag != 1) {
						break;
					}
					flag = 1;
				} else if(src->rgba[3][pos] == 255) {
					/* 不透明 */
					if(flag>0 && flag != 2) {
						break;
					}
					flag = 2;
				} else {
					/* 有透明效果 */
					if(flag>0 && flag != 3) {
						break;
					}
					flag = 3;
				}
			}
			if( x < src_rect.width ) {
				break;
			}
			start_pos += src->width;
		}
		/* 如果全部遍历完了 */
		if( x >= src_rect.width && y >= src_rect.height ) {
			switch(flag) {
			    case 1:
				buff->not_visible = TRUE;
				buff->is_opaque = FALSE;
				break;
			    case 2:
				buff->not_visible = FALSE;
				buff->is_opaque = TRUE;
				break;
			    default:
				buff->not_visible = FALSE;
				buff->is_opaque = FALSE;
				break;
			}
		} else { /* 否则，中途结束，说明像素点的透明度不全一致 */
			buff->not_visible = FALSE;
			buff->is_opaque = FALSE;
		}
	} else {
		/* 否则，不透明 */
		buff->is_opaque = TRUE;
	}
}

/* 解除互斥锁，以结束图像数据的使用 */
LCUI_API void
Graph_Unlock( LCUI_Graph *graph )
{
	LCUI_Graph *src;
	src = Graph_GetQuote(graph);
	LCUIMutex_Unlock( &src->mutex );
}

/* 获取图像中指定坐标的像素点的颜色 */
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
	i = graph->width*(pos.y+rect.y) + pos.x + rect.x;
	pixel->red = graph->rgba[0][i];
	pixel->green = graph->rgba[1][i];
	pixel->blue = graph->rgba[2][i];
	
	if(Graph_HaveAlpha(graph)) {
		pixel->alpha = graph->rgba[3][i];
	} else {
		pixel->alpha = 255;
	}
	return TRUE;
}

/* 返回图形的类型 */
LCUI_API int
Graph_GetType( LCUI_Graph *pic )
{
	return pic->type;
}

/* 获取图形的尺寸 */
LCUI_API LCUI_Size
Graph_GetSize( LCUI_Graph *pic )
{
	if(NULL == pic) {
		return Size(0,0);
	}
	return Size(pic->width, pic->height);
}

/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回TRUE，否则返回FALSE
 * */
LCUI_API LCUI_BOOL
Graph_IsPNG( LCUI_Graph *pic )
{
	if( Graph_GetType(pic) == TYPE_PNG) {
		return TRUE;
	}
	return FALSE;
}

/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回TRUE，否则返回FALSE
 * */
LCUI_API LCUI_BOOL
Graph_IsBMP( LCUI_Graph *pic )
{
	if( Graph_GetType(pic) == TYPE_BMP) {
		return TRUE;
	}
	return FALSE;
}

/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */
LCUI_API LCUI_BOOL
Graph_IsJPG( LCUI_Graph *pic )
{
	if( Graph_GetType(pic) == TYPE_JPG) {
		return TRUE;
	}
	return FALSE;
}

/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */
LCUI_API LCUI_BOOL
Graph_HaveAlpha( LCUI_Graph *pic )
{
	pic = Graph_GetQuote(pic);
	if( pic->have_alpha ) {
		return 1;
	}
	return 0; 
}

/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */
LCUI_API LCUI_BOOL
Graph_IsValid( LCUI_Graph *pic )
{
	LCUI_Graph *p;
	p = Graph_GetQuote(pic);
	if(p != NULL && p->rgba != NULL
	  && p->height > 0 && p->width > 0 ) {
		return 1; 
	}
	return 0;
}

/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */
LCUI_API int
Graph_IsOpaque( LCUI_Graph *graph )
{
	if( !Graph_HaveAlpha(graph) ) {
		return 1; 
	}
	if( graph->alpha == 0 ) {
		return -1;
	} else if(graph->alpha < 255) {
		return 0;
	}
		
	if( graph->is_opaque ) {
		return 1;
	}
	if( graph->not_visible ) {
		return -1;
	}
	return 0;
}

/* 打印图像的信息 */
LCUI_API void
Graph_PrintInfo( LCUI_Graph *pic )
{
	printf("address:%p\n", pic);
	if( !pic ) {
		return;
	}
	
	printf("width:%d, ", pic->width);
	printf("height:%d, ", pic->height);
	printf("alpha:%u", pic->alpha);
	printf("%s, ", pic->have_alpha ? "have alpha channel":"no alpha channel");
	printf("%s, ", pic->not_visible ? "not visible":"visible");
	printf("%s\n", pic->is_opaque ? "is opaque":"not opaque");
	if( pic->quote ) {
		printf("graph src:");
		Graph_PrintInfo(Graph_GetQuote(pic));
	}
}

/* 初始化图片数据结构体 */
LCUI_API void
Graph_Init( LCUI_Graph *pic )
{
	if( !pic ) {
		return;
	}
	
	pic->quote	= FALSE; 
	pic->src	= NULL;
	pic->have_alpha	= FALSE;
	pic->is_opaque	= FALSE;
	pic->not_visible = FALSE;
	pic->rgba	= NULL;
	pic->alpha	= 255;
	pic->mem_size	= 0;
	pic->pos.x	= 0;
	pic->pos.y	= 0;
	pic->width	= 0;
	pic->height	= 0;
	pic->type	= 0;
	pic->bit_depth	= 8;	/* 位深 */
	LCUIMutex_Init( &pic->mutex );
}

/* 为图形数据申请内存空间，并初始化该内存空间为零 */
static uchar_t** 
New_Graph(int width, int height, int have_alpha)
{
	uint_t size;
	uchar_t** out_buff;
	
	size = sizeof(uchar_t)*width*height;
	if( have_alpha ) { 
		out_buff = (uchar_t**)malloc(sizeof(uchar_t*)*4);
	} else {
		out_buff = (uchar_t**)malloc(sizeof(uchar_t*)*3);
	}
	if(!out_buff) {
		return NULL;
	}
	
	out_buff[0] = (uchar_t*)malloc(size);
	if(!out_buff[0]) {
		free(out_buff);
		return NULL;
	}
	out_buff[1] = (uchar_t*)malloc(size);
	if(!out_buff[1]) {
		free(out_buff);
		free(out_buff[0]);
		return NULL;
	}
	out_buff[2] = (uchar_t*)malloc(size);
	if(!out_buff[2]) {
		free(out_buff);
		free(out_buff[0]);
		free(out_buff[1]);
		return NULL;
	}
	 
	if( have_alpha ) {
		out_buff[3] = (uchar_t*)calloc(1, size);
		if(!out_buff[3]) {
			free(out_buff);
			free(out_buff[0]);
			free(out_buff[1]);
			free(out_buff[2]);
			return NULL;
		}
	}
	
	return out_buff;
}

/* 为图像数据分配内存资源 */
LCUI_API int
Graph_Create( LCUI_Graph *graph, int width, int height )
{
	if(width > 10000 || height > 10000) {
		printf("error: can not allocate too much memory!\n");
		abort();
	}
	if(height < 0 || width < 0) {
		Graph_Free( graph );
		return -1; 
	}
	
	if( Graph_IsValid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if(graph->mem_size >= (size_t)width*height) {
			graph->width  = width;
			graph->height = height;
			return 0;
		}
		Graph_Free( graph ); 
	}
	graph->mem_size = (size_t)width*height;
	graph->rgba = New_Graph( width, height, graph->have_alpha ); 
	if(NULL == graph->rgba) {
		graph->width  = 0;
		graph->height = 0;
		return -1;
	}
	
	graph->width  = width;
	graph->height = height;
	return 0;
}

/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */
LCUI_API void
Graph_Copy( LCUI_Graph *des, LCUI_Graph *src )
{
	if( !des ) {
		return;
	}
	
	if( Graph_IsValid(src) ) { 
		if( Graph_HaveAlpha(src) ) {
			des->have_alpha = TRUE;
		} else {
			des->have_alpha = FALSE;
		}
		/* 创建合适尺寸的Graph */
		Graph_Create(des, src->width, src->height);
		Graph_Replace( des, src, Pos(0,0) );
		des->type = src->type;       /* 存储图片类型 */
		des->bit_depth = src->bit_depth;  /* 位深 */
		des->alpha = src->alpha;      /* 全局透明度 */
	}
}

/* 释放LCUI_Graph内的图像数据占用的内存资源 */
LCUI_API void
Graph_Free( LCUI_Graph *pic )
{
	LCUI_Graph *p;
	p = Graph_GetQuote(pic);
	if(Graph_IsValid(p)) {
		if( pic->quote ) {
			pic->src = NULL; 
			pic->quote = FALSE;
		} else {
			Graph_Lock( p );
			free(p->rgba[0]);
			free(p->rgba[1]);
			free(p->rgba[2]);
			if(p->have_alpha) {
				free(p->rgba[3]);
			}
			free(p->rgba);
			p->rgba = NULL;
			p->width = 0;
			p->height = 0;
			Graph_Unlock( p );
		}
		LCUIMutex_Destroy( &pic->mutex );
	}
}

/************************ Graph Quote *********************************/
/* 
 * 功能：引用另一个图层中指定区域里的图形 
 * 说明：src是被引用的对象，des是引用者，area是引用的src中的图形所在的区域
 * */
LCUI_API int
Graph_Quote( LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area )
{
	if( !src || !des ) {
		return -1;
	}
	//printf("Graph_Quote(), before, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	area = LCUIRect_ValidArea(Size(src->width, src->height), area);
	//printf("Graph_Quote(), after, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	if(!LCUIRect_IsValid( area )) { 
		des->src = NULL;
		des->pos.x = 0;
		des->pos.y = 0;
		des->width = 0;
		des->height= 0;
		des->alpha = 255;
		des->quote = FALSE;
		return -1;
	} 
	des->src = src;
	des->pos.x = area.x;
	des->pos.y = area.y;
	des->width = area.width;
	des->height= area.height;
	des->alpha = 255;
	des->quote = TRUE;
	return 0;
}

/* 获取被引用的图形所在的有效区域 */
LCUI_API LCUI_Rect
Graph_GetValidRect( LCUI_Graph *graph )
{
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect rect, cut_rect;
	
	pos = graph->pos; 
	cut_rect.x = pos.x;
	cut_rect.y = pos.y;
	cut_rect.width = graph->width;
	cut_rect.height = graph->height; 
	
	if( !graph->quote ) {
		return cut_rect; 
	} else {
		w = graph->src->width;
		h = graph->src->height;
	} 
	/* 获取需裁剪的区域 */
	if(pos.x < 0) {
		cut_rect.width += pos.x;
		cut_rect.x = 0 - pos.x; 
	}
	if(pos.x + graph->width > w) {
		cut_rect.width -= (pos.x +  graph->width - w); 
	}
	
	if(pos.y < 0) {
		cut_rect.height += pos.y;
		cut_rect.y = 0 - pos.y; 
	}
	if(pos.y + graph->height > h) {
		cut_rect.height -= (pos.y +  graph->height - h); 
	}
	
	/* 获取引用的图像的有效显示范围 */
	rect = Graph_GetValidRect(graph->src);
	/* 如果引用的图像需要裁剪，那么，该图像根据情况，也需要进行裁剪 */
	if(rect.x > 0) { 
		temp = pos.x + cut_rect.x;
		if(temp < rect.x) { 
			temp = rect.x - pos.x; 
			cut_rect.width -= (temp - cut_rect.x); 
			cut_rect.x = temp; 
		}
	}
	if(rect.y > 0) {
		temp = pos.y + cut_rect.y;
		if(pos.y < rect.y) {
			temp = rect.y - pos.y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.width < w) { 
		temp = pos.x+cut_rect.x+cut_rect.width;
		if(temp > rect.x+rect.width) {
			cut_rect.width -= (temp-(rect.x+rect.width));
		}
	}
	if(rect.height < h) {
		temp = pos.y+cut_rect.y+cut_rect.height;
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
LCUI_API LCUI_Graph*
Graph_GetQuote( LCUI_Graph *graph )
{
	if( !graph ) {
		return NULL;
	}
	if( !graph->quote ) {
		return graph; 
	}
	return Graph_GetQuote(graph->src);
}

/************************ End Graph Quote *****************************/

/* 根据指定模式，对图像进行缩放 */
LCUI_API void
Graph_Zoom(	LCUI_Graph *in,		LCUI_Graph *out, 
		LCUI_BOOL keep_scale,	LCUI_Size size )
{
	LCUI_Graph *src;
	LCUI_Rect rect;
	LCUI_Pos pos; 
	int count, temp, x, y, k, m;
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
	out->have_alpha = in->have_alpha;
	if( Graph_Create(out, size.w, size.h) < 0) {
		return;
	}
	for (y=0; y < size.h; ++y)  {
		pos.y = y*scale_y;
		k = y*size.w;
		m = (pos.y+rect.y)*src->width+rect.x;
		for (x = 0; x < size.w; ++x) {
			pos.x = x*scale_x; 
			temp  = k + x;
			count = m + pos.x;
			out->rgba[0][temp] = src->rgba[0][count];
			out->rgba[1][temp] = src->rgba[1][count];
			out->rgba[2][temp] = src->rgba[2][count];
			if(in->have_alpha) {
				out->rgba[3][temp] = src->rgba[3][count];
			}
		}
	} 
}

/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */
LCUI_API int
Graph_Cut( LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out )
{
	int x, y;
	int temp,count = 0, k; 
	
	out->have_alpha = src->have_alpha;
	out->alpha = src->alpha;
	rect = LCUIRect_ValidArea(Size(src->width, src->height), rect); 
	
	if(Graph_IsValid(src) && rect.width > 0 && rect.height > 0);
	else {
		return -1;
	}
	
	temp = Graph_Create(out, rect.width, rect.height);
	if(temp != 0) {
		return -1; 
	}
	
	/* 开始读取图片中的图形数组并写入窗口 */ 
	for (y=0;y< rect.height; ++y) {
		k = (rect.y+y)*src->width + rect.x;
		for (x=0;x<rect.width; ++x) {
			temp = k + x; /* 计算图片内需要读取的区域的各点坐标 */
			out->rgba[0][count] = src->rgba[0][temp];
			out->rgba[1][count] = src->rgba[1][temp];
			out->rgba[2][count] = src->rgba[2][temp];
			if(Graph_HaveAlpha(src))
				out->rgba[3][count] = src->rgba[3][temp];
			++count;
		}
	}
	return 0; 
}

/* 将图像进行水平翻转 */  
LCUI_API int
Graph_HorizFlip( LCUI_Graph *img, LCUI_Graph *out )
{
	int x, y, temp; 
	int src_left_pos, src_right_pos;
	int des_left_pos, des_right_pos;
	int width, height;  
	uchar_t buff;  
	LCUI_Graph *src;
	LCUI_Rect rect;
	
	if(!Graph_IsValid(img)) {
		return -1;
	}
	src = Graph_GetQuote( img );
	rect = Graph_GetValidRect( img );
	width = img->width;
	height = img->height;
	out->have_alpha = src->have_alpha;
	Graph_Create(out, width, height);
	/* 水平翻转其实也就是交换两边的数据 */  
	temp = (int)(width / 2.0);
	for (y = 0; y < height; ++y) {
		des_left_pos = y * width;
		des_right_pos = des_left_pos + width -1;
		src_left_pos = (y + rect.y) * src->width + rect.x;
		src_right_pos = src_left_pos + width -1;
		for (x = 0; x <= temp; ++x)  {
			buff = src->rgba[0][src_left_pos]; 
			out->rgba[0][des_left_pos] = src->rgba[0][src_right_pos];  
			out->rgba[0][des_right_pos] = buff;
			 
			buff = src->rgba[1][src_left_pos]; 
			out->rgba[1][des_left_pos] = src->rgba[1][src_right_pos];  
			out->rgba[1][des_right_pos] = buff;
			 
			buff = src->rgba[2][src_left_pos]; 
			out->rgba[2][des_left_pos] = src->rgba[2][src_right_pos];  
			out->rgba[2][des_right_pos] = buff;
			 
			if(src->have_alpha) {
				buff = src->rgba[3][src_left_pos]; 
				out->rgba[3][des_left_pos] = src->rgba[3][src_right_pos];  
				out->rgba[3][des_right_pos] = buff;
			}
			++src_left_pos;
			++des_left_pos;
			--src_right_pos;
			--des_right_pos;
		}
	} 
	return 0;  
}

/* 将图像进行垂直翻转 */  
LCUI_API int
Graph_VertiFlip( LCUI_Graph *img, LCUI_Graph *out )
{
	int x, y, temp; 
	int src_top_pos, src_bottom_pos;
	int des_top_pos, des_bottom_pos;
	int src_start_top_pos, src_start_bottom_pos;
	int des_start_top_pos, des_start_bottom_pos;
	int width, height;  
	uchar_t buff;  
	LCUI_Graph *src;
	LCUI_Rect rect;
	
	if(!Graph_IsValid(img)) {
		return -1;
	}
	src = Graph_GetQuote( img );
	rect = Graph_GetValidRect( img );
	width = img->width;
	height = img->height;
	out->have_alpha = src->have_alpha;
	Graph_Create(out, width, height); 

	temp = (int)(height / 2.0);
	/* 记录基坐标 */
	des_start_top_pos = 0;
	des_start_bottom_pos = (height-1)*width;
	src_start_top_pos = rect.y * src->width + rect.x;
	src_start_bottom_pos = (rect.y + height-1)*src->width + rect.x;
	
	for (x=0; x < width; ++x) {
		/* 当前坐标=基坐标+x */
		des_top_pos = des_start_top_pos + x;
		des_bottom_pos = des_start_bottom_pos + x;
		src_top_pos = src_start_top_pos + x;
		src_bottom_pos = src_start_bottom_pos + x;
		for (y = 0; y <= temp; ++y) {
			buff = src->rgba[0][src_top_pos]; 
			out->rgba[0][des_top_pos] = src->rgba[0][src_bottom_pos];  
			out->rgba[0][des_bottom_pos] = buff;
			
			buff = src->rgba[1][src_top_pos]; 
			out->rgba[1][des_top_pos] = src->rgba[1][src_bottom_pos];  
			out->rgba[1][des_bottom_pos] = buff;
			
			buff = src->rgba[2][src_top_pos]; 
			out->rgba[2][des_top_pos] = src->rgba[2][src_bottom_pos];  
			out->rgba[2][des_bottom_pos] = buff;
			
			if(src->have_alpha) {
				buff = src->rgba[3][src_top_pos]; 
				out->rgba[3][des_top_pos] = src->rgba[3][src_bottom_pos];  
				out->rgba[3][des_bottom_pos] = buff;
			}
			src_top_pos += src->width;
			des_top_pos += width;
			src_bottom_pos -= src->width;
			des_bottom_pos -= width;
		}
	}
	return 0;  
}

/* 填充矩形 */
LCUI_API int
Graph_FillRect( LCUI_Graph *graph, LCUI_RGB color, LCUI_Rect rect )
{
	int pos, i, size;
	LCUI_Rect src_rect;
	uchar_t *r_ptr, *g_ptr, *b_ptr;
	
	src_rect = Graph_GetValidRect( graph ); 
	graph = Graph_GetQuote( graph );
	
	if(! Graph_IsValid(graph) ) {
		return -1;
	}
	rect.x = rect.x + src_rect.x;
	rect.y = rect.y + src_rect.y;

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
	pos = rect.x + rect.y * graph->width;
	r_ptr = graph->rgba[0] + pos;
	g_ptr = graph->rgba[1] + pos;
	b_ptr = graph->rgba[2] + pos;
	for(i=0; i<rect.height; ++i) { 
		memset(r_ptr, color.red, size);
		memset(g_ptr, color.green, size);
		memset(b_ptr, color.blue, size);
		r_ptr += graph->width;
		g_ptr += graph->width;
		b_ptr += graph->width;
	}
	return 0; 
}

LCUI_API int
Graph_FillColor( LCUI_Graph *graph, LCUI_RGB color )
/* 填充颜色 */
{
	int i, pos;
	size_t size;
	LCUI_Rect src_rect;
	uchar_t *r_ptr, *g_ptr, *b_ptr;
	
	src_rect = Graph_GetValidRect( graph ); 
	graph = Graph_GetQuote( graph );
	
	if(! Graph_IsValid(graph) ) {
		return -1;
	} 
	
	size = sizeof(uchar_t) * src_rect.width;
	pos = src_rect.x + src_rect.y * graph->width;
	r_ptr = graph->rgba[0] + pos;
	g_ptr = graph->rgba[1] + pos;
	b_ptr = graph->rgba[2] + pos;
	for(i=0; i<src_rect.height; ++i){ 
		memset(r_ptr, color.red, size);
		memset(g_ptr, color.green, size);
		memset(b_ptr, color.blue, size);
		r_ptr += graph->width;
		g_ptr += graph->width;
		b_ptr += graph->width;
	}
	return 0; 
}

/* 平铺图形 */
LCUI_API int
Graph_Tile( LCUI_Graph *src, LCUI_Graph *des_buff, LCUI_BOOL replace )
{
	int ret = 0;
	LCUI_Pos pos;
	
	if(!Graph_IsValid(src) || !Graph_IsValid(des_buff)) {
		return -1;
	}
	for(pos.y=0; pos.y<des_buff->height; pos.y+=src->height) {
		for(pos.x=0; pos.x<des_buff->width; pos.x+=src->width) {
			if( replace ) {
				ret += Graph_Replace( des_buff, src, pos );
			} else {
				ret += Graph_Mix( des_buff, src, pos );
			}
		}
	}
	return ret;
}

/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */
LCUI_API int
Graph_Mix( LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos )
{
	uchar_t j;
	uchar_t *r1, *g1, *a1, *b1, *r2, *g2, *b2; 

	float k;
	int total, y = 0, m, n, tmp_m, tmp_n;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;

	if(!Graph_IsValid(back_graph) || !Graph_IsValid(fore_graph)) {
		return -1;
	}
	if(fore_graph->width < 0 || fore_graph->height < 0 
	|| back_graph->width < 0 || back_graph->height < 0 ) {
		return -1;
	}
	src_rect = Graph_GetValidRect(fore_graph);
	des_rect = Graph_GetValidRect(back_graph);
	src = Graph_GetQuote(fore_graph);
	des = Graph_GetQuote(back_graph); 
	
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	}
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -1;
	}
	
	/* 如果前景图尺寸超出背景图的范围，需要更改前景图的像素读取范围 */ 
	if( LCUIRect_GetCutArea(
		Size( des_rect.width, des_rect.height),
		Rect( des_pos.x, des_pos.y, 
			src_rect.width, src_rect.height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	/* 根据左上角点的二维坐标，计算出一维坐标 */
	m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
	n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
	/* 如果前景图形有alpha通道 */
	if(Graph_HaveAlpha(src)) {
		k = src->alpha / 255.0;
		if( src->alpha < 255 ) {
			for (y = 0; y < cut.height; ++y) {
				tmp_n = n; tmp_m = m;
				total = tmp_n + cut.width; 
				for (; tmp_n < total; ++tmp_n,++tmp_m) { 
					j = src->rgba[3][tmp_m] * k;
					ALPHA_BLEND( src->rgba[0][tmp_m], des->rgba[0][tmp_n], j );
					ALPHA_BLEND( src->rgba[1][tmp_m], des->rgba[1][tmp_n], j );
					ALPHA_BLEND( src->rgba[2][tmp_m], des->rgba[2][tmp_n], j );
				} 
				m += src->width;
				n += des->width; 
			}
			return 0;
		}
		for (y = 0; y < cut.height; ++y) { 
			/* 保存这一行开头的像素点的位置 */
			tmp_n = n; tmp_m = m;
			total = tmp_n + cut.width;
			/* 根据alpha通道来混合像素点 */
			for (; tmp_n < total; ++tmp_n,++tmp_m) { 
				ALPHA_BLEND( src->rgba[0][tmp_m], des->rgba[0][tmp_n], src->rgba[3][tmp_m] );
				ALPHA_BLEND( src->rgba[1][tmp_m], des->rgba[1][tmp_n], src->rgba[3][tmp_m] );
				ALPHA_BLEND( src->rgba[2][tmp_m], des->rgba[2][tmp_n], src->rgba[3][tmp_m] );
			}
			/* 切换到下一行像素点 */
			m += src->width;
			n += des->width; 
		}
		return 0;
	}
	/* 如果全局透明度为255，说明前景图形没有透明效果 */
	if( fore_graph->alpha == 255 ) { 
		for (y=0; y < cut.height; ++y) {
			/* 使用指针来引用 */
			r1 = des->rgba[0] + n;
			g1 = des->rgba[1] + n;
			b1 = des->rgba[2] + n;
			a1 = des->rgba[3] + n;
			
			r2 = src->rgba[0] + m;
			g2 = src->rgba[1] + m;
			b2 = src->rgba[2] + m;
			/* 拷贝 */
			memcpy(r1, r2, cut.width);
			memcpy(g1, g2, cut.width);
			memcpy(b1, b2, cut.width);
			/* 透明度取决于前景图的透明度 */
			if( des->have_alpha ) {
				memset(a1, src->alpha, sizeof(uchar_t)*cut.width);
			}
			m += src->width;
			n += des->width; 
		}
		return 0;
	}
	/* 全局透明度低于255，那么就用全局透明度进行混合 */
	for (y = 0; y < cut.height; ++y) {
		tmp_n = n; tmp_m = m;
		total = tmp_n + cut.width; 
		for (; tmp_n < total; ++tmp_n,++tmp_m) { 
			ALPHA_BLEND( src->rgba[0][tmp_m], des->rgba[0][tmp_n], fore_graph->alpha );
			ALPHA_BLEND( src->rgba[1][tmp_m], des->rgba[1][tmp_n], fore_graph->alpha );
			ALPHA_BLEND( src->rgba[2][tmp_m], des->rgba[2][tmp_n], fore_graph->alpha );
		}
		m += src->width;
		n += des->width; 
	}
	return 0; 
}

/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */
LCUI_API int
Graph_Replace( LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos )
{
	uchar_t *r1, *g1, *a1, *a2, *b1, *r2, *g2, *b2;

	int y,total, m, n, tmp_m, tmp_n;
	uchar_t j, k; 
	
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;
	
	src_rect = Graph_GetValidRect(fore_graph);
	des_rect = Graph_GetValidRect(back_graph); 
	src = Graph_GetQuote(fore_graph);
	des = Graph_GetQuote(back_graph);
	
	if(!Graph_IsValid(des) || !Graph_IsValid(src)) {
		return -1;
	}
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -1;
	} 
	 
	if(fore_graph->width < 0 || fore_graph->height < 0 
	|| back_graph->width < 0 || back_graph->height < 0 ) {
		return -1;
	}
	if( LCUIRect_GetCutArea(
		Size( back_graph->width, back_graph->height),
		Rect( des_pos.x, des_pos.y, 
			fore_graph->width, fore_graph->height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	k = src->alpha / 255.0;
	m = (cut.y + src_rect.y) *src->width + cut.x + src_rect.x;
	n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
	/* 如果前景图像有透明度，且背景图没有透明度 */
	if(Graph_HaveAlpha(src) && !Graph_HaveAlpha(des)) { 
		for (y = 0; y < cut.height; ++y) {
			tmp_n = n; tmp_m = m;
			total = tmp_n + cut.width; 
			for (; tmp_n < total; ++tmp_n,++tmp_m) { 
				j = src->rgba[3][tmp_m] * k;
				ALPHA_BLEND( 255, src->rgba[0][tmp_m], j );
				ALPHA_BLEND( 255, src->rgba[1][tmp_m], j );
				ALPHA_BLEND( 255, src->rgba[2][tmp_m], j );
			} 
			m += src->width;
			n += des->width; 
		}
		return 0;
	}
	for (y = 0; y < cut.height; ++y) {
		/* 使用指针来引用 */
		r1 = des->rgba[0] + n;
		g1 = des->rgba[1] + n;
		b1 = des->rgba[2] + n;
			
		r2 = src->rgba[0] + m;
		g2 = src->rgba[1] + m;
		b2 = src->rgba[2] + m; 
		/* 拷贝 */
		memcpy(r1, r2, cut.width);
		memcpy(g1, g2, cut.width);
		memcpy(b1, b2, cut.width);
		if(des->have_alpha) {
			a1 = des->rgba[3] + n;
			if(!src->have_alpha) {
				memset(a1, src->alpha, cut.width);
			} else {
				a2 = src->rgba[3] + m; 
				memcpy(a1, a2, cut.width);  
			}
		}
		m += src->width;
		n += des->width; 
	}
	return 0; 
} 

/* 将图片以指定对齐方式粘贴至背景图上 */
LCUI_API int
Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int flag )
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
		pos.x = (graph->width - image->width) / 2;  
	}/* 向右上角对齐 */
	else if((flag & ALIGN_TOP_RIGHT) == ALIGN_TOP_RIGHT) {
		pos.x = graph->width - image->width; 
	}/* 向中央偏左对齐 */ 
	else if((flag & ALIGN_MIDDLE_LEFT) == ALIGN_MIDDLE_LEFT) {
		pos.y = (graph->height - image->height) / 2; 
	}/* 向正中央对齐 */
	else if((flag & ALIGN_MIDDLE_CENTER) == ALIGN_MIDDLE_CENTER) { 
		pos.x = (graph->width - image->width) / 2;
		pos.y = (graph->height - image->height) / 2;
	}/* 向中央偏右对齐 */
	else if((flag & ALIGN_MIDDLE_RIGHT) == ALIGN_MIDDLE_RIGHT) { 
		pos.x = graph->width - image->width;
		pos.y = (graph->height - image->height) / 2;
	}/* 向底部偏左对齐 */ 
	else if((flag & ALIGN_BOTTOM_LEFT) == ALIGN_BOTTOM_LEFT) {
		pos.y = graph->height - image->height; 
	}/* 向底部居中对齐 */
	else if((flag & ALIGN_BOTTOM_CENTER) == ALIGN_BOTTOM_CENTER) { 
		pos.x = (graph->width - image->width) / 2;
		pos.y = graph->height - image->height;
	}/* 向底部偏右对齐 */
	else if((flag & ALIGN_BOTTOM_RIGHT) == ALIGN_BOTTOM_RIGHT) { 
		pos.x = graph->width - image->width;
		pos.y = graph->height - image->height;
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

/* 为指定图形填充背景图像 */
LCUI_API int
Graph_FillImage(	LCUI_Graph *graph,	LCUI_Graph *bg, 
			int mode,		LCUI_RGB color )
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
	size.w = graph->width;
	size.h = graph->height;
	Graph_Init(&temp_bg);
	pos = Pos(0,0);
	/* 平铺 */
	if( Check_Option( mode, LAYOUT_TILE ) ) {
		return Graph_Tile( bg, graph, replace_mix );
	}
	/* 缩放 */
	if( Check_Option( mode, LAYOUT_ZOOM ) ) {
		Graph_Zoom( bg, &temp_bg, TRUE, size );
		pos.x = (size.w - temp_bg.width) / 2.0;
		pos.y = (size.h - temp_bg.height) / 2.0;
		bg = &temp_bg;
	}
	/* 拉伸 */
	else if( Check_Option( mode, LAYOUT_STRETCH ) ) {
		Graph_Zoom( bg, &temp_bg, FALSE, size );
		bg = &temp_bg;
	}
	/* 居中 */
	else if( Check_Option( mode, LAYOUT_CENTER ) ) {
		pos.x = (size.w - bg->width) / 2.0;
		pos.y = (size.h - bg->height) / 2.0;
	}
	if( replace_mix ) {
		Graph_Replace( graph, bg, pos );
	} else {
		Graph_Mix( graph, bg, pos );
	}
	Graph_Free( &temp_bg );
	return 0; 
}

/* 填充图形的alpha通道的所有值 */
LCUI_API int
Graph_FillAlpha( LCUI_Graph *src, uchar_t alpha )
{
	int i;
	size_t size;
	uchar_t *ptr;
	LCUI_Rect src_rect;
	
	/* 获取引用的区域在源图形中的有效区域 */
	src_rect = Graph_GetValidRect( src );
	/* 获取引用的源图指针 */
	src = Graph_GetQuote( src );
	
	if(! Graph_IsValid(src) ) {
		return -1;
	}
	if( !Graph_HaveAlpha(src) ) {
		return -1;
	}
	
	size = sizeof(uchar_t) * src_rect.width;
	ptr = src->rgba[3] + src_rect.x + src_rect.y * src->width;
	for(i=0; i<src_rect.height; ++i) {
		memset(ptr, alpha, size);
		ptr += src->width;
	}
	return 0; 
}
