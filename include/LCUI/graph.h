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

/* 将两个像素点的颜色值进行alpha混合 */
#define _ALPHA_BLEND(__back__, __fore__, __alpha__) \
	((((__fore__ - __back__) * (__alpha__)) >> 8) + __back__)

#define ALPHA_BLEND(__back__, __fore__, __alpha__)                      \
	{                                                               \
		__back__ = _ALPHA_BLEND(__back__, __fore__, __alpha__); \
	}

#define PIXEL_BLEND(px1, px2, a)                    \
	{                                           \
		ALPHA_BLEND((px1)->r, (px2)->r, a); \
		ALPHA_BLEND((px1)->g, (px2)->g, a); \
		ALPHA_BLEND((px1)->b, (px2)->b, a); \
	}

/* 获取像素的RGB值 */
#define RGB_FROM_RGB565(pixel, r, g, b)              \
	{                                            \
		r = (((pixel & 0xF800) >> 11) << 3); \
		g = (((pixel & 0x07E0) >> 5) << 2);  \
		b = ((pixel & 0x001F) << 3);         \
	}

#define RGB_FROM_RGB555(pixel, r, g, b)              \
	{                                            \
		r = (((pixel & 0x7C00) >> 10) << 3); \
		g = (((pixel & 0x03E0) >> 5) << 3);  \
		b = ((pixel & 0x001F) << 3);         \
	}

#define RGB_FROM_RGB888(pixel, r, g, b)         \
	{                                       \
		r = ((pixel & 0xFF0000) >> 16); \
		g = ((pixel & 0xFF00) >> 8);    \
		b = (pixel & 0xFF);             \
	}

/* 混合像素的RGB值 */
#define RGB565_FROM_RGB(pixel, r, g, b)                                \
	{                                                              \
		pixel = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3); \
	}

#define RGB555_FROM_RGB(pixel, r, g, b)                                \
	{                                                              \
		pixel = ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3); \
	}

#define RGB888_FROM_RGB(pixel, r, g, b)           \
	{                                         \
		pixel = (r << 16) | (g << 8) | b; \
	}

#define pd_graph_get_quote(g) ((g)->quote.is_valid ? (g)->quote.source : (g))

#define pd_graph_set_pixel(G, X, Y, C)                                        \
	if ((G)->color_type == LCUI_COLOR_TYPE_ARGB) {                    \
		(G)->argb[(G)->width * (Y) + (X)] = (C);                  \
	} else {                                                          \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3] = (C).b;     \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3 + 1] = (C).g; \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3 + 2] = (C).r; \
	}

#define pd_graph_set_pixel_alpha(G, X, Y, A) \
	(G)->argb[(G)->width * (Y) + (X)].alpha = (A)

#define pd_graph_get_pixel(G, X, Y, C)                                            \
	if ((G)->color_type == LCUI_COLOR_TYPE_ARGB) {                        \
		(C) = (G)->argb[(G)->width * ((Y) % (G)->height) +            \
				((X) % (G)->width)];                          \
	} else {                                                              \
		(C).value =                                                   \
		    (G)->bytes[(G)->bytes_per_row * ((Y) % (G)->height) +     \
			       ((X) % (G)->width) * (G)->bytes_per_pixel]     \
			<< 0 |                                                \
		    (G)->bytes[(G)->bytes_per_row * ((Y) % (G)->height) +     \
			       ((X) % (G)->width) * (G)->bytes_per_pixel + 1] \
			<< 8 |                                                \
		    (G)->bytes[(G)->bytes_per_row * ((Y) % (G)->height) +     \
			       ((X) % (G)->width) * (G)->bytes_per_pixel + 2] \
			<< 16 |                                               \
		    0xff << 24;                                               \
	}

#define pd_graph_get_pixel_pointer(G, X, Y) ((G)->argb + (G)->width * (Y) + (X))

/** 判断图像是否有Alpha通道 */
#define pd_graph_has_alpha(G)                                              \
	((G)->quote.is_valid                                           \
	     ? ((G)->quote.source->color_type == LCUI_COLOR_TYPE_ARGB) \
	     : ((G)->color_type == LCUI_COLOR_TYPE_ARGB))

#define pd_graph_is_writable(G)  \
	(pd_graph_is_valid(G) && \
	 ((G)->quote.is_valid ? (G)->quote.is_writable : TRUE))

/*
 * Pixel over operator with alpha channel
 * See more: https://en.wikipedia.org/wiki/Alpha_compositing
 */
