/* ***************************************************************************
 * LCUI_Graph.h -- The base graphics handling module of LCUI
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
 * LCUI_Graph.h -- LCUI的基本图形处理模块
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
#ifndef __LCUI_GRAPH_H__
#define __LCUI_GRAPH_H__

LCUI_BEGIN_HEADER

#define ALPHA_BLENDING(__fore__ , __back__, __alpha__) \
	((__fore__*__alpha__+__back__*(255-__alpha__))/255)
	
LCUI_RGB RGB ( uchar_t red, uchar_t green, uchar_t blue ) ;
/* 将三个颜色值转换成LCUI_RGB型数据 */

void Graph_Lock(LCUI_Graph *pic, int mode);
/* 功能：以指定模式使用图像数据
 * 说明：参数mode的值为0时，以“读”模式使用数据，其它值时，以“写模式使用数据” */ 

void Graph_Unlock(LCUI_Graph *pic);
/* 功能：结束图像数据的使用 */

LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore);
/* 功能：混合两个像素点的颜色 */ 

LCUI_RGBA Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos);
/* 功能：获取图像中指定坐标的像素点的颜色 */ 

int Get_Graph_Type(LCUI_Graph *pic);
/* 返回图形的类型 */ 

LCUI_Size Get_Graph_Size(LCUI_Graph *pic);
/* 功能：获取图形的尺寸 */ 

BOOL Graph_Is_PNG(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回1，否则返回0
 * */ 

BOOL Graph_Is_BMP(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回1，否则返回0
 * */ 

BOOL Graph_Is_JPG(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */ 

BOOL Graph_Have_Alpha(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */ 

BOOL Graph_Valid(LCUI_Graph *pic);
/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */ 

int Graph_Is_Opaque(LCUI_Graph *graph);
/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */ 

void Print_Graph_Info(LCUI_Graph *pic);
/* 功能：打印图像的信息 */ 

void Graph_Init(LCUI_Graph *pic);
/* 初始化图片数据结构体 */ 

int Graph_Create(LCUI_Graph *graph, int width, int height);
/* 功能：创建一个Graph对象 */ 

void Graph_Copy(LCUI_Graph *des, LCUI_Graph *src);
/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */ 

void Graph_Free(LCUI_Graph *pic);
/* 功能：释放LCUI_Graph内的图像数据占用的内存资源 */ 

/************************ Graph Quote *********************************/
int Quote_Graph(LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area);
/* 
 * 功能：引用另一个图层中指定区域里的图形 
 * 说明：src是被引用的对象，des是引用者，area是引用的src中的图形所在的区域
 * */ 

LCUI_Rect Get_Graph_Valid_Rect(LCUI_Graph *graph);
/* 功能：获取被引用的图形所在的有效区域 */ 

LCUI_Graph *Get_Quote_Graph(LCUI_Graph *graph);
/* 
 * 功能：获取指向被引用的图形的指针 
 * 说明：如果当前图形引用了另一个图形，并且，该图形处于一条引用链中，那么，本函数会返
 * 回指向被引用的最终图形的指针。
 * */ 

/************************ End Graph Quote *****************************/
void Graph_Zoom(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size);
/* 功能：根据指定模式，对图像进行缩放 */ 

int Graph_Cut(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out);
/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */ 

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out);
/* 功能：将图像进行水平翻转 */

int Get_Screen_Graph(LCUI_Graph *out);
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */ 

int Graph_Fill_Color(LCUI_Graph *pic, LCUI_RGB color);
/* 功能：为传入的图形填充颜色 */ 

int Graph_Tile(LCUI_Graph *src, LCUI_Graph *out, int width, int height);
/* 
 * 功能：平铺图形
 * 参数说明：
 * src 原始图形
 * out 填充后的图形
 * width 填充的宽度
 * height 填充的高度
 **/ 

int Graph_Mix(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */ 
 
int Graph_Replace(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */ 

int Graph_Put_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag);
/* 功能：将图片以指定对齐方式粘贴至背景图上 */ 

int Graph_Fill_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color);
/* 功能：为指定图形填充背景图像 */ 

int Graph_Fill_Alpha(LCUI_Graph *src, uchar_t alpha);
/* 功能：填充图形的alpha通道的所有值 */ 
 
LCUI_END_HEADER

#ifdef __cplusplus
#include LC_GRAPH_HPP
#endif

#endif /* __LCUI_GRAPHICS_H__ */

