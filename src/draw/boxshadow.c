/*
 * boxshadow.c -- Box shadow drawing
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#define BLUR_N 1.5
#define BLUR_WIDTH(sd) (int)(sd->blur * BLUR_N)
#define INNER_SHADOW_WIDTH(sd) (SHADOW_WIDTH(sd) - BLUR_WIDTH(sd))
#define POW2(X) ((X) * (X))

typedef struct BoxShadowRenderingContextRec {
	int max_radius;
	const LCUI_BoxShadow *shadow;
	const LCUI_Rect *box;
	LCUI_Rect shadow_box;
	LCUI_Rect content_box;
	LCUI_PaintContext paint;
} BoxShadowRenderingContextRec, *BoxShadowRenderingContext;

typedef struct gradient {
	int s;
	double v;
	double a;
} gradient_t;

/** 初始化渐变生成器 */
static void gradient_init(gradient_t *g, double t)
{
	/**
	 * s = vt - at²/2
	 * a = 2x(vt - s)/t²
	 */
	g->s = 255;
	g->v = 512.0f / t;
	g->a = 2 * (g->v * t - g->s) / (t * t);
}

/** 计算渐变后的值 */
static uchar_t gradient_compute(gradient_t *g, double t, int val)
{
	val *= g->s - (int)(g->v * t - (g->a * t * t) / 2.0);
	return (uchar_t)(val >> 8);
}

static INLINE int BoxShadow_GetWidth(const LCUI_BoxShadow *shadow,
				     int content_width)
{
	return content_width + SHADOW_WIDTH(shadow) * 2;
}

static INLINE int BoxShadow_GetHeight(const LCUI_BoxShadow *shadow,
				      int content_height)
{
	return content_height + SHADOW_WIDTH(shadow) * 2;
}

static int BoxShadow_GetBoxX(const LCUI_BoxShadow *shadow)
{
	return shadow->x >= SHADOW_WIDTH(shadow)
		   ? 0
		   : SHADOW_WIDTH(shadow) - shadow->x;
}

static int BoxShadow_GetBoxY(const LCUI_BoxShadow *shadow)
{
	return shadow->y >= SHADOW_WIDTH(shadow)
		   ? 0
		   : SHADOW_WIDTH(shadow) - shadow->y;
}

static int BoxShadow_GetY(const LCUI_BoxShadow *shadow)
{
	return shadow->y <= SHADOW_WIDTH(shadow)
		   ? 0
		   : shadow->y - SHADOW_WIDTH(shadow);
}

static int BoxShadow_GetX(const LCUI_BoxShadow *shadow)
{
	return shadow->x <= SHADOW_WIDTH(shadow)
		   ? 0
		   : shadow->x - SHADOW_WIDTH(shadow);
}

void BoxShadow_Init(LCUI_BoxShadow *shadow)
{
	shadow->color.r = 0;
	shadow->color.g = 0;
	shadow->color.b = 0;
	shadow->blur = 0;
	shadow->spread = 0;
	shadow->x = 0;
	shadow->y = 0;
}

void BoxShadow_GetCanvasRect(const LCUI_BoxShadow *shadow,
			     const LCUI_Rect *content_rect,
			     LCUI_Rect *canvas_rect)
{
	LCUI_Rect shadow_rect;

	shadow_rect.x = content_rect->x - SHADOW_WIDTH(shadow) + shadow->x;
	shadow_rect.y = content_rect->y - SHADOW_WIDTH(shadow) + shadow->y;
	shadow_rect.width = BoxShadow_GetWidth(shadow, content_rect->width);
	shadow_rect.height = BoxShadow_GetHeight(shadow, content_rect->height);
	canvas_rect->x = min(content_rect->x, shadow_rect.x);
	canvas_rect->y = min(content_rect->y, shadow_rect.y);
	canvas_rect->width = max(shadow_rect.x + shadow_rect.width,
				 content_rect->x + content_rect->width) -
			     canvas_rect->x;
	canvas_rect->height = max(shadow_rect.y + shadow_rect.height,
				  content_rect->y + content_rect->height) -
			      canvas_rect->y;
}

