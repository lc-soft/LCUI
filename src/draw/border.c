/*
 * border.c -- graph border draw support.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <math.h>

#define Graph_GetPixelPointer(G, X, Y) ((G)->argb + (G)->width * (Y) + (X))

#define BorderRenderer_Begin()                          \
	int x, y;                                       \
	int di, wi;                                     \
	double d, w;                                    \
	double opacity;                                 \
                                                        \
	LCUI_Rect rect;                                 \
	LCUI_ARGB *row, *p;                             \
	LCUI_Color tmpColor;                            \
                                                        \
	/* Get the actual rectagle that can be drawn */ \
	Graph_GetValidRect(dst, &rect);                 \
	dst = Graph_GetQuote(dst);                      \
	if (!Graph_IsValid(dst)) {                      \
		return -1;                              \
	}                                               \
	opacity = color.alpha / 255.0;

#define BorderRenderer_BeginXToY() \
	double x2;                 \
	double inner_x, inner_w;   \
                                   \
	BorderRenderer_Begin();    \
	inner_w = radius - width;

#define BorderRenderer_BeginYToX() \
	double y2;                 \
	double inner_y, inner_h;   \
                                   \
	BorderRenderer_Begin();    \
	inner_h = radius - width;

#define BorderRenderer_FillOuterEdgePixel() \
	*p = color;                         \
	p->alpha = (uchar_t)(opacity * 255.0 * (1.0 - d + di * 1.0));

#define BorderRenderer_FillInnerEdgePixel()                               \
	tmpColor = color;                                                 \
	tmpColor.alpha =                                                  \
	    (uchar_t)(opacity * 255.0 * (d - di * 1.0) * (w - wi * 1.0)); \
	LCUI_OverPixel(p, &tmpColor);

#define BorderRenderer_FillPixel()                          \
	/* If outside the border */                         \
	if (di > radius) {                                  \
		p->alpha = 0;                               \
	} else if (di == radius) {                          \
		/* Outer edge antialiasing */               \
		BorderRenderer_FillOuterEdgePixel();        \
	} else if (wi > 0 && di > radius - wi) {            \
		/* If inside the border line, fill pixel */ \
		LCUI_OverPixel(p, &color);                  \
	} else if (di == radius - wi) {                     \
		/* Inner edge anti-aliasing */              \
		BorderRenderer_FillInnerEdgePixel();        \
	}

#define CIRCLE_X(X, Y) sqrt(fabs((X) * (X) - (Y) * (Y)))
#define X_ELLIPSE_X(A, B, Y) \
	sqrt(fabs((1.0 - 1.0 * ((Y) * (Y)) / ((B) * (B))) * (A) * (A)))
#define Y_ELLIPSE_X(A, B, Y) \
	sqrt(fabs((1.0 - 1.0 * ((Y) * (Y)) / ((A) * (A))) * (B) * (B)))

