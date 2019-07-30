/*
 * border.c -- Border drawing
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

#include <math.h>
#include <LCUI.h>
#include <LCUI/graph.h>

#define Graph_GetPixelPointer(G, X, Y) ((G)->argb + (G)->width * (Y) + (X))

#define BorderRenderContext()                            \
	int x, y;                                        \
	int right;                                       \
	int circle_y;                                    \
	int outer_xi, split_xi, inner_xi;                \
	double outer_x, split_x, inner_x;                \
                                                         \
	const unsigned radius_x = radius - yline->width; \
	const unsigned radius_y = radius - xline->width; \
	const int width = max(radius, yline->width);     \
                                                         \
	LCUI_Rect rect;                                  \
	LCUI_ARGB *p;

static double ellipse_x(unsigned int radius_x, unsigned int radius_y, int y)
{
	double value;
	if (radius_x == radius_y) {
		value = radius_x * radius_x - y * y;
	} else {
		value = (1.0 - 1.0 * (y * y) / (radius_y * radius_y)) *
			radius_x * radius_x;
	}
	if (value < 0) {
		value = value;
	}
	return sqrt(value);
}

static int DrawBorderTopLeft(LCUI_Graph *dst, int bound_left, int bound_top,
			     const LCUI_BorderLine *xline,
			     const LCUI_BorderLine *yline, unsigned int radius)
{
	BorderRenderContext();

	int circle_top = bound_top;
	int circle_center = circle_top + radius;
	int inner_ellipse_top = circle_top + xline->width;

	/* Get the actual rectagle that can be drawn */
	Graph_GetValidRect(dst, &rect);
	dst = Graph_GetQuote(dst);
	if (!Graph_IsValid(dst)) {
		return -1;
	}
	right = min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = 0;
		split_x = 0;
		inner_x = width;
		if (radius > 0 && y <= circle_center) {
			circle_y = circle_center - y;
			outer_x = radius - ellipse_x(radius, radius, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x = radius - ellipse_x(radius_x, radius_y,
							     circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = 1.0 * y * yline->width / xline->width;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = max(0, min(right, outer_x));
		inner_x = max(0, min(right, inner_x));
		split_x = min(inner_x, split_x);
		/* Coordinate rasterization */
		outer_xi = (int)outer_x;
		inner_xi = (int)inner_x;
		split_xi = (int)split_x;
		p = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		/* Clear pixels outside the border */
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < split_xi; ++x, ++p) {
			LCUI_OverPixel(p, &yline->color);
		}
		for (; x < inner_xi; ++x, ++p) {
			LCUI_OverPixel(p, &xline->color);
		}
	}
	return 0;
}

static int DrawBorderTopRight(LCUI_Graph *dst, int bound_left, int bound_top,
			      const LCUI_BorderLine *xline,
			      const LCUI_BorderLine *yline, unsigned int radius)
{
	BorderRenderContext();

	int circle_top = bound_top;
	int circle_center = circle_top + radius;
	int inner_ellipse_top = circle_top + xline->width;

	/* Get the actual rectagle that can be drawn */
	Graph_GetValidRect(dst, &rect);
	dst = Graph_GetQuote(dst);
	if (!Graph_IsValid(dst)) {
		return -1;
	}
	right = min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = width;
		split_x = 0;
		inner_x = 0;
		if (radius > 0 && y <= circle_center) {
			circle_y = circle_center - y;
			outer_x = width - radius +
				  ellipse_x(radius, radius, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x =
				    width - radius +
				    ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x =
			    1.0 * width - 1.0 * y * yline->width / xline->width;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = max(0, min(right, outer_x));
		inner_x = max(0, min(outer_x, inner_x));
		split_x = min(outer_x, split_x);
		/* Coordinate rasterization */
		outer_xi = (int)outer_x;
		inner_xi = (int)inner_x;
		split_xi = (int)split_x;
		p = Graph_GetPixelPointer(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_x; x < split_xi; ++x, ++p) {
			LCUI_OverPixel(p, &xline->color);
		}
		for (; x < outer_xi; ++x, ++p) {
			LCUI_OverPixel(p, &yline->color);
		}
		/* Clear pixels outside the border */
		for (; x < right; ++x, ++p) {
			p->alpha = 0;
		}
	}
	return 0;
}

static int DrawBorderBottomLeft(LCUI_Graph *dst, int bound_left, int bound_top,
				const LCUI_BorderLine *xline,
				const LCUI_BorderLine *yline,
				unsigned int radius)
{
	BorderRenderContext();

	int height = max(radius, xline->width);
	int circle_bottom = bound_top + height;
	int circle_center = circle_bottom - radius;
	int inner_ellipse_bottom = circle_center + radius_y;

	/* Get the actual rectagle that can be drawn */
	Graph_GetValidRect(dst, &rect);
	dst = Graph_GetQuote(dst);
	if (!Graph_IsValid(dst)) {
		return -1;
	}
	right = min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = 0;
		split_x = 0;
		inner_x = width;
		if (radius > 0 && y >= circle_center) {
			circle_y = y - circle_center;
			outer_x = radius - ellipse_x(radius, radius, circle_y);
			if (radius_y > 0 && y <= inner_ellipse_bottom) {
				inner_x = radius - ellipse_x(radius_x, radius_y,
							     circle_y);
			}
		}
		if (xline->width > 0) {
			split_x =
			    1.0 * (height - y) * yline->width / xline->width;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = max(0, min(right, outer_x));
		inner_x = max(0, min(right, inner_x));
		split_x = min(inner_x, split_x);
		/* Coordinate rasterization */
		outer_xi = (int)outer_x;
		inner_xi = (int)inner_x;
		split_xi = (int)split_x;
		p = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		/* Clear pixels outside the border */
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < split_xi; ++x, ++p) {
			LCUI_OverPixel(p, &yline->color);
		}
		for (; x < inner_xi; ++x, ++p) {
			LCUI_OverPixel(p, &xline->color);
		}
	}
	return 0;
}

static int DrawBorderBottomRight(LCUI_Graph *dst, int bound_left, int bound_top,
				 const LCUI_BorderLine *xline,
				 const LCUI_BorderLine *yline,
				 unsigned int radius)
{
	BorderRenderContext();

	int height = max(radius, xline->width);
	int circle_bottom = bound_top + height;
	int circle_center = circle_bottom - radius;
	int inner_ellipse_bottom = circle_center + radius_y;

	/* Get the actual rectagle that can be drawn */
	Graph_GetValidRect(dst, &rect);
	dst = Graph_GetQuote(dst);
	if (!Graph_IsValid(dst)) {
		return -1;
	}
	right = min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = width;
		split_x = 0;
		inner_x = 0;
		if (radius > 0 && y >= circle_center) {
			circle_y = y - circle_center;
			outer_x = width - radius +
				  ellipse_x(radius, radius, circle_y);
			if (radius_y > 0 && y <= inner_ellipse_bottom) {
				inner_x =
				    width - radius +
				    ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = width - 1.0 * (height - y) * yline->width /
					      xline->width;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = max(0, min(right, outer_x));
		inner_x = max(0, min(right, inner_x));
		split_x = min(outer_x, split_x);
		/* Coordinate rasterization */
		outer_xi = (int)outer_x;
		inner_xi = (int)inner_x;
		split_xi = (int)split_x;
		p = Graph_GetPixelPointer(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_x; x < split_xi; ++x, ++p) {
			LCUI_OverPixel(p, &xline->color);
		}
		for (; x < outer_xi; ++x, ++p) {
			LCUI_OverPixel(p, &yline->color);
		}
		/* Clear pixels outside the border */
		for (; x < right; ++x, ++p) {
			p->alpha = 0;
		}
	}
	return 0;
}

int Border_Paint(const LCUI_Border *border, const LCUI_Rect *box,
		 LCUI_PaintContext paint)
{
	LCUI_Graph canvas;
	LCUI_Rect bound, rect;
	int bound_top, bound_left;
	int tl_width = max(border->top_left_radius, border->left.width);
	int tl_height = max(border->top_left_radius, border->top.width);
	int tr_width = max(border->top_right_radius, border->right.width);
	int tr_height = max(border->top_right_radius, border->top.width);
	int bl_width = max(border->bottom_left_radius, border->left.width);
	int bl_height = max(border->bottom_left_radius, border->bottom.width);
	int br_width = max(border->bottom_right_radius, border->right.width);
	int br_height = max(border->bottom_right_radius, border->bottom.width);

	if (!Graph_IsValid(&paint->canvas)) {
		return -1;
	}
	/* Draw border top left angle */
	bound.x = box->x;
	bound.y = box->y;
	bound.width = tl_width;
	bound.height = tl_height;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		bound_top = rect.y - bound.y;
		bound_left = rect.x - bound.x;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
		DrawBorderTopLeft(&canvas, bound_left, bound_top, &border->top,
				  &border->left, border->top_left_radius);
	}
	/* Draw border top right angle */
	bound.y = box->y;
	bound.width = tr_width;
	bound.height = tr_height;
	bound.x = box->x + box->width - bound.width;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		bound_top = rect.y - bound.y;
		bound_left = rect.x - bound.x;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
		DrawBorderTopRight(&canvas, bound_left, bound_top, &border->top,
				   &border->right, border->top_right_radius);
	}
	/* Draw border bottom left angle */
	bound.x = box->x;
	bound.width = bl_width;
	bound.height = bl_height;
	bound.y = box->y + box->height - bound.height;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		bound_top = rect.y - bound.y;
		bound_left = rect.x - bound.x;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
		DrawBorderBottomLeft(&canvas, bound_left, bound_top,
				     &border->bottom, &border->left,
				     border->bottom_left_radius);
	}
	/* Draw border bottom right angle */
	bound.width = br_width;
	bound.height = br_height;
	bound.x = box->x + box->width - bound.width;
	bound.y = box->y + box->height - bound.height;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &rect)) {
		bound_top = rect.y - bound.y;
		bound_left = rect.x - bound.x;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &rect);
		DrawBorderBottomRight(&canvas, bound_left, bound_top,
				      &border->bottom, &border->right,
				      border->bottom_right_radius);
	}
	/* Draw top border line */
	bound.x = box->x + tl_width;
	bound.y = box->y;
	bound.width = box->width - tl_width - tr_width;
	bound.height = border->top.width;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->top.color, NULL, TRUE);
	}
	/* Draw bottom border line */
	bound.x = box->x + bl_width;
	bound.y = box->y + box->height - border->bottom.width;
	bound.width = box->width - bl_width - br_width;
	bound.height = border->bottom.width;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->bottom.color, NULL, TRUE);
	}
	/* Draw left border line */
	bound.y = box->y + tl_height;
	bound.x = box->x;
	bound.width = border->left.width;
	bound.height = box->height - tl_height - bl_height;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->left.color, NULL, TRUE);
	}
	/* Draw right border line */
	bound.x = box->x + box->width - border->right.width;
	bound.y = box->y + tr_height;
	bound.width = border->right.width;
	bound.height = box->height - tr_height - br_height;
	if (LCUIRect_GetOverlayRect(&bound, &paint->rect, &bound)) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote(&canvas, &paint->canvas, &bound);
		Graph_FillRect(&canvas, border->right.color, NULL, TRUE);
	}
	return 0;
}
