/*
 * lib/pandagl/src/border.c: -- Border drawing
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <yutil.h>
#include <pandagl.h>

#define POW2(X) ((X) * (X))
#define CIRCLE_R(R) (R - 0.5)

/*  Convert screen Y coordinate to geometric Y coordinate */
#define ToGeoY(Y, CENTER_Y) ((CENTER_Y)-Y)

/*  Convert screen X coordinate to geometric X coordinate */
#define ToGeoX(X, CENTER_X) (X - (CENTER_X))

#define smooth_left_pixel(PX, X) (uint8_t)((PX)->a * (1.0 - (X - 1.0 * (int)X)))
#define smooth_right_pixel(PX, X) (uint8_t)((PX)->a * (X - 1.0 * (int)X))

#define BorderRenderContext()                               \
	int x, y;                                           \
	int right;                                          \
	double circle_x, circle_y;                          \
	int outer_xi, inner_xi;                             \
	double outer_x, split_x, inner_x;                   \
	double outer_d, inner_d;                            \
                                                            \
	const double r = CIRCLE_R(radius);                  \
	const double radius_x = y_max(0, r - yline->width); \
	const double radius_y = y_max(0, r - xline->width); \
	const int width = y_max(radius, yline->width);      \
                                                            \
	pd_rect_t rect;                                     \
	pd_color_t *p;                                      \
	pd_color_t color;

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

/**
 * FIXME: Improve the rounded border drawing code
 * Merge the four functions of DrawBorder* into one function and make it simple.
 */

typedef struct pd_corner_flags_t {
	int is_right;
	int is_bottom;
} pd_corner_flags_t;