static int DrawRoundedBorder(LCUI_Graph *dst, int bound_left, int bound_top,
			     const LCUI_BorderLine *vline,
			     const LCUI_BorderLine *hline, int radius)
{
	int x, y;
	int a, b;
	int circle_y;
	int center_y;
	int right;
	int width;
	int outer_xi, split_xi, inner_xi;
	double outer_x, split_x, inner_x;

	LCUI_Rect rect;
	LCUI_ARGB *p;
	LCUI_Color outer_color, inner_color;

	/* Get the actual rectagle that can be drawn */
	Graph_GetValidRect(dst, &rect);
	dst = Graph_GetQuote(dst);
	if (!Graph_IsValid(dst)) {
		return -1;
	}
	width = max(radius, max(vline->width, hline->width));
	right = min(rect.width, bound_left + width);
	center_y = bound_top + radius;
	// 计算椭圆的长轴短轴
	if (vline->width == hline->width) {
		a = b = radius - vline->width;
	} else if (vline->width < hline->width) {
		a = radius - vline->width;
		b = radius - hline->width;
	} else {
		a = radius - hline->width;
		b = radius - vline->width;
	}
	for (y = 0; y < rect.height; ++y) {
		circle_y = center_y - y;
		if (circle_y > 0) {
			outer_x = CIRCLE_X(radius, circle_y);
			if (a == b) {
				if (y >= bound_top + a) {
					inner_x = CIRCLE_X(a, circle_y);
					inner_x = radius - inner_x;
				} else {
					inner_x = width;
				}
			} else if (vline->width < hline->width) {
				// 椭圆焦点在 x 轴上
				circle_y = b - (y - (bound_top + hline->width));
				if (circle_y >= 0 && circle_y < b) {
					inner_x = X_ELLIPSE_X(a, b, circle_y);
					inner_x = radius - inner_x;
				} else {
					inner_x = width;
				}
			} else {
				// 椭圆焦点在 y 轴上
				circle_y = a - (y - (bound_top + hline->width));
				if (circle_y >= 0 && circle_y < a) {
					inner_x = Y_ELLIPSE_X(a, b, circle_y);
					inner_x = radius - inner_x;
				} else {
					inner_x = width;
				}
			}
		} else {
			outer_x = 0;
			inner_x = width;
		}
		split_x = 0;
		if (b > 0) {
			split_x = a / b * y;
		}
		outer_x = bound_left + radius - outer_x;
		inner_x = bound_left + inner_x;
		if (split_x < outer_x) {
			outer_color = hline->color;
		} else {
			outer_color = vline->color;
		}
		if (split_x < inner_x) {
			inner_color = vline->color;
		} else {
			inner_color = hline->color;
		}
		/* Limit coordinates into the current drawing region */
		outer_x = max(0, min(right, outer_x));
		inner_x = max(0, min(right, inner_x));
		split_x = max(outer_x, min(inner_x, split_x));
		/* Coordinate rasterization */
		outer_xi = (int)outer_x;
		inner_xi = (int)inner_x;
		split_xi = (int)split_x;
		p = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		/* Clear pixels outside the border */
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		*p = outer_color;
		/* Outer edge pixel anti aliasing */
		p->alpha =
		    (uchar_t)(p->alpha * (1.0 - outer_x + outer_xi * 1.0));
		for (x = outer_xi; x < split_xi; ++x, ++p) {
			LCUI_OverPixel(p, &hline->color);
		}
		for (x = split_xi; x < inner_xi; ++x, ++p) {
			LCUI_OverPixel(p, &vline->color);
		}
		if (inner_xi < right) {
			/* Inner edge pixel anti aliasing */
			inner_color.alpha = (uchar_t)(
			    inner_color.alpha * (inner_x - inner_xi * 1.0));
			LCUI_OverPixel(p, &inner_color);
		}
	}
	return 0;
}

static int Border_DrawTopLeft(const LCUI_Border *border, const LCUI_Rect *box,
			      LCUI_PaintContext paint)
{
	int center_x, center_y;

	LCUI_Rect bound, rect;
	LCUI_Graph canvas, *dest;

	bound.x = box->x;
	bound.y = box->y;
	bound.width = border->top_left_radius;
	bound.height = border->top_left_radius;
	if (!LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		return -1;
	}
	center_x = border->top_left_radius;
	center_y = border->top_left_radius;
	center_x -= rect.x - bound.x;
	center_y -= rect.y - bound.y;
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	Graph_Quote(&canvas, &paint->canvas, &rect);

	Graph_GetValidRect(&canvas, &rect);
	dest = Graph_GetQuote(&canvas);
	if (!Graph_IsValid(dest)) {
		return -1;
	}
}

static int BorderRenderer_DrawTopRoundedLine(LCUI_Graph *dst, LCUI_Pos center,
					     int radius, int width,
					     LCUI_Color color)
{
	BorderRenderer_BeginYToX();
	for (y = 0; y < rect.height; ++y) {
		y2 = pow(y - center.y, 2);
		inner_y = y - (center.y - inner_h);
		w = (1.0 - 1.0 * inner_y / inner_h) * width;
		wi = (int)w;
		/* Position the current pixel row */
		row = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		for (x = 0; x < rect.width; ++x) {
			p = row + x;
			/* Get the distance from the center of the circle */
			d = sqrt(pow(x - center.x, 2) + y2);
			di = (int)d;
			BorderRenderer_FillPixel();
		}
	}
	return 0;
}

static int BorderRenderer_DrawLeftRoundedLine(LCUI_Graph *dst, LCUI_Pos center,
					      int radius, int width,
					      LCUI_Color color)
{
	BorderRenderer_BeginXToY();
	for (x = 0; x < rect.width; ++x) {
		x2 = pow(x - center.x, 2);
		inner_x = x - (center.x - inner_w);
		w = (1.0 - 1.0 * inner_x / inner_w) * width;
		wi = (int)w;
		/* Position the current pixel row */
		row = Graph_GetPixelPointer(dst, rect.x + x, rect.y);
		for (y = 0; y < rect.height; ++y) {
			p = row + y * dst->width;
			/* Get the distance from the center of the circle */
			d = sqrt(pow(y - center.y, 2) + x2);
			di = (int)d;
			BorderRenderer_FillPixel();
		}
	}
	return 0;
}

/** Draw a line from right to top right */
static int BorderRenderer_DrawRightToTopRight(LCUI_Graph *dst, LCUI_Pos center,
					      int radius, int width,
					      LCUI_Color color)
{
	BorderRenderer_BeginXToY();
	for (x = 0; x < rect.width; ++x) {
		x2 = pow(x - center.x, 2);
		inner_x = x - (center.x - inner_w);
		w = (1.0 - 1.0 * inner_x / inner_w) * width;
		wi = (int)w;
		/* Position the current pixel row */
		row = Graph_GetPixelPointer(dst, rect.x + x, rect.y);
		for (y = 0; y < rect.height; ++y) {
			p = row + y * dst->width;
			/* Get the distance from the center of the circle */
			d = sqrt(pow(y - center.y, 2) + x2);
			di = (int)d;
			BorderRenderer_FillPixel();
		}
	}
	return 0;
}

/** Draw a line from left to bottom left */
static int BorderRenderer_DrawLeftToBottomLeft(LCUI_Graph *dst, LCUI_Pos center,
					       int radius, int width,
					       LCUI_Color color)
{
}

/** Draw a line from bottom left to left */
static int BorderRenderer_DrawBottomLeftToLeft(LCUI_Graph *dst, LCUI_Pos center,
					       int radius, int width,
					       LCUI_Color color)
{
}

/** Draw a line from right to bottom right */
static int BorderRenderer_DrawRightToBottomRight(LCUI_Graph *dst,
						 LCUI_Pos center, int radius,
						 int width, LCUI_Color color)
{
}

/** Draw a line from bottom right to right */
static int BorderRenderer_DrawBottomRightToRight(LCUI_Graph *dst,
						 LCUI_Pos center, int radius,
						 int width, LCUI_Color color)
{
}

int Border_Paint(const LCUI_Border *border, const LCUI_Rect *box,
		 LCUI_PaintContext paint)
{
	LCUI_Pos pos;
	LCUI_Graph canvas;
	LCUI_Rect bound, rect;

	if (!Graph_IsValid(&paint->canvas)) {
		return -1;
	}
	/* 左上角的圆角 */
	bound.x = box->x;
	bound.y = box->y;
	bound.width = max(border->top_left_radius, border->left.width);
	bound.height = max(border->top_left_radius, border->top.width);
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		int bound_top, bound_left;

		bound_top = rect.y - paint->rect.y;
		bound_left = rect.x - paint->rect.x;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
		DrawRoundedBorder(&canvas, bound_left, bound_top, &border->left,
				  &border->top, border->top_left_radius);
	}
	/* 右上角的圆角 */
	bound.y = box->y;
	bound.width = border->top_right_radius;
	bound.height = border->top_right_radius;
	bound.x = box->x + box->width - border->top_right_radius;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		pos.x = 0;
		pos.y = border->top_right_radius;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
	}
	/* 左下角的圆角 */
	bound.x = box->x;
	bound.width = border->bottom_left_radius;
	bound.height = border->bottom_left_radius;
	bound.y = box->y + box->height - border->bottom_left_radius;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		pos.y = 0;
		pos.x = border->bottom_left_radius;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
	}
	/* 右下角的圆角 */
	bound.width = border->bottom_left_radius;
	bound.height = border->bottom_left_radius;
	bound.x = box->x + box->width - border->bottom_right_radius;
	bound.y = box->y + box->height - border->bottom_right_radius;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		pos.x = pos.y = 0;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
	}
	/* 绘制上边框线 */
	bound.x = box->x + border->top_left_radius;
	bound.y = box->y;
	bound.width = box->width - border->top_right_radius;
	bound.width -= border->top_left_radius;
	bound.height = border->top.width;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		//Graph_FillRect(&canvas, border->top.color, NULL, TRUE);
	}
	/* 绘制下边框线 */
	bound.x = box->x + border->bottom_left_radius;
	bound.y = box->y + box->height - border->bottom.width;
	bound.width = box->width - border->bottom_right_radius;
	bound.width -= border->bottom_left_radius;
	bound.height = border->bottom.width;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->bottom.color, NULL, TRUE);
	}
	/* 绘制左边框线 */
	bound.y = box->y + border->top_left_radius;
	bound.x = box->x;
	bound.width = border->left.width;
	bound.height = box->height - border->top_left_radius;
	bound.height -= border->bottom_left_radius;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		//Graph_FillRect(&canvas, border->left.color, NULL, TRUE);
	}
	/* 绘制右边框线 */
	bound.x = box->x + box->width - border->right.width;
	bound.y = box->y + border->top_right_radius;
	bound.width = border->right.width;
	bound.height = box->height - border->top_right_radius;
	bound.height -= border->bottom_right_radius;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->right.color, NULL, TRUE);
	}
	return 0;
}
