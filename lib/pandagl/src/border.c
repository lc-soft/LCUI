/*
 * lib/pandagl/src/border.c: -- Border drawing
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
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

/** Draw border top left corner */
static int draw_border_top_left(pd_canvas_t *dst, int bound_left, int bound_top,
				const pd_border_line_t *xline,
				const pd_border_line_t *yline,
				unsigned int radius)
{
	BorderRenderContext();

	double cirlce_center_x = bound_left + r;
	double circle_center_y = bound_top + r;
	double split_k = 1.0 * yline->width / xline->width;
	double split_center_x = bound_left + 1.0 * yline->width;
	double split_center_y = bound_top + 1.0 * xline->width;
	int inner_ellipse_top = (int)split_center_y;

	/* Get the actual rectangle that can be drawn */
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	right = y_min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = 0;
		split_x = 0;
		inner_x = width;
		circle_y = ToGeoY(y, circle_center_y);
		if (r > 0 && circle_y >= 0) {
			outer_x = r - ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x =
				    r - ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = split_center_x -
				  ToGeoY(y, split_center_y) * split_k;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = y_max(0, y_min(right, outer_x));
		inner_x = y_max(0, y_min(right, inner_x));
		outer_xi = y_max(0, (int)outer_x - (int)radius / 2);
		inner_xi = y_min(right, (int)inner_x + (int)radius / 2);
		p = pd_canvas_pixel_at(dst, rect.x, rect.y + y);
		/* Clear the outer pixels */
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < inner_xi; ++x, ++p) {
			outer_d = -1;
			inner_d = inner_x - 1.0 * x;
			circle_x = ToGeoX(x, cirlce_center_x);
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
				p->a = smooth_left_pixel(p, outer_d);
			} else if (inner_d >= 1.0) {
				pd_over_pixel(p, &color, 1.0);
			} else if (inner_d >= 0) {
				color.a = smooth_right_pixel(&color, inner_d);
				pd_over_pixel(p, &color, 1.0);
			} else {
				break;
			}
		}
	}
	return 0;
}

static int draw_border_top_right(pd_canvas_t *dst, int bound_left,
				 int bound_top, const pd_border_line_t *xline,
				 const pd_border_line_t *yline,
				 unsigned int radius)
{
	BorderRenderContext();

	double circle_center_y = bound_top + r;
	double circle_center_x = bound_left + width - 1.0 * radius - 0.5;
	double split_k = 1.0 * yline->width / xline->width;
	double split_center_x = bound_left + width - 1.0 * yline->width;
	double split_center_y = bound_top + 1.0 * xline->width;
	double inner_ellipse_top = split_center_y;

	/* Get the actual rectangle that can be drawn */
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	right = y_min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = width;
		split_x = 0;
		inner_x = -1.0;
		circle_y = ToGeoY(y, circle_center_y);
		if (r > 0 && circle_y >= 0) {
			outer_x = width - radius + ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y >= inner_ellipse_top) {
				inner_x =
				    width - radius - 0.5 +
				    ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = split_center_x +
				  ToGeoY(y, split_center_y) * split_k;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = y_max(0, y_min(right, outer_x));
		inner_x = y_max(-1.0, y_min(outer_x, inner_x));
		inner_xi = y_max(0, (int)inner_x - (int)radius / 2);
		outer_xi = y_min(right, (int)outer_x + (int)radius / 2);
		p = pd_canvas_pixel_at(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_xi; x < outer_xi; ++x, ++p) {
			outer_d = -1.0;
			inner_d = x - inner_x;
			circle_x = ToGeoX(x, circle_center_x);
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
				p->a = smooth_left_pixel(p, outer_d);
			} else if (inner_d >= 0.5) {
				pd_over_pixel(p, &color, 1.0);
			} else if (inner_d >= 0) {
				color.a = smooth_right_pixel(&color, inner_d);
				pd_over_pixel(p, &color, 1.0);
			}
		}
		/* Clear the outer pixels */
		for (; x < right; ++x, ++p) {
			p->alpha = 0;
		}
	}
	return 0;
}

