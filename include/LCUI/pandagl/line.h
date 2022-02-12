
#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API void pd_canvas_draw_horizontal_line(pd_canvas_t *canvas,
					     pd_color_t color, int size,
					     pd_pos_t start, int end_x);

LCUI_API void pd_canvas_draw_vertical_line(pd_canvas_t *canvas,
					   pd_color_t color, int size,
					   pd_pos_t start, int end_y);

INLINE void pd_canvas_draw_hline(pd_canvas_t *canvas, pd_color_t color,
				 int size, pd_pos_t start, int end_x)
{
	pd_canvas_draw_horizontal_line(canvas, color, size, start, end_x);
}

INLINE pd_canvas_draw_vline(pd_canvas_t *canvas, pd_color_t color, int size,
			    pd_pos_t start, int end_y)
{
	pd_canvas_draw_vertical_line(canvas, color, size, start, end_y);
}

LCUI_END_HEADER

#endif
