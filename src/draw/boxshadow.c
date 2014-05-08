/* ****************************************************************************
* boxshadow.c -- draw box shadow.
* 
* Copyright (C) 2014 by
* Liu Chao <lc-soft@live.cn>
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
* boxshadow.c -- 矩形阴影
*
* 版权所有 (C) 2014 归属于
* 刘超 <lc-soft@live.cn>
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
#include LC_LCUI_H
#include LC_GRAPH_H
#include <math.h>

static void 
draw_gradient_line( LCUI_Graph *graph, LCUI_Pos begin, LCUI_Pos end, 
			LCUI_ARGB line_color, LCUI_Rect *area )
{
	float k, v, a;
	int s, t, x, y, b, i;
	LCUI_ARGB pixel_color;
	
	s = 128;
	t = (end.y-begin.y)*(end.y-begin.y);
	t += (end.x-begin.x)*(end.x-begin.x);
	t = (int)((double)sqrt(1.0*t)+0.5);
	v = 255.0/t;
	a = 2*(v*t-s)/(t*t);
	pixel_color = line_color;
	/* 如果与X轴平行 */
	if( end.y - begin.y == 0 ) {
		if( begin.y < area->y || begin.y >= area->y + area->h ) {
			return;
		}
		if( begin.x > end.x ) {
			i = -1;
		} else {
			i = 1;
		}
		for( t=0,x=begin.x; x!=end.x; x+=i,++t ) {
			if( x < area->x || x >= area->x + area->w ) {
				continue;
			}
			pixel_color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
			pixel_color.alpha *= line_color.a/255.0;
			Graph_SetPixel( graph, x, begin.y, pixel_color );
		}
		return;
	}
	/* 如果与Y轴平行 */
	if( end.x - begin.x == 0 ) {
		if( begin.x < area->x || begin.x >= area->x + area->w ) {
			return;
		}
		if( begin.y > end.y ) {
			i = -1;
		} else {
			i = 1;
		}
		for( t=0,y=begin.y; y!=end.y; y+=i,++t ) {
			if( y < area->y || y >= area->y + area->h ) {
				continue;
			}
			pixel_color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
			pixel_color.alpha *= line_color.a/255.0;
			Graph_SetPixel( graph, begin.x, y, pixel_color );
		}
		return;
	}

	k = 1.0*(end.y-begin.y)/(end.x-begin.x);
	b = begin.y - (int)(begin.x*k);
	if( begin.x > end.x ) {
		i = -1;
	} else {
		i = 1;
	}
	for( x=begin.x; x!=end.x; x+=i ) {
		if( x < area->x || x >= area->x + area->w ) {
			continue;
		}
		y = (int)(x*k)+b;
		if( y < area->y || y >= area->y + area->h ) {
			continue;
		}
		t = (y-begin.y)*(y-begin.y);
		t += (x-begin.x)*(x-begin.x);
		t = (int)((double)sqrt(1.0*t)+0.5);
		pixel_color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		pixel_color.alpha *= line_color.a/255.0;
		Graph_SetPixel( graph, x, y, pixel_color );
	}

	if( begin.y > end.y ) {
		i = -1;
	} else {
		i = 1;
	}
	for( y=begin.y; y!=end.y; y+=i ) {
		if( y < area->y || y >= area->y + area->h ) {
			continue;
		}
		x = (int)((y-b)/k);
		if( x < area->x || x >= area->x + area->w ) {
			continue;
		}
		t = (y-begin.y)*(y-begin.y);
		t += (x-begin.x)*(x-begin.x);
		t = (int)((double)sqrt(1.0*t)+0.5);
		pixel_color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		pixel_color.alpha *= line_color.a/255.0;
		Graph_SetPixel( graph, x, y, pixel_color );
	}
}

