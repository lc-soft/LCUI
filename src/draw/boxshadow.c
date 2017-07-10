/* ****************************************************************************
* boxshadow.c -- graph box shadow draw support.
*
* Copyright (C) 2014-2017 by Liu Chao <lc-soft@live.cn>
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
* 版权所有 (C) 2014-2017 归属于 刘超 <lc-soft@live.cn>
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
#define BLUR_WIDTH(sd)		(int)(sd->blur*BLUR_N)
#define INNER_SHADOW_WIDTH(sd)	(SHADOW_WIDTH(sd)-BLUR_WIDTH(sd))

typedef struct gradient {
	int s;
	float v;
	float a;
} gradient_t;

/** 初始化渐变生成器 */
static void gradient_init( gradient_t *g, int t )
{
	/**
	* 这里采用匀减速直线运动的公式： s = vt - at²/2
	* 加速度 a 的求值公式为：a = 2x(vt - s)/t²
	*/
	g->s = 255;
	g->v = 512.0f / t;
	g->a = 2 * (g->v*t - g->s) / (t*t);
}

/** 计算渐变后的值 */
static uchar_t gradient_compute( gradient_t *g, int t, int val )
{
	val *= g->s - (int)(g->v*t - (g->a*t*t) / 2);
	return (uchar_t)(val >> 8);
}

 /** 计算Box在添加阴影后的宽度 */
static int BoxShadow_GetBoxWidth( const LCUI_BoxShadow *shadow, int w )
{
	w -= SHADOW_WIDTH( shadow ) * 2;
	/* 如果水平向右的偏移距离大于阴影宽度，说明内容区左上角与容器一致 */
	if( shadow->x >= SHADOW_WIDTH( shadow ) ) {
		return w - (shadow->x - SHADOW_WIDTH( shadow ));
	}
	/* 如果水平向左的偏移距离大于阴影宽度，说明阴影区左上角与容器一致 */
	else if( shadow->x <= -SHADOW_WIDTH( shadow ) ) {
		return w + shadow->x + SHADOW_WIDTH( shadow );
	}
	/* 水平偏移距离没有超出阴影宽度，容器大小会是固定的，所以直接减
	* 去两边的阴影宽度即可得到内容区宽度
	*/
	return w;
}

/** 计算Box在添加阴影后的高度 */
static int BoxShadow_GetBoxHeight( const LCUI_BoxShadow *shadow, int h )
{
	h -= SHADOW_WIDTH( shadow ) * 2;
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return h - shadow->y + SHADOW_WIDTH( shadow );
	} else if( shadow->y <= -SHADOW_WIDTH( shadow ) ) {
		return h + shadow->y + SHADOW_WIDTH( shadow );
	}
	return h;
}

/** 计算Box在添加阴影后的宽度 */
static int BoxShadow_GetWidth( const LCUI_BoxShadow *shadow, int box_w )
{
	if( shadow->x >= SHADOW_WIDTH( shadow ) ) {
		return box_w + SHADOW_WIDTH( shadow ) + shadow->x;
	} else if( shadow->x <= -SHADOW_WIDTH( shadow ) ) {
		return box_w + SHADOW_WIDTH( shadow ) - shadow->x;
	}
	return box_w + SHADOW_WIDTH( shadow ) * 2;
}

/** 计算Box在添加阴影后的高度 */
static int BoxShadow_GetHeight( const LCUI_BoxShadow *shadow, int box_h )
{
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return box_h + SHADOW_WIDTH( shadow ) + shadow->y;
	} else if( shadow->y <= -SHADOW_WIDTH( shadow ) ) {
		return box_h + SHADOW_WIDTH( shadow ) - shadow->y;
	}
	return box_h + SHADOW_WIDTH( shadow ) * 2;
}

static int BoxShadow_GetBoxX( const LCUI_BoxShadow *shadow )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

static int BoxShadow_GetBoxY( const LCUI_BoxShadow *shadow )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

