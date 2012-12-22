
#ifndef __LCUI_DRAW_BORDER_H__
#define __LCUI_DRAW_BORDER_H__

LCUI_Border Border( int width_px, BORDER_STYLE style, LCUI_RGB color );
/* 简单的设置边框样式，并获取该样式数据 */

void Border_Init( LCUI_Border *border );
/* 初始化边框数据 */

int Graph_Draw_Border( LCUI_Graph *des, LCUI_Border border );
/* 简单的为图形边缘绘制边框 */

#endif
