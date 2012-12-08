/* ***************************************************************************
 * LCUI_Graphics.c -- graphics handle
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Graphics.c -- 图形处理
 *
 * 版权所有 (C) 2012 归属于 
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

LCUI_RGB RGB ( uchar_t red, uchar_t green, uchar_t blue )
/* 将三个颜色值转换成LCUI_RGB型数据 */
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

void Graph_Lock(LCUI_Graph *pic, int mode)
/* 功能：以指定模式使用图像数据
 * 说明：参数mode的值为0时，以“读”模式使用数据，其它值时，以“写模式使用数据” */
{
	LCUI_Graph *src;
	src = Get_Quote_Graph(pic);
	
	if(mode == 0) {
		thread_rwlock_rdlock(&src->lock);
	} else {
		thread_rwlock_wrlock(&src->lock);
	}
}

void Graph_Unlock(LCUI_Graph *pic)
/* 功能：结束图像数据的使用 */
{
	rwlock_status status;
	LCUI_Graph *src;
	src = Get_Quote_Graph(pic);
	status = thread_rwlock_get_status(&src->lock);
	/* 如果当前为写锁，那就说明这个图形数据被修改了 */
	if(status == RWLOCK_READ) {
		goto skip_graph_data_update;
	}
	/* 根据图形数据内容来设定各个标志 */
	if(Graph_Valid(pic)){ 
		if( Graph_Have_Alpha(pic) ) {
			int flag;
			unsigned int i, total;
			total = pic->width*pic->height;
			/* 遍历alpha通道中的每个元素的值 */
			for(flag=0,i=0; i<total; ++i) { 
				if(src->rgba[3][i] == 0) {
					if(i>0 && flag != -1) {
						break;
					}
					flag = -1;
				} else if(src->rgba[3][i] == 255) {
					if(i>0 && flag != 0) {
						break;
					}
					flag = 0;
				} else {
					if(i>0 && flag != 1) {
						break;
					}
					flag = 1;
				}
			}
			if(i == total) {
				switch(flag) {
				    case -1:
					pic->not_visible = TRUE;
					pic->is_opaque = FALSE;
					break;
				    case 1:
					pic->not_visible = FALSE;
					pic->is_opaque = FALSE;
					break;
				    default:
					pic->not_visible = FALSE;
					pic->is_opaque = TRUE;
					break;
				}
			} else {
				pic->not_visible = FALSE;
				pic->is_opaque = FALSE;
			}
		} else {
			pic->is_opaque = TRUE;
		}
	} else {
		pic->not_visible = TRUE;
		pic->is_opaque = FALSE;
	} 
skip_graph_data_update:;
	thread_rwlock_unlock(&src->lock);
}


LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore)
/* 功能：混合两个像素点的颜色 */
{
	if(fore.alpha == 255) {
		back.red	= fore.red;
		back.blue	= fore.blue;
		back.green	= fore.green;
	}
	else if(fore.alpha == 0);
	else {
		back.red = ALPHA_BLENDING(fore.red, back.red, fore.alpha); 
		back.green = ALPHA_BLENDING(fore.red, back.green, fore.alpha); 
		back.red = ALPHA_BLENDING(fore.red, back.blue, fore.alpha); 
	}
	
	return back;
}

BOOL Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos, LCUI_RGBA *pixel)
/* 功能：获取图像中指定坐标的像素点的颜色 */
{
	int i;
	LCUI_Rect rect;
	
	if( pos.x < 0 || pos.y < 0 ) {
		return FALSE;
	}
	rect = Get_Graph_Valid_Rect( graph );
	graph = Get_Quote_Graph( graph );
	if( !Graph_Valid(graph) ) {
		return FALSE;
	}
	i = graph->width*(pos.y+rect.y) + pos.x + rect.x;
	pixel->red = graph->rgba[0][i];
	pixel->green = graph->rgba[1][i];
	pixel->blue = graph->rgba[2][i];
	
	if(Graph_Have_Alpha(graph)) {
		pixel->alpha = graph->rgba[3][i];
	} else {
		pixel->alpha = 255;
	}
	return TRUE;
}

