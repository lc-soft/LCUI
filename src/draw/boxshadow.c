/*
 * boxshadow.c -- Box shadow draw support.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


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

int BoxShadow_GetBoxWidth( const LCUI_BoxShadow *shadow, int w )
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

int BoxShadow_GetBoxHeight( const LCUI_BoxShadow *shadow, int h )
{
	h -= SHADOW_WIDTH( shadow ) * 2;
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return h - shadow->y + SHADOW_WIDTH( shadow );
	} else if( shadow->y <= -SHADOW_WIDTH( shadow ) ) {
		return h + shadow->y + SHADOW_WIDTH( shadow );
	}
	return h;
}

int BoxShadow_GetWidth( const LCUI_BoxShadow *shadow, int box_w )
{
	if( shadow->x >= SHADOW_WIDTH( shadow ) ) {
		return box_w + SHADOW_WIDTH( shadow ) + shadow->x;
	} else if( shadow->x <= -SHADOW_WIDTH( shadow ) ) {
		return box_w + SHADOW_WIDTH( shadow ) - shadow->x;
	}
	return box_w + SHADOW_WIDTH( shadow ) * 2;
}

int BoxShadow_GetHeight( const LCUI_BoxShadow *shadow, int box_h )
{
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return box_h + SHADOW_WIDTH( shadow ) + shadow->y;
	} else if( shadow->y <= -SHADOW_WIDTH( shadow ) ) {
		return box_h + SHADOW_WIDTH( shadow ) - shadow->y;
	}
	return box_h + SHADOW_WIDTH( shadow ) * 2;
}

int BoxShadow_GetBoxX( const LCUI_BoxShadow *shadow )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

int BoxShadow_GetBoxY( const LCUI_BoxShadow *shadow )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

int BoxShadow_GetY( const LCUI_BoxShadow *shadow )
{
	if( shadow->y <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->y - SHADOW_WIDTH(shadow);
}

int BoxShadow_GetX( const LCUI_BoxShadow *shadow )
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

static void BoxShadow_PaintTopLeft( const LCUI_BoxShadow *sd,
				    const LCUI_Rect *box,
				    LCUI_PaintContext paint )
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

static void BoxShadow_PaintTopRight( const LCUI_BoxShadow *sd,
				     const LCUI_Rect *box,
				     LCUI_PaintContext paint )
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

static void BoxShadow_PaintBottomLeft( const LCUI_BoxShadow *sd,
				       const LCUI_Rect *box,
				       LCUI_PaintContext paint )
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

static void BoxShadow_PaintBottomRight( const LCUI_BoxShadow *sd,
					const LCUI_Rect *box,
					LCUI_PaintContext paint )
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
static void BoxShadow_PaintTop( const LCUI_BoxShadow *sd,
				const LCUI_Rect *box,
				LCUI_PaintContext paint )
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

static void BoxShadow_PaintBottom( const LCUI_BoxShadow *sd,
				   const LCUI_Rect *box,
				   LCUI_PaintContext paint )
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

static void BoxShadow_PaintLeft( const LCUI_BoxShadow *sd,
				 const LCUI_Rect *box,
				 LCUI_PaintContext paint )
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

static void BoxShadow_PaintRight( const LCUI_BoxShadow *sd,
				  const LCUI_Rect *box,
				  LCUI_PaintContext paint )
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

static void BoxShadow_PaintInner( const LCUI_BoxShadow *s,
				  const LCUI_Rect *box,
				  LCUI_PaintContext paint )
{
	int i;
	LCUI_Graph canvas;
	LCUI_Rect rsd, rb, rects[4];
	rb.x = BoxShadow_GetBoxX( s );
	rb.y = BoxShadow_GetBoxY( s );
	rb.width = BoxShadow_GetBoxWidth( s, box->width );
	rb.height = BoxShadow_GetBoxHeight( s, box->height );
	rsd.x = BoxShadow_GetX( s ) + BLUR_WIDTH( s );
	rsd.y = BoxShadow_GetY( s ) + BLUR_WIDTH( s );
	rsd.width = rb.width + INNER_SHADOW_WIDTH( s ) * 2;
	rsd.height = rb.height + INNER_SHADOW_WIDTH( s ) * 2;
	/* 截取出与内容区重叠的区域 */
	if( LCUIRect_GetOverlayRect( &rb, &rsd, &rb ) ) {
		LCUIRect_CutFourRect( &rb, &rsd, rects );
		/* 从阴影区域中排除部件占用的区域 */
		for( i = 0; i < 4; ++i ) {
			if( !LCUIRect_GetOverlayRect(
				&paint->rect, &rects[i], &rects[i] ) ) {
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
	if( LCUIRect_GetOverlayRect( &paint->rect, &rsd, &rb ) ) {
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

void BoxShadow_GetCanvasRect( const LCUI_BoxShadow *shadow,
			      const LCUI_Rect *box_rect,
			      LCUI_Rect *canvas_rect )
{
	canvas_rect->x = box_rect->x - BoxShadow_GetBoxX( shadow );
	canvas_rect->y = box_rect->y - BoxShadow_GetBoxY( shadow );
	canvas_rect->width = BoxShadow_GetWidth( shadow, box_rect->width );
	canvas_rect->height = BoxShadow_GetHeight( shadow, box_rect->height );
}

int BoxShadow_Paint( const LCUI_BoxShadow *shadow,
		     const LCUI_Rect *box,
		     LCUI_PaintContext paint )
{
	/* 判断容器尺寸是否低于阴影占用的最小尺寸 */
	if( box->width < BoxShadow_GetWidth( shadow, 0 ) ||
	    box->height < BoxShadow_GetHeight( shadow, 0 ) ) {
		return -1;
	}
	BoxShadow_PaintInner( shadow, box, paint );
	BoxShadow_PaintTopLeft( shadow, box, paint );
	BoxShadow_PaintTopRight( shadow, box, paint );
	BoxShadow_PaintBottomLeft( shadow, box, paint );
	BoxShadow_PaintBottomRight( shadow, box, paint );
	BoxShadow_PaintTop( shadow, box, paint );
	BoxShadow_PaintBottom( shadow, box, paint );
	BoxShadow_PaintLeft( shadow, box, paint );
	BoxShadow_PaintRight( shadow, box, paint );
	return 0;
}
