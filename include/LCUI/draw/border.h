
#ifndef __LCUI_DRAW_BORDER_H__
#define __LCUI_DRAW_BORDER_H__

/*************** 边框风格 *****************/
typedef enum _BORDER_STYLE
{
	BORDER_STYLE_NONE,	/* 无边框 */
	BORDER_STYLE_SOLID,	/* 实线 */
	BORDER_STYLE_DOTTED,	/* 点状 */
	BORDER_STYLE_DOUBLE,	/* 双线 */
	BORDER_STYLE_DASHED,	/* 虚线 */
}
BORDER_STYLE;
/*****************************************/

/* 完整的边框信息 */
typedef struct _LCUI_Border
{
	int top_width;
	int bottom_width;
	int left_width;
	int right_width;
	BORDER_STYLE top_style;
	BORDER_STYLE bottom_style;
	BORDER_STYLE left_style;
	BORDER_STYLE right_style;
	LCUI_RGB top_color;
	LCUI_RGB bottom_color;
	LCUI_RGB left_color;
	LCUI_RGB right_color;
	int top_left_radius;
	int top_right_radius;
	int bottom_left_radius;
	int bottom_right_radius;
}
LCUI_Border;

LCUI_BEGIN_HEADER

void Border_Init( LCUI_Border *border );
/* 初始化边框数据 */

LCUI_Border Border( int width_px, BORDER_STYLE style, LCUI_RGB color );
/* 简单的设置边框样式，并获取该样式数据 */

void Border_Radius( LCUI_Border *border, int radius );
/* 设置边框的圆角半径 */

int Graph_Draw_Border( LCUI_Graph *des, LCUI_Border border );
/* 简单的为图形边缘绘制边框 */

LCUI_END_HEADER

#endif