/** Check if a pixel is in the content area */
static LCUI_BOOL CheckPixelInContentBox(BoxShadowRenderingContext ctx, int x,
					int y)
{
	double r;
	double circle_x;
	double circle_y;
	double center_x;
	double center_y;

	x -= ctx->content_box.x;
	y -= ctx->content_box.y;
	if (x < 0 || x >= ctx->content_box.width || y < 0 ||
	    y >= ctx->content_box.height) {
		return FALSE;
	}

	r = ctx->shadow->top_left_radius;
	if (x <= r && y <= r) {
		circle_x = x - r;
		circle_y = r - y;
		return circle_x >= -sqrt(r * r - circle_y * circle_y);
	}

	r = ctx->shadow->top_right_radius;
	center_y = r - 0.5;
	center_x = ctx->content_box.width - r - 0.5;
	if (x >= center_x && y <= center_y) {
		circle_x = x - center_x;
		circle_y = center_y - y;
		return circle_x <= sqrt(r * r - circle_y * circle_y);
	}

	r = ctx->shadow->bottom_left_radius;
	center_x = r - 0.5;
	center_y = ctx->content_box.height - r - 0.5;
	if (x <= center_x && y >= center_y) {
		circle_x = x - center_x;
		circle_y = center_y - y;
		return circle_x >= -sqrt(r * r - circle_y * circle_y);
	}

	r = ctx->shadow->bottom_right_radius;
	center_x = ctx->content_box.width - r - 0.5;
	center_y = ctx->content_box.height - r - 0.5;
	if (x >= center_x && y >= center_y) {
		circle_x = x - center_x;
		circle_y = center_y - y;
		return circle_x <= sqrt(r * r - circle_y * circle_y);
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintLeftBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int ix, iy;
	int right;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;
	LCUI_Color color = ctx->shadow->color;

	rect.x = ctx->shadow_box.x;
	rect.y = ctx->shadow_box.y;
	rect.width = BLUR_WIDTH(ctx->shadow);
	rect.height = ctx->shadow_box.height;
	right = rect.x + rect.width;
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, &rect, &rect)) {
		return FALSE;
	}
	right -= rect.x;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		iy = rect.y + y;
		pixel = canvas->argb + (paint_rect.y + y) * canvas->width +
			paint_rect.x;
		for (x = 0; x < paint_rect.width; ++x, ++pixel) {
			ix = rect.x + x;
			if (CheckPixelInContentBox(ctx, ix, iy)) {
				continue;
			}
			color.alpha = gradient_compute(&g, right - x,
						       ctx->shadow->color.a);
			*pixel = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintRightBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int ix, iy;
	int left;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;
	LCUI_Color color = ctx->shadow->color;

	rect.width = BLUR_WIDTH(ctx->shadow);
	rect.height = ctx->shadow_box.height;
	rect.x = ctx->shadow_box.x + ctx->shadow_box.width - rect.width;
	rect.y = ctx->shadow_box.y;
	left = rect.x;
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, &rect, &rect)) {
		return FALSE;
	}
	left -= rect.x;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		iy = rect.y + y;
		pixel = canvas->argb + (paint_rect.y + y) * canvas->width +
			paint_rect.x;
		for (x = 0; x < paint_rect.width; ++x, ++pixel) {
			ix = rect.x + x;
			if (CheckPixelInContentBox(ctx, ix, iy)) {
				continue;
			}
			color.alpha = gradient_compute(&g, x - left,
						       ctx->shadow->color.a);
			*pixel = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintTopBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int ix, iy;
	int bottom;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;
	LCUI_Color color = ctx->shadow->color;

	rect.width = ctx->shadow_box.width;
	rect.height = BLUR_WIDTH(ctx->shadow);
	rect.x = ctx->shadow_box.x;
	rect.y = ctx->shadow_box.y;
	bottom = rect.y + rect.height;
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, &rect, &rect)) {
		return FALSE;
	}
	bottom -= rect.y;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		iy = rect.y + y;
		pixel = canvas->argb + (paint_rect.y + y) * canvas->width +
			paint_rect.x;
		color.alpha =
		    gradient_compute(&g, bottom - y, ctx->shadow->color.a);
		for (x = 0; x < paint_rect.width; ++x, ++pixel) {
			ix = rect.x + x;
			if (CheckPixelInContentBox(ctx, ix, iy)) {
				continue;
			}
			*pixel = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintBottomBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int ix, iy;
	int top;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;
	LCUI_Color color = ctx->shadow->color;

	rect.width = ctx->shadow_box.width;
	rect.height = BLUR_WIDTH(ctx->shadow);
	rect.x = ctx->shadow_box.x;
	rect.y = ctx->shadow_box.y + ctx->shadow_box.height - rect.height;
	top = rect.y;
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, &rect, &rect)) {
		return FALSE;
	}
	top -= rect.y;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		iy = rect.y + y;
		pixel = canvas->argb + (paint_rect.y + y) * canvas->width +
			paint_rect.x;
		color.alpha =
		    gradient_compute(&g, y - top, ctx->shadow->color.a);
		for (x = 0; x < paint_rect.width; ++x, ++pixel) {
			ix = rect.x + x;
			if (CheckPixelInContentBox(ctx, ix, iy)) {
				continue;
			}
			*pixel = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintCircleBlur(BoxShadowRenderingContext ctx,
					   const LCUI_Rect *circle_rect,
					   double center_x, double center_y,
					   int radius)
{
	int x, y;
	int y2;
	int left, top;
	double r;
	double r2;
	double outer_r2;
	double inner_r2;
	double inner_r;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Graph ref;
	LCUI_Color color;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;

	r = max(0, radius);
	inner_r = max(0, r - BLUR_WIDTH(ctx->shadow));
	inner_r2 = POW2(inner_r);
	outer_r2 = POW2(r);
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, circle_rect, &rect)) {
		return FALSE;
	}
	top = rect.y;
	left = rect.x;
	color = ctx->shadow->color;
	rect.x -= ctx->paint->rect.x;
	rect.y -= ctx->paint->rect.y;
	center_x = center_x + circle_rect->x - rect.x;
	center_y = center_y + circle_rect->y - rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &rect);
	Graph_GetValidRect(&ref, &rect);
	canvas = Graph_GetQuote(&ref);
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	for (y = 0; y < rect.height; ++y) {
		y2 = POW2(center_y - y);
		pixel = Graph_GetPixelPointer(canvas, rect.x, rect.y + y);
		for (x = 0; x < rect.width; ++x, ++pixel) {
			if (CheckPixelInContentBox(ctx, left + x, top + y)) {
				continue;
			}
			r2 = y2 + POW2(x - center_x);
			if (r2 > outer_r2) {
				color.alpha = 0;
				*pixel = color;
				continue;
			}
			if (r2 > inner_r2) {
				r = sqrt(r2);
				color.alpha = gradient_compute(
				    &g, r - inner_r, ctx->shadow->color.alpha);
			} else {
				color.alpha = ctx->shadow->color.alpha;
			}
			*pixel = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintTopLeftBlur(BoxShadowRenderingContext ctx)
{
	int radius;
	LCUI_Rect rect;

	radius = BLUR_WIDTH(ctx->shadow) + ctx->shadow->top_left_radius;
	radius = min(ctx->max_radius, radius);
	rect.width = radius;
	rect.height = rect.width;
	rect.x = ctx->shadow_box.x;
	rect.y = ctx->shadow_box.y;
	return BoxShadow_PaintCircleBlur(ctx, &rect, rect.width, rect.height,
					 radius);
}

static LCUI_BOOL BoxShadow_PaintTopRightBlur(BoxShadowRenderingContext ctx)
{
	int radius;
	LCUI_Rect rect;

	radius = BLUR_WIDTH(ctx->shadow) + ctx->shadow->top_right_radius;
	radius = min(ctx->max_radius, radius);
	rect.width = radius;
	rect.height = rect.width;
	rect.x = ctx->shadow_box.x + ctx->shadow_box.width - rect.width;
	rect.y = ctx->shadow_box.y;
	return BoxShadow_PaintCircleBlur(ctx, &rect, 0, rect.height, radius);
}

static LCUI_BOOL BoxShadow_PaintBottomLeftBlur(BoxShadowRenderingContext ctx)
{
	int radius;
	LCUI_Rect rect;

	radius = BLUR_WIDTH(ctx->shadow) + ctx->shadow->bottom_left_radius;
	radius = min(ctx->max_radius, radius);
	rect.width = radius;
	rect.height = rect.width;
	rect.x = ctx->shadow_box.x;
	rect.y = ctx->shadow_box.y + ctx->shadow_box.height - rect.height;
	return BoxShadow_PaintCircleBlur(ctx, &rect, rect.width, 0, radius);
}

static LCUI_BOOL BoxShadow_PaintBottomRightBlur(BoxShadowRenderingContext ctx)
{
	int radius;
	LCUI_Rect rect;

	radius = BLUR_WIDTH(ctx->shadow) + ctx->shadow->bottom_right_radius;
	radius = min(ctx->max_radius, radius);
	rect.width = radius;
	rect.height = rect.width;
	rect.x = ctx->shadow_box.x + ctx->shadow_box.width - rect.width;
	rect.y = ctx->shadow_box.y + ctx->shadow_box.height - rect.height;
	return BoxShadow_PaintCircleBlur(ctx, &rect, 0, 0, radius);
}

static LCUI_BOOL FillPixelsOutsideCircle(BoxShadowRenderingContext ctx,
					 const LCUI_Rect *circle_rect,
					 int center_x, int center_y, int radius)
{
	int x, y;
	double circle_x;
	int right;
	int r2 = radius * radius;

	LCUI_Rect rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *pixel;
	LCUI_ARGB *row;

	if (!LCUIRect_GetOverlayRect(&ctx->shadow_box, circle_rect, &rect)) {
		return FALSE;
	}
	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, &rect, &rect)) {
		return FALSE;
	}
	center_x = circle_rect->x + center_x - rect.x;
	center_y = circle_rect->y + center_y - rect.y;
	rect.x -= ctx->paint->rect.x;
	rect.y -= ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &rect);
	Graph_GetValidRect(&ref, &rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < rect.height; ++y) {
		row = canvas->argb + (rect.y + y) * canvas->width + rect.x;
		circle_x = sqrt(r2 - POW2(center_y - y));
		pixel = row;
		right = center_x - (int)circle_x - 1;
		right = min(right, rect.width);
		for (x = 0; x < right; ++x) {
			LCUI_OverPixel(pixel, &ctx->shadow->color);
			++pixel;
		}
		x = center_x + (int)circle_x + 1;
		pixel = row + x;
		for (; x < rect.width; ++x) {
			LCUI_OverPixel(pixel, &ctx->shadow->color);
			++pixel;
		}
	}
	return TRUE;
}

