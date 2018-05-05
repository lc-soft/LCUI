
#ifndef LCUI_DRAW_LINE_H
#define LCUI_DRAW_LINE_H

LCUI_API void Graph_DrawHorizLine(LCUI_Graph *graph, LCUI_Color color,
				  int size, LCUI_Pos start, int end_x);

LCUI_API void Graph_DrawVertiLine(LCUI_Graph *graph, LCUI_Color color,
				  int size, LCUI_Pos start, int end_y);

#endif
