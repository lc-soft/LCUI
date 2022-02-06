#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CANVAS_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

#define PD_CANVAS_WRITING_CONTEXT(NAME, RECT) \
	pd_canvas_t ref_##NAME;\
	\
	if (!pd_canvas_is_valid(##NAME)) { \
		return -1; \
	}\
	pd_canvas_quote(&ref_##NAME, ##NAME, &RECT);\
	pd_canvas_get_valid_rect(&ref_##NAME, &RECT);\
	if (RECT.width < 1 || RECT.height < 1) {\
		return -1;\
	}\
	##NAME = pd_canvas_get_quote(&ref_##NAME);\

INLINE unsigned char *pd_canvas_get_pixel_pointer(pd_canvas_t *canvas, unsigned y, unsigned x)
{
	return canvas->bytes + canvas->bytes_per_row * y + (x % canvas->width) * canvas->bytes_per_pixel;
}

#define pd_canvas_get_pixel(G, X, Y, C)                                            \
	if ((G)->color_type == PD_COLOR_TYPE_ARGB) {                        \
		(C) = (G)->pixels[(G)->width * ((Y) % (G)->height) +            \
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

INLINE pd_bool pd_canvas_get_quote(pd_canvas_t *canvas)
{
	return canvas->quote.is_valid ? canvas->quote.source : canvas;
}

#endif
