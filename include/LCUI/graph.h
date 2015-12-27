/* ***************************************************************************
 * LCUI_Graph.h -- The base graphics handling module for LCUI
 *
 * Copyright (C) 2012-2015 by
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
 * 版权所有 (C) 2012-2015 归属于
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
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果s
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ***************************************************************************/
#ifndef __LCUI_GRAPH_H__
#define __LCUI_GRAPH_H__

LCUI_BEGIN_HEADER

#define COLOR_TURQUOISE		RGB(26, 188, 156)
#define COLOR_EMERALD		RGB(46, 204, 113)
#define COLOR_PETER_RIVER	RGB(52, 152, 219)
#define COLOR_AMETHYST		RGB(155, 89, 182)
#define COLOR_WET_ASPHALT	RGB(52, 73, 94)
#define COLOR_GREEN_SEA		RGB(22, 160, 133)
#define COLOR_NEPHRITIS		RGB(39, 174, 96)
#define COLOR_BELIZE_HOLE	RGB(41, 128, 185)
#define COLOR_WISTERIA		RGB(142, 68, 173)
#define COLOR_MIDNIGHT_BLUE	RGB(44, 62, 80)
#define COLOR_SUN_FLOWER	RGB(241, 196, 15)
#define COLOR_CARROT		RGB(230, 126, 34)
#define COLOR_ALIZARIN		RGB(231, 76, 60)
#define COLOR_CLOUNDS		RGB(236, 240, 241)
#define COLOR_CONCRETE		RGB(149, 165, 166)
#define COLOR_ORANGE		RGB(243, 156, 18)
#define COLOR_PUMPKIN		RGB(211, 84, 0)
#define COLOR_POMEGRANATE	RGB(192, 57, 43)
#define COLOR_SILVER		RGB(189, 195, 199)
#define COLOR_ASBESTOS		RGB(127, 140, 141)

/** 色彩模式 */
enum GraphColorType {
	COLOR_TYPE_INDEX8,	/**< 8位索引 */
	COLOR_TYPE_GRAY8,	/**< 8位灰度 */
	COLOR_TYPE_RGB323,	/**< RGB323 */
	COLOR_TYPE_ARGB2222,	/**< ARGB2222 */
	COLOR_TYPE_RGB555,	/**< RGB555 */
	COLOR_TYPE_RGB565,	/**< RGB565 */
	COLOR_TYPE_RGB888,	/**< RGB888 */
	COLOR_TYPE_ARGB8888	/**< RGB8888 */
};

#define COLOR_TYPE_RGB COLOR_TYPE_RGB888
#define COLOR_TYPE_ARGB COLOR_TYPE_ARGB8888

/* 将两个像素点的颜色值进行alpha混合 */
#define _ALPHA_BLEND(__back__ , __fore__, __alpha__)	\
    ((((__fore__-__back__)*(__alpha__))>>8)+__back__)

#define ALPHA_BLEND(__back__ , __fore__, __alpha__)		\
{								\
    __back__ =_ALPHA_BLEND(__back__,__fore__,__alpha__);	\
}

#define PIXEL_BLEND(px1, px2, a) {		\
	ALPHA_BLEND( (px1)->r, (px1)->r, a );	\
	ALPHA_BLEND( (px1)->g, (px1)->g, a );	\
	ALPHA_BLEND( (px1)->b, (px1)->b, a );	\
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

#define Graph_GetQuote(g) (g)->quote.is_valid ? (g)->quote.source:(g)

#define Graph_SetPixel(G, X, Y, C) 				\
	if( (G)->color_type == COLOR_TYPE_ARGB ) {		\
		(G)->argb[(G)->w*(Y)+(X)] = (C);		\
	} else {						\
		(G)->bytes[((G)->w*(Y)+(X))*3] = (C).value>>8;	\
	}

#define Graph_SetPixelAlpha(G, X, Y, A) (G)->argb[(G)->w*(Y)+(X)].alpha = (A)

LCUI_API void Graph_PrintInfo( LCUI_Graph *graph );

LCUI_API void Graph_Init( LCUI_Graph *graph );

LCUI_API LCUI_Graph *Graph_New(void);