static void 
draw_circle( LCUI_Graph *graph, LCUI_Pos center, int r, LCUI_ARGB color )
{
	LCUI_Graph *src;
	LCUI_Pos pos;
	LCUI_Rect area;
	int circle_right, circle_left, circle_bottom, circle_top;
	
	area = Graph_GetValidRect( graph );
	src = Graph_GetQuote( graph );
	circle_right = center.x + r;
	circle_left = center.x - r;
	circle_bottom = center.y + r;
	circle_top = center.y - r;
	for( pos.x=circle_left; pos.x<circle_right; ++pos.x ) {
		pos.y = r*r - (pos.x-center.x)*(pos.x-center.x);
		pos.y = -sqrt(pos.y) + center.y;
		draw_gradient_line( src, center, pos, color, &area );
	}

	for( pos.x=circle_left; pos.x<circle_right; ++pos.x ) {
		pos.y = r*r - (pos.x-center.x)*(pos.x-center.x);
		pos.y = sqrt(pos.y) + center.y;
		draw_gradient_line( src, center, pos, color, &area );
	}

	for( pos.y=circle_top; pos.y<circle_bottom; ++pos.y ) {
		pos.x = r*r - (pos.y-center.y)*(pos.y-center.y);
		pos.x = sqrt(pos.x) + center.x;
		draw_gradient_line( src, center, pos, color, &area );
	}
	
	for( pos.y=circle_top; pos.y<circle_bottom; ++pos.y ) {
		pos.x = r*r - (pos.y-center.y)*(pos.y-center.y);
		pos.x = -sqrt(pos.x) + center.x;
		draw_gradient_line( src, center, pos, color, &area );
	}
	
}

static void test_draw( void )
{
	static int count = 0;
	LCUI_Graph graph;
	LCUI_Rect area;

	if( ++count > 1 ) {
		return;
	}
	Graph_Init( &graph );
	graph.color_type = COLOR_TYPE_ARGB;
	Graph_Create( &graph, 200, 200 );
	area.x = area.y = 0;
	area.w = area.h = 200;
	draw_circle( &graph, Pos(100,100), 100, ARGB(255,255,0,0) );
	Graph_WritePNG( "circle.png", &graph );
	Graph_Free( &graph );
}

static void
Graph_DrawTopLeftShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	LCUI_Graph box;
	LCUI_Rect bound;
	LCUI_Pos pos;
	
	pos.x = pos.y = shadow.size;
	bound.x = bound.y = 0;
	bound.w = bound.h = shadow.size;
	Graph_Quote( &box, graph, bound );
	Graph_FillAlpha( &box, 0 );
	draw_circle( &box, pos, shadow.size, shadow.top_color );
}

static void
Graph_DrawTopRightShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	LCUI_Graph box;
	LCUI_Rect bound;
	LCUI_Pos pos;

	pos.x = BoxShadow_GetX( &shadow ) + shadow.size;
	pos.x += BoxShadow_GetBoxWidth( &shadow, graph->w );
	pos.y = BoxShadow_GetY( &shadow ) + shadow.size;
	bound.x = pos.x;
	bound.y = 0;
	bound.w = bound.h = shadow.size;
	Graph_Quote( &box, graph, bound );
	Graph_FillAlpha( &box, 0 );
	draw_circle( &box, pos, shadow.size, shadow.top_color );
}

static void
Graph_DrawBottomLeftShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	LCUI_Graph box;
	LCUI_Rect bound;
	LCUI_Pos pos;
	
	pos.x = BoxShadow_GetX( &shadow ) + shadow.size;
	pos.y = BoxShadow_GetY( &shadow ) + shadow.size;
	pos.y += BoxShadow_GetBoxHeight( &shadow, graph->h );
	bound.x = pos.x - shadow.size;
	bound.y = pos.y;
	bound.w = bound.h = shadow.size;
	Graph_Quote( &box, graph, bound );
	Graph_FillAlpha( &box, 0 );
	draw_circle( &box, pos, shadow.size, shadow.bottom_color );
}

