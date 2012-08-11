/* ***************************************************************************
 * LCUI_Graphics.h -- graphics handle
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
 * LCUI_Graphics.h -- 图形处理
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
#ifndef __LCUI_GRAPHICS_H__
#define __LCUI_GRAPHICS_H__
LCUI_RGB
RGB (unsigned char red, unsigned char green,
	 unsigned char blue);
/* 功能：将三个颜色值转换成LCUI_RGB型数据 */

int Get_Graph_Type(LCUI_Graph *pic);
/* 返回图形的类型 */ 

LCUI_Size Get_Graph_Size(LCUI_Graph *pic);
/* 功能：获取图形的尺寸 */ 

int Graph_Is_PNG(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回1，否则返回0
 * */ 

int Graph_Is_BMP(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回1，否则返回0
 * */ 

int Graph_Is_JPG(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */ 

int Graph_Have_Alpha(LCUI_Graph *pic);
/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */ 

int Valid_Graph(LCUI_Graph *pic);
/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */ 

int Valid_Bitmap(LCUI_Bitmap *bitmap);
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回1，无效返回0
 */ 

void Print_Bitmap_Info(LCUI_Bitmap *bitmap);
/* 功能：打印位图的信息 */ 

void Print_Graph_Info(LCUI_Graph *pic);
/* 功能：打印图像的信息 */ 

void Bitmap_Init(LCUI_Bitmap *in);
/* 功能：初始化LCUI_Bitmap结构体数据 */ 

void Graph_Init(LCUI_Graph *pic);
/* 初始化图片数据结构体 */ 

void Zoom_Graph(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size);
/* 功能：根据指定模式，对图像进行缩放 */ 

int Load_Image(char *filepath, LCUI_Graph *out);
/* 
 * 功能：载入指定图片文件的图形数据
 * 说明：打开图片文件，并解码至内存，打开的图片文件越大，占用的内存也就越大 
 * */ 

int Cut_Graph(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out);
/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */ 

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out);
/* 功能：将图像进行水平翻转 */ 

int Cut_And_Overlay_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des);
/*
 * 功能：从源图形中截取出一块图形并根据alpha通道叠加至背景图上
 * 说明：这个函数是截取和覆盖一起进行的，在截取的同时，将图形覆盖至目标图形上，效率比分步调用函数高一点。
 */ 

int Cut_And_Replace_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des);
/*
 * 功能：从源图形中截取出一块图形并覆盖至背景图上
 * 说明：与其它函数不同，这个是截取和合成一起进行的，在截取的同时，将图形覆盖至目标图形上，效率比分步调用函数高一点。
 */ 

int Get_Screen_Graph(LCUI_Graph *out);
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */ 

void Fill_Color(LCUI_Graph *pic, LCUI_RGB color);
/* 功能：为传入的图形填充颜色 */ 

void Tile_Graph(LCUI_Graph *src, LCUI_Graph *out, int width, int height);
/* 
 * 功能：平铺图形
 * 参数说明：
 * src 原始图形
 * out 填充后的图形
 * width 填充的宽度
 * height 填充的高度
 **/ 

int Mix_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */ 

int Replace_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */ 

int Align_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag);
/* 功能：将图片以指定对齐方式粘贴至背景图上 */ 

int Fill_Background_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color);
/* 功能：为指定图形填充背景图像 */ 

int Fill_Graph_Alpha(LCUI_Graph *src, unsigned char alpha);
/* 功能：填充图形的alpha通道的所有值 */ 

int Draw_Graph_Border(LCUI_Graph *src,LCUI_RGB color, LCUI_Border border);
/* 功能：为图形边缘绘制矩形边框 */ 

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue);
/* 功能：获取与指定区域重叠的部件 */ 

LCUI_RGBA Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos);
/* 功能：获取图像中指定坐标的像素点的颜色 */ 

int Graph_Is_Opaque(LCUI_Graph *graph);
/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */ 
int write_png_file(char *file_name , LCUI_Graph *graph);
/* 功能：将LCUI_Graph结构中的数据写入至png文件 */

int Widget_Layer_Is_Opaque(LCUI_Widget *widget);
/* 功能：判断部件图形是否不透明 */ 

int Widget_Layer_Not_Visible(LCUI_Widget *widget);
/* 功能：检测部件图形是否完全透明 */ 

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph);
/* 
 * 功能：获取屏幕中指定区域内实际要显示的图形 
 * 说明：指定的区域必须是与部件区域不部分重叠的
 * */ 

LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore);
/* 功能：混合两个像素点的颜色 */ 

int Rotate_Graph(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des);
/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自：http://read.pudn.com/downloads154/sourcecode/graph/684994/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E6%BA%90%E4%BB%A3%E7%A0%81/MyDIPView.cpp__.htm
 * 算法有待优化完善。
 */ 
#endif /* __LCUI_GRAPHICS_H__ */