static int draw_border_bottom_left(pd_canvas_t *dst, int bound_left,
				   int bound_top, const pd_border_line_t *xline,
				   const pd_border_line_t *yline,
				   unsigned int radius)
{
	BorderRenderContext();

	int height = y_max(radius, xline->width);
	double cirlce_center_x = bound_left + r;
	double circle_center_y = bound_top + height - 1.0 * radius - 0.5;
	double split_k = 1.0 * yline->width / xline->width;
	double split_center_x = bound_left + 1.0 * yline->width;
	double split_center_y = bound_top + height - 1.0 * xline->width;
	double inner_ellipse_bottom = circle_center_y + radius_y;

	/* Get the actual rectangle that can be drawn */
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	right = y_min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = 0;
		split_x = 0;
		inner_x = width;
		circle_y = ToGeoY(y, circle_center_y);
		if (r > 0 && circle_y <= 0) {
			outer_x = r - ellipse_x(r, r, circle_y);
			if (radius_y > 0 && y <= inner_ellipse_bottom) {
				inner_x =
				    r - ellipse_x(radius_x, radius_y, circle_y);
			}
		}
		if (xline->width > 0) {
			split_x = split_center_x +
				  ToGeoY(y, split_center_y) * split_k;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		/* Limit coordinates into the current drawing region */
		outer_x = y_max(0, y_min(right, outer_x));
		inner_x = y_max(0, y_min(right, inner_x));
		outer_xi = y_max(0, (int)outer_x - (int)radius / 2);
		inner_xi = y_min(right, (int)inner_x + (int)radius / 2);
		p = pd_canvas_pixel_at(dst, rect.x, rect.y + y);
		for (x = 0; x < outer_xi; ++x, ++p) {
			p->alpha = 0;
		}
		for (; x < inner_xi; ++x, ++p) {
			outer_d = -1;
			inner_d = inner_x - 1.0 * x;
			circle_x = ToGeoX(x, cirlce_center_x);
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
				p->a = smooth_left_pixel(p, outer_d);
			} else if (inner_d >= 1.0) {
				pd_over_pixel(p, &color, 1.0);
			} else if (inner_d >= 0) {
				color.a = smooth_right_pixel(&color, inner_d);
				pd_over_pixel(p, &color, 1.0);
			} else {
				break;
			}
		}
	}
	return 0;
}

static int draw_border_bottom_right(pd_canvas_t *dst, int bound_left,
				    int bound_top,
				    const pd_border_line_t *xline,
				    const pd_border_line_t *yline,
				    unsigned int radius)
{
	BorderRenderContext();

	int height = y_max(radius, xline->width);
	double circle_center_y = bound_top + height - 1.0 * radius - 0.5;
	double circle_center_x = bound_left + width - 1.0 * radius - 0.5;
	double split_k = 1.0 * yline->width / xline->width;
	double split_center_x = bound_left + width - 1.0 * yline->width;
	double split_center_y = bound_top + height - 1.0 * xline->width;
	double inner_ellipse_bottom = circle_center_y + radius_y;

	/* Get the actual rectangle that can be drawn */
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	right = y_min(rect.width, bound_left + width);
	for (y = 0; y < rect.height; ++y) {
		outer_x = width;
		split_x = 0;
		inner_x = -1.0;
		circle_y = ToGeoY(y, circle_center_y);
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
			split_x = split_center_x -
				  ToGeoY(y, split_center_y) * split_k;
		}
		outer_x = bound_left + outer_x;
		inner_x = bound_left + inner_x;
		outer_x = y_max(0, y_min(right, outer_x));
		inner_x = y_max(-1.0, y_min(outer_x, inner_x));
		inner_xi = y_max(0, (int)inner_x - (int)radius / 2);
		outer_xi = y_min(right, (int)outer_x + (int)radius / 2);
		p = pd_canvas_pixel_at(dst, rect.x + inner_xi, rect.y + y);
		for (x = inner_xi; x < outer_xi; ++x, ++p) {
			outer_d = -1.0;
			inner_d = 1.0 * x - inner_x;
			circle_x = ToGeoX(x, circle_center_x);
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
				p->a = smooth_left_pixel(p, outer_d);
			} else if (inner_d >= 0.5) {
				pd_over_pixel(p, &color, 1.0);
			} else if (inner_d >= 0) {
				color.a = smooth_right_pixel(&color, inner_d);
				pd_over_pixel(p, &color, 1.0);
			}
		}
		/* Clear the outer pixels */
		for (; x < right; ++x, ++p) {
			p->alpha = 0;
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
static int crop_content_top_left(pd_canvas_t *dst, int bound_left,
				 int bound_top, double radius_x,
				 double radius_y)
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
	center_x = bound_left + radius_x;
	center_y = bound_top + radius_y;
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	for (yi = 0; yi < rect.height; ++yi) {
		y = ToGeoY(yi, center_y);
		x = ellipse_x(radius_x + 1.0, radius_y + 1.0, y);
		outer_xi = (int)(center_x - x);
		outer_xi = y_max(0, y_min(outer_xi, rect.width));
		p = pd_canvas_pixel_at(dst, rect.x, rect.y + yi);
		for (xi = 0; xi < outer_xi; ++xi, ++p) {
			p->alpha = 0;
		}
		/* If inner ellipse is circle */
		if (radius_x == radius_y) {
			for (; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = sqrt(x * x + y * y) - radius_x;
				if (d >= 1.0) {
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else {
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
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else {
					break;
				}
			}
		}
	}
	return 0;
}

static int crop_content_top_right(pd_canvas_t *dst, int bound_left,
				  int bound_top, double radius_x,
				  double radius_y)
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
	center_x = bound_left;
	center_y = bound_top + radius_y;
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	for (yi = 0; yi < rect.height; ++yi) {
		y = ToGeoY(yi, center_y);
		x = ellipse_x(y_max(0, radius_x - 1), y_max(0, radius_y - 1),
			      y);
		outer_xi = (int)(center_x + x);
		outer_xi = y_max(0, outer_xi);
		p = pd_canvas_pixel_at(dst, rect.x + outer_xi, rect.y + yi);
		if (radius_x == radius_y) {
			for (xi = outer_xi; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = sqrt(x * x + y * y) - radius_x;
				if (d >= 1.0) {
					break;
				}
				if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				}
			}
		} else {
			outer_x =
			    ToGeoX(ellipse_x(radius_x, radius_y, y), center_x);
			for (xi = outer_xi; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = x - outer_x;
				if (d >= 1.0) {
					break;
				}
				if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				}
			}
		}
		for (; xi < rect.width; ++xi, ++p) {
			p->alpha = 0;
		}
	}
	return 0;
}