static void
Graph_DrawBottomRightShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	LCUI_Graph box;
	LCUI_Rect bound;
	LCUI_Pos pos;
	
	pos.x = BoxShadow_GetX( &shadow ) + shadow.size;
	pos.x += BoxShadow_GetBoxWidth( &shadow, graph->w );
	pos.y = BoxShadow_GetY( &shadow ) + shadow.size;
	pos.y += BoxShadow_GetBoxHeight( &shadow, graph->h );
	bound.x = pos.x;
	bound.y = pos.y;
	bound.w = bound.h = shadow.size;
	Graph_Quote( &box, graph, bound );
	Graph_FillAlpha( &box, 0 );
	draw_circle( &box, pos, shadow.size, shadow.bottom_color );
}

static void 
Graph_DrawTopShadow( LCUI_Graph *graph, LCUI_Rect area,
				LCUI_BoxShadow shadow )
{
	float v, a;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Color color;
	LCUI_Rect shadow_area, box_area;

	box_area.x = BoxShadow_GetBoxX( &shadow );
	box_area.y = BoxShadow_GetBoxY( &shadow );
	box_area.w = BoxShadow_GetBoxWidth( &shadow, graph->w );
	box_area.h = BoxShadow_GetBoxHeight( &shadow, graph->h );
	/* 计算需要绘制上边阴影的区域 */
	shadow_area.x = BoxShadow_GetX( &shadow ) + shadow.size;
	shadow_area.y = BoxShadow_GetY( &shadow );
	shadow_area.w = box_area.w;
	shadow_area.h = shadow.size;
	color = shadow.top_color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	
	/** 
	* 这里采用匀减速直线运动的公式： s = vt - at²/2
	* 加速度 a 的求值公式为：a = 2x(vt - s)/t²
	*/
	s = 128;
	t = shadow.size;
	v = 255.0/t;
	a = 2*(v*t-s)/(t*t);

	for( y=shadow_area.y; y<bound_y; ++y,--t ) {
		/* 忽略不在有效区域内的像素 */
		if( y < area.y || y >= area.y + area.h 
		 || (y >= box_area.y && y < box_area.y + box_area.h) ) {
			continue;
		}
		/* 计算当前行阴影的透明度 */
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow.right_color.a/255.0;
		for( x=shadow_area.x; x<bound_x; ++x ) {
			if( x < area.x || x >= area.x + area.w ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawBottomShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	float v, a;
	int x, y, s, t, bound_x, bound_y;
	LCUI_Color color;
	LCUI_Rect shadow_area, box_area;

	box_area.x = BoxShadow_GetBoxX( &shadow );
	box_area.y = BoxShadow_GetBoxY( &shadow );
	box_area.w = BoxShadow_GetBoxWidth( &shadow, graph->w );
	box_area.h = BoxShadow_GetBoxHeight( &shadow, graph->h );
	shadow_area.x = BoxShadow_GetX( &shadow ) + shadow.size;
	shadow_area.y = BoxShadow_GetY( &shadow ) + shadow.size + box_area.h;
	shadow_area.w = box_area.w;
	shadow_area.h = shadow.size;
	color = shadow.bottom_color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	
	s = 128;
	t = shadow.size;
	v = 255.0/t;
	a = 2*(v*t-s)/(t*t);

	for( t=0,y=shadow_area.y; y<bound_y; ++y,++t ) {
		if( y < area.y || y >= area.y + area.h 
				|| (y >= box_area.y && y < box_area.y + box_area.h) ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow.right_color.a/255.0;
		for( x=shadow_area.x; x<bound_x; ++x ) {
			if( x < area.x || x >= area.x + area.w  ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawLeftShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	float v, a;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Color color;
	LCUI_Rect shadow_area, box_area;

	box_area.x = BoxShadow_GetBoxX( &shadow );
	box_area.y = BoxShadow_GetBoxY( &shadow );
	box_area.w = BoxShadow_GetBoxWidth( &shadow, graph->w );
	box_area.h = BoxShadow_GetBoxHeight( &shadow, graph->h );
	shadow_area.x = BoxShadow_GetX( &shadow );
	shadow_area.y = box_area.y;
	shadow_area.w = shadow.size;
	shadow_area.h = box_area.h;
	color = shadow.left_color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;

	s = 128;
	t = shadow.size;
	v = 255.0/t;
	a = 2*(v*t-s)/(t*t);

	for( x=shadow_area.x; x<bound_x; ++x,--t ) {
		if( x < area.x || x >= area.x + area.w
		 || (x >= box_area.x && x < box_area.x + box_area.w) ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow.right_color.a/255.0;
		for( y=shadow_area.y; y<bound_y; ++y ) {
			if( y < area.y || y >= area.y + area.h ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawRightShadow( LCUI_Graph *graph, LCUI_Rect area,
					LCUI_BoxShadow shadow )
{
	int x, y, s, t, bound_x, bound_y;
	LCUI_Color color;
	float v, a;
	LCUI_Rect shadow_area, box_area;
	
	if( shadow.size == 0 ) {
		return;
	}

	box_area.x = BoxShadow_GetBoxX( &shadow );
	box_area.y = BoxShadow_GetBoxY( &shadow );
	box_area.w = BoxShadow_GetBoxWidth( &shadow, graph->w );
	box_area.h = BoxShadow_GetBoxHeight( &shadow, graph->h );
	shadow_area.x = BoxShadow_GetX( &shadow ) + shadow.size + box_area.w;
	shadow_area.y = BoxShadow_GetY( &shadow ) + shadow.size;
	shadow_area.w = shadow.size;
	shadow_area.h = box_area.h;
	color = shadow.right_color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;

	s = 128;
	t = shadow.size;
	v = 255.0/t;
	a = 2*(v*t-s)/(t*t);

	for( t=0,x=shadow_area.x; x<bound_x; ++x,++t ) {
		if( x < area.x || x >= area.x + area.w
				|| (x >= box_area.x && x < box_area.x + box_area.w) ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow.right_color.a/255.0;
		for( y=shadow_area.y; y<bound_y; ++y ) {
			if( y < area.y || y >= area.y + area.h ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

LCUI_API void BoxShadow_Init( LCUI_BoxShadow *shadow )
{
	shadow->top_color.r = 0;
	shadow->top_color.g = 0;
	shadow->top_color.b = 0;
	shadow->bottom_color = shadow->top_color;
	shadow->left_color = shadow->top_color;
	shadow->right_color = shadow->top_color;
	shadow->size = 0;
	shadow->offset_x = 0;
	shadow->offset_y = 0;
}

int Graph_DrawBoxShadowEx( LCUI_Graph *graph, LCUI_Rect area,
				LCUI_BoxShadow shadow )
{
	/* 判断图像尺寸是否低于阴影占用的最小尺寸 */
	if( graph->w < BoxShadow_GetWidth(&shadow, 0)
	 || graph->h < BoxShadow_GetHeight(&shadow, 0) ) {
		return -1;
	}
	Graph_DrawTopLeftShadow( graph, area, shadow );
	Graph_DrawTopRightShadow( graph, area, shadow );
	Graph_DrawBottomLeftShadow( graph, area, shadow );
	Graph_DrawBottomRightShadow( graph, area, shadow );
	Graph_DrawTopShadow( graph, area, shadow );
	Graph_DrawBottomShadow( graph, area, shadow );
	Graph_DrawLeftShadow( graph, area, shadow );
	Graph_DrawRightShadow( graph, area, shadow );
	return 0;
}

int Graph_DrawBoxShadow( LCUI_Graph *graph, LCUI_BoxShadow shadow )
{
	LCUI_Rect rect;
	rect.x = rect.y = 0;
	rect.w = graph->w;
	rect.h = graph->h;
	return Graph_DrawBoxShadowEx( graph, rect, shadow );
}