/*
 * graph.h -- The base graphics handling module for LCUI
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LCUI_GRAPH_H
#define LCUI_GRAPH_H

LCUI_BEGIN_HEADER

/* 解除RGB宏 */
#ifdef RGB
#undef RGB
#endif

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
	ALPHA_BLEND( (px1)->r, (px2)->r, a );	\
	ALPHA_BLEND( (px1)->g, (px2)->g, a );	\
	ALPHA_BLEND( (px1)->b, (px2)->b, a );	\
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

#define Graph_GetQuote(g) (g)->quote.is_valid ? (g)->quote.source:(g)

#define Graph_SetPixel(G, X, Y, C) 						\
	if( (G)->color_type == COLOR_TYPE_ARGB ) {				\
		(G)->argb[(G)->width*(Y)+(X)] = (C);			\
	} else {								\
		(G)->bytes[(G)->bytes_per_row*(Y)+(X)*3] = (C).value>>8;	\
	}

#define Graph_SetPixelAlpha(G, X, Y, A)\
(G)->argb[(G)->width*(Y)+(X)].alpha = (A)

LCUI_API void Graph_PrintInfo( LCUI_Graph *graph );

LCUI_API void Graph_Init( LCUI_Graph *graph );

LCUI_API LCUI_Graph *Graph_New(void);

LCUI_API void Graph_Delete( LCUI_Graph *graph );

LCUI_API LCUI_Color RGB( uchar_t r, uchar_t g, uchar_t b );

LCUI_API LCUI_Color ARGB( uchar_t a, uchar_t r, uchar_t g, uchar_t b );

LCUI_API void PixelsFormat( const uchar_t *in_pixels, int in_color_type,
		   	    uchar_t *out_pixels, int out_color_type,
		   	    size_t pixel_count );

/** 改变色彩类型 */
LCUI_API int Graph_SetColorType( LCUI_Graph *graph, int color_type );

LCUI_API int Graph_Create( LCUI_Graph *graph, size_t width, size_t height );

LCUI_API void Graph_Copy( LCUI_Graph *des, const LCUI_Graph *src );

LCUI_API void Graph_Free( LCUI_Graph *graph );

/**
 * 为图像创建一个引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
LCUI_API int Graph_Quote( LCUI_Graph *self, LCUI_Graph *source, const LCUI_Rect *rect );

/**
* 为图像创建一个只读引用
* @param self 用于存放图像引用的缓存区
* @param source 引用的源图像
* &param rect 引用的区域，若为NULL，则引用整个图像
*/
LCUI_API int Graph_QuoteReadOnly( LCUI_Graph *self,
				  const LCUI_Graph *source,
				  const LCUI_Rect *rect );

/** 判断图像是否有Alpha透明通道 */
#define Graph_HasAlpha(G) 						\
	((G)->quote.is_valid ? (					\
		(G)->quote.source->color_type == COLOR_TYPE_ARGB	\
	) : ((G)->color_type == COLOR_TYPE_ARGB))

/** 判断图像是否有效 */
#define Graph_IsValid(G)							\
	((G)->quote.is_valid ? ((G)->quote.source				\
	 && (G)->quote.source->width > 0 && (G)->quote.source->height > 0	\
	) : ((G) && (G)->bytes && (G)->height > 0 && (G)->width > 0))

#define Graph_IsWritable(G) (Graph_IsValid(G) && \
	((G)->quote.is_valid ? (G)->quote.is_writable : TRUE))

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

/**
 * 用颜色填充一块区域
 * @param[in][out] graph 需要填充的图层
 * @param[in] color 颜色
 * @param[in] rect 区域，若值为 NULL，则填充整个图层
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int Graph_FillRect( LCUI_Graph *graph, LCUI_Color color,
			     LCUI_Rect *rect, LCUI_BOOL with_alpha );

LCUI_API int Graph_FillAlpha( LCUI_Graph *graph, uchar_t alpha );

LCUI_API int Graph_Tile( LCUI_Graph *buff, const LCUI_Graph *graph,
			 LCUI_BOOL replace, LCUI_BOOL with_alpha );

/**
 * 混合两张图层
 * 将前景图混合到背景图上
 * @param[in][out] back 背景图层
 * @param[in] fore 前景图层
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int Graph_Mix( LCUI_Graph *back, const LCUI_Graph *fore,
			int left, int top, LCUI_BOOL with_alpha );

LCUI_API int Graph_Replace( LCUI_Graph *back, const LCUI_Graph *fore, int left, int top );

LCUI_END_HEADER

#include <LCUI/draw.h>

#endif /* LCUI_GRAPH_H */
