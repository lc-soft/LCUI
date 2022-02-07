
#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_LINE_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API void pd_graph_draw_horiz_line(pd_canvas_t *canvas, pd_color_t color,
				  int size, pd_pos_t start, int end_x);

LCUI_API void pd_graph_draw_verti_line(pd_canvas_t *canvas, pd_color_t color,
				  int size, pd_pos_t start, int end_y);

LCUI_END_HEADER

#endif
