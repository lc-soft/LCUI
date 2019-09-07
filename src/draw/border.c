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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

/**
 * FIXME: 精简圆角边框绘制算法
 * 现在的圆角是分成四个部分绘制的，四个函数的代码大部分一样，应该将它们合并成一个
 */

#define POW2(X) ((X) * (X))
#define CIRCLE_R(R) (R - 0.5)
#define CIRCLE_Y(Y, CENTER_Y) ((CENTER_Y)-Y)
#define CIRCLE_X(X, CENTER_X) (X - (CENTER_X))

#define SmoothLeftPixel(PX, X) (uchar_t)((PX)->a * (1.0 - (X - 1.0 * (int)X)))
#define SmoothRightPixel(PX, X) (uchar_t)((PX)->a * (X - 1.0 * (int)X))

#define BorderRenderContext()                             \
	int x, y;                                         \
	int right;                                        \
	double circle_x, circle_y;                        \
	int outer_xi, inner_xi;                           \
	double outer_x, split_x, inner_x;                 \
	double outer_d, inner_d;                          \
                                                          \
	const double r = CIRCLE_R(radius);                \
	const double radius_x = max(0, r - yline->width); \
	const double radius_y = max(0, r - xline->width); \
	const int width = max(radius, yline->width);      \
                                                          \
	LCUI_Rect rect;                                   \
	LCUI_ARGB *p;                                     \
	LCUI_Color color;

static double ellipse_x(double radius_x, double radius_y, double y)
{
	double value;
	if (radius_x == radius_y) {
		value = radius_x * radius_x - y * y;
	} else {
		value = (1.0 - 1.0 * (y * y) / (radius_y * radius_y)) *
			radius_x * radius_x;
	}
	if (value < 0) {
		value = -value;
	}
	return sqrt(value);
}

static int DrawBorderTopLeft(LCUI_Graph *dst, int bound_left, int bound_top,
			     const LCUI_BorderLine *xline,
			     const LCUI_BorderLine *yline, unsigned int radius)
{
	BorderRenderContext();

	double cirlce_center_x = bound_left + r;
	double circle_center_y = bound_top + r;
	int inner_ellipse_top = bound_top + 1.0 * xline->width;

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
		circle_y = CIRCLE_Y(y, circle_center_y);
		if (r > 0 && circle_y >= 0) {
			outer_x = r - ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x =
				    r - ellipse_x(radius_x, radius_y, circle_y);
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
		outer_xi = max(0, (int)outer_x - (int)radius / 2);
		inner_xi = min(right, (int)inner_x + (int)radius / 2);
		p = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		/* Clear the outer pixels */
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < inner_xi; ++x, ++p) {
			outer_d = -1;
			inner_d = inner_x - 1.0 * x;
			circle_x = CIRCLE_X(x, cirlce_center_x);
			/* If in the circle */
			if (r > 0 && circle_y >= 0 && circle_x <= 0) {
				outer_d =
				    sqrt(POW2(circle_x) + POW2(circle_y)) - r;
				/* If the inside is a circle is not an ellipse,
				 * Use the same anti-aliasing method
				 */
				if (radius_x == radius_y && radius_y > 0 &&
				    y >= inner_ellipse_top) {
					inner_d = outer_d + r - radius_x;
				}
			}
			if (outer_d >= 1.0) {
				p->alpha = 0;
				continue;
			}
			if (x < split_x) {
				color = yline->color;
			} else {
				color = xline->color;
			}
			if (outer_d >= 0) {
				/* Fill the border color if the border width is
				 * valid */
				if (inner_d - outer_d >= 0.5) {
					*p = color;
				}
				p->a = SmoothLeftPixel(p, outer_d);
			} else if (inner_d >= 1.0) {
				LCUI_OverPixel(p, &color);
			} else if (inner_d >= 0) {
				color.a = SmoothRightPixel(&color, inner_d);
				LCUI_OverPixel(p, &color);
			} else {
				break;
			}
		}
	}
	return 0;
}

