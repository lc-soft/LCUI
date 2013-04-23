/* ***************************************************************************
 * LCUI_Graph.h -- The base graphics handling module for LCUI
 * 
 * Copyright (C) 2013 by
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
#ifndef __LCUI_GRAPH_H__
#define __LCUI_GRAPH_H__

LCUI_BEGIN_HEADER

/* 将两个像素点的颜色值进行alpha混合 */
#define ALPHA_BLEND(__src__ , __des__, __alpha__)		\
{								\
    __des__ = (((__src__-__des__)*(__alpha__))>>8)+__des__;	\
}

/* 获取像素的RGB值 */
#define RGB_FROM_RGB565(pixel, r, g, b)	\
{\
	r = (((pixel&0xF800)>>11)<<3);	\
	g = (((pixel&0x07E0)>>5)<<2);	\
	b = ((pixel&0x001F)<<3);	\
}

#define RGB_FROM_RGB555(pixel, r, g, b)	\
{\
	r = (((pixel&0x7C00)>>10)<<3);	\
	g = (((pixel&0x03E0)>>5)<<3);	\
	b = ((pixel&0x001F)<<3);	\
}

#define RGB_FROM_RGB888(pixel, r, g, b)	\
{\
	r = ((pixel&0xFF0000)>>16);	\
	g = ((pixel&0xFF00)>>8);	\
	b = (pixel&0xFF);		\
}

/* 混合像素的RGB值 */
#define RGB565_FROM_RGB(pixel, r, g, b)			\
{							\
    pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);		\
}

#define RGB555_FROM_RGB(pixel, r, g, b)			\
{							\
	pixel = ((r>>3)<<10)|((g>>3)<<5)|(b>>3);	\
}

#define RGB888_FROM_RGB(pixel, r, g, b)			\
{							\
	pixel = (r<<16)|(g<<8)|b;			\
}

/* 解除RGB宏 */
#ifdef RGB
#undef RGB
#endif

/* 将三个颜色值转换成LCUI_RGB型数据 */
LCUI_EXPORT(LCUI_RGB)
RGB ( uchar_t red, uchar_t green, uchar_t blue );

/* 为图形数据设定互斥锁 */
LCUI_EXPORT(void)
Graph_Lock( LCUI_Graph *graph );

/* 更新图层的属性 */
LCUI_EXPORT(void)
Graph_UpdateAttr( LCUI_Graph *buff );

/* 解除互斥锁，以结束图像数据的使用 */
LCUI_EXPORT(void)
Graph_Unlock( LCUI_Graph *graph );

/* 获取图像中指定坐标的像素点的颜色 */
LCUI_EXPORT(LCUI_BOOL)
Graph_GetPixel( LCUI_Graph *graph, LCUI_Pos pos, LCUI_RGBA *pixel );

/* 返回图形的类型 */
LCUI_EXPORT(int)
Graph_GetType( LCUI_Graph *pic );

/* 获取图形的尺寸 */
LCUI_EXPORT(LCUI_Size)
Graph_GetSize( LCUI_Graph *pic );

/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回TRUE，否则返回FALSE
 * */
LCUI_EXPORT(LCUI_BOOL)
Graph_IsPNG( LCUI_Graph *pic );

/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回TRUE，否则返回FALSE
 * */
LCUI_EXPORT(LCUI_BOOL)
Graph_IsBMP( LCUI_Graph *pic );

/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */
LCUI_EXPORT(LCUI_BOOL)
Graph_IsJPG( LCUI_Graph *pic );

/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */
LCUI_EXPORT(LCUI_BOOL)
Graph_HaveAlpha( LCUI_Graph *pic );

/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */
LCUI_EXPORT(LCUI_BOOL)
Graph_IsValid( LCUI_Graph *pic );

/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */
LCUI_EXPORT(int)
Graph_IsOpaque( LCUI_Graph *graph );

/* 打印图像的信息 */
LCUI_EXPORT(void)
Graph_PrintInfo( LCUI_Graph *pic );

/* 初始化图片数据结构体 */
LCUI_EXPORT(void)
Graph_Init( LCUI_Graph *pic );

/* 为图像数据分配内存资源 */
LCUI_EXPORT(int)
Graph_Create( LCUI_Graph *graph, int width, int height );

/* 
 * 功能：拷贝图像数据
 * 说明：将src的数据拷贝至des 
 * */
LCUI_EXPORT(void)
Graph_Copy( LCUI_Graph *des, LCUI_Graph *src );

/* 释放LCUI_Graph内的图像数据占用的内存资源 */
LCUI_EXPORT(void)
Graph_Free( LCUI_Graph *pic );

/************************ Graph Quote *********************************/
LCUI_EXPORT(int)
Graph_Quote( LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area );

/* 获取被引用的图形所在的有效区域 */
LCUI_EXPORT(LCUI_Rect)
Graph_GetValidRect( LCUI_Graph *graph );

/* 
 * 功能：获取指向被引用的图形的指针 
 * 说明：如果当前图形引用了另一个图形，并且，该图形处于一条引用链中，那么，本函数会返
 * 回指向被引用的最终图形的指针。
 * */
LCUI_EXPORT(LCUI_Graph*)
Graph_GetQuote( LCUI_Graph *graph );

/************************ End Graph Quote *****************************/

/* 根据指定模式，对图像进行缩放 */
LCUI_EXPORT(void)
Graph_Zoom( LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size );

/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */
LCUI_EXPORT(int)
Graph_Cut( LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out );

/* 将图像进行水平翻转 */  
LCUI_EXPORT(int)
Graph_HorizFlip( LCUI_Graph *src, LCUI_Graph *out );

/* 将图像进行垂直翻转 */  
LCUI_EXPORT(int)
Graph_VertiFlip( LCUI_Graph *img, LCUI_Graph *out );

/* 填充矩形 */
LCUI_EXPORT(int)
Graph_FillRect( LCUI_Graph *graph, LCUI_RGB color, LCUI_Rect rect );

/* 填充颜色 */
LCUI_EXPORT(int)
Graph_FillColor( LCUI_Graph *graph, LCUI_RGB color );

/* 平铺图形 */
LCUI_EXPORT(int)
Graph_Tile( LCUI_Graph *src, LCUI_Graph *des_buff, LCUI_BOOL replace );

/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */
LCUI_EXPORT(int)
Graph_Mix( LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos );

/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */
LCUI_EXPORT(int)
Graph_Replace( LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos );

/* 将图片以指定对齐方式粘贴至背景图上 */
LCUI_EXPORT(int)
Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int flag );

/* 为指定图形填充背景图像 */
LCUI_EXPORT(int)
Graph_FillImage(	LCUI_Graph *graph,	LCUI_Graph *bg, 
			int mode,		LCUI_RGB color );

/* 填充图形的alpha通道的所有值 */
LCUI_EXPORT(int)
Graph_FillAlpha( LCUI_Graph *src, uchar_t alpha );
 
LCUI_END_HEADER

#ifdef __cplusplus
#include LC_GRAPH_HPP
#endif

#endif /* __LCUI_GRAPHICS_H__ */

