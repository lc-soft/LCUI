/* ****************************************************************************
* boxshadow.c -- graph box shadow draw support.
*
* Copyright (C) 2014-2016 by Liu Chao <lc-soft@live.cn>
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
* boxshadow.c -- 矩形阴影绘制支持
*
* 版权所有 (C) 2014-2016 归属于 刘超 <lc-soft@live.cn>
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

/**
 * 本模块的主要用于实现阴影绘制，但不是通用的，仅适用于矩形框，不能用于绘制文
 * 字、多边形等不规则图形的阴影。
 */

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <math.h>

#define BLUR_N			1.5
#define SHADOW_WIDTH(sd)	(sd->blur + sd->spread)
#define BLUR_WIDTH(sd)		(int)(sd->blur*BLUR_N)
#define INNER_SHADOW_WIDTH(sd)	(SHADOW_WIDTH(sd)-BLUR_WIDTH(sd))

LCUI_BoxShadow BoxShadow( int x, int y, int blur, LCUI_Color color )
{
	LCUI_BoxShadow shadow;
	shadow.x = x;
	shadow.y = y;
	shadow.blur = blur;
	shadow.spread = 0;
	shadow.color = color;
	return shadow;
}

int BoxShadow_GetBoxWidth( LCUI_BoxShadow *shadow, int w )
{
	w -= SHADOW_WIDTH(shadow)*2;
	/* 如果水平向右的偏移距离大于阴影宽度，说明内容区左上角与容器一致 */
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return w - (shadow->x - SHADOW_WIDTH(shadow));
	}
	/* 如果水平向左的偏移距离大于阴影宽度，说明阴影区左上角与容器一致 */
	else if( shadow->x <= -SHADOW_WIDTH(shadow) ) {
		return w + shadow->x + SHADOW_WIDTH(shadow);
	}
	/* 水平偏移距离没有超出阴影宽度，容器大小会是固定的，所以直接减
	* 去两边的阴影宽度即可得到内容区宽度  
	*/
	return w;
}

int BoxShadow_GetBoxHeight( LCUI_BoxShadow *shadow, int h )
{
	h -= SHADOW_WIDTH(shadow)*2;
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return h - shadow->y + SHADOW_WIDTH(shadow);
	}
	else if( shadow->y <= -SHADOW_WIDTH(shadow) ) {
		return h + shadow->y + SHADOW_WIDTH(shadow);
	}
	return h;
}

/** 计算Box在添加阴影后的宽度 */
int BoxShadow_GetWidth( LCUI_BoxShadow *shadow, int box_w )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return box_w + SHADOW_WIDTH(shadow) + shadow->x;
	}
	else if( shadow->x <= -SHADOW_WIDTH(shadow) ) {
		return box_w + SHADOW_WIDTH(shadow) - shadow->x;
	}
	return box_w + SHADOW_WIDTH(shadow)*2;
}

/** 计算Box在添加阴影后的高度 */
int BoxShadow_GetHeight( LCUI_BoxShadow *shadow, int box_h )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return box_h + SHADOW_WIDTH(shadow) + shadow->y;
	}
	else if( shadow->y <= -SHADOW_WIDTH(shadow) ) {
		return box_h + SHADOW_WIDTH(shadow) - shadow->y;
	}
	return box_h + SHADOW_WIDTH(shadow)*2;
}

int BoxShadow_GetBoxX( LCUI_BoxShadow *shadow )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

int BoxShadow_GetBoxY( LCUI_BoxShadow *shadow )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