static int BoxShadow_GetY( const LCUI_BoxShadow *shadow )
{
	if( shadow->y <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->y - SHADOW_WIDTH(shadow);
}

static int BoxShadow_GetX( const LCUI_BoxShadow *shadow )
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
	gradient_t g;
	LCUI_Rect area, box_rect;
	LCUI_Graph *src, box_graph;
	LCUI_ARGB tmp, *px;
	uchar_t *px_row_bytes;
	LCUI_Rect2 circle;

	tmp = color;
	gradient_init( &g, t );
	Graph_GetValidRect( graph, &area );
	src = Graph_GetQuote( graph );
	center.x += area.x;
	center.y += area.y;
	circle.right = center.x + r;
	circle.left = center.x - r;
	circle.bottom = center.y + r;
	circle.top = center.y - r;
	/* 先填充 圆外切矩形 外的矩形区域 */
	box_rect.x = area.x;
	box_rect.y = area.y;
	box_rect.width = area.width;
	box_rect.height = circle.top - area.y;
	Graph_Quote( &box_graph, src, &box_rect );
	Graph_FillAlpha( &box_graph, 0 );
	box_rect.y = circle.top;
	box_rect.width = circle.left - area.x;
	box_rect.height = area.y + area.height - circle.top;
	Graph_Quote( &box_graph, src, &box_rect );
	Graph_FillAlpha( &box_graph, 0 );
	/* 调整圆的区域 */
	if( circle.left < area.x ) {
		circle.left = area.x;
	}
	if( circle.top < area.y ) {
		circle.top = area.y;
	}
	if( circle.right > area.x + area.width ) {
		circle.right = area.x + area.width;
	}
	if( circle.bottom > area.y + area.height ) {
		circle.bottom = area.y + area.height;
	}
	if( src->color_type != COLOR_TYPE_ARGB ) {
		return;
	}
	px_row_bytes = src->bytes + circle.top*src->bytes_per_row;
	px_row_bytes += circle.left * src->bytes_per_pixel;
	/* 遍历区域内每个像素点，根据点到圆心的距离，计算其alpha透明度 */
	for( y = circle.top; y < circle.bottom; ++y ) {
		px = (LCUI_ARGB*)px_row_bytes;
		for( x = circle.left; x < circle.right; ++x ) {
			t = (y - center.y)*(y - center.y);
			t += (x - center.x)*(x - center.x);
			t = (int)((double)sqrt( 1.0*t ) + 0.5);
			if( t <= r ) {
				tmp.a = gradient_compute( &g, t, color.a );
			} else {
				tmp.alpha = 0;
			}
			*px++ = tmp;
		}
		px_row_bytes += src->bytes_per_row;
	}
}

static void Graph_DrawTopLeftShadow( LCUI_PaintContext paint,
				     const LCUI_Rect *box,
				     const LCUI_BoxShadow *sd )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	/* 计算阴影边界区域，相对于部件内容框 */
	bound.width = bound.height = BLUR_WIDTH( sd );
	bound.x = box->x + BoxShadow_GetX( sd );
	bound.y = box->y + BoxShadow_GetY( sd );
	/* 确定圆心的坐标 */
	pos.x = bound.x + BLUR_WIDTH( sd );
	pos.y = bound.y + BLUR_WIDTH( sd );
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		Graph_Init( &canvas );
		/* 将圆心的坐标转换为相对于边界区域 */
		pos.x -= bound.x;
		pos.y -= bound.y;
		/* 将边界区域转换为相对于绘制区域 */
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		/* 从绘制区域中引用实际的绘制区域 */
		Graph_Quote( &canvas, &paint->canvas, &bound );
		/* 在该区域里绘制阴影 */
		DrawCircle( &canvas, pos, BLUR_WIDTH( sd ), sd->color );
	}
}

static void Graph_DrawTopRightShadow( LCUI_PaintContext paint,
				      const LCUI_Rect *box,
				      const LCUI_BoxShadow *sd )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	bound.width = bound.height = BLUR_WIDTH( sd );
	bound.y = box->y + BoxShadow_GetY( sd );
	bound.x = box->x + BoxShadow_GetX( sd );
	bound.x += BoxShadow_GetBoxWidth( sd, box->width );
	bound.x += BLUR_WIDTH( sd ) + INNER_SHADOW_WIDTH( sd ) * 2;
	pos.y = bound.y + BLUR_WIDTH( sd );
	pos.x = bound.x;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		Graph_Init( &canvas );
		pos.x -= bound.x;
		pos.y -= bound.y;
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, BLUR_WIDTH( sd ), sd->color );
	}
}

static void Graph_DrawBottomLeftShadow( LCUI_PaintContext paint,
					const LCUI_Rect *box,
					const LCUI_BoxShadow *sd )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	bound.x = box->x + BoxShadow_GetX( sd );
	bound.y = box->y + BoxShadow_GetY( sd );
	bound.y += BoxShadow_GetBoxHeight( sd, box->height );
	bound.y += INNER_SHADOW_WIDTH( sd ) * 2 + BLUR_WIDTH( sd );
	bound.width = bound.height = BLUR_WIDTH( sd );
	pos.x = bound.x + BLUR_WIDTH( sd );
	pos.y = bound.y;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		pos.x -= bound.x;
		pos.y -= bound.y;
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, BLUR_WIDTH( sd ), sd->color );
	}
}