static int crop_content_bottom_left(pd_canvas_t *dst, int bound_left,
				    int bound_top, double radius_x,
				    double radius_y)
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
	center_x = bound_left + radius_x;
	center_y = bound_top;
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	for (yi = 0; yi < rect.height; ++yi) {
		y = ToGeoY(yi, center_y);
		x = ellipse_x(radius_x + 1.0, radius_y + 1.0, y);
		outer_xi = (int)(center_x - x);
		outer_xi = y_max(0, y_min(outer_xi, rect.width));
		p = pd_canvas_pixel_at(dst, rect.x, rect.y + yi);
		for (xi = 0; xi < outer_xi; ++xi, ++p) {
			p->alpha = 0;
		}
		if (radius_x == radius_y) {
			for (; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = sqrt(x * x + y * y) - radius_x;
				if (d >= 1.0) {
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else {
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
					p->alpha = 0;
				} else if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				} else {
					break;
				}
			}
		}
	}
	return 0;
}

static int crop_content_bottom_right(pd_canvas_t *dst, int bound_left,
				     int bound_top, double radius_x,
				     double radius_y)
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
	center_x = bound_left;
	center_y = bound_top;
	pd_canvas_get_quote_rect(dst, &rect);
	dst = pd_canvas_get_quote_source(dst);
	if (!pd_canvas_is_valid(dst)) {
		return -1;
	}
	for (yi = 0; yi < rect.height; ++yi) {
		y = ToGeoY(yi, center_y);
		x = ellipse_x(y_max(0, radius_x - 1), y_max(0, radius_y - 1),
			      y);
		outer_xi = (int)(center_x + x);
		outer_xi = y_max(0, outer_xi);
		p = pd_canvas_pixel_at(dst, rect.x + outer_xi, rect.y + yi);
		if (radius_x == radius_y) {
			for (xi = outer_xi; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = sqrt(x * x + y * y) - radius_x;
				if (d >= 1.0) {
					break;
				}
				if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				}
			}
		} else {
			outer_x =
			    ToGeoX(ellipse_x(radius_x, radius_y, y), center_x);
			for (xi = outer_xi; xi < rect.width; ++xi, ++p) {
				x = ToGeoX(xi, center_x);
				d = x - outer_x;
				if (d >= 1.0) {
					break;
				}
				if (d >= 0) {
					p->alpha = smooth_left_pixel(p, d);
				}
			}
		}
		for (; xi < rect.width; ++xi, ++p) {
			p->alpha = 0;
		}
	}
	return 0;
}