int BoxShadow_GetY( LCUI_BoxShadow *shadow )
{
	if( shadow->y <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->y - SHADOW_WIDTH(shadow);
}

int BoxShadow_GetX( LCUI_BoxShadow *shadow )
{
	if( shadow->x <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->x - SHADOW_WIDTH(shadow);
}

static void DrawCircle( LCUI_Graph *graph, LCUI_Pos center,
			int r, LCUI_ARGB color )
{
	int x, y;
	int t = r;
	int s = 255;
	float v = 512.0/t;
	float a = 2*(v*t-s)/(t*t);
	LCUI_Rect area, box_rect;
	LCUI_Graph *src, box_graph;
	LCUI_ARGB tmp_px, *px;
	uchar_t *px_row_bytes;
	LCUI_Rect2 circle;

	tmp_px = color;
	Graph_GetValidRect( graph, &area );
	src = Graph_GetQuote( graph );
	center.x += area.x;
	center.y += area.y;
	circle.r = center.x + r;
	circle.l = center.x - r;
	circle.b = center.y + r;
	circle.t = center.y - r;
	/* 先填充 圆外切矩形 外的矩形区域 */
	box_rect.x = area.x;
	box_rect.y = area.y;
	box_rect.w = area.w;
	box_rect.h = circle.t - area.y;
	Graph_Quote( &box_graph, src, &box_rect );
	Graph_FillAlpha( &box_graph, 0 );
	box_rect.y = circle.t;
	box_rect.w = circle.l - area.x;
	box_rect.h = area.y + area.h - circle.t;
	Graph_Quote( &box_graph, src, &box_rect );
	Graph_FillAlpha( &box_graph, 0 );
	/* 调整圆的区域 */
	if( circle.l < area.x ) {
		circle.l = area.x;
	}
	if( circle.t < area.y ) {
		circle.t = area.y;
	}
	if( circle.r > area.x + area.w ) {
		circle.r = area.x + area.w;
	}
	if( circle.b > area.y + area.h ) {
		circle.b = area.y + area.h;
	}
	if( src->color_type != COLOR_TYPE_ARGB ) {
		return;
	}
	px_row_bytes = src->bytes + circle.top*src->bytes_per_row;
	px_row_bytes += circle.left * src->bytes_per_pixel;
	/* 遍历区域内每个像素点，根据点到圆心的距离，计算其alpha透明度 */
	for( y=circle.top; y<circle.bottom; ++y ) {
		px = (LCUI_ARGB*)px_row_bytes;
		for( x=circle.left; x<circle.right; ++x ) {
			t = (y-center.y)*(y-center.y);
			t += (x-center.x)*(x-center.x);
			t = (int)((double)sqrt(1.0*t)+0.5);
			if( t <= r ) {
				tmp_px.a = (uchar_t)(s-(v*t-(a*t*t)/2));
				tmp_px.a *= color.a/255.0;
			} else {
				tmp_px.alpha = 0;
			}
			*px++ = tmp_px;
		}
		px_row_bytes += src->bytes_per_row;
	}
}

static void Graph_DrawTopLeftShadow( LCUI_PaintContext paint, LCUI_Rect *box, 
				     LCUI_BoxShadow *shadow)
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	bound.x = BoxShadow_GetX( shadow );
	bound.y = BoxShadow_GetY( shadow );
	pos.y = pos.x = bound.w = bound.h = BLUR_WIDTH(shadow);
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		pos.x -= paint->rect.x;
		pos.y -= paint->rect.y;
		Graph_Init( &canvas );
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, bound.w, shadow->color );
	}
}

static void Graph_DrawTopRightShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				      LCUI_BoxShadow *shadow )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;

	pos.x = 0;
	bound.y = BoxShadow_GetY( shadow );
	pos.y = bound.w = bound.h = BLUR_WIDTH(shadow);
	bound.x = BoxShadow_GetX( shadow ) + BLUR_WIDTH(shadow);
	bound.x += BoxShadow_GetBoxWidth( shadow, box->w );
	bound.x += INNER_SHADOW_WIDTH(shadow)*2;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		pos.x -= paint->rect.x;
		pos.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, bound.w, shadow->color );
	}
}

static void Graph_DrawBottomLeftShadow( LCUI_PaintContext paint, LCUI_Rect *box,
					LCUI_BoxShadow *shadow )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;

	pos.y = 0;
	bound.x = BoxShadow_GetX( shadow );
	pos.x = bound.w = bound.h = BLUR_WIDTH(shadow);
	bound.y = BoxShadow_GetY( shadow ) + BLUR_WIDTH(shadow);
	bound.y += BoxShadow_GetBoxHeight( shadow, box->h );
	bound.y += INNER_SHADOW_WIDTH(shadow)*2;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		pos.x -= paint->rect.x;
		pos.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, bound.w, shadow->color );
	}
}