static int draw_border_corner(pd_canvas_t *dst, int bound_left, int bound_top,
			      const pd_border_line_t *xline,
			      const pd_border_line_t *yline,
			      unsigned int radius,
			      const pd_corner_flags_t *flags)
{
	BorderRenderContext();
	int bottom_corner_height = y_max(radius, xline->width);
	double circle_center_y = bound_top +
				 (flags->is_bottom
				      ? bottom_corner_height - 1.0 * radius - 0.5
				      : r);
	double circle_center_x = bound_left +
				 (flags->is_right ? width - 1.0 * radius - 0.5
						  : r);
	double split_slope =
	    xline->width > 0 ? 1.0 * yline->width / xline->width : 0.0;
	double split_center_x = bound_left +
				(flags->is_right ? width - 1.0 * yline->width
						 : 1.0 * yline->width);
	double split_center_y = bound_top +
				(flags->is_bottom
				     ? bottom_corner_height - 1.0 * xline->width
				     : 1.0 * xline->width);
	double inner_ellipse_limit = flags->is_bottom
					 ? circle_center_y + radius_y
					 : split_center_y;
	int split_sign = flags->is_right == flags->is_bottom ? -1 : 1;

	/* Get the actual rectangle that can be drawn */
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	right = y_min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = flags->is_right ? width : 0;
		split_x = 0;
		inner_x = flags->is_right ? -1.0 : width;
		circle_y = ToGeoY(y, circle_center_y);
		if (r > 0 && (flags->is_bottom ? circle_y <= 0 : circle_y >= 0)) {
			int should_draw_inner_ellipse =
			    !flags->is_bottom || !flags->is_right ||
			    y >= circle_center_y;
			outer_x = flags->is_right
				      ? width - radius + ellipse_x(r, r, circle_y)
				      : r - ellipse_x(r, r, circle_y);
			if (radius_y > 0 && should_draw_inner_ellipse &&
			    (flags->is_bottom ? y <= inner_ellipse_limit
					      : y >= inner_ellipse_limit)) {
				inner_x = flags->is_right
					      ? width - radius - 0.5 +
						    ellipse_x(radius_x, radius_y,
							      circle_y)
					      : r - ellipse_x(radius_x, radius_y,
							      circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = split_center_x + split_sign *
					      ToGeoY(y, split_center_y) *
					      split_slope;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		outer_x = y_max(0, y_min(right, outer_x));
		if (flags->is_right) {
			inner_x = y_max(-1.0, y_min(outer_x, inner_x));
			inner_xi = y_max(0, (int)inner_x - (int)radius / 2);
			outer_xi = y_min(right, (int)outer_x + (int)radius / 2);
			x = inner_xi;
			p = pd_canvas_pixel_at(dst, rect.x + inner_xi,
					       rect.y + y);
		} else {
			inner_x = y_max(0, y_min(right, inner_x));
			outer_xi = y_max(0, (int)outer_x - (int)radius / 2);
			inner_xi = y_min(right, (int)inner_x + (int)radius / 2);
			x = 0;
			p = pd_canvas_pixel_at(dst, rect.x, rect.y + y);
			for (; x < outer_xi; ++x, ++p) {
				p->alpha = 0;
			}
		}
		for (; x < (flags->is_right ? outer_xi : inner_xi); ++x, ++p) {
			outer_d = -1.0;
			inner_d = flags->is_right ? x - inner_x : inner_x - x;
			circle_x = ToGeoX(x, circle_center_x);
			if (r > 0 &&
			    (flags->is_bottom ? circle_y <= 0 : circle_y >= 0) &&
			    (flags->is_right ? circle_x >= 0 : circle_x <= 0)) {
				outer_d =
				    sqrt(POW2(circle_x) + POW2(circle_y)) - r;
				if (radius_x == radius_y && radius_y > 0 &&
				    (flags->is_bottom
					 ? y <= inner_ellipse_limit
					 : y >= inner_ellipse_limit)) {
					inner_d = outer_d + r - radius_x;
				}
			}
			if (outer_d >= 1.0) {
				if (flags->is_right) {
					break;
				}
				p->alpha = 0;
				continue;
			}
			if (x < split_x) {
				color = flags->is_right ? xline->color
							: yline->color;
			} else {
				color = flags->is_right ? yline->color
							: xline->color;
			}
			if (outer_d >= 0) {
				if (inner_d - outer_d >= 0.5) {
					*p = color;
				}
				p->a = smooth_left_pixel(p, outer_d);
			} else if (inner_d >= (flags->is_right ? 0.5 : 1.0)) {
				pd_over_pixel(p, &color, 1.0);
			} else if (inner_d >= 0) {
				color.a = smooth_right_pixel(&color, inner_d);
				pd_over_pixel(p, &color, 1.0);
			} else if (!flags->is_right) {
				break;
			}
		}
		if (flags->is_right) {
			for (; x < right; ++x, ++p) {
				p->alpha = 0;
			}
		}
	}
	return 0;
}

/**
 * FIXME: Improve the content cropping code
 * Merge the four functions of CropContent* into one function and make it
 * simple.
 */

/** Crop the top left corner of the content area */
static int crop_content_corner(pd_canvas_t *dst, int bound_left, int bound_top,
			       double radius_x, double radius_y,
			       const pd_corner_flags_t *flags)
{
	int xi, yi;
	int outer_xi;
	double x, y, d;
	double outer_x;
	double center_x, center_y;

	pd_rect_t rect;
	pd_color_t *p;

	radius_x -= 0.5;
	radius_y -= 0.5;
	center_x = flags->is_right ? bound_left : bound_left + radius_x;
	center_y = flags->is_bottom ? bound_top : bound_top + radius_y;
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	for (yi = 0; yi < rect.height; ++yi) {
		y = ToGeoY(yi, center_y);
		x = flags->is_right
			    ? ellipse_x(y_max(0, radius_x - 1),
					y_max(0, radius_y - 1), y)
			    : ellipse_x(radius_x + 1.0, radius_y + 1.0, y);
		outer_xi = flags->is_right ? (int)(center_x + x)
					   : (int)(center_x - x);
		if (flags->is_right) {
			outer_xi = y_max(0, outer_xi);
			xi = outer_xi;
			p = pd_canvas_pixel_at(dst, rect.x + outer_xi, rect.y + yi);
		} else {
			outer_xi = y_max(0, y_min(outer_xi, rect.width));
			xi = 0;
			p = pd_canvas_pixel_at(dst, rect.x, rect.y + yi);
			for (; xi < outer_xi; ++xi, ++p) {
				p->alpha = 0;
			}
		}
		if (radius_x == radius_y) {
			for (; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = sqrt(x * x + y * y) - radius_x;
				if (d >= 1.0) {
					if (flags->is_right) {
						break;
					}
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else if (!flags->is_right) {
					break;
				}
			}
		} else {
			outer_x =
			    ToGeoX(ellipse_x(radius_x, radius_y, y), center_x);
			for (; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = x - outer_x;
				if (d >= 1.0) {
					if (flags->is_right) {
						break;
					}
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else if (!flags->is_right) {
					break;
				}
			}
		}
		if (flags->is_right) {
			for (; xi < rect.width; ++xi, ++p) {
				p->alpha = 0;
			}
		}
	}
	return 0;
}

int pd_crop_border_content(pd_context_t *ctx, const pd_border_t *border,
			   const pd_rect_t *box)
{
	pd_canvas_t canvas;
	pd_rect_t bound, rect;
	int bound_top, bound_left;
	int i;
	struct {
		pd_corner_flags_t flags;
		int x, y;
		int width, height;
	} corners[] = {
		{
		    .flags = {.is_right = 0, .is_bottom = 0},
		    .x = box->x + border->left.width,
		    .y = box->y + border->top.width,
		    .width = border->top_left_radius - border->left.width,
		    .height = border->top_left_radius - border->top.width,
		},
		{
		    .flags = {.is_right = 1, .is_bottom = 0},
		    .x = box->x + box->width - border->top_right_radius,
		    .y = box->y + border->top.width,
		    .width = border->top_right_radius - border->right.width,
		    .height = border->top_right_radius - border->top.width,
		},
		{
		    .flags = {.is_right = 0, .is_bottom = 1},
		    .x = box->x + border->left.width,
		    .y = box->y + box->height - border->bottom_left_radius,
		    .width = border->bottom_left_radius - border->left.width,
		    .height = border->bottom_left_radius - border->bottom.width,
		},
		{
		    .flags = {.is_right = 1, .is_bottom = 1},
		    .x = box->x + box->width - border->bottom_right_radius,
		    .y = box->y + box->height - border->bottom_right_radius,
		    .width = border->bottom_right_radius - border->right.width,
		    .height = border->bottom_right_radius - border->bottom.width,
		},
	};

	for (i = 0; i < 4; ++i) {
		bound.x = corners[i].x;
		bound.y = corners[i].y;
		bound.width = corners[i].width;
		bound.height = corners[i].height;
		if (bound.width > 0 && bound.height > 0 &&
		    pd_rect_overlap(&bound, &ctx->rect, &rect)) {
			bound_left = bound.x - rect.x;
			bound_top = bound.y - rect.y;
			rect.x -= ctx->rect.x;
			rect.y -= ctx->rect.y;
			pd_canvas_quote(&canvas, &ctx->canvas, &rect);
			crop_content_corner(&canvas, bound_left, bound_top,
					    bound.width, bound.height,
					    &corners[i].flags);
		}
	}
	return 0;
}

int pd_paint_border(pd_context_t *ctx, const pd_border_t *border,
		    const pd_rect_t *box)
{
	pd_canvas_t canvas;
	pd_rect_t bound, rect;
	int bound_top, bound_left;
	int i;
	int tl_width = y_max(border->top_left_radius, border->left.width);
	int tl_height = y_max(border->top_left_radius, border->top.width);
	int tr_width = y_max(border->top_right_radius, border->right.width);
	int tr_height = y_max(border->top_right_radius, border->top.width);
	int bl_width = y_max(border->bottom_left_radius, border->left.width);
	int bl_height = y_max(border->bottom_left_radius, border->bottom.width);
	int br_width = y_max(border->bottom_right_radius, border->right.width);
	int br_height =
	    y_max(border->bottom_right_radius, border->bottom.width);
	struct {
		pd_corner_flags_t flags;
		int x, y;
		int width, height;
		const pd_border_line_t *xline;
		const pd_border_line_t *yline;
		unsigned int radius;
	} corners[] = {
		{
		    .flags = {.is_right = 0, .is_bottom = 0},
		    .x = box->x,
		    .y = box->y,
		    .width = tl_width,
		    .height = tl_height,
		    .xline = &border->top,
		    .yline = &border->left,
		    .radius = border->top_left_radius,
		},
		{
		    .flags = {.is_right = 1, .is_bottom = 0},
		    .x = box->x + box->width - tr_width,
		    .y = box->y,
		    .width = tr_width,
		    .height = tr_height,
		    .xline = &border->top,
		    .yline = &border->right,
		    .radius = border->top_right_radius,
		},
		{
		    .flags = {.is_right = 0, .is_bottom = 1},
		    .x = box->x,
		    .y = box->y + box->height - bl_height,
		    .width = bl_width,
		    .height = bl_height,
		    .xline = &border->bottom,
		    .yline = &border->left,
		    .radius = border->bottom_left_radius,
		},
		{
		    .flags = {.is_right = 1, .is_bottom = 1},
		    .x = box->x + box->width - br_width,
		    .y = box->y + box->height - br_height,
		    .width = br_width,
		    .height = br_height,
		    .xline = &border->bottom,
		    .yline = &border->right,
		    .radius = border->bottom_right_radius,
		},
	};

	if (!pd_canvas_is_valid(&ctx->canvas)) {
		return -1;
	}
	for (i = 0; i < 4; ++i) {
		bound.x = corners[i].x;
		bound.y = corners[i].y;
		bound.width = corners[i].width;
		bound.height = corners[i].height;
		if (pd_rect_overlap(&bound, &ctx->rect, &rect)) {
			bound_left = bound.x - rect.x;
			bound_top = bound.y - rect.y;
			rect.x -= ctx->rect.x;
			rect.y -= ctx->rect.y;
			pd_canvas_quote(&canvas, &ctx->canvas, &rect);
			draw_border_corner(&canvas, bound_left, bound_top,
					   corners[i].xline, corners[i].yline,
					   corners[i].radius,
					   &corners[i].flags);
		}
	}
	/* Draw top border line */
	bound.x = box->x + tl_width;
	bound.y = box->y;
	bound.width = box->width - tl_width - tr_width;
	bound.height = border->top.width;
	if (pd_rect_overlap(&bound, &ctx->rect, &bound)) {
		bound.x -= ctx->rect.x;
		bound.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &bound);
		pd_canvas_fill(&canvas, border->top.color);
	}
	/* Draw bottom border line */
	bound.x = box->x + bl_width;
	bound.y = box->y + box->height - border->bottom.width;
	bound.width = box->width - bl_width - br_width;
	bound.height = border->bottom.width;
	if (pd_rect_overlap(&bound, &ctx->rect, &bound)) {
		bound.x -= ctx->rect.x;
		bound.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &bound);
		pd_canvas_fill(&canvas, border->bottom.color);
	}
	/* Draw left border line */
	bound.y = box->y + tl_height;
	bound.x = box->x;
	bound.width = border->left.width;
	bound.height = box->height - tl_height - bl_height;
	if (pd_rect_overlap(&bound, &ctx->rect, &bound)) {
		bound.x -= ctx->rect.x;
		bound.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &bound);
		pd_canvas_fill(&canvas, border->left.color);
	}
	/* Draw right border line */
	bound.x = box->x + box->width - border->right.width;
	bound.y = box->y + tr_height;
	bound.width = border->right.width;
	bound.height = box->height - tr_height - br_height;
	if (pd_rect_overlap(&bound, &ctx->rect, &bound)) {
		bound.x -= ctx->rect.x;
		bound.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &bound);
		pd_canvas_fill(&canvas, border->right.color);
	}
	return 0;
}