static void Graph_DrawBottomRightShadow( LCUI_PaintContext paint,
					 const LCUI_Rect *box,
					 const LCUI_BoxShadow *sd )
{
	LCUI_Pos pos;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	bound.width = bound.height = BLUR_WIDTH( sd );
	bound.x = BoxShadow_GetX( sd ) + BLUR_WIDTH( sd );
	bound.y = BoxShadow_GetY( sd ) + BLUR_WIDTH( sd );
	bound.x += BoxShadow_GetBoxWidth( sd, box->width );
	bound.y += BoxShadow_GetBoxHeight( sd, box->height );
	bound.y += INNER_SHADOW_WIDTH( sd ) * 2 + box->x;
	bound.x += INNER_SHADOW_WIDTH( sd ) * 2 + box->y;
	pos.x = bound.x;
	pos.y = bound.y;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		pos.x -= bound.x;
		pos.y -= bound.y;
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		DrawCircle( &canvas, pos, BLUR_WIDTH( sd ), sd->color );
	}
}

/**
 * 绘制上边阴影
 * @param[in] paint 绘制上下文
 * @param[in] box 可供绘制阴影的区域范围
 * @param[in] shadow 阴影参数
 */
static void Graph_DrawTopShadow( LCUI_PaintContext paint,
				 const LCUI_Rect *box,
				 const LCUI_BoxShadow *sd )
{
	gradient_t g;
	LCUI_Color color;
	LCUI_Graph *graph;
	int t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, paint_area, area;

	t = BLUR_WIDTH( sd );
	gradient_init( &g, t );
	color = sd->color;
	/* 计算阴影内框区域 */
	box_area.x = BoxShadow_GetBoxX( sd );
	box_area.y = BoxShadow_GetBoxY( sd );
	box_area.width = BoxShadow_GetBoxWidth( sd, box->width );
	box_area.height = BoxShadow_GetBoxHeight( sd, box->height );
	/* 计算需要绘制的阴影区域 */
	shadow_area.x = BoxShadow_GetX( sd ) + BLUR_WIDTH( sd );
	shadow_area.y = BoxShadow_GetY( sd );
	shadow_area.width = box_area.width + INNER_SHADOW_WIDTH( sd ) * 2;
	shadow_area.height = BLUR_WIDTH( sd );
	/* 调整坐标 */
	box_area.x += box->x;
	box_area.y += box->y;
	shadow_area.x += box->x;
	shadow_area.y += box->y;
	/* 如果阴影区域不在绘制范围内 */
	if( !LCUIRect_GetOverlayRect( &shadow_area, &paint->rect, &area ) ) {
		return;
	}
	/* 获取有效的绘制范围 */
	Graph_GetValidRect( &paint->canvas, &paint_area );
	graph = Graph_GetQuote( &paint->canvas );
	/* 将坐标转换成相对于绘制区域 */
	shadow_area.x -= paint->rect.x;
	shadow_area.y -= paint->rect.y;
	box_area.x -= paint->rect.x;
	box_area.y -= paint->rect.y;
	area.x -= paint->rect.x;
	area.y -= paint->rect.y;
	bound_x = area.x + area.width;
	bound_y = area.y + area.height;
	/* 避免超出绘制范围 */
	if( bound_x > paint_area.x + paint_area.width ) {
		bound_x = paint_area.x + paint_area.width;
	}
	if( bound_y > paint_area.y + paint_area.height ) {
		bound_y = paint_area.y + paint_area.height;
	}
	t -= area.y - shadow_area.y;
	for( y = area.y; y < bound_y; ++y, --t ) {
		/* 计算当前行像素点的透明度 */
		color.alpha = gradient_compute( &g, t, sd->color.a );
		for( x = area.x; x < bound_x; ++x ) {
			if( y >= box_area.y && x >= box_area.x &&
			    y < box_area.y + box_area.height &&
			    x < box_area.x + box_area.width ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawBottomShadow( LCUI_PaintContext paint,
				    const LCUI_Rect *box,
				    const LCUI_BoxShadow *sd )
{
	gradient_t g;
	LCUI_Color color;
	LCUI_Graph *graph;
	int t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, paint_area, area;

	t = BLUR_WIDTH( sd );
	gradient_init( &g, t );
	color = sd->color;
	box_area.x = BoxShadow_GetBoxX( sd );
	box_area.y = BoxShadow_GetBoxY( sd );
	box_area.width = BoxShadow_GetBoxWidth( sd, box->width );
	box_area.height = BoxShadow_GetBoxHeight( sd, box->height );
	shadow_area.x = BoxShadow_GetX( sd ) + BLUR_WIDTH( sd );
	shadow_area.y = BoxShadow_GetY( sd ) + BLUR_WIDTH( sd );
	shadow_area.y += box_area.height + INNER_SHADOW_WIDTH( sd ) * 2;
	shadow_area.width = box_area.width + INNER_SHADOW_WIDTH( sd ) * 2;
	shadow_area.height = BLUR_WIDTH( sd );
	box_area.x += box->x;
	box_area.y += box->y;
	shadow_area.x += box->x;
	shadow_area.y += box->y;
	if( !LCUIRect_GetOverlayRect( &shadow_area, &paint->rect, &area ) ) {
		return;
	}
	Graph_GetValidRect( &paint->canvas, &paint_area );
	graph = Graph_GetQuote( &paint->canvas );
	shadow_area.x -= paint->rect.x;
	shadow_area.y -= paint->rect.y;
	box_area.x -= paint->rect.x;
	box_area.y -= paint->rect.y;
	area.x -= paint->rect.x;
	area.y -= paint->rect.y;
	bound_x = area.x + area.width;
	bound_y = area.y + area.height;
	if( bound_x > paint_area.x + paint_area.width ) {
		bound_x = paint_area.x + paint_area.width;
	}
	if( bound_y > paint_area.y + paint_area.height ) {
		bound_y = paint_area.y + paint_area.height;
	}
	for( t = area.y - shadow_area.y, y = area.y; y < bound_y; ++y, ++t ) {
		color.alpha = gradient_compute( &g, t, sd->color.a );
		for( x = area.x; x < bound_x; ++x ) {
			if( y >= box_area.y && x >= box_area.x && 
			    y < box_area.y + box_area.height &&
			    x < box_area.x + box_area.width ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawLeftShadow( LCUI_PaintContext paint,
				  const LCUI_Rect *box,
				  const LCUI_BoxShadow *sd )
{
	gradient_t g;
	LCUI_Color color;
	LCUI_Graph *graph;
	int t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, paint_area, area;

	t = BLUR_WIDTH( sd );
	gradient_init( &g, t );
	color = sd->color;
	box_area.x = BoxShadow_GetBoxX( sd );
	box_area.y = BoxShadow_GetBoxY( sd );
	box_area.width = BoxShadow_GetBoxWidth( sd, box->width );
	box_area.height = BoxShadow_GetBoxHeight( sd, box->height );
	shadow_area.x = BoxShadow_GetX( sd );
	shadow_area.y = BoxShadow_GetY( sd ) + BLUR_WIDTH( sd );
	shadow_area.width = BLUR_WIDTH( sd );
	shadow_area.height = box_area.height + INNER_SHADOW_WIDTH( sd ) * 2;
	box_area.x += box->x;
	box_area.y += box->y;
	shadow_area.x += box->x;
	shadow_area.y += box->y;
	if( !LCUIRect_GetOverlayRect( &shadow_area, &paint->rect, &area ) ) {
		return;
	}
	Graph_GetValidRect( &paint->canvas, &paint_area );
	graph = Graph_GetQuote( &paint->canvas );
	shadow_area.x -= paint->rect.x;
	shadow_area.y -= paint->rect.y;
	box_area.x -= paint->rect.x;
	box_area.y -= paint->rect.y;
	area.x -= paint->rect.x;
	area.y -= paint->rect.y;
	bound_x = area.x + area.width;
	bound_y = area.y + area.height;
	if( bound_x > paint_area.x + paint_area.width ) {
		bound_x = paint_area.x + paint_area.width;
	}
	if( bound_y > paint_area.y + paint_area.height ) {
		bound_y = paint_area.y + paint_area.height;
	}
	t -= area.x - shadow_area.x;
	for( x = area.x; x < bound_x; ++x, --t ) {
		color.alpha = gradient_compute( &g, t, sd->color.a );
		for( y = area.y; y < bound_y; ++y ) {
			if( y >= box_area.y && x >= box_area.x  &&
			    y < box_area.y + box_area.height &&
			    x < box_area.x + box_area.width ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

static void Graph_DrawRightShadow( LCUI_PaintContext paint,
				   const LCUI_Rect *box,
				   const LCUI_BoxShadow *sd )
{
	gradient_t g;
	LCUI_Color color;
	LCUI_Graph *graph;
	int t, x, y, bound_x, bound_y;
	LCUI_Rect shadow_area, box_area, paint_area, area;

	t = BLUR_WIDTH( sd );
	gradient_init( &g, t );
	color = sd->color;
	box_area.x = BoxShadow_GetBoxX( sd );
	box_area.y = BoxShadow_GetBoxY( sd );
	box_area.width = BoxShadow_GetBoxWidth( sd, box->width );
	box_area.height = BoxShadow_GetBoxHeight( sd, box->height );
	shadow_area.x = BoxShadow_GetX( sd ) + SHADOW_WIDTH( sd );
	shadow_area.x += box_area.width + INNER_SHADOW_WIDTH( sd );
	shadow_area.y = BoxShadow_GetY( sd ) + BLUR_WIDTH( sd );
	shadow_area.width = BLUR_WIDTH( sd );
	shadow_area.height = box_area.height + INNER_SHADOW_WIDTH( sd ) * 2;
	box_area.x += box->x;
	box_area.y += box->y;
	shadow_area.x += box->x;
	shadow_area.y += box->y;
	if( !LCUIRect_GetOverlayRect( &shadow_area, &paint->rect, &area ) ) {
		return;
	}
	Graph_GetValidRect( &paint->canvas, &paint_area );
	graph = Graph_GetQuote( &paint->canvas );
	shadow_area.x -= paint->rect.x;
	shadow_area.y -= paint->rect.y;
	box_area.x -= paint->rect.x;
	box_area.y -= paint->rect.y;
	area.x -= paint->rect.x;
	area.y -= paint->rect.y;
	bound_x = area.x + area.width;
	bound_y = area.y + area.height;
	if( bound_x > paint_area.x + paint_area.width ) {
		bound_x = paint_area.x + paint_area.width;
	}
	if( bound_y > paint_area.y + paint_area.height ) {
		bound_y = paint_area.y + paint_area.height;
	}
	t = area.x - shadow_area.x;
	for( x = area.x; x < bound_x; ++x, ++t ) {
		color.alpha = gradient_compute( &g, t, sd->color.a );
		for( y = area.y; y < bound_y; ++y ) {
			if( y >= box_area.y && x >= box_area.x &&
			    y < box_area.y + box_area.height &&
			    x < box_area.x + box_area.width ) {
				continue;
			}
			Graph_SetPixel( graph, x, y, color );
		}
	}
}

void Graph_ClearShadowArea( LCUI_PaintContext paint,
			    const LCUI_Rect *box,
			    const LCUI_BoxShadow *shadow )
{
	int i;
	LCUI_Graph canvas;
	LCUI_Color color = { 0 };
	LCUI_Rect rect, box_area, rects[4];

	rect.width = box->width;
	rect.height = box->height;
	rect.x = rect.y = 0;
	/* 获取内容框区域 */
	box_area.x = box->x + BoxShadow_GetBoxX( shadow );
	box_area.y = box->y + BoxShadow_GetBoxY( shadow );
	box_area.width = BoxShadow_GetBoxWidth( shadow, box->width );
	box_area.height = BoxShadow_GetBoxHeight( shadow, box->height );
	/* 获取内容框外的阴影区域 */
	LCUIRect_CutFourRect( &box_area, &rect, rects );
	for( i = 0; i < 4; ++i ) {
		if( LCUIRect_GetOverlayRect( &paint->rect,
					     &rects[i], &rects[i] ) ) {
			rects[i].x -= paint->rect.x;
			rects[i].y -= paint->rect.y;
			Graph_Quote( &canvas, &paint->canvas, &rects[i] );
			Graph_FillRect( &canvas, color, NULL, TRUE );
		}
	}
}

static void Graph_DrawInnerShadow( LCUI_PaintContext paint,
				   const LCUI_Rect *box,
				   const LCUI_BoxShadow *s )
{
	int i;
	LCUI_Graph canvas;
	LCUI_Rect rsd, rb, rects[4];
	rb.x = BoxShadow_GetBoxX( s );
	rb.y = BoxShadow_GetBoxY( s );
	rb.width = BoxShadow_GetBoxWidth( s, box->width );
	rb.height = BoxShadow_GetBoxHeight( s, box->height );
	rsd.x = BoxShadow_GetX( s ) + BLUR_WIDTH(s);
	rsd.y = BoxShadow_GetY( s ) + BLUR_WIDTH(s);
	rsd.width = rb.width + INNER_SHADOW_WIDTH(s)*2;
	rsd.height = rb.height + INNER_SHADOW_WIDTH(s)*2;
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

int Graph_DrawBoxShadow( LCUI_PaintContext paint,
			 const LCUI_Rect *box,
			 const LCUI_BoxShadow *shadow )
{
	/* 判断容器尺寸是否低于阴影占用的最小尺寸 */
	if( box->width < BoxShadow_GetWidth(shadow, 0)
	 || box->height < BoxShadow_GetHeight(shadow, 0) ) {
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
