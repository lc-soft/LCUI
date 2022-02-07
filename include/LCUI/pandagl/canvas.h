#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

LCUI_BEGIN_HEADER

INLINE void *pd_canvas_pixel_at(const pd_canvas_t *canvas, unsigned x,
				unsigned y)
{
	return canvas->bytes + canvas->bytes_per_row * y +
	       (x % canvas->width) * canvas->bytes_per_pixel;
}

INLINE pd_color_t pd_canvas_get_pixel(const pd_canvas_t *canvas, unsigned x,
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

INLINE void pd_canvas_set_pixel(pd_canvas_t *canvas, unsigned x, unsigned y,
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

INLINE pd_canvas_t *pd_canvas_get_quote_source(pd_canvas_t *canvas)
{
	return canvas->quote.is_valid ? canvas->quote.source : canvas;
}

INLINE const pd_canvas_t *pd_canvas_get_quote_source_readonly(
    const pd_canvas_t *canvas)
{
	return canvas->quote.is_valid ? canvas->quote.source : canvas;
}

#define PD_CANVAS_READING_CONTEXT(CANVAS)                   \
	pd_rect_t CANVAS##_rect;                            \
	pd_canvas_get_quote_rect(##CANVAS, &CANVAS##_rect); \
	##CANVAS = pd_canvas_get_quote_source_readonly(##CANVAS);

#define PD_CANVAS_ROW_READING_BEGIN(CANVAS)                                 \
	int CANVAS##_y;                                                     \
	unsigned char *CANVAS##_row;                                        \
                                                                            \
	CANVAS##_row =                                                      \
	    pd_canvas_pixel_at(##CANVAS, CANVAS##_rect.x, CANVAS##_rect.y); \
	for (CANVAS##_y = 0; CANVAS##_y < CANVAS##_rect.height; ++CANVAS##_y) {
#define PD_CANVAS_ROW_READING_END(CANVAS)        \
	CANVAS##_row += ##CANVAS->bytes_per_row; \
	}

#define PD_CANVAS_ROW_WRITING_BEGIN(CANVAS, RECT)                        \
	int CANVAS##_y;                                                  \
	unsigned char *CANVAS##_row;                                     \
                                                                         \
	CANVAS##_row = pd_canvas_pixel_at(##CANVAS, RECT##.x, RECT##.y); \
	for (CANVAS##_y = 0; CANVAS##_y < RECT##.height; ++CANVAS##_y) {
#define PD_CANVAS_ARGB_PIXEL_BEGIN(CANVAS, RECT, PIXEL)      \
	if (canvas->color_type == PD_COLOR_TYPE_ARGB) {      \
		int CANVAS##_x;                              \
		for (CANVAS##_x = 0, ##PIXEL = CANVAS##_row; \
		     CANVAS##_x < RECT##.width; ++CANVAS##_x, ##PIXEL += 4) {
#define PD_CANVAS_ARGB_PIXEL_END \
	}                        \
	}
#define PD_CANVAS_RGB_PIXEL_BEGIN(CANVAS, RECT, PIXEL)       \
	if (canvas->color_type == PD_COLOR_TYPE_RGB) {       \
		int CANVAS##_x;                              \
		for (CANVAS##_x = 0, ##PIXEL = CANVAS##_row; \
		     CANVAS##_x < RECT##.width; ++CANVAS##_x, ##PIXEL += 3) {
#define PD_CANVAS_RGB_PIXEL_END \
	}                       \
	}

#define PD_CANVAS_ROW_WRITING_END(CANVAS)        \
	CANVAS##_row += ##CANVAS->bytes_per_row; \
	}

LCUI_API void pd_canvas_init(pd_canvas_t *canvas);

LCUI_API pd_bool pd_canvas_is_valid(const pd_canvas_t *canvas);

LCUI_API int pd_canvas_quote(pd_canvas_t *self, pd_canvas_t *source,
			     const pd_rect_t *rect);

LCUI_API void pd_canvas_get_quote_rect(const pd_canvas_t *canvas,
				       pd_rect_t *rect);

LCUI_API void pd_canvas_destroy(pd_canvas_t *canvas);

LCUI_API int pd_canvas_create(pd_canvas_t *canvas, unsigned width,
			      unsigned height);

LCUI_API int pd_canvas_replace(pd_canvas_t *back, const pd_canvas_t *fore,
			       int left, int top);

LCUI_API int pd_canvas_set_color_type(pd_canvas_t *canvas, int color_type);

LCUI_API int pd_canvas_cut(const pd_canvas_t *canvas, pd_rect_t rect,
			   pd_canvas_t *out_canvas);

LCUI_API void pd_canvas_copy(pd_canvas_t *des, const pd_canvas_t *src);

LCUI_API int pd_canvas_mix(pd_canvas_t *back, const pd_canvas_t *fore, int left,
			   int top, pd_bool with_alpha);

LCUI_API int pd_canvas_fill_rect(pd_canvas_t *canvas, pd_color_t color,
				 pd_rect_t rect);

INLINE int pd_canvas_fill(pd_canvas_t *canvas, pd_color_t color)
{
	pd_rect_t rect = { 0, 0, canvas->width, canvas->height };
	return pd_canvas_fill_rect(canvas, color, rect);
}

LCUI_API int pd_canvas_tile(pd_canvas_t *buff, const pd_canvas_t *canvas,
			    pd_bool replace, pd_bool with_alpha);

LCUI_API int pd_canvas_veri_flip(const pd_canvas_t *canvas, pd_canvas_t *buff);

LCUI_API int pd_canvas_zoom(const pd_canvas_t *canvas, pd_canvas_t *buff,
			    pd_bool keep_scale, int width, int height);

LCUI_API int pd_canvas_zoom_bilinear(const pd_canvas_t *canvas,
				     pd_canvas_t *buff, pd_bool keep_scale,
				     int width, int height);

LCUI_END_HEADER

#endif