INLINE void pd_over_pixel(LCUI_ARGB *dst, const LCUI_ARGB *src)
{
	/*
	 * Original formula:
	 *   Co = (Ca * aa + Cb * ab * (1 - aa)) / (aa + ab * (1 - aa))
	 *   ao = aa + ab * (1 - aa)
	 *
	 * Variable full name:
	 *   Co => colorOut
	 *   Ca => colorA
	 *   Cb => colorB
	 *   aa => colorA.alpha
	 *   ab => colorB.alpha
	 *   ao => colorOut.alpha
	 *
	 * The formula used in the code:
	 *   ai = ab * (1 - aa)
	 *   Co = (Ca * aa + Cb * ai) / (aa + ai)
	 *   ao = aa + ai
	 */
	double src_a = src->a / 255.0;
	double a = (1.0 - src_a) * dst->a / 255.0;
	double out_a = src_a + a;

	if (out_a > 0) {
		src_a /= out_a;
		a /= out_a;
	}
	dst->r = (unsigned char)(src->r * src_a + dst->r * a);
	dst->g = (unsigned char)(src->g * src_a + dst->g * a);
	dst->b = (unsigned char)(src->b * src_a + dst->b * a);
	dst->a = (unsigned char)(255.0 * out_a);

	/* If it is assumed that all color values are premultiplied by their
	 * alpha values, we can rewrite the equation for output color as:

		const double a = 1.0 - src->a / 255.0;
		dst->r = src->r + (unsigned char)(dst->r * a);
		dst->g = src->g + (unsigned char)(dst->g * a);
		dst->b = src->b + (unsigned char)(dst->b * a);
		dst->a = src->a + (unsigned char)(dst->a * a);
	*/
}

LCUI_API void Graph_PrintInfo(LCUI_Graph *graph);

LCUI_API void pd_graph_init(LCUI_Graph *graph);

LCUI_API LCUI_Color RGB(uchar_t r, uchar_t g, uchar_t b);

LCUI_API LCUI_Color pd_color(uchar_t a, uchar_t r, uchar_t g, uchar_t b);

LCUI_API void pixels_format(const uchar_t *in_pixels, int in_color_type,
			   uchar_t *out_pixels, int out_color_type,
			   size_t pixel_count);

/** 改变色彩类型 */
LCUI_API int Graph_SetColorType(LCUI_Graph *graph, int color_type);

LCUI_API int pd_graph_create(LCUI_Graph *graph, unsigned width, unsigned height);

LCUI_API void pd_graph_copy(LCUI_Graph *des, const LCUI_Graph *src);

LCUI_API void pd_graph_free(LCUI_Graph *graph);

/**
 * 为图像创建一个引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
LCUI_API int pd_graph_quote(LCUI_Graph *self, LCUI_Graph *source,
			 const LCUI_Rect *rect);

/**
 * 为图像创建一个只读引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
LCUI_API int pd_graph_quote_read_only(LCUI_Graph *self, const LCUI_Graph *source,
				 const LCUI_Rect *rect);

LCUI_API LCUI_BOOL pd_graph_is_valid(const LCUI_Graph *graph);

LCUI_API void pd_graph_get_valid_rect(const LCUI_Graph *graph, LCUI_Rect *rect);

LCUI_API int pd_graph_set_alpha_bits(LCUI_Graph *graph, uchar_t *a, size_t size);

LCUI_API int pd_graph_set_red_bits(LCUI_Graph *graph, uchar_t *r, size_t size);

LCUI_API int pd_graph_set_green_bits(LCUI_Graph *graph, uchar_t *g, size_t size);

LCUI_API int pd_graph_set_blue_bits(LCUI_Graph *graph, uchar_t *b, size_t size);

LCUI_API int pd_graph_zoom(const LCUI_Graph *graph, LCUI_Graph *buff,
			LCUI_BOOL keep_scale, int width, int height);

LCUI_API int pd_graph_zoom_bilinear(const LCUI_Graph *graph, LCUI_Graph *buff,
				LCUI_BOOL keep_scale, int width, int height);

LCUI_API int pd_graph_cut(const LCUI_Graph *graph, LCUI_Rect rect,
		       LCUI_Graph *buff);

LCUI_API int pd_graph_horiz_flip(const LCUI_Graph *graph, LCUI_Graph *buff);

LCUI_API int pd_graph_verti_flip(const LCUI_Graph *graph, LCUI_Graph *buff);

/**
 * 用颜色填充一块区域
 * @param[in][out] graph 需要填充的图层
 * @param[in] color 颜色
 * @param[in] rect 区域，若值为 NULL，则填充整个图层
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int pd_graph_fill_rect(LCUI_Graph *graph, LCUI_Color color,
			    LCUI_Rect *rect, LCUI_BOOL with_alpha);

LCUI_API int pd_graph_fill_alpha(LCUI_Graph *graph, uchar_t alpha);

LCUI_API int pd_graph_tile(LCUI_Graph *buff, const LCUI_Graph *graph,
			LCUI_BOOL replace, LCUI_BOOL with_alpha);

/**
 * 混合两张图层
 * 将前景图混合到背景图上
 * @param[in][out] back 背景图层
 * @param[in] fore 前景图层
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int pd_graph_mix(LCUI_Graph *back, const LCUI_Graph *fore, int left,
		       int top, LCUI_BOOL with_alpha);

LCUI_API int pd_graph_replace(LCUI_Graph *back, const LCUI_Graph *fore, int left,
			   int top);

LCUI_END_HEADER

#include <LCUI/draw.h>

#endif /* LCUI_GRAPH_H */
