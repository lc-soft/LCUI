#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_BORDER_H
#include LC_DRAW_LINE_H
#include <math.h>

/** 初始化边框数据 */
LCUI_API void Border_Init( LCUI_Border *border )
{
	border->top_width = 0;
	border->bottom_width = 0;
	border->left_width = 0;
	border->right_width = 0;
	border->top_style = BORDER_STYLE_NONE;
	border->bottom_style = BORDER_STYLE_NONE;
	border->left_style = BORDER_STYLE_NONE;
	border->right_style = BORDER_STYLE_NONE;
	border->top_color = RGB(0,0,0);
	border->bottom_color = RGB(0,0,0);
	border->left_color = RGB(0,0,0);
	border->right_color = RGB(0,0,0);
	border->top_left_radius = 0;
	border->top_right_radius = 0;
	border->bottom_left_radius = 0;
	border->bottom_right_radius = 0;
}

/** 简单的设置边框样式，并获取该样式数据 */
LCUI_API LCUI_Border Border( unsigned int width_px, BORDER_STYLE style, LCUI_Color color )
{
	LCUI_Border border;
	border.top_width = width_px;
	border.bottom_width = width_px;
	border.left_width = width_px;
	border.right_width = width_px;
	border.top_style = style;
	border.bottom_style = style;
	border.left_style = style;
	border.right_style = style;
	border.top_color = color;
	border.bottom_color = color;
	border.left_color = color;
	border.right_color = color;
	border.top_left_radius = 0;
	border.top_right_radius = 0;
	border.bottom_left_radius = 0;
	border.bottom_right_radius = 0;
	return border; 
}

/** 设置边框的圆角半径 */
LCUI_API void Border_Radius( LCUI_Border *border, unsigned int radius )
{
	border->top_left_radius = radius;
	border->top_right_radius = radius;
	border->bottom_left_radius = radius;
	border->bottom_right_radius = radius;
}

/** 绘制左上角的圆角，从左边框的上端到上边框的左端绘制一条圆角线 */
static int Graph_DrawRoundBorderLeftTop( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;

	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	/* 根据y轴计算各点的x轴坐标并填充点 */
	for( y=0; y<=radius; ++y ) {
		/* 计算出x轴整数坐标 */
		x = (int)sqrt( 1.0*radius*radius - y*y );
		// ...
	}
	return 0;
}

/** 绘制左上角的圆角，从上边框的左端到左边框的上端绘制一条圆角线 */
static int Graph_DrawRoundBorderTopLeft( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	for( x=0; x<=radius; ++x ) {
		y = sqrt( 1.0*radius*radius - x*x );
		// ...
	}
	return 0;
}

/** 绘制右上角的圆角，从右边框的上端到上边框的右端绘制一条圆角线 */
static int Graph_DrawRoundBorderRightTop( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	for( y=0; y<=radius; ++y ) {
		x = (int)sqrt( 1.0*radius*radius - y*y );
		// ...
	}
	return 0;
}

/* 绘制右上角的圆角，从上边框的右端到右边框的上端绘制一条圆角线 */
static int Graph_DrawRoundBorderTopRight( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	for( x=0; x<=radius; ++x ) {
		y = (int)sqrt( 1.0*radius*radius - x*x );
		// ...
	}
	return 0;
}

/** 绘制左下角的圆角，从左边框的下端到下边框的左端绘制一条圆角线 */
static int Graph_DrawRoundBorderLeftBottom(
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	for( y=0; y<=radius; ++y ) {
		x = (int)sqrt( 1.0*radius*radius - y*y );
		// ...
	}
	return 0;
}

/** 绘制左下角的圆角，从下边框的左端到左边框的下端绘制一条圆角线 */
static int Graph_DrawRoundBorderBottomLeft(
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	for( x=0; x<=radius; ++x ) {
		y = (int)sqrt( 1.0*radius*radius - x*x );
		// ...
	}
	return 0;
}

/** 绘制右下角的圆角，从右边框的下端到下边框的右端绘制一条圆角线 */
static int Graph_DrawRoundBorderRightBottom( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	for( y=0; y<=radius; ++y ) {
		x = (int)sqrt( 1.0*radius*radius - y*y );
		// ...
	}
	return 0;
}

