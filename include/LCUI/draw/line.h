
#ifndef LCUI_DRAW_LINE_H
#define LCUI_DRAW_LINE_H

LCUI_API void pd_graph_draw_horiz_line(pd_canvas_t *graph, pd_color_t color,
				  int size, pd_pos_t start, int end_x);

LCUI_API void pd_graph_draw_verti_line(pd_canvas_t *graph, pd_color_t color,
				  int size, pd_pos_t start, int end_y);

#endif
