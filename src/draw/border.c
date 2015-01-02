/* ***************************************************************************
 * border.c -- graph border draw support.
 * 
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************/
 
/* ****************************************************************************
 * border.c -- 边框绘制支持
 *
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <math.h>

/** 初始化边框数据 */
void Border_Init( LCUI_Border *border )
{
	border->top.width = 0;
	border->bottom.width = 0;
	border->left.width = 0;
	border->right.width = 0;
	border->top.style = 0;
	border->bottom.style = 0;
	border->left.style = 0;
	border->right.style = 0;
	border->top.color = RGB(0,0,0);
	border->bottom.color = RGB(0,0,0);
	border->left.color = RGB(0,0,0);
	border->right.color = RGB(0,0,0);
	border->top_left_radius = 0;
	border->top_right_radius = 0;
	border->bottom_left_radius = 0;
	border->bottom_right_radius = 0;
}

/** 简单的设置边框样式，并获取该样式数据 */
LCUI_Border Border( unsigned int width_px, int style, LCUI_Color color )
{
	LCUI_Border border;
	border.top.width = width_px;
	border.bottom.width = width_px;
	border.left.width = width_px;
	border.right.width = width_px;
	border.top.style = style;
	border.bottom.style = style;
	border.left.style = style;
	border.right.style = style;
	border.top.color = color;
	border.bottom.color = color;
	border.left.color = color;
	border.right.color = color;
	border.top_left_radius = 0;
	border.top_right_radius = 0;
	border.bottom_left_radius = 0;
	border.bottom_right_radius = 0;
	return border; 
}

/** 设置边框的圆角半径 */
void Border_Radius( LCUI_Border *border, unsigned int radius )
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
int Graph_DrawBorderEx( LCUI_Graph *des, LCUI_Border border,
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
		radius			, border.left.width, 
		border.left.color	, TRUE
	);
	Graph_DrawRoundBorderTopLeft( 
		&des_area		, Pos( radius, radius ), 
		radius			, border.top.width, 
		border.top.color	, TRUE 
	);
	
	/* 右上角 */
	radius = border.top_right_radius;
	bound.x = des->w-radius-1;
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderRightTop( 
		&des_area		, Pos( 0, radius ), 
		radius			, border.right.width, 
		border.right.color	, TRUE 
	);
	Graph_DrawRoundBorderTopRight( 
		&des_area		, Pos( 0, radius ), 
		radius			, border.top.width, 
		border.top.color	, TRUE 
	);
	
	/* 左下角 */
	radius = border.bottom_left_radius;
	bound.x = 0;
	bound.y = des->h-radius-1;
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderLeftBottom( 
		&des_area		, Pos( radius, 0 ), 
		radius			, border.left.width, 
		border.left.color	, TRUE 
	);
	Graph_DrawRoundBorderBottomLeft( 
		&des_area		, Pos( radius, 0 ), 
		radius			, border.bottom.width, 
		border.bottom.color	, TRUE 
	);
	
	/* 右下角 */
	radius = border.bottom_left_radius;
	bound.x = des->w-radius-1,
	Graph_Quote( &des_area, des, bound );
	Graph_DrawRoundBorderRightBottom( 
		&des_area		, Pos( 0, 0 ), 
		radius			, border.right.width, 
		border.right.color	, TRUE 
	);
	Graph_DrawRoundBorderBottomRight( 
		&des_area		, Pos( 0, 0 ), 
		radius			, border.bottom.width, 
		border.bottom.color	, TRUE 
	);
	
	start.x = border.top_left_radius;
	start.y = 0;
	end.x = des->w - border.top_right_radius;
	/* 引用目标区域 */
	Graph_Quote( &des_area, des, area );
	/* 绘制上边框 */
	Graph_DrawHorizLine( des, border.top.color, border.top.width, start, end.x );
	/* 绘制下边的线 */
	start.y = des->h - border.bottom.width;
	end.x = des->w - border.bottom_right_radius;
	Graph_DrawHorizLine( des, border.bottom.color, border.bottom.width, start, end.x );
	/* 绘制左边的线 */
	start.x = start.y = 0;
	end.y = des->h - border.bottom_left_radius;
	Graph_DrawVertiLine( des, border.left.color, border.left.width, start, end.y );
	/* 绘制右边的线 */
	start.x = des->w - border.right.width;
	start.y = border.top_right_radius;
	end.y = des->h - border.bottom_right_radius;
	Graph_DrawVertiLine( des, border.right.color, border.right.width, start, end.y );
	/* 边框线绘制完成 */
	return 0;
}

/** 简单的为图形边缘绘制边框 */
int Graph_DrawBorder( LCUI_Graph *des, LCUI_Border border )
{
	LCUI_Rect area;
	area.x = area.y = 0;
	area.w = des->w;
	area.h = des->h;
	return Graph_DrawBorderEx( des, border, area );
}
