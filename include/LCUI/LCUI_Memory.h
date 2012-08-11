/* ***************************************************************************
 * LCUI_Memory.h -- Some of the data type of memory management
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
 * LCUI_Memory.h -- 一些数据类型的内存管理
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

#ifndef __LCUI_MEMORY_H__ /* 如果没有定义 __LCUI_MEMORY_H__ 宏 */
#define __LCUI_MEMORY_H__  /* 定义 __LCUI_MEMORY_H__ 宏 */

void Using_Graph(LCUI_Graph *pic, int mode);
/* 功能：以指定模式使用图像数据
 * 说明：参数mode的值为0时，以“读”模式使用数据，其它值时，以“写"模式使用数据 */ 

void End_Use_Graph(LCUI_Graph *pic);
/* 功能：结束图像数据的使用 */ 

unsigned char** Get_Malloc(int width,int height,int flag);
/* 功能：为图形数据申请内存空间，并初始化该内存空间为零 */ 
 
void Free_Graph(LCUI_Graph *pic);
/* 功能：释放LCUI_Graph内的图像数据占用的内存资源 */ 

void *Malloc_Widget_Private(LCUI_Widget *widget, size_t size);
/* 功能：为部件私有结构体指针分配内存 */ 

int Malloc_Graph(LCUI_Graph *pic, int width, int height);
/* 功能：为图像数据分配内存资源 */ 

void Copy_Graph(LCUI_Graph *des, LCUI_Graph *src);
/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */ 

void Free_String(LCUI_String *in);
/* 功能：释放String结构体中的指针占用的内存空间 */ 

void Free_Bitmap(LCUI_Bitmap *bitmap);
/* 功能：释放单色位图占用的内存资源，并初始化 */ 

void Free_WChar_T(LCUI_WChar_T *ch);
/* 功能：释放LCUI_Wchar_T型体中的指针变量占用的内存 */ 

void Free_WString(LCUI_WString *str);
/* 功能：释放LCUI_WString型结构体中的指针变量占用的内存 */ 

void Malloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height);
/* 功能：为Bitmap内的数据分配内存资源，并初始化 */ 

void Realloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height);
/* 功能：更改位图的尺寸 */ 

void Free_Font(LCUI_Font *in);
/* 功能：释放Font结构体数据占用的内存资源 */ 

void Free_LCUI_Font();
/* 功能：释放LCUI默认的Font结构体数据占用的内存资源 */ 
#endif  /*  __LCUI_MEMORY_H__ */

