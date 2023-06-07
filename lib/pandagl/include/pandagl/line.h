
#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC void pd_canvas_draw_horizontal_line(pd_canvas_t *canvas,
					     pd_color_t color, int size,
					     pd_pos_t start, int end_x);

PD_PUBLIC void pd_canvas_draw_vertical_line(pd_canvas_t *canvas,
					   pd_color_t color, int size,
					   pd_pos_t start, int end_y);

PD_INLINE void pd_canvas_draw_hline(pd_canvas_t *canvas, pd_color_t color,
				 int size, pd_pos_t start, int end_x)
{
	pd_canvas_draw_horizontal_line(canvas, color, size, start, end_x);
}

PD_INLINE void pd_canvas_draw_vline(pd_canvas_t *canvas, pd_color_t color, int size,
			    pd_pos_t start, int end_y)
{
	pd_canvas_draw_vertical_line(canvas, color, size, start, end_y);
}

PD_END_DECLS

#endif
