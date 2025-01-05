/*
 * lib/pandagl/include/pandagl/canvas.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_INLINE void *pd_canvas_pixel_at(const pd_canvas_t *canvas, unsigned x,
				unsigned y)
{
	return canvas->bytes + canvas->bytes_per_row * y +
	       (x % canvas->width) * canvas->bytes_per_pixel;
}

PD_INLINE pd_color_t pd_canvas_get_pixel(const pd_canvas_t *canvas, unsigned x,
				      unsigned y)
{
	pd_color_t color;
	unsigned char *p = pd_canvas_pixel_at(canvas, x, y);

	if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
		return *(pd_color_t *)p;
	}
	color.b = p[0];
	color.g = p[1];
	color.r = p[2];
	color.a = 255;
	return color;
}

PD_INLINE void pd_canvas_set_pixel(pd_canvas_t *canvas, unsigned x, unsigned y,
				pd_color_t pixel)
{
	unsigned char *p = pd_canvas_pixel_at(canvas, x, y);

	if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
		*(pd_color_t *)p = pixel;
	} else {
		p[0] = pixel.b;
		p[1] = pixel.g;
		p[2] = pixel.r;
	}
}

PD_INLINE pd_canvas_t *pd_canvas_get_quote_source(pd_canvas_t *canvas)
{
	return canvas->quote.is_valid ? canvas->quote.source : canvas;
}

PD_INLINE const pd_canvas_t *pd_canvas_get_quote_source_readonly(
    const pd_canvas_t *canvas)
{
	return canvas->quote.is_valid ? canvas->quote.source : canvas;
}

PD_PUBLIC void pd_canvas_init(pd_canvas_t *canvas);

PD_PUBLIC bool pd_canvas_is_valid(const pd_canvas_t *canvas);

PD_PUBLIC int pd_canvas_quote(pd_canvas_t *self, pd_canvas_t *source,
			     const pd_rect_t *rect);

PD_PUBLIC void pd_canvas_get_quote_rect(const pd_canvas_t *canvas,
				       pd_rect_t *rect);

PD_PUBLIC void pd_canvas_destroy(pd_canvas_t *canvas);

PD_PUBLIC int pd_canvas_create(pd_canvas_t *canvas, unsigned width,
			      unsigned height);

PD_PUBLIC int pd_canvas_replace(pd_canvas_t *back, const pd_canvas_t *fore,
			       int left, int top);

PD_PUBLIC int pd_canvas_set_color_type(pd_canvas_t *canvas, int color_type);

PD_PUBLIC int pd_canvas_cut(const pd_canvas_t *canvas, pd_rect_t rect,
			   pd_canvas_t *out_canvas);

PD_PUBLIC void pd_canvas_copy(pd_canvas_t *des, const pd_canvas_t *src);

PD_PUBLIC int pd_canvas_mix(pd_canvas_t *back, const pd_canvas_t *fore, int left,
			   int top, bool with_alpha);

PD_PUBLIC int pd_canvas_fill_rect(pd_canvas_t *canvas, pd_color_t color,
				 pd_rect_t rect);

PD_INLINE int pd_canvas_fill(pd_canvas_t *canvas, pd_color_t color)
{
	pd_rect_t rect = { 0, 0, canvas->width, canvas->height };
	return pd_canvas_fill_rect(canvas, color, rect);
}

PD_PUBLIC int pd_canvas_tile(pd_canvas_t *buff, const pd_canvas_t *canvas,
			    bool replace, bool with_alpha);

PD_PUBLIC int pd_canvas_veri_flip(const pd_canvas_t *canvas, pd_canvas_t *buff);

PD_PUBLIC int pd_canvas_horiz_flip(const pd_canvas_t *canvas, pd_canvas_t *buff);

PD_PUBLIC int pd_canvas_zoom(const pd_canvas_t *canvas, pd_canvas_t *buff,
			    bool keep_scale, int width, int height);

PD_PUBLIC int pd_canvas_zoom_bilinear(const pd_canvas_t *canvas,
				     pd_canvas_t *buff, bool keep_scale,
				     int width, int height);

PD_END_DECLS

#endif