int Get_Graph_Type(LCUI_Graph *pic)
/* 返回图形的类型 */
{
	return pic->type;
}

LCUI_Size Get_Graph_Size(LCUI_Graph *pic)
/* 功能：获取图形的尺寸 */
{
	if(NULL == pic) {
		return Size(0,0);
	}
	return Size(pic->width, pic->height);
}

BOOL Graph_Is_PNG(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_PNG) {
		return TRUE;
	}
	return FALSE;
}

BOOL Graph_Is_BMP(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_BMP) {
		return TRUE;
	}
	return FALSE;
}

BOOL Graph_Is_JPG(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_JPG) {
		return TRUE;
	}
	return FALSE;
}

BOOL Graph_Have_Alpha(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */
{
	pic = Get_Quote_Graph(pic);
	if( pic->have_alpha ) {
		return 1;
	}
	return 0; 
}

BOOL Graph_Valid(LCUI_Graph *pic)
/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */
{
	LCUI_Graph *p;
	p = Get_Quote_Graph(pic);
	if(p != NULL && p->rgba != NULL
	  && p->height > 0 && p->width > 0 ) {
		return 1; 
	}
	return 0;
}

int Graph_Is_Opaque(LCUI_Graph *graph)
/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */
{
	if( !Graph_Have_Alpha(graph) ) {
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

void Print_Graph_Info(LCUI_Graph *pic)
/* 功能：打印图像的信息 */
{
	printf("address:%p\n",pic);
	if( pic ) {
		return;
	}
	
	printf("width:%d, height:%d, alpha:%u, %s, %s, %s\n", 
	pic->width, pic->height, pic->alpha, 
	pic->have_alpha ? "have alpha channel":"no alpha channel",
	pic->not_visible ? "not visible":"visible",
	pic->is_opaque ? "is opaque":"not opaque");
	if( pic->quote ) {
		printf("graph src:");
		Print_Graph_Info(Get_Quote_Graph(pic));
	}
}

void Graph_Init(LCUI_Graph *pic)
/* 初始化图片数据结构体 */
{
	if( !pic ) {
		return;
	}
	pic->r_count = pic->w_count = 0;
	pic->quote	= FALSE; 
	pic->have_alpha	= FALSE;
	pic->is_opaque	= FALSE;
	pic->not_visible = FALSE;
	pic->rgba	= NULL;
	pic->alpha	= 255;
	pic->pos	= Pos(0, 0);
	pic->width	= 0;
	pic->height	= 0;
	pic->type	= DEFAULT;
	pic->bit_depth	= 8;	/* 位深 */
	thread_rwlock_init(&pic->lock);	/* 读/写/互斥锁 */
}

static uchar_t** 
New_Graph(int width, int height, int have_alpha)
/* 功能：为图形数据申请内存空间，并初始化该内存空间为零 */
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

int Graph_Create(LCUI_Graph *graph, int width, int height)
/* 功能：为图像数据分配内存资源 */
{
	if(width > 10000 || height > 10000) {
		printf("error: can not allocate too much memory!\n");
		abort();
	}
	if(height < 0 || width < 0) {
		Graph_Free( graph );
		return -1; 
	}
	
	if( Graph_Valid(graph) ) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if(graph->width*graph->height > width*height) {
			graph->width  = width;
			graph->height = height;
			return 1;
		}
		Graph_Free( graph ); 
	}
	
	Graph_Lock(graph, 1); 
	graph->rgba = New_Graph( width, height, graph->have_alpha ); 
	if(NULL == graph->rgba) {
		graph->width  = 0;
		graph->height = 0;
		Graph_Unlock( graph ); 
		return -1;
	}
	
	graph->width  = width;
	graph->height = height; 
	Graph_Unlock( graph ); 
	return 0;
}

void Graph_Copy(LCUI_Graph *des, LCUI_Graph *src)
/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */
{
	if( !des ) {
		return;
	}
	
	int size;
	
	if( Graph_Valid(src) ) { 
		if( Graph_Have_Alpha(src) ) {
			des->have_alpha = TRUE;
		} else {
			des->have_alpha = FALSE;
		}
		/* 创建合适尺寸的Graph */
		Graph_Create(des, src->width, src->height);
		/* 开始拷贝图像数组 */
		Graph_Lock(des, 1);
		Graph_Lock(src, 0);
		size = sizeof(uchar_t)*src->width*src->height;
		memcpy(des->rgba[0], src->rgba[0], size);
		memcpy(des->rgba[1], src->rgba[1], size);
		memcpy(des->rgba[2], src->rgba[2], size);
		if(Graph_Have_Alpha(src)) {
			memcpy(des->rgba[3], src->rgba[3], size);
		}
		des->type = src->type;       /* 存储图片类型 */
		des->bit_depth = src->bit_depth;  /* 位深 */
		des->alpha = src->alpha;      /* 全局透明度 */
		Graph_Unlock(des);
		Graph_Unlock(src);
	}
}

void Graph_Free(LCUI_Graph *pic)
/* 功能：释放LCUI_Graph内的图像数据占用的内存资源 */
{
	LCUI_Graph *p;
	p = Get_Quote_Graph(pic);
	if(Graph_Valid(p)) {
		Graph_Lock(p, 1);
		if( pic->quote ) {
			pic->src = NULL; 
			pic->quote = FALSE;
		} else {
			free(pic->rgba[0]);
			free(pic->rgba[1]);
			free(pic->rgba[2]);
			if(Graph_Have_Alpha(pic)) {
				free(pic->rgba[3]);
			}
			free(pic->rgba);
			pic->rgba = NULL;
			pic->width = 0;
			pic->height = 0;
		}
		Graph_Unlock(p); 
	}
}

/************************ Graph Quote *********************************/
int Quote_Graph(LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area)
/* 
 * 功能：引用另一个图层中指定区域里的图形 
 * 说明：src是被引用的对象，des是引用者，area是引用的src中的图形所在的区域
 * */
{
	if( !src || !des ) {
		return -1;
	}
	//printf("Quote_Graph(), before, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	area = Get_Valid_Area(Size(src->width, src->height), area);
	//printf("Quote_Graph(), after, area: %d,%d,%d,%d\n",
	//	area.x, area.y, area.width, area.height);
	if(!Rect_Valid( area )) { 
		des->src = NULL;
		des->pos.x = 0;
		des->pos.y = 0;
		des->width = 0;
		des->height= 0;
		des->quote = FALSE;
		return -1;
	} 
	des->src = src;
	des->pos.x = area.x;
	des->pos.y = area.y;
	des->width = area.width;
	des->height= area.height;
	des->quote = TRUE;
	return 0;
}

LCUI_Rect Get_Graph_Valid_Rect(LCUI_Graph *graph)
/* 功能：获取被引用的图形所在的有效区域 */
{
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect;
	
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
	
	LCUI_Rect rect;
	/* 获取引用的图像的有效显示范围 */
	rect = Get_Graph_Valid_Rect(graph->src);
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

LCUI_Graph *Get_Quote_Graph(LCUI_Graph *graph)
/* 
 * 功能：获取指向被引用的图形的指针 
 * 说明：如果当前图形引用了另一个图形，并且，该图形处于一条引用链中，那么，本函数会返
 * 回指向被引用的最终图形的指针。
 * */
{
	if( !graph ) {
		return NULL;
	}
	if( !graph->quote ) {
		return graph; 
	}
	return Get_Quote_Graph(graph->src);
}

/************************ End Graph Quote *****************************/


void Graph_Zoom(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size)
/* 功能：根据指定模式，对图像进行缩放 */
{
	LCUI_Graph *src;
	LCUI_Rect rect;
	LCUI_Pos pos; 
	int count, temp, x, y, k, m;
	double scale_x,scale_y;
	
	if(!Graph_Valid(in)) {
		return; 
	}
	
	if(size.w <=0 || size.h <= 0) { 
		Graph_Free(out);
		return;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	rect = Get_Graph_Valid_Rect(in);
	src = Get_Quote_Graph(in);
	
	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	/* 如果缩放方式为缺省，图像的宽和高的缩放比例将会一样 */
	if(flag == DEFAULT) {
		if (scale_x<scale_y) scale_y = scale_x; 
		else scale_x = scale_y;
	}
	out->have_alpha = in->have_alpha;
	Graph_Create(out, size.w, size.h);/* 申请内存 */ 
	Graph_Lock(out, 1);
	Graph_Lock(in, 0); 
	
	for (y=0; y < size.h; ++y)  {
		pos.y = y*scale_y;
		k = y*size.w;
		m = (pos.y+rect.y)*src->width+rect.x;
		for (x = 0; x < size.w; ++x) {
			pos.x = x*scale_x; 
			temp  = k + x;
			count = m + pos.x;
			out->rgba[0][temp] = in->rgba[0][count];
			out->rgba[1][temp] = in->rgba[1][count];
			out->rgba[2][temp] = in->rgba[2][count];
			if(Graph_Have_Alpha(in)) {
				out->rgba[3][temp] = in->rgba[3][count];
			}
		}
	} 
	
	Graph_Unlock(out);
	Graph_Unlock(in);
}

int Graph_Cut(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out)
/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */
{
	int x, y;
	int temp,count = 0, k; 
	
	out->have_alpha = src->have_alpha;
	out->alpha = src->alpha;
	rect = Get_Valid_Area(Size(src->width, src->height), rect); 
	
	if(Graph_Valid(src) && rect.width > 0 && rect.height > 0);
	else {
		return -1;
	}
	
	temp = Graph_Create(out, rect.width, rect.height);
	if(temp != 0) {
		return -1; 
	}
	
	Graph_Lock(out, 1);
	Graph_Lock(src, 0);
	/* 开始读取图片中的图形数组并写入窗口 */ 
	for (y=0;y< rect.height; ++y) {
		k = (rect.y+y)*src->width + rect.x;
		for (x=0;x<rect.width; ++x) {
			temp = k + x; /* 计算图片内需要读取的区域的各点坐标 */
			out->rgba[0][count] = src->rgba[0][temp];
			out->rgba[1][count] = src->rgba[1][temp];
			out->rgba[2][count] = src->rgba[2][temp];
			if(Graph_Have_Alpha(src))
				out->rgba[3][count] = src->rgba[3][temp];
			++count;
		}
	} 
	Graph_Unlock(out);
	Graph_Unlock(src);
	return 0; 
}

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out)  
/* 功能：将图像进行水平翻转 */  
{
	int x, y, pos, left, temp, count;  
	int width = src->width, height = src->height;  
	uchar_t buff;  
	
	if(!Graph_Valid(src)) {
		return -1;
	}
	
	if(Graph_Have_Alpha(src)) {
		out->have_alpha = TRUE;
	}
	Graph_Create(out, width, height);
	/* 水平翻转其实也就是交换两边的数据 */  
	temp = width / 2.0;
	for (y = 0; y < height; ++y) {   
		pos = y * width;
		left = pos;
		for (x = 0; x <= temp; ++x)  {
			count = left + width - x - 1;
			
			buff = src->rgba[0][pos]; 
			out->rgba[0][pos] = src->rgba[0][count];  
			out->rgba[0][count] = buff;
			 
			buff = src->rgba[1][pos]; 
			out->rgba[1][pos] = src->rgba[1][count];  
			out->rgba[1][count] = buff;
			
			buff = src->rgba[2][pos]; 
			out->rgba[2][pos] = src->rgba[2][count];  
			out->rgba[2][count] = buff;
			
			if(Graph_Have_Alpha(src)) {
				buff = src->rgba[3][pos]; 
				out->rgba[3][pos] = src->rgba[3][count];  
				out->rgba[3][count] = buff;
			}
			++pos;
		}
	} 
	return 0;  
}

int Graph_Fill_Color(LCUI_Graph *graph, LCUI_RGB color)
/* 功能：为传入的图形填充颜色 */
{
	uchar_t *r_ptr, *g_ptr, *b_ptr;
	LCUI_Rect src_rect;
	
	src_rect = Get_Graph_Valid_Rect( graph ); 
	graph = Get_Quote_Graph( graph );
	
	if(! Graph_Valid(graph) ) {
		return -1;
	} 
	
	int i, pos;
	size_t size;
	
	Graph_Lock( graph, 1);
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
	Graph_Unlock( graph );
	return 0; 
}

int Graph_Tile(LCUI_Graph *src, LCUI_Graph *out, int width, int height)
/* 
 * 功能：平铺图形
 * 参数说明：
 * src 原始图形
 * out 填充后的图形
 * width 填充的宽度
 * height 填充的高度
 **/
{
	if(!Graph_Valid(src) || out == NULL 
	 || width <=0 || height <= 0) {
		return -1;
	}
		
	int x,y,w,h,temp = 0,count, m;
		
	if( Graph_Create(out, width, height) != 0) {
		return -2;
	}
	
	if(Graph_Have_Alpha(src)) {
		Graph_Lock(out, 1);
		Graph_Lock(src, 0);
		for(y = 0;y < height;++y) {
			h = y % src->height;
			m = h * src->width;
			for(x = 0;x < width;++x,++temp) {
				w = x % src->width;
				count = m + w;
				out->rgba[0][temp] = ALPHA_BLENDING(src->rgba[0][count], out->rgba[0][temp], src->rgba[3][count]);
				out->rgba[1][temp] = ALPHA_BLENDING(src->rgba[1][count], out->rgba[1][temp], src->rgba[3][count]);
				out->rgba[2][temp] = ALPHA_BLENDING(src->rgba[2][count], out->rgba[2][temp], src->rgba[3][count]);
				//out->rgba[0][temp] = (src->rgba[0][count] * src->rgba[3][count] + out->rgba[0][temp] * (255 - src->rgba[3][count])) /255;
				//out->rgba[1][temp] = (src->rgba[1][count] * src->rgba[3][count] + out->rgba[1][temp] * (255 - src->rgba[3][count])) /255;
				//out->rgba[2][temp] = (src->rgba[2][count] * src->rgba[3][count] + out->rgba[2][temp] * (255 - src->rgba[3][count])) /255;
			} 
		}
		Graph_Unlock(out);
		Graph_Unlock(src); 
	} else {
		if(Graph_Have_Alpha(out)) {
			Graph_Fill_Alpha(out, 255);
		}
		Graph_Lock(out, 1);
		Graph_Lock(src, 0);
		for(y = 0;y < height;++y) {
			h = y%src->height;
			m = h * src->width;
			for(x = 0;x < width;++x) {
				w = x % src->width;
				count = m + w;
				out->rgba[0][temp] = src->rgba[0][count];
				out->rgba[1][temp] = src->rgba[1][count];
				out->rgba[2][temp] = src->rgba[2][count];
				++temp;
			}
		}
		Graph_Unlock(out);
		Graph_Unlock(src); 
	} 
	return 0;
}

int Graph_Mix(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */
{
	uchar_t j;//, alpha; 
	uchar_t *r1, *g1, *a1, *b1, *r2, *g2, *b2; 

	float k;
	uint_t total, y = 0, m, n, tmp_m, tmp_n;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;
	
	src_rect = Get_Graph_Valid_Rect(fore_graph);
	des_rect = Get_Graph_Valid_Rect(back_graph);
	src = Get_Quote_Graph(fore_graph);
	des = Get_Quote_Graph(back_graph); 
	
	if(!Graph_Valid(des) || !Graph_Valid(src)) {
		return -1;
	}
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -1;
	}
	
	if(fore_graph->width < 0 || fore_graph->height < 0 
	|| back_graph->width < 0 || back_graph->height < 0 ) {
		return -1;
	}
	/* 如果前景图尺寸超出背景图的范围，需要更改前景图的像素读取范围 */ 
	if( Get_Cut_Area(
		Size( des_rect.width, des_rect.height),
		Rect( des_pos.x, des_pos.y, 
			src_rect.width, src_rect.height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	Graph_Lock(src, 0);
	Graph_Lock(des, 1);
	/* 如果前景图形有alpha通道 */
	if(Graph_Have_Alpha(src)) {
		k = src->alpha / 255.0;
		if(src->alpha == 255){
			/* 根据左上角点的二维坐标，计算出一维坐标 */
			m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
			n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			for (y = 0; y < cut.height; ++y) { 
				/* 保存这一行开头的像素点的位置 */
				tmp_n = n; tmp_m = m;
				total = tmp_n + cut.width;
				/* 根据alpha通道来混合像素点 */
				for (; tmp_n < total; ++tmp_n,++tmp_m) { 
					des->rgba[0][tmp_n] = ALPHA_BLENDING(src->rgba[0][tmp_m], des->rgba[0][tmp_n], src->rgba[3][tmp_m]); 
					des->rgba[1][tmp_n] = ALPHA_BLENDING(src->rgba[1][tmp_m], des->rgba[1][tmp_n], src->rgba[3][tmp_m]);
					des->rgba[2][tmp_n] = ALPHA_BLENDING(src->rgba[2][tmp_m], des->rgba[2][tmp_n], src->rgba[3][tmp_m]); 
				}
				/* 切换到下一行像素点 */
				m += src->width;
				n += des->width; 
			}
		} else {
			m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
			n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			for (y = 0; y < cut.height; ++y) {
				tmp_n = n; tmp_m = m;
				total = tmp_n + cut.width; 
				for (; tmp_n < total; ++tmp_n,++tmp_m) { 
					j = src->rgba[3][tmp_m] * k;
					des->rgba[0][tmp_n] = ALPHA_BLENDING(src->rgba[0][tmp_m], des->rgba[0][tmp_n], j); 
					des->rgba[1][tmp_n] = ALPHA_BLENDING(src->rgba[1][tmp_m], des->rgba[1][tmp_n], j);
					des->rgba[2][tmp_n] = ALPHA_BLENDING(src->rgba[2][tmp_m], des->rgba[2][tmp_n], j);  
				} 
				m += src->width;
				n += des->width; 
			}
		} 
	} else if(fore_graph->alpha < 255) { 
		m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
		n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
		for (y = 0; y < cut.height; ++y) {
			tmp_n = n; tmp_m = m;
			total = tmp_n + cut.width; 
			for (; tmp_n < total; ++tmp_n,++tmp_m) { 
				des->rgba[0][tmp_n] = ALPHA_BLENDING(src->rgba[0][tmp_m], des->rgba[0][tmp_n], fore_graph->alpha); 
				des->rgba[1][tmp_n] = ALPHA_BLENDING(src->rgba[1][tmp_m], des->rgba[1][tmp_n], fore_graph->alpha);
				des->rgba[2][tmp_n] = ALPHA_BLENDING(src->rgba[2][tmp_m], des->rgba[2][tmp_n], fore_graph->alpha); 
			}
			m += src->width;
			n += des->width; 
		}
	} else {/* 如果前景图形没有透明效果 */
		m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
		n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
		for (y = 0; y < cut.height; ++y) {
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
			if(Graph_Have_Alpha(des)) {
				memset(a1, src->alpha, sizeof(uchar_t)*cut.width);
			}
			m += src->width;
			n += des->width; 
		}
	} 
	
	Graph_Unlock(des);
	Graph_Unlock(src); 
	return 0; 
}


int Graph_Replace(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */
{
	uchar_t *r1, *g1, *a1, *a2, *b1, *r2, *g2, *b2;

	uint_t y,total, m, n, tmp_m, tmp_n;
	uchar_t j, k; 
	
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;
	
	src_rect = Get_Graph_Valid_Rect(fore_graph);
	des_rect = Get_Graph_Valid_Rect(back_graph); 
	src = Get_Quote_Graph(fore_graph);
	des = Get_Quote_Graph(back_graph);
	
	if(!Graph_Valid(des) || !Graph_Valid(src)) {
		return -1;
	}
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -1;
	} 
	 
	if(fore_graph->width < 0 || fore_graph->height < 0 
	|| back_graph->width < 0 || back_graph->height < 0 ) {
		return -1;
	}
	if( Get_Cut_Area(
		Size( back_graph->width, back_graph->height),
		Rect( des_pos.x, des_pos.y, 
			fore_graph->width, fore_graph->height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	Graph_Lock(src, 0);
	Graph_Lock(des, 1);
	k = src->alpha / 255.0;
	if(Graph_Have_Alpha(src) && !Graph_Have_Alpha(des)) { 
		m = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
		n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
		for (y = 0; y < cut.height; ++y) {
			tmp_n = n; tmp_m = m;
			total = tmp_n + cut.width; 
			for (; tmp_n < total; ++tmp_n,++tmp_m) { 
				j = src->rgba[3][tmp_m] * k;
				des->rgba[0][tmp_n] = ALPHA_BLENDING(src->rgba[0][tmp_m], 255, j); 
				des->rgba[1][tmp_n] = ALPHA_BLENDING(src->rgba[1][tmp_m], 255, j);
				des->rgba[2][tmp_n] = ALPHA_BLENDING(src->rgba[2][tmp_m], 255, j);  
			} 
			m += src->width;
			n += des->width; 
		}
	} else {
		m = (cut.y + src_rect.y) *src->width + cut.x + src_rect.x;
		n = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
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
			if(Graph_Have_Alpha(des)) {
				a1 = des->rgba[3] + n;
				if(!Graph_Have_Alpha(src)) {
					memset(a1, src->alpha, cut.width);
				} else {
					a2 = src->rgba[3] + m; 
					memcpy(a1, a2, cut.width);  
				}
			}
			m += src->width;
			n += des->width; 
		}
	} 
	Graph_Unlock(des);
	Graph_Unlock(src);
	return 0; 
} 

int Graph_Put_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag)
/* 功能：将图片以指定对齐方式粘贴至背景图上 */
{
	LCUI_Pos pos;
	pos = Pos(0,0);
	/* 如果图片无效 */
	if(!Graph_Valid(graph) || ! Graph_Valid(image)) {
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

int Graph_Fill_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color)
/* 功能：为指定图形填充背景图像 */
{
	Graph_Fill_Color(graph, color); /* 填充背景色 */
	if(!Graph_Valid(bg) || graph == NULL) {
		return -1; 
	}
	
	LCUI_Pos pos;
	LCUI_Graph temp_bg;
	
	pos = Pos(0,0);
	Graph_Init(&temp_bg);
	
	switch( flag ) {
	    case LAYOUT_ZOOM:/* 缩放 */
		Graph_Zoom( bg, &temp_bg, DEFAULT,
			Size(graph->width, graph->height) );
		pos.x = (graph->width - temp_bg.width) / 2.0;
		pos.y = (graph->height - temp_bg.height) / 2.0;
		Graph_Mix(graph, &temp_bg, pos);
		break;
	    case LAYOUT_STRETCH:/* 拉伸 */
		Graph_Zoom( bg,  &temp_bg, CUSTOM, 
			Size(graph->width, graph->height) );
		Graph_Mix(graph, &temp_bg, pos);
		break;
	    case LAYOUT_CENTER:/* 居中 */
		pos.x = (graph->width - bg->width) / 2.0;
		pos.y = (graph->height - bg->height) / 2.0;
		Graph_Mix(graph, bg, pos);
		break;
	    case LAYOUT_TILE:/* 平铺 */
		Graph_Tile(bg, graph, graph->width, graph->height);
		break;
	    default: 
		Graph_Mix(graph, bg, pos);
		break;
	}
	Graph_Free(&temp_bg);
	return 0; 
}

int Graph_Fill_Alpha(LCUI_Graph *src, uchar_t alpha)
/* 功能：填充图形的alpha通道的所有值 */
{
	uchar_t *ptr;
	LCUI_Rect src_rect;
	
	/* 获取引用的区域在源图形中的有效区域 */
	src_rect = Get_Graph_Valid_Rect( src );
	/* 获取引用的源图指针 */
	src = Get_Quote_Graph( src );
	
	if(! Graph_Valid(src) ) {
		return -1;
	}
	if( !Graph_Have_Alpha(src) ) {
		return -1;
	}
	
	int i;
	size_t size;
	
	Graph_Lock( src, 1);
	size = sizeof(uchar_t) * src_rect.width;
	ptr = src->rgba[3] + src_rect.x + src_rect.y * src->width;
	for(i=0; i<src_rect.height; ++i) {
		memset(ptr, alpha, size);
		ptr += src->width;
	}
	Graph_Unlock( src );
	return 0; 
}