static int DrawBorderTopRight(LCUI_Graph *dst, int bound_left, int bound_top,
			      const LCUI_BorderLine *xline,
			      const LCUI_BorderLine *yline, unsigned int radius)
{
	BorderRenderContext();

	double circle_center_y = bound_top + r;
	double circle_center_x = bound_left + width - 1.0 * radius - 0.5;
	double inner_ellipse_top = bound_top + 1.0 * xline->width;

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
		inner_x = -1.0;
		circle_y = CIRCLE_Y(y, circle_center_y);
		if (r > 0 && circle_y >= 0) {
			outer_x = width - radius + ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x =
				    width - radius - 0.5 +
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
		inner_x = max(-1.0, min(outer_x, inner_x));
		inner_xi = max(0, (int)inner_x - (int)radius / 2);
		outer_xi = min(right, (int)outer_x + (int)radius / 2);
		p = Graph_GetPixelPointer(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_xi; x < outer_xi; ++x, ++p) {
			outer_d = -1.0;
			inner_d = x - inner_x;
			circle_x = CIRCLE_X(x, circle_center_x);
			if (r > 0 && circle_y >= 0 && circle_x >= 0) {
				outer_d =
				    sqrt(POW2(circle_x) + POW2(circle_y)) - r;
				if (radius_x == radius_y && radius_y > 0 &&
				    y >= inner_ellipse_top) {
					inner_d = outer_d + r - radius_x;
				}
			}
			if (outer_d >= 1.0) {
				break;
			}
			if (x < split_x) {
				color = xline->color;
			} else {
				color = yline->color;
			}
			if (outer_d >= 0) {
				if (inner_d - outer_d >= 0.5) {
					*p = color;
				}
				p->a = SmoothLeftPixel(p, outer_d);
			} else if (inner_d >= 0.5) {
				LCUI_OverPixel(p, &color);
			} else if (inner_d >= 0) {
				color.a = SmoothRightPixel(&color, inner_d);
				LCUI_OverPixel(p, &color);
			}
		}
		/* Clear the outer pixels */
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
	double cirlce_center_x = bound_left + r;
	double circle_center_y = bound_top + height - 1.0 * radius - 0.5;
	double inner_ellipse_bottom = circle_center_y + radius_y;

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
		circle_y = CIRCLE_Y(y, circle_center_y);
		if (r > 0 && circle_y <= 0) {
			outer_x = r - ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y <= inner_ellipse_bottom) {
				inner_x =
				    r - ellipse_x(radius_x, radius_y, circle_y);
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
		outer_xi = max(0, (int)outer_x - (int)radius / 2);
		inner_xi = min(right, (int)inner_x + (int)radius / 2);
		p = Graph_GetPixelPointer(dst, rect.x, rect.y + y);
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < inner_xi; ++x, ++p) {
			outer_d = -1;
			inner_d = inner_x - 1.0 * x;
			circle_x = CIRCLE_X(x, cirlce_center_x);
			if (r > 0 && circle_y <= 0 && circle_x <= 0) {
				outer_d =
				    sqrt(POW2(circle_x) + POW2(circle_y)) - r;
				if (radius_x == radius_y && radius_y > 0 &&
				    y <= inner_ellipse_bottom) {
					inner_d = outer_d + r - radius_x;
				}
			}
			if (outer_d >= 1.0) {
				p->alpha = 0;
				continue;
			}
			if (x < split_x) {
				color = yline->color;
			} else {
				color = xline->color;
			}
			if (outer_d >= 0) {
				if (inner_d - outer_d >= 0.5) {
					*p = color;
				}
				p->a = SmoothLeftPixel(p, outer_d);
			} else if (inner_d >= 1.0) {
				LCUI_OverPixel(p, &color);
			} else if (inner_d >= 0) {
				color.a = SmoothRightPixel(&color, inner_d);
				LCUI_OverPixel(p, &color);
			} else {
				break;
			}
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
	double circle_center_y = bound_top + height - 1.0 * radius - 0.5;
	double circle_center_x = bound_left + width - 1.0 * radius - 0.5;
	double inner_ellipse_bottom = circle_center_y + radius_y;

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
		inner_x = -1.0;
		circle_y = CIRCLE_Y(y, circle_center_y);
		if (r > 0 && circle_y <= 0) {
			outer_x = width - r + ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y <= inner_ellipse_bottom &&
			    y >= circle_center_y) {
				inner_x =
				    width - radius - 0.5 +
				    ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = width - 1.0 * (height - y) * yline->width /
					      xline->width;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		outer_x = max(0, min(right, outer_x));
		inner_x = max(-1.0, min(outer_x, inner_x));
		inner_xi = max(0, (int)inner_x - (int)radius / 2);
		outer_xi = min(right, (int)outer_x + (int)radius / 2);
		p = Graph_GetPixelPointer(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_xi; x < outer_xi; ++x, ++p) {
			outer_d = -1.0;
			inner_d = 1.0 * x - inner_x;
			circle_x = CIRCLE_X(x, circle_center_x);
			if (r > 0 && circle_y <= 0 && circle_x >= 0) {
				outer_d =
				    sqrt(POW2(circle_x) + POW2(circle_y)) - r;
				if (radius_x == radius_y && radius_y > 0 &&
				    y <= inner_ellipse_bottom) {
					inner_d = outer_d + r - radius_x;
				}
			}
			if (outer_d >= 1.0) {
				break;
			}
			if (x < split_x) {
				color = xline->color;
			} else {
				color = yline->color;
			}
			if (outer_d >= 0) {
				if (inner_d - outer_d >= 0.5) {
					*p = color;
				}
				p->a = SmoothLeftPixel(p, outer_d);
			} else if (inner_d >= 0.5) {
				LCUI_OverPixel(p, &color);
			} else if (inner_d >= 0) {
				color.a = SmoothRightPixel(&color, inner_d);
				LCUI_OverPixel(p, &color);
			}
		}
		/* Clear the outer pixels */
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
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
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
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
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
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
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
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
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
