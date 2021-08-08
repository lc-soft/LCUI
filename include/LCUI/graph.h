/*
 * canvas.h -- The base graphics handling module for LCUI
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

#include <LCUI/header.h>
#include <LCUI/types.h>

LCUI_BEGIN_HEADER

/* 解除RGB宏 */
#ifdef RGB
#undef RGB
#endif

/* 将两个像素点的颜色值进行alpha混合 */
#define _pd_alpha_blend(__back__, __fore__, __alpha__) \
	((((__fore__ - __back__) * (__alpha__)) >> 8) + __back__)

#define pd_alpha_blend(__back__, __fore__, __alpha__)                      \
	{                                                               \
		__back__ = _pd_alpha_blend(__back__, __fore__, __alpha__); \
	}

#define pd_pixel_blend(px1, px2, a)                    \
	{                                           \
		pd_alpha_blend((px1)->r, (px2)->r, a); \
		pd_alpha_blend((px1)->g, (px2)->g, a); \
		pd_alpha_blend((px1)->b, (px2)->b, a); \
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

#define pd_canvas_get_quote(g) ((g)->quote.is_valid ? (g)->quote.source : (g))

#define pd_canvas_set_pixel(G, X, Y, C)                                        \
	if ((G)->color_type == PD_COLOR_TYPE_ARGB) {                    \
		(G)->argb[(G)->width * (Y) + (X)] = (C);                  \
	} else {                                                          \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3] = (C).b;     \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3 + 1] = (C).g; \
		(G)->bytes[(G)->bytes_per_row * (Y) + (X)*3 + 2] = (C).r; \
	}

#define pd_canvas_set_pixel_alpha(G, X, Y, A) \
	(G)->argb[(G)->width * (Y) + (X)].alpha = (A)

#define pd_canvas_get_pixel(G, X, Y, C)                                            \
	if ((G)->color_type == PD_COLOR_TYPE_ARGB) {                        \
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

#define pd_canvas_get_pixel_pointer(G, X, Y) ((G)->argb + (G)->width * (Y) + (X))

/** 判断图像是否有Alpha通道 */
#define Graph_HasAlpha(G)                                              \
	((G)->quote.is_valid                                           \
	     ? ((G)->quote.source->color_type == PD_COLOR_TYPE_ARGB) \
	     : ((G)->color_type == PD_COLOR_TYPE_ARGB))

#define pd_canvas_is_writable(G)  \
	(pd_canvas_is_valid(G) && \
	 ((G)->quote.is_valid ? (G)->quote.is_writable : TRUE))

/*
 * Pixel over operator with alpha channel
 * See more: https://en.wikipedia.org/wiki/Alpha_compositing
 */
INLINE void pd_over_pixel(pd_color_t *dst, const pd_color_t *src)
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

LCUI_API void pd_canvas_print_info(pd_canvas_t *canvas);

LCUI_API void pd_canvas_init(pd_canvas_t *canvas);

LCUI_API pd_color_t RGB(uchar_t r, uchar_t g, uchar_t b);

LCUI_API pd_color_t ARGB(uchar_t a, uchar_t r, uchar_t g, uchar_t b);

LCUI_API void pd_pixels_formmat(const uchar_t *in_pixels, int in_color_type,
			   uchar_t *out_pixels, int out_color_type,
			   size_t pixel_count);

/** 改变色彩类型 */
LCUI_API int pd_canvas_set_color_type(pd_canvas_t *canvas, int color_type);

LCUI_API int pd_canvas_create(pd_canvas_t *canvas, unsigned width, unsigned height);

LCUI_API void pd_canvas_copy(pd_canvas_t *des, const pd_canvas_t *src);

LCUI_API void pd_canvas_free(pd_canvas_t *canvas);

/**
 * 为图像创建一个引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
LCUI_API int pd_canvas_quote(pd_canvas_t *self, pd_canvas_t *source,
			 const pd_rect_t *rect);

/**
 * 为图像创建一个只读引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
LCUI_API int pd_canvas_quote_read_only(pd_canvas_t *self, const pd_canvas_t *source,
				 const pd_rect_t *rect);

LCUI_API pd_bool pd_canvas_is_valid(const pd_canvas_t *canvas);

LCUI_API void pd_canvas_get_valid_rect(const pd_canvas_t *canvas, pd_rect_t *rect);

LCUI_API int pd_canvas_set_alpha_bits(pd_canvas_t *canvas, uchar_t *a, size_t size);

LCUI_API int pd_canvas_set_red_bits(pd_canvas_t *canvas, uchar_t *r, size_t size);

LCUI_API int pd_canvas_set_green_bits(pd_canvas_t *canvas, uchar_t *g, size_t size);

LCUI_API int pd_canvas_set_blue_bits(pd_canvas_t *canvas, uchar_t *b, size_t size);

LCUI_API int pd_canvas_zoom(const pd_canvas_t *canvas, pd_canvas_t *buff,
			pd_bool keep_scale, int width, int height);

LCUI_API int pd_canvas_zoom_bilinear(const pd_canvas_t *canvas, pd_canvas_t *buff,
				pd_bool keep_scale, int width, int height);

LCUI_API int pd_canvas_cut(const pd_canvas_t *canvas, pd_rect_t rect,
		       pd_canvas_t *buff);

LCUI_API int pd_canvas_horzi_flip(const pd_canvas_t *canvas, pd_canvas_t *buff);

LCUI_API int pd_canvas_verti_flip(const pd_canvas_t *canvas, pd_canvas_t *buff);

/**
 * 用颜色填充一块区域
 * @param[in][out] canvas 需要填充的图层
 * @param[in] color 颜色
 * @param[in] rect 区域，若值为 NULL，则填充整个图层
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int pd_canvas_fill_rect(pd_canvas_t *canvas, pd_color_t color,
			    pd_rect_t *rect, pd_bool with_alpha);

LCUI_API int pd_canvas_fill_alpha(pd_canvas_t *canvas, uchar_t alpha);

LCUI_API int pd_canvas_tile(pd_canvas_t *buff, const pd_canvas_t *canvas,
			pd_bool replace, pd_bool with_alpha);

/**
 * 混合两张图层
 * 将前景图混合到背景图上
 * @param[in][out] back 背景图层
 * @param[in] fore 前景图层
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 * @param[in] with_alpha 是否需要处理alpha通道
 */
LCUI_API int pd_canvas_mix(pd_canvas_t *back, const pd_canvas_t *fore, int left,
		       int top, pd_bool with_alpha);

LCUI_API int pd_canvas_replace(pd_canvas_t *back, const pd_canvas_t *fore, int left,
			   int top);

LCUI_END_HEADER

#include <LCUI/draw.h>

#endif /* LCUI_GRAPH_H */