static void Graph_DrawBottomRightShadow( LCUI_PaintContext paint, LCUI_Rect *box,
					 LCUI_BoxShadow *shadow )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;

	pos.x = pos.y = 0;
	bound.x = BoxShadow_GetX( shadow ) + BLUR_WIDTH(shadow);
	bound.x += BoxShadow_GetBoxWidth( shadow, box->w );
	bound.x += INNER_SHADOW_WIDTH(shadow)*2;
	bound.y = BoxShadow_GetY( shadow ) + BLUR_WIDTH(shadow);
	bound.y += BoxShadow_GetBoxHeight( shadow, box->h );
	bound.y += INNER_SHADOW_WIDTH(shadow)*2;
	bound.w = bound.h = BLUR_WIDTH(shadow);
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		pos.x -= paint->rect.x;
		pos.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, bound.w, shadow->color );
	}
}

static void Graph_DrawTopShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				 LCUI_BoxShadow *shadow )
{
	float v, a;
	LCUI_Color color;
	LCUI_Graph *graph;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, area;

	box_area.x = BoxShadow_GetBoxX( shadow );
	box_area.y = BoxShadow_GetBoxY( shadow );
	box_area.w = BoxShadow_GetBoxWidth( shadow, box->w );
	box_area.h = BoxShadow_GetBoxHeight( shadow, box->h );
	/* 计算需要绘制上边阴影的区域 */
	shadow_area.x = BoxShadow_GetX( shadow ) + BLUR_WIDTH(shadow);
	shadow_area.y = BoxShadow_GetY( shadow );
	shadow_area.w = box_area.w + INNER_SHADOW_WIDTH(shadow)*2;
	shadow_area.h = BLUR_WIDTH(shadow);
	color = shadow->color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	graph = Graph_GetQuote( &paint->canvas );
	Graph_GetValidRect( &paint->canvas, &area );
	if( !LCUIRect_GetOverlayRect( &area, &paint->rect, &area ) ) {
		return;
	}
	/**
	 * 这里采用匀减速直线运动的公式： s = vt - at²/2
	 * 加速度 a 的求值公式为：a = 2x(vt - s)/t²
	 */
	s = 255;
	t = BLUR_WIDTH(shadow);
	v = 512.0/t;
	a = 2*(v*t-s)/(t*t);
	for( y=shadow_area.y; y<bound_y; ++y,--t ) {
		/* 忽略不在有效区域内的像素 */
		if( y < area.y || y >= area.y + area.h ) {
			continue;
		}
		/* 计算当前行阴影的透明度 */
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow->color.a/255.0;
		for( x=shadow_area.x; x<bound_x; ++x ) {
			if( x < area.x || x >= area.x + area.w ) {
				continue;
			}
			if( y >= box_area.y && y < box_area.y + box_area.h
			 && x >= box_area.x && x < box_area.x + box_area.w ) {
				 continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawBottomShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				    LCUI_BoxShadow *shadow )
{
	float v, a;
	LCUI_Color color;
	LCUI_Graph *graph;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, area;

	box_area.x = BoxShadow_GetBoxX( shadow );
	box_area.y = BoxShadow_GetBoxY( shadow );
	box_area.w = BoxShadow_GetBoxWidth( shadow, box->w );
	box_area.h = BoxShadow_GetBoxHeight( shadow, box->h );
	shadow_area.x = BoxShadow_GetX( shadow ) + BLUR_WIDTH(shadow);
	shadow_area.y = BoxShadow_GetY( shadow ) + BLUR_WIDTH(shadow);
	shadow_area.y += box_area.h + INNER_SHADOW_WIDTH(shadow)*2;
	shadow_area.w = box_area.w + INNER_SHADOW_WIDTH(shadow)*2;
	shadow_area.h = BLUR_WIDTH(shadow);
	color = shadow->color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	graph = Graph_GetQuote( &paint->canvas );
	Graph_GetValidRect( &paint->canvas, &area );
	if( !LCUIRect_GetOverlayRect( &area, &paint->rect, &area ) ) {
		return;
	}
	s = 255;
	t = BLUR_WIDTH(shadow);
	v = 512.0/t;
	a = 2*(v*t-s)/(t*t);
	for( t=0,y=shadow_area.y; y<bound_y; ++y,++t ) {
		if( y < area.y || y >= area.y + area.h ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow->color.a/255.0;
		for( x=shadow_area.x; x<bound_x; ++x ) {
			if( x < area.x || x >= area.x + area.w ) {
				continue;
			}
			if( y >= box_area.y && y < box_area.y + box_area.h
			 && x >= box_area.x && x < box_area.x + box_area.w ) {
				 continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawLeftShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				  LCUI_BoxShadow *shadow )
{
	float v, a;
	LCUI_Color color;
	LCUI_Graph *graph;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, area;
	box_area.x = BoxShadow_GetBoxX( shadow );
	box_area.y = BoxShadow_GetBoxY( shadow );
	box_area.w = BoxShadow_GetBoxWidth( shadow, box->w );
	box_area.h = BoxShadow_GetBoxHeight( shadow, box->h );
	shadow_area.x = BoxShadow_GetX( shadow );
	shadow_area.y = BoxShadow_GetY( shadow ) + BLUR_WIDTH(shadow);
	shadow_area.w = BLUR_WIDTH(shadow);
	shadow_area.h = box_area.h + INNER_SHADOW_WIDTH(shadow)*2;
	color = shadow->color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	graph = Graph_GetQuote( &paint->canvas );
	Graph_GetValidRect( &paint->canvas, &area );
	if( !LCUIRect_GetOverlayRect( &area, &paint->rect, &area ) ) {
		return;
	}
	s = 255;
	t = BLUR_WIDTH(shadow);
	v = 512.0/t;
	a = 2*(v*t-s)/(t*t);
	for( x=shadow_area.x; x<bound_x; ++x,--t ) {
		if( x < area.x || x >= area.x + area.w ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow->color.a/255.0;
		for( y=shadow_area.y; y<bound_y; ++y ) {
			if( y < area.y || y >= area.y + area.h ) {
				continue;
			}
			if( y >= box_area.y && y < box_area.y + box_area.h
			 && x >= box_area.x && x < box_area.x + box_area.w ) {
				 continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawRightShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				   LCUI_BoxShadow *shadow )
{
	float v, a;
	LCUI_Color color;
	LCUI_Graph *graph;
	int s, t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, area;
	box_area.x = BoxShadow_GetBoxX( shadow );
	box_area.y = BoxShadow_GetBoxY( shadow );
	box_area.w = BoxShadow_GetBoxWidth( shadow, box->w );
	box_area.h = BoxShadow_GetBoxHeight( shadow, box->h );
	shadow_area.x = BoxShadow_GetX( shadow ) + SHADOW_WIDTH(shadow);
	shadow_area.x += box_area.w + INNER_SHADOW_WIDTH(shadow);
	shadow_area.y = BoxShadow_GetY( shadow ) + BLUR_WIDTH(shadow);
	shadow_area.w = BLUR_WIDTH(shadow);
	shadow_area.h = box_area.h + INNER_SHADOW_WIDTH(shadow)*2;
	color = shadow->color;
	bound_x = shadow_area.x + shadow_area.w;
	bound_y = shadow_area.y + shadow_area.h;
	graph = Graph_GetQuote( &paint->canvas );
	Graph_GetValidRect( &paint->canvas, &area );
	if( !LCUIRect_GetOverlayRect( &area, &paint->rect, &area ) ) {
		return;
	}
	s = 255;
	t = BLUR_WIDTH(shadow);
	v = 512.0/t;
	a = 2*(v*t-s)/(t*t);

	for( t=0,x=shadow_area.x; x<bound_x; ++x,++t ) {
		if( x < area.x || x >= area.x + area.w ) {
			continue;
		}
		color.alpha = (uchar_t)(s-(v*t-(a*t*t)/2));
		color.alpha *= shadow->color.a/255.0;
		for( y=shadow_area.y; y<bound_y; ++y ) {
			if( y < area.y || y >= area.y + area.h ) {
				continue;
			}
			if( y >= box_area.y && y < box_area.y + box_area.h
			 && x >= box_area.x && x < box_area.x + box_area.w ) {
				 continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

void Graph_ClearShadowArea( LCUI_PaintContext paint, LCUI_Rect *box,
			    LCUI_BoxShadow *shadow )
{
	int i;
	LCUI_Graph canvas;
	LCUI_Rect rect, box_area, rects[4];

	rect.w = box->w;
	rect.h = box->h;
	rect.x = rect.y = 0;
	/* 获取内容框区域 */
	box_area.x = box->x + BoxShadow_GetBoxX( shadow );
	box_area.y = box->y + BoxShadow_GetBoxY( shadow );
	box_area.w = BoxShadow_GetBoxWidth( shadow, box->w );
	box_area.h = BoxShadow_GetBoxHeight( shadow, box->h );
	/* 获取内容框外的阴影区域 */
	LCUIRect_CutFourRect( &box_area, &rect, rects );
	for( i=0; i<4; ++i ) {
		if( LCUIRect_GetOverlayRect( &paint->rect, &rects[i], &rects[i] ) ) {
			rects[i].x -= paint->rect.x;
			rects[i].y -= paint->rect.y;
			Graph_Quote( &canvas, &paint->canvas, &rects[i] );
			Graph_FillRect( &canvas, shadow->color, NULL, FALSE );
		}
	}
}

static void Graph_DrawInnerShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				   LCUI_BoxShadow *s )
{
	int i;
	LCUI_Graph canvas;
	LCUI_Rect rsd, rb, rects[4];
	rb.x = BoxShadow_GetBoxX( s );
	rb.y = BoxShadow_GetBoxY( s );
	rb.w = BoxShadow_GetBoxWidth( s, box->w );
	rb.h = BoxShadow_GetBoxHeight( s, box->h );
	rsd.x = BoxShadow_GetX( s ) + BLUR_WIDTH(s);
	rsd.y = BoxShadow_GetY( s ) + BLUR_WIDTH(s);
	rsd.w = rb.w + INNER_SHADOW_WIDTH(s)*2;
	rsd.h = rb.h + INNER_SHADOW_WIDTH(s)*2;
	/* 截取出与内容区重叠的区域 */
	if( LCUIRect_GetOverlayRect(&rb, &rsd, &rb) ) {
		LCUIRect_CutFourRect( &rb, &rsd, rects );
		/* 从阴影区域中排除部件占用的区域 */
		for( i=0; i<4; ++i ) {
			if( !LCUIRect_GetOverlayRect(
				&paint->rect, &rects[i],&rects[i]) ) {
				continue;
			}
			rects[i].x -= paint->rect.x;
			rects[i].y -= paint->rect.y;
			Graph_Quote( &canvas, &paint->canvas, &rects[i] );
			Graph_FillRect( &canvas, s->color, NULL, TRUE );
		}
		return;
	}
	/* 不重叠则直接填充 */
	if( LCUIRect_GetOverlayRect(&paint->rect, &rsd, &rb) ) {
		rb.x -= paint->rect.x;
		rb.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &rb );
		Graph_FillRect( &canvas, s->color, NULL, TRUE );
	}
}

void BoxShadow_Init( LCUI_BoxShadow *shadow )
{
	shadow->color.r = 0;
	shadow->color.g = 0;
	shadow->color.b = 0;
	shadow->blur = 0;
	shadow->spread = 0;
	shadow->x = 0;
	shadow->y = 0;
}

int Graph_DrawBoxShadow( LCUI_PaintContext paint, LCUI_Rect *box,
			 LCUI_BoxShadow *shadow )
{
	/* 判断容器尺寸是否低于阴影占用的最小尺寸 */
	if( box->w < BoxShadow_GetWidth(shadow, 0)
	 || box->h < BoxShadow_GetHeight(shadow, 0) ) {
		return -1;
	}
	Graph_DrawInnerShadow( paint, box, shadow );
	Graph_DrawTopLeftShadow( paint, box, shadow );
	Graph_DrawTopRightShadow( paint, box, shadow );
	Graph_DrawBottomLeftShadow( paint, box, shadow );
	Graph_DrawBottomRightShadow( paint, box, shadow );
	Graph_DrawTopShadow( paint, box, shadow );
	Graph_DrawBottomShadow( paint, box, shadow );
	Graph_DrawLeftShadow( paint, box, shadow );
	Graph_DrawRightShadow( paint, box, shadow );
	return 0;
}
