/* ***************************************************************************
 * LCUI_Graph.h -- The base graphics handling module for LCUI
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
	
/* 图像类型 */
#define TYPE_PNG	1
#define TYPE_JPG	2
#define TYPE_BMP	3

/* 图像的混合方式 */
#define GRAPH_MIX_FLAG_OVERLAY	 1<<7
#define GRAPH_MIX_FLAG_REPLACE	 1<<8

/* 将两个像素点的颜色值进行alpha混合 */
#define _ALPHA_BLEND(__src__ , __des__, __alpha__)	\
    ((((__src__-__des__)*(__alpha__))>>8)+__des__)

#define ALPHA_BLEND(__src__ , __des__, __alpha__)	\
{							\
    __des__ =_ALPHA_BLEND(__src__,__des__,__alpha__);	\
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

LCUI_API LCUI_RGB RGB ( uchar_t red, uchar_t green, uchar_t blue );

LCUI_API void Graph_Lock( LCUI_Graph *graph );

LCUI_API void Graph_Unlock( LCUI_Graph *graph );

LCUI_API LCUI_BOOL Graph_GetPixel(	LCUI_Graph *graph,
					LCUI_Pos pos,
					LCUI_RGBA *pixel );

LCUI_API LCUI_Size Graph_GetSize( LCUI_Graph *graph );

LCUI_API LCUI_BOOL Graph_HaveAlpha( LCUI_Graph *graph );

LCUI_API LCUI_BOOL Graph_IsValid( LCUI_Graph *graph );

LCUI_API void Graph_PrintInfo( LCUI_Graph *graph );

LCUI_API void Graph_Init( LCUI_Graph *graph );

LCUI_API int Graph_Create( LCUI_Graph *graph, int w, int h );

LCUI_API void Graph_Copy( LCUI_Graph *des, LCUI_Graph *src );

LCUI_API void Graph_Free( LCUI_Graph *pic );

LCUI_API int Graph_Quote( LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area );

LCUI_API LCUI_Rect Graph_GetValidRect( LCUI_Graph *graph );

LCUI_API LCUI_Graph* Graph_GetQuote( LCUI_Graph *graph );

LCUI_API int Graph_Zoom(	LCUI_Graph *in,
				LCUI_Graph *out, 
				LCUI_BOOL keep_scale,
				LCUI_Size size );

LCUI_API int Graph_Cut( LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out );

LCUI_API int Graph_HorizFlip( LCUI_Graph *src, LCUI_Graph *out );

LCUI_API int Graph_VertiFlip( LCUI_Graph *src, LCUI_Graph *out );

LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_RGB color, LCUI_Rect rect );

LCUI_API int Graph_FillColor( LCUI_Graph *graph, LCUI_RGB color );

LCUI_API int Graph_Tile( LCUI_Graph *src, LCUI_Graph *des, LCUI_BOOL replace );

LCUI_API int Graph_Mix(	LCUI_Graph *back_graph, 
			LCUI_Graph *fore_graph,
			LCUI_Pos des_pos );

LCUI_API int Graph_Replace(	LCUI_Graph *back_graph,
				LCUI_Graph *fore_graph,
				LCUI_Pos des_pos );

LCUI_API int Graph_PutImage( LCUI_Graph *graph, LCUI_Graph *image, int flag );


LCUI_API int Graph_FillImage(	LCUI_Graph *graph,
				LCUI_Graph *bg, 
				int mode,
				LCUI_RGB color );

LCUI_API int Graph_FillAlpha( LCUI_Graph *src, uchar_t alpha );
 
LCUI_END_HEADER
	
#include LC_DRAW_H
#include LC_GRAPHLAYER_H

#ifdef __cplusplus
#include LC_GRAPH_HPP
#endif


#endif /* __LCUI_GRAPH_H__ */

