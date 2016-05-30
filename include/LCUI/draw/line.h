
#ifndef LCUI_DRAW_LINE_H
#define LCUI_DRAW_LINE_H
/* 绘制水平线 */
LCUI_API void
Graph_DrawHorizLine( LCUI_Graph *graph, LCUI_Color color, int size, LCUI_Pos start, int end_x );

/* 绘制垂直线 */
LCUI_API void
Graph_DrawVertiLine( LCUI_Graph *graph, LCUI_Color color, int size, LCUI_Pos start, int end_y );

#endif