
#ifndef __LCUI_DRAW_LINE_H__
#define __LCUI_DRAW_LINE_H__
/* 绘制水平线 */
LCUI_API void
Graph_DrawHorizLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_x );

/* 绘制垂直线 */
LCUI_API void
Graph_DrawVertiLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_y );

/* 绘制线条框 */
LCUI_API void
Graph_DrawBoxLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Rect rect );
#endif