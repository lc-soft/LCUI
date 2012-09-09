/* ***************************************************************************
 * LCUI_Memory.c -- Some of the data type of memory management
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
 * LCUI_Memory.c -- 一些数据类型的内存管理
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
#include LC_GRAPHICS_H
#include LC_FONT_H
#include LC_MISC_H
#include LC_WIDGET_H 

#include <unistd.h>

void Using_Graph(LCUI_Graph *pic, int mode)
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

void End_Use_Graph(LCUI_Graph *pic)
/* 功能：结束图像数据的使用 */
{
	rwlock_status status;
	LCUI_Graph *src;
	src = Get_Quote_Graph(pic);
	status = thread_rwlock_get_status(&src->lock);
	/* 如果当前为写锁，那就说明这个图形数据被修改了 */
	if(status == RWLOCK_WRITE){
		/* 根据图形数据内容来设定各个标志 */
		if(Valid_Graph(pic)){ 
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
						pic->not_visible = IS_TRUE;
						pic->is_opaque = IS_FALSE;
						break;
					    case 1:
						pic->not_visible = IS_FALSE;
						pic->is_opaque = IS_FALSE;
						break;
					    default:
						pic->not_visible = IS_FALSE;
						pic->is_opaque = IS_TRUE;
						break;
					}
				} else {
					pic->not_visible = IS_FALSE;
					pic->is_opaque = IS_FALSE;
				}
			} else {
				pic->is_opaque = IS_TRUE;
			}
		}
	}
	thread_rwlock_unlock(&src->lock);
}

unsigned char** Get_Malloc(int width,int height,int have_alpha)
/* 功能：为图形数据申请内存空间，并初始化该内存空间为零 */
{
	unsigned int size;
	unsigned char** out_buff;
	size = sizeof(unsigned char)*width*height;
	if(have_alpha == IS_TRUE) { 
		out_buff = (unsigned char**)malloc(sizeof(unsigned char*)*4);
	} else {
		out_buff = (unsigned char**)malloc(sizeof(unsigned char*)*3);
	}
	out_buff[0] = (unsigned char*)malloc(size);
	out_buff[1] = (unsigned char*)malloc(size);
	out_buff[2] = (unsigned char*)malloc(size);
	if(have_alpha == IS_TRUE) {
		out_buff[3] = (unsigned char*)calloc(1, size);
	}
	
	if(!out_buff || !out_buff[0] || !out_buff[1] || !out_buff[2] 
	|| (have_alpha == IS_TRUE && !out_buff[3])) {
		return NULL; 
	} 
	return out_buff;
}
 