int pd_crop_border_content(pd_context_t *ctx, const pd_border_t *border,
			   const pd_rect_t *box)
{
	pd_canvas_t canvas;
	pd_rect_t bound, rect;

	int radius;
	int bound_top, bound_left;

	radius = border->top_left_radius;
	bound.x = box->x + border->left.width;
	bound.y = box->y + border->top.width;
	bound.width = radius - border->left.width;
	bound.height = radius - border->top.width;
	if (bound.width > 0 && bound.height > 0 &&
	    pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		crop_content_top_left(&canvas, bound_left, bound_top,
				      bound.width, bound.height);
	}

	radius = border->top_right_radius;
	bound.x = box->x + box->width - radius;
	bound.y = box->y + border->top.width;
	bound.width = radius - border->right.width;
	bound.height = radius - border->top.width;
	if (bound.width > 0 && bound.height > 0 &&
	    pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		crop_content_top_right(&canvas, bound_left, bound_top,
				       bound.width, bound.height);
	}

	radius = border->bottom_left_radius;
	bound.x = box->x + border->left.width;
	bound.y = box->y + box->height - radius;
	bound.width = radius - border->left.width;
	bound.height = radius - border->bottom.width;
	if (bound.width > 0 && bound.height > 0 &&
	    pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		crop_content_bottom_left(&canvas, bound_left, bound_top,
					 bound.width, bound.height);
	}

	radius = border->bottom_right_radius;
	bound.x = box->x + box->width - radius;
	bound.y = box->y + box->height - radius;
	;
	bound.width = radius - border->right.width;
	bound.height = radius - border->bottom.width;
	if (bound.width > 0 && bound.height > 0 &&
	    pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		crop_content_bottom_right(&canvas, bound_left, bound_top,
					  bound.width, bound.height);
	}
	return 0;
}

int pd_paint_border(pd_context_t *ctx, const pd_border_t *border,
		    const pd_rect_t *box)
{
	pd_canvas_t canvas;
	pd_rect_t bound, rect;

	int bound_top, bound_left;
	int tl_width = y_max(border->top_left_radius, border->left.width);
	int tl_height = y_max(border->top_left_radius, border->top.width);
	int tr_width = y_max(border->top_right_radius, border->right.width);
	int tr_height = y_max(border->top_right_radius, border->top.width);
	int bl_width = y_max(border->bottom_left_radius, border->left.width);
	int bl_height = y_max(border->bottom_left_radius, border->bottom.width);
	int br_width = y_max(border->bottom_right_radius, border->right.width);
	int br_height =
	    y_max(border->bottom_right_radius, border->bottom.width);

	if (!pd_canvas_is_valid(&ctx->canvas)) {
		return -1;
	}
	/* Draw border top left angle */
	bound.x = box->x;
	bound.y = box->y;
	bound.width = tl_width;
	bound.height = tl_height;
	if (pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		draw_border_top_left(&canvas, bound_left, bound_top,
				     &border->top, &border->left,
				     border->top_left_radius);
	}
	/* Draw border top right angle */
	bound.y = box->y;
	bound.width = tr_width;
	bound.height = tr_height;
	bound.x = box->x + box->width - bound.width;
	if (pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		draw_border_top_right(&canvas, bound_left, bound_top,
				      &border->top, &border->right,
				      border->top_right_radius);
	}
	/* Draw border bottom left angle */
	bound.x = box->x;
	bound.width = bl_width;
	bound.height = bl_height;
	bound.y = box->y + box->height - bound.height;
	if (pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		draw_border_bottom_left(&canvas, bound_left, bound_top,
					&border->bottom, &border->left,
					border->bottom_left_radius);
	}
	/* Draw border bottom right angle */
	bound.width = br_width;
	bound.height = br_height;
	bound.x = box->x + box->width - bound.width;
	bound.y = box->y + box->height - bound.height;
	if (pd_rect_overlap(&bound, &ctx->rect, &rect)) {
		bound_left = bound.x - rect.x;
		bound_top = bound.y - rect.y;
		rect.x -= ctx->rect.x;
		rect.y -= ctx->rect.y;
		pd_canvas_quote(&canvas, &ctx->canvas, &rect);
		draw_border_bottom_right(&canvas, bound_left, bound_top,
					 &border->bottom, &border->right,
					 border->bottom_right_radius);
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
