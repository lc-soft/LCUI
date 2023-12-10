/*
 * lib/pandagl/include/pandagl/pixel.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_PIXEL_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_PIXEL_H

#include "common.h"
#include "types.h"

#define _pd_alpha_blend(__back__, __fore__, __alpha__) \
	((((__fore__ - __back__) * (__alpha__)) >> 8) + __back__)

#define pd_alpha_blend(__back__, __fore__, __alpha__)                      \
	{                                                               \
		__back__ = _pd_alpha_blend(__back__, __fore__, __alpha__); \
	}

#define pd_blend_pixel(px1, px2, a)                    \
	{                                           \
		pd_alpha_blend((px1)->r, (px2)->r, a); \
		pd_alpha_blend((px1)->g, (px2)->g, a); \
		pd_alpha_blend((px1)->b, (px2)->b, a); \
	}

PD_BEGIN_DECLS

PD_PUBLIC unsigned pd_get_pixel_size(pd_color_type_t color_type);

PD_PUBLIC unsigned pd_get_pixel_row_size(pd_color_type_t color_type, size_t len);

PD_PUBLIC int pd_format_pixels(const uint8_t *in_pixels,
				   pd_color_type_t in_color_type,
				   uint8_t *out_pixels,
				   pd_color_type_t out_color_type,
				   size_t count);

/*
 * Pixel over operator with alpha channel
 * See more: https://en.wikipedia.org/wiki/Alpha_compositing
 */
PD_INLINE void pd_over_pixel(pd_color_t *dst, const pd_color_t *src, double opacity)
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
	double src_a = src->a * opacity / 255.0;
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

PD_END_DECLS

#endif