static void BoxShadow_FillRect(BoxShadowRenderingContext ctx)
{
	int i;

	LCUI_Rect rect;
	LCUI_Rect rects[4];

	/* Fill the shadow area, not including the content area */
	LCUIRect_Split(&ctx->content_box, &ctx->shadow_box, rects);
	for (i = 0; i < 4; ++i) {
		if (rects[i].width < 1 || rects[i].height < 1) {
			continue;
		}
		LCUIRect_GetOverlayRect(&rects[i], &ctx->paint->rect, &rect);
		Graph_FillRect(&ctx->paint->canvas, ctx->shadow->color, &rect,
			       TRUE);
	}
	rect.width = rect.height = ctx->shadow->top_left_radius;
	rect.x = ctx->content_box.x;
	rect.y = ctx->content_box.y;
	FillPixelsOutsideCircle(ctx, &rect, rect.width, rect.width, rect.width);

	rect.width = rect.height = ctx->shadow->top_right_radius;
	rect.x = ctx->content_box.x + ctx->content_box.width - rect.width;
	rect.y = ctx->content_box.y;
	FillPixelsOutsideCircle(ctx, &rect, 0, rect.height, rect.width);

	rect.width = rect.height = ctx->shadow->bottom_left_radius;
	rect.x = ctx->content_box.x;
	rect.y = ctx->content_box.y + ctx->content_box.height - rect.height;
	FillPixelsOutsideCircle(ctx, &rect, rect.width, 0, rect.width);

	rect.width = rect.height = ctx->shadow->bottom_right_radius;
	rect.x = ctx->content_box.x + ctx->content_box.width - rect.width;
	rect.y = ctx->content_box.y + ctx->content_box.height - rect.height;
	FillPixelsOutsideCircle(ctx, &rect, 0, 0, rect.width);
}