LCUI_API LCUI_Color RGB( uchar_t r, uchar_t g, uchar_t b );

LCUI_API LCUI_Color ARGB( uchar_t a, uchar_t r, uchar_t g, uchar_t b );

LCUI_API void PixelsFormat( const uchar_t *in_pixels, int in_color_type,
		   	    uchar_t *out_pixels, int out_color_type,
		   	    size_t pixel_count );

/** 改变色彩类型 */
LCUI_API int Graph_SetColorType( LCUI_Graph *graph, int color_type );

LCUI_API int Graph_Create( LCUI_Graph *graph, int w, int h );

LCUI_API void Graph_Copy( LCUI_Graph *des, const LCUI_Graph *src );

LCUI_API void Graph_Free( LCUI_Graph *graph );

/**
* 为图像创建一个引用
* @param self	用于存放图像引用的缓存区
* @param source	引用的源图像
* &param rect	引用的区域，若为NULL，则引用整个图像
*/
LCUI_API int Graph_Quote( LCUI_Graph *self, LCUI_Graph *source, const LCUI_Rect *rect );

/** 判断图像是否有Alpha透明通道 */
#define Graph_HasAlpha(G) 						\
	((G)->quote.is_valid ? (					\
		(G)->quote.source->color_type == COLOR_TYPE_ARGB	\
	) : ((G)->color_type == COLOR_TYPE_ARGB))

/** 判断图像是否有效 */
#define Graph_IsValid(G)						\
	((G)->quote.is_valid ? ((G)->quote.source			\
	 && (G)->quote.source->w > 0 && (G)->quote.source->h > 0	\
	) : ((G) && (G)->bytes && (G)->h > 0 && (G)->w > 0))

#define Graph_GetSize(G, S) ((S)->w = (G)->w, (S)->h = (G)->h)

LCUI_API void Graph_GetValidRect( const LCUI_Graph *graph, LCUI_Rect *rect );

LCUI_API int Graph_SetAlphaBits( LCUI_Graph *graph, uchar_t *a, size_t size );

LCUI_API int Graph_SetRedBits( LCUI_Graph *graph, uchar_t *r, size_t size );

LCUI_API int Graph_SetGreenBits( LCUI_Graph *graph, uchar_t *g, size_t size );

LCUI_API int Graph_SetBlueBits( LCUI_Graph *graph, uchar_t *b, size_t size );

LCUI_API int Graph_Zoom( const LCUI_Graph *graph, LCUI_Graph *buff,
			 LCUI_BOOL keep_scale, int width, int height );

LCUI_API int Graph_Cut( const LCUI_Graph *graph, LCUI_Rect rect,
		        LCUI_Graph *buff );

LCUI_API int Graph_HorizFlip( const LCUI_Graph *graph, LCUI_Graph *buff );

LCUI_API int Graph_VertiFlip( const LCUI_Graph *graph, LCUI_Graph *buff );

LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_Color color,
				LCUI_Rect rect );

LCUI_API int Graph_FillColor( LCUI_Graph *graph, LCUI_Color color );

LCUI_API int Graph_FillAlpha( LCUI_Graph *graph, uchar_t alpha );

LCUI_API int Graph_Tile( LCUI_Graph *buff,  const LCUI_Graph *graph,
			 LCUI_BOOL replace );

/** 混合两张图层
 * 将前景图混合到背景图上
 * @param[in][out] back 背景图层
 * @param[in] fore 前景图层
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 */
LCUI_API int Graph_Mix( LCUI_Graph *back, const LCUI_Graph *fore, int left, int top );

/** 混合两张图层，忽略背景图层的Alpha通道
 * 该函数主要针对不透明但又有Alpha通道的背景图，省去了多余的Alpha通道混合步骤。
 * @warning 两张图层必须都有Alpha通道
 * @param[in][out] back 背景图层
 * @param[in] fore 前景图层
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 */
LCUI_API int Graph_Mix2( LCUI_Graph *back, const LCUI_Graph *fore, int left, int top );

LCUI_API int Graph_Replace( LCUI_Graph *back, const LCUI_Graph *fore, int left, int top );

LCUI_END_HEADER

#include <LCUI/draw.h>

#endif /* __LCUI_GRAPH_H__ */