void Free_Graph(LCUI_Graph *pic)
/* 功能：释放LCUI_Graph内的图像数据占用的内存资源 */
{
	LCUI_Graph *p;
	p = Get_Quote_Graph(pic);
	if(Valid_Graph(p)) {
		Using_Graph(p, 1);
		if(pic->quote == IS_TRUE) {
			pic->src = NULL; 
			pic->quote = IS_FALSE;
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
		End_Use_Graph(p); 
	}
}

void *Malloc_Widget_Private(LCUI_Widget *widget, size_t size)
/* 功能：为部件私有结构体指针分配内存 */
{
	widget->private_data = malloc(size);
	return widget->private_data;
}


int Malloc_Graph(LCUI_Graph *pic, int width, int height)
/* 功能：为图像数据分配内存资源 */
{
	if(width > 10000 || height > 10000) {
		printf("error: can not allocate too much memory!\n");
		exit(-1);
	}
	if(height < 0 || width < 0) {
		Free_Graph(pic);
		return -1; 
	}
	
	if(Valid_Graph(pic)) {
		if(pic->width*pic->height > width*height) {
			pic->width  = width;
			pic->height = height;
			return 1;
		}
		Free_Graph(pic); 
	}
	
	Using_Graph(pic, 1); 
	pic->rgba = Get_Malloc(width, height, pic->have_alpha); 
	if(NULL == pic->rgba) {
		pic->width  = 0;
		pic->height = 0;
		End_Use_Graph(pic); 
		return -1;
	}
	
	pic->width  = width;
	pic->height = height; 
	End_Use_Graph(pic); 
	return 0;
}

void Copy_Graph(LCUI_Graph *des, LCUI_Graph *src)
/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */
{
	int size;
	des->have_alpha = src->have_alpha; /* 是否需要透明度 */
	if(Valid_Graph(src)) {
		if(Valid_Graph(des)) {
			Free_Graph(des);
		}
		if(Graph_Have_Alpha(src)) {
			des->have_alpha = IS_TRUE;
		} else {
			des->have_alpha = IS_FALSE;
		}
		Malloc_Graph(des, src->width, src->height);/* 重新分配 */
		/* 拷贝图像数组 */
		Using_Graph(des, 1);
		Using_Graph(src, 0);
		size = sizeof(unsigned char)*src->width*src->height;
		memcpy(des->rgba[0], src->rgba[0], size);
		memcpy(des->rgba[1], src->rgba[1], size);
		memcpy(des->rgba[2], src->rgba[2], size);
		if(Graph_Have_Alpha(src)) {
			memcpy(des->rgba[3], src->rgba[3], size);
		}
		des->type = src->type;       /* 存储图片类型 */
		des->bit_depth = src->bit_depth;  /* 位深 */
		des->alpha = src->alpha;      /* 透明度 */
		End_Use_Graph(des);
		End_Use_Graph(src);
	}
}

void Free_String(LCUI_String *in)
/* 功能：释放String结构体中的指针占用的内存空间 */
{
	if(in->size > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

void Free_Bitmap(LCUI_Bitmap *bitmap)
/* 功能：释放单色位图占用的内存资源，并初始化 */
{
	int y;
	if(Valid_Bitmap(bitmap)) {
		for(y = 0; y < bitmap->height; ++y) {
			free(bitmap->data[y]); 
		}
		free(bitmap->data);
		bitmap->alpha    = 255;/* 字体位图默认不透明 */
		bitmap->width    = 0;
		bitmap->height   = 0;
		bitmap->malloc   = IS_FALSE;/* 字体位图没有分配内存 */
		bitmap->data     = NULL;
	}
}

void Free_WChar_T(LCUI_WChar_T *ch)
/* 功能：释放LCUI_Wchar_T型体中的指针变量占用的内存 */
{
	Free_Bitmap(&ch->bitmap);
}

void Free_WString(LCUI_WString *str)
/* 功能：释放LCUI_WString型结构体中的指针变量占用的内存 */
{
	int i;
	if(str != NULL) {
		if(str->size > 0 && str->string != NULL) {
			for(i = 0; i < str->size; ++i) {
				Free_WChar_T(&str->string[i]); 
			}
			free(str->string);
			str->string = NULL;
		}
	}
}



void Malloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height)
/* 功能：为Bitmap内的数据分配内存资源，并初始化 */
{
	int i;
	if(width * height > 0) {
		if(Valid_Bitmap(bitmap)) Free_Bitmap(bitmap);
		bitmap->width = width;
		bitmap->height = height;
		bitmap->data = (unsigned char**)malloc(
				bitmap->height*sizeof(unsigned char*));
		for(i = 0; i < bitmap->height; ++i) { 
			/* 为背景图的每一行分配内存 */
			bitmap->data[i] = (unsigned char*)calloc(1,
				sizeof(unsigned char) * bitmap->width); 
		}
		bitmap->alpha    = 255; /* 字体默认不透明 */
		bitmap->malloc = IS_TRUE;
	}
}

void Realloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height)
/* 功能：更改位图的尺寸 */
{
	int i, j;
	if(width >= bitmap->width && height >= bitmap->height) {
	/* 如果新尺寸大于原来的尺寸 */
		bitmap->data = (unsigned char**)realloc(
			bitmap->data, height*sizeof(unsigned char*));
		for(i = 0; i < height; ++i) {   /* 为背景图的每一行扩增内存 */
			bitmap->data[i] = (unsigned char*)realloc(
				bitmap->data[i], sizeof(unsigned char) * width); 
			/* 把扩增的部分置零 */
			if(i < bitmap->height) {
				for(j = bitmap->width; j < width; ++j) {
					bitmap->data[i][j] = 0; 
				}
			} else {
				memset(bitmap->data[i], 0 , bitmap->width); 
			}
		}
	}
}

void Free_Font(LCUI_Font *in)
/* 功能：释放Font结构体数据占用的内存资源 */
{
	Free_String(&in->font_file);
	Free_String(&in->family_name);
	Free_String(&in->style_name);
	if(in->status == ACTIVE) { 
		in->status = KILLED;
		if(in->type == CUSTOM) { 
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
		}
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
}

void Free_LCUI_Font()
/* 功能：释放LCUI默认的Font结构体数据占用的内存资源 */
{ 
	Free_String(&LCUI_Sys.default_font.font_file);
	Free_String(&LCUI_Sys.default_font.family_name);
	Free_String(&LCUI_Sys.default_font.style_name);
	if(LCUI_Sys.default_font.status == ACTIVE) {
		LCUI_Sys.default_font.status = KILLED;
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
}
