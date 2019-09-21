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
#define FULL_SHADOW_WIDTH(CTX)                                    \
	SHADOW_WIDTH((CTX)->shadow) + BLUR_WIDTH((CTX)->shadow) - \
	    (CTX)->shadow->blur

#define POW2(X) ((X) * (X))
#define CIRCLE_R(R) (R - 0.5)

/*  Convert screen Y coordinate to geometric Y coordinate */
#define ToGeoY(Y, CENTER_Y) ((CENTER_Y)-Y)

/*  Convert screen X coordinate to geometric X coordinate */
#define ToGeoX(X, CENTER_X) (X - (CENTER_X))

#define SmoothLeftPixel(PX, X) (uchar_t)((PX)->a * (1.0 - (X - 1.0 * (int)X)))
#define SmoothRightPixel(PX, X) (uchar_t)((PX)->a * (X - 1.0 * (int)X))

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

INLINE int BoxShadow_GetWidth(const LCUI_BoxShadow *shadow, int content_width)
{
	return content_width + SHADOW_WIDTH(shadow) * 2;
}

INLINE int BoxShadow_GetHeight(const LCUI_BoxShadow *shadow, int content_height)
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

static LCUI_BOOL BoxShadow_PaintLeftBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int right;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *p;
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
		p = Graph_GetPixelPointer(canvas, paint_rect.x,
					  paint_rect.y + y);
		for (x = 0; x < paint_rect.width; ++x, ++p) {
			color.alpha = gradient_compute(&g, right - x,
						       ctx->shadow->color.a);
			*p = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintRightBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int left;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *p;
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
	left -= rect.x + 1;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		p = Graph_GetPixelPointer(canvas, paint_rect.x,
					  paint_rect.y + y);
		for (x = 0; x < paint_rect.width; ++x, ++p) {
			color.alpha = gradient_compute(&g, x - left,
						       ctx->shadow->color.a);
			*p = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintTopBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int bottom;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *p;
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
		p = Graph_GetPixelPointer(canvas, paint_rect.x,
					  paint_rect.y + y);
		color.alpha =
		    gradient_compute(&g, bottom - y, ctx->shadow->color.a);
		for (x = 0; x < paint_rect.width; ++x, ++p) {
			*p = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintBottomBlur(BoxShadowRenderingContext ctx)
{
	int x, y;
	int top;
	gradient_t g;

	LCUI_Rect rect;
	LCUI_Rect paint_rect;
	LCUI_Graph ref;
	LCUI_Graph *canvas;
	LCUI_ARGB *p;
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
	top -= rect.y + 1.0;
	paint_rect.width = rect.width;
	paint_rect.height = rect.height;
	paint_rect.x = rect.x - ctx->paint->rect.x;
	paint_rect.y = rect.y - ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &paint_rect);
	Graph_GetValidRect(&ref, &paint_rect);
	canvas = Graph_GetQuote(&ref);
	for (y = 0; y < paint_rect.height; ++y) {
		p = Graph_GetPixelPointer(canvas, paint_rect.x,
					  paint_rect.y + y);
		color.a = gradient_compute(&g, y - top, ctx->shadow->color.a);
		for (x = 0; x < paint_rect.width; ++x, ++p) {
			*p = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintCircleBlur(BoxShadowRenderingContext ctx,
					   const LCUI_Rect *circle_rect,
					   double center_x, double center_y,
					   int radius)
{
	double d;
	double y2;
	double r = max(0, CIRCLE_R(radius));
	double outer_r2 = POW2(r);
	double inner_r = max(0, r - BLUR_WIDTH(ctx->shadow));
	double inner_smooth_r2 = POW2(inner_r + 1.0);
	double inner_r2 = POW2(inner_r);

	int x, y;
	unsigned char a = ctx->shadow->color.alpha;

	gradient_t g;

	LCUI_Rect rect;
	LCUI_Graph ref;
	LCUI_Color color;
	LCUI_Graph *canvas;
	LCUI_ARGB *p;

	if (!LCUIRect_GetOverlayRect(&ctx->paint->rect, circle_rect, &rect)) {
		return FALSE;
	}
	color = ctx->shadow->color;
	center_x = center_x + circle_rect->x - rect.x - 0.5;
	center_y = center_y + circle_rect->y - rect.y - 0.5;
	rect.x -= ctx->paint->rect.x;
	rect.y -= ctx->paint->rect.y;
	Graph_Quote(&ref, &ctx->paint->canvas, &rect);
	Graph_GetValidRect(&ref, &rect);
	canvas = Graph_GetQuote(&ref);
	gradient_init(&g, BLUR_WIDTH(ctx->shadow));
	for (y = 0; y < rect.height; ++y) {
		y2 = POW2(ToGeoY(y, center_y));
		p = Graph_GetPixelPointer(canvas, rect.x, rect.y + y);
		for (x = 0; x < rect.width; ++x, ++p) {
			d = y2 + POW2(ToGeoX(x, center_x));
			if (r - inner_r < 0.1) {
				if (d >= inner_smooth_r2) {
					p->alpha = 0;
					continue;
				}
				color.a = a;
				d = sqrt(d) - inner_r;
				if (d > 0) {
					color.a = SmoothLeftPixel(&color, d);
				}
				*p = color;
				continue;
			}
			if (d >= outer_r2) {
				p->alpha = 0;
				continue;
			}
			d = sqrt(d) - inner_r;
			if (d < 0) {
				color.a = a;
				*p = color;
				continue;
			}
			color.a = gradient_compute(&g, d, a);
			*p = color;
		}
	}
	return TRUE;
}

static LCUI_BOOL BoxShadow_PaintTopLeftBlur(BoxShadowRenderingContext ctx)
{
	int radius;
	LCUI_Rect rect;

	radius = FULL_SHADOW_WIDTH(ctx) + ctx->shadow->top_left_radius;
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

	radius = FULL_SHADOW_WIDTH(ctx) + ctx->shadow->top_right_radius;
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

	radius = FULL_SHADOW_WIDTH(ctx) + ctx->shadow->bottom_left_radius;
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

	radius = FULL_SHADOW_WIDTH(ctx) + ctx->shadow->bottom_right_radius;
	radius = min(ctx->max_radius, radius);
	rect.width = radius;
	rect.height = rect.width;
	rect.x = ctx->shadow_box.x + ctx->shadow_box.width - rect.width;
	rect.y = ctx->shadow_box.y + ctx->shadow_box.height - rect.height;
	return BoxShadow_PaintCircleBlur(ctx, &rect, 0, 0, radius);
}

static int ClearPixelsOfCircle(LCUI_Graph *canvas, double center_x,
			       double center_y, int radius)
{
	double r = CIRCLE_R(radius);
	double outer_r2 = POW2(r + 1.0);
	double d;
	double y2;

	int xi, yi;

	LCUI_Rect rect;
	LCUI_ARGB *p;

	Graph_GetValidRect(canvas, &rect);
	canvas = Graph_GetQuote(canvas);
	if (!Graph_IsValid(canvas)) {
		return -1;
	}
	center_x -= 0.5;
	center_y -= 0.5;
	for (yi = 0; yi < rect.height; ++yi) {
		y2 = POW2(ToGeoY(yi, center_y));
		p = Graph_GetPixelPointer(canvas, rect.x, rect.y + yi);
		for (xi = 0; xi < rect.width; ++xi, ++p) {
			d = y2 + POW2(ToGeoX(xi, center_x));
			if (d >= outer_r2) {
				continue;
			}
			d = sqrt(d) - r;
			if (d <= 0) {
				p->alpha = 0;
			} else {
				p->alpha = SmoothRightPixel(p, d);
			}
		}
	}
	return 0;
}

static void BoxShadow_FillRect(BoxShadowRenderingContext ctx)
{
	LCUI_Rect rect;

	rect = ctx->shadow_box;
	if (LCUIRect_GetOverlayRect(&rect, &ctx->paint->rect, &rect)) {
		rect.x -= ctx->paint->rect.x;
		rect.y -= ctx->paint->rect.y;
		Graph_FillRect(&ctx->paint->canvas, ctx->shadow->color, &rect,
			       TRUE);
	}
}

static void BoxShadow_ClearContentRect(BoxShadowRenderingContext ctx)
{
	int r;
	double center_x, center_y;

	LCUI_Rect rect;
	LCUI_Rect bound;
	LCUI_Graph canvas;
	LinkedList rects;
	LinkedListNode *node;

	/* Initialize a queue for recording the area after the split content
	 * area */
	LinkedList_Init(&rects);
	RectList_Add(&rects, &ctx->content_box);

	r = ctx->shadow->top_left_radius;
	bound.x = ctx->content_box.x;
	bound.y = ctx->content_box.y;
	bound.width = r;
	bound.height = r;
	/* Delete the top left corner of the content area */
	RectList_Delete(&rects, &bound);
	if (LCUIRect_GetOverlayRect(&ctx->paint->rect, &bound, &rect)) {
		center_x = bound.x - rect.x + r;
		center_y = bound.y - rect.y + r;
		rect.x -= ctx->paint->rect.x;
		rect.y -= ctx->paint->rect.y;
		Graph_Quote(&canvas, &ctx->paint->canvas, &rect);
		ClearPixelsOfCircle(&canvas, center_x, center_y, r);
	}

	r = ctx->shadow->top_right_radius;
	bound.x = ctx->content_box.x + ctx->content_box.width - r;
	bound.y = ctx->content_box.y;
	bound.width = r;
	bound.height = r;
	/* Delete the top right corner of the content area */
	RectList_Delete(&rects, &bound);
	if (LCUIRect_GetOverlayRect(&ctx->paint->rect, &bound, &rect)) {
		center_x = bound.x - rect.x;
		center_y = bound.y - rect.y + r;
		rect.x -= ctx->paint->rect.x;
		rect.y -= ctx->paint->rect.y;
		Graph_Quote(&canvas, &ctx->paint->canvas, &rect);
		ClearPixelsOfCircle(&canvas, center_x, center_y, r);
	}

	r = ctx->shadow->bottom_left_radius;
	bound.x = ctx->content_box.x;
	bound.y = ctx->content_box.y + ctx->content_box.height - r;
	bound.width = r;
	bound.height = r;
	/* Delete the bottom left corner of the content area */
	RectList_Delete(&rects, &bound);
	if (LCUIRect_GetOverlayRect(&ctx->paint->rect, &bound, &rect)) {
		center_x = bound.x - rect.x + r;
		center_y = bound.y - rect.y;
		rect.x -= ctx->paint->rect.x;
		rect.y -= ctx->paint->rect.y;
		Graph_Quote(&canvas, &ctx->paint->canvas, &rect);
		ClearPixelsOfCircle(&canvas, center_x, center_y, r);
	}

	r = ctx->shadow->bottom_right_radius;
	bound.x = ctx->content_box.x + ctx->content_box.width - r;
	bound.y = ctx->content_box.y + ctx->content_box.height - r;
	bound.width = r;
	bound.height = r;
	/* Delete the bottom right corner of the content area */
	RectList_Delete(&rects, &bound);
	if (LCUIRect_GetOverlayRect(&ctx->paint->rect, &bound, &rect)) {
		center_x = bound.x - rect.x;
		center_y = bound.y - rect.y;
		rect.x -= ctx->paint->rect.x;
		rect.y -= ctx->paint->rect.y;
		Graph_Quote(&canvas, &ctx->paint->canvas, &rect);
		ClearPixelsOfCircle(&canvas, center_x, center_y, r);
	}

	/* Clear pixels in the remaining areas of the content area */
	for (LinkedList_Each(node, &rects)) {
		if (LCUIRect_GetOverlayRect(&ctx->paint->rect, node->data,
					    &rect)) {
			rect.x -= ctx->paint->rect.x;
			rect.y -= ctx->paint->rect.y;
			Graph_FillRect(&ctx->paint->canvas, ARGB(0, 0, 0, 0),
				       &rect, TRUE);
		}
	}
	RectList_Clear(&rects);
}

int BoxShadow_Paint(const LCUI_BoxShadow *shadow, const LCUI_Rect *box,
		    int content_width, int content_height,
		    LCUI_PaintContext paint)
{
	LCUI_PaintContextRec shadow_paint;
	BoxShadowRenderingContextRec ctx;

	/* 判断容器尺寸是否低于阴影占用的最小尺寸 */
	if (box->width < BoxShadow_GetWidth(shadow, 0) ||
	    box->height < BoxShadow_GetHeight(shadow, 0)) {
		return -1;
	}
	if (SHADOW_WIDTH(shadow) == 0 && shadow->x == 0 && shadow->y == 0) {
		return 0;
	}

	/* Initialize a rendering context for render shadow */
	ctx.box = box;
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

	/* Create a paint context for render shadow */
	ctx.paint = &shadow_paint;
	Graph_Init(&shadow_paint.canvas);
	shadow_paint.rect = paint->rect;
	shadow_paint.with_alpha = TRUE;
	shadow_paint.canvas.color_type = LCUI_COLOR_TYPE_ARGB;
	Graph_Create(&ctx.paint->canvas, paint->rect.width, paint->rect.height);

	/* Render box shadow */
	BoxShadow_FillRect(&ctx);
	BoxShadow_PaintLeftBlur(&ctx);
	BoxShadow_PaintRightBlur(&ctx);
	BoxShadow_PaintTopBlur(&ctx);
	BoxShadow_PaintBottomBlur(&ctx);
	BoxShadow_PaintTopLeftBlur(&ctx);
	BoxShadow_PaintTopRightBlur(&ctx);
	BoxShadow_PaintBottomLeftBlur(&ctx);
	BoxShadow_PaintBottomRightBlur(&ctx);
	/* Clear pixels that overlap the content area */
	BoxShadow_ClearContentRect(&ctx);

	/* Render the rendered shadow bitmap to the canvas */
	Graph_Mix(&paint->canvas, &ctx.paint->canvas, 0, 0, paint->with_alpha);
	Graph_Free(&ctx.paint->canvas);
	return 0;
}