int BoxShadow_Paint(const LCUI_BoxShadow *shadow, const LCUI_Rect *box,
		    int content_width, int content_height,
		    LCUI_PaintContext paint)
{
	BoxShadowRenderingContextRec ctx;

	/* 判断容器尺寸是否低于阴影占用的最小尺寸 */
	if (box->width < BoxShadow_GetWidth(shadow, 0) ||
	    box->height < BoxShadow_GetHeight(shadow, 0)) {
		return -1;
	}
	if (SHADOW_WIDTH(shadow) == 0 && shadow->x == 0 && shadow->y == 0) {
		return 0;
	}
	ctx.box = box;
	ctx.paint = paint;
	ctx.shadow = shadow;
	ctx.max_radius =
	    min(content_width, content_height) / 2 + SHADOW_WIDTH(shadow);
	ctx.shadow_box.x = BoxShadow_GetX(shadow);
	ctx.shadow_box.y = BoxShadow_GetY(shadow);
	ctx.shadow_box.width = BoxShadow_GetWidth(shadow, content_width);
	ctx.shadow_box.height = BoxShadow_GetWidth(shadow, content_height);
	ctx.content_box.x = BoxShadow_GetBoxX(shadow);
	ctx.content_box.y = BoxShadow_GetBoxY(shadow);
	ctx.content_box.width = content_width;
	ctx.content_box.height = content_height;
	BoxShadow_FillRect(&ctx);
	BoxShadow_PaintLeftBlur(&ctx);
	BoxShadow_PaintRightBlur(&ctx);
	BoxShadow_PaintTopBlur(&ctx);
	BoxShadow_PaintBottomBlur(&ctx);
	BoxShadow_PaintTopLeftBlur(&ctx);
	BoxShadow_PaintTopRightBlur(&ctx);
	BoxShadow_PaintBottomLeftBlur(&ctx);
	BoxShadow_PaintBottomRightBlur(&ctx);
	return 0;
}