/** 绘制右下角的圆角，从下边框的右端到右边框的下端绘制一条圆角线 */
static int Graph_DrawRoundBorderBottomRight(
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_Color line_color,	LCUI_BOOL hide_outarea )
{
	int y, x;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	for( x=0; x<=radius; ++x ) {
		y = (int)sqrt( 1.0*radius*radius - x*x );
		// ...
	}
	return 0;
}

/** 只绘制目标区域内的边框 */
LCUI_API int Graph_DrawBorderEx( LCUI_Graph *des, LCUI_Border border,
							LCUI_Rect area )
{
	int  radius;
	LCUI_Rect bound;
	LCUI_Pos start, end;
	LCUI_Graph des_area;
	
	if( !Graph_IsValid(des) ) {
		return -1;
	}
	
	/* 绘制左上角的圆角，先引用左上角区域，再将圆绘制到这个区域里 */
	radius = border.top_left_radius;
	bound.x = bound.y = 0;
	bound.w = bound.h = radius;
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderLeftTop( 
		&des_area		, Pos( radius, radius ), 
		radius			, border.left_width, 
		border.left_color	, TRUE
	);
	Graph_DrawRoundBorderTopLeft( 
		&des_area		, Pos( radius, radius ), 
		radius			, border.top_width, 
		border.top_color	, TRUE 
	);
	
	/* 右上角 */
	radius = border.top_right_radius;
	bound.x = des->w-radius-1;
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderRightTop( 
		&des_area		, Pos( 0, radius ), 
		radius			, border.right_width, 
		border.right_color	, TRUE 
	);
	Graph_DrawRoundBorderTopRight( 
		&des_area		, Pos( 0, radius ), 
		radius			, border.top_width, 
		border.top_color	, TRUE 
	);
	
	/* 左下角 */
	radius = border.bottom_left_radius;
	bound.x = 0;
	bound.y = des->h-radius-1;
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderLeftBottom( 
		&des_area		, Pos( radius, 0 ), 
		radius			, border.left_width, 
		border.left_color	, TRUE 
	);
	Graph_DrawRoundBorderBottomLeft( 
		&des_area		, Pos( radius, 0 ), 
		radius			, border.bottom_width, 
		border.bottom_color	, TRUE 
	);
	
	/* 右下角 */
	radius = border.bottom_left_radius;
	bound.x = des->w-radius-1,
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderRightBottom( 
		&des_area		, Pos( 0, 0 ), 
		radius			, border.right_width, 
		border.right_color	, TRUE 
	);
	Graph_DrawRoundBorderBottomRight( 
		&des_area		, Pos( 0, 0 ), 
		radius			, border.bottom_width, 
		border.bottom_color	, TRUE 
	);
	
	start.x = border.top_left_radius;
	start.y = 0;
	end.x = des->w - border.top_right_radius;
	/* 引用目标区域 */
	Graph_Quote( &des_area, des, area );
	/* 绘制上边框 */
	Graph_DrawHorizLine( des, border.top_color, border.top_width, start, end.x );
	/* 绘制下边的线 */
	start.y = des->h - border.bottom_width;
	end.x = des->w - border.bottom_right_radius;
	Graph_DrawHorizLine( des, border.bottom_color, border.bottom_width, start, end.x );
	/* 绘制左边的线 */
	start.x = start.y = 0;
	end.y = des->h - border.bottom_left_radius;
	Graph_DrawVertiLine( des, border.left_color, border.left_width, start, end.y );
	/* 绘制右边的线 */
	start.x = des->w - border.right_width;
	start.y = border.top_right_radius;
	end.y = des->h - border.bottom_right_radius;
	Graph_DrawVertiLine( des, border.right_color, border.right_width, start, end.y );
	/* 边框线绘制完成 */
	return 0;
}

/** 简单的为图形边缘绘制边框 */
LCUI_API int Graph_DrawBorder( LCUI_Graph *des, LCUI_Border border )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = des->w;
	area.h = des->h;
	return Graph_DrawBorderEx( des, border, area );
}
