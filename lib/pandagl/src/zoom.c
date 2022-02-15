#include "../include/pandagl.h"

static uchar_t canvas_bilinear_resampling_core(uchar_t a, uchar_t b, uchar_t c,
					       uchar_t d, float dx, float dy)
{
	return (uchar_t)(a * (1 - dx) * (1 - dy) + b * (dx) * (1 - dy) +
			 c * (dy) * (1 - dx) + d * (dx * dy));
}

int pd_canvas_zoom(const pd_canvas_t *canvas, pd_canvas_t *buff,
		   pd_bool keep_scale, int width, int height)
{
	pd_rect_t rect;
	int x, y, src_x, src_y;
	double scale_x = 0.0, scale_y = 0.0;

	if (!pd_canvas_is_valid(canvas) || (width <= 0 && height <= 0)) {
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	pd_canvas_get_quote_rect(canvas, &rect);
	canvas = pd_canvas_get_quote_source_readonly(canvas);
	if (width > 0) {
		scale_x = 1.0 * rect.width / width;
	}
	if (height > 0) {
		scale_y = 1.0 * rect.height / height;
	}
	if (width <= 0) {
		scale_x = scale_y;
		width = (int)(0.5 + 1.0 * canvas->width / scale_x);
	}
	if (height <= 0) {
		scale_y = scale_x;
		height = (int)(0.5 + 1.0 * canvas->height / scale_y);
	}
	/* 如果保持宽高比 */
	if (keep_scale) {
		if (scale_x < scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = canvas->color_type;
	if (pd_canvas_create(buff, width, height) < 0) {
		return -2;
	}
	if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
		pd_color_t *px_src, *px_des, *px_row_src;
		for (y = 0; y < height; ++y) {
			src_y = (int)(y * scale_y);
			px_row_src = canvas->argb;
			px_row_src += (src_y + rect.y) * canvas->width + rect.x;
			px_des = buff->argb + y * width;
			for (x = 0; x < width; ++x) {
				src_x = (int)(x * scale_x);
				px_src = px_row_src + src_x;
				*px_des++ = *px_src;
			}
		}
	} else {
		uchar_t *byte_src, *byte_des, *byte_row_src;
		for (y = 0; y < height; ++y) {
			byte_row_src = pd_canvas_pixel_at(
			    canvas, rect.x, (int)(y * scale_y) + rect.y);
			byte_des = pd_canvas_pixel_at(buff, 0, y);
			for (x = 0; x < width; ++x) {
				src_x = (int)(x * scale_x);
				src_x *= canvas->bytes_per_pixel;
				byte_src = byte_row_src + src_x;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src;
			}
		}
	}
	return 0;
}

int pd_canvas_zoom_bilinear(const pd_canvas_t *canvas, pd_canvas_t *buff,
			    pd_bool keep_scale, int width, int height)
{
	pd_rect_t rect;
	pd_color_t a, b, c, d, t_color;

	int x, y, i, j;
	float x_diff, y_diff;
	double scale_x = 0.0, scale_y = 0.0;

	if (canvas->color_type != PD_COLOR_TYPE_RGB &&
	    canvas->color_type != PD_COLOR_TYPE_ARGB) {
		/* fall back to nearest scaling */
		logger_debug("[canvas] unable to perform bilinear scaling, "
			     "fallback...\n");
		return pd_canvas_zoom(canvas, buff, keep_scale, width, height);
	}
	if (!pd_canvas_is_valid(canvas) || (width <= 0 && height <= 0)) {
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	pd_canvas_get_quote_rect(canvas, &rect);
	canvas = pd_canvas_get_quote_source_readonly(canvas);
	if (width > 0) {
		scale_x = 1.0 * rect.width / width;
	}
	if (height > 0) {
		scale_y = 1.0 * rect.height / height;
	}
	if (width <= 0) {
		scale_x = scale_y;
		width = (int)(0.5 + 1.0 * canvas->width / scale_x);
	}
	if (height <= 0) {
		scale_y = scale_x;
		height = (int)(0.5 + 1.0 * canvas->height / scale_y);
	}
	/* 如果保持宽高比 */
	if (keep_scale) {
		if (scale_x < scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = canvas->color_type;
	if (pd_canvas_create(buff, width, height) < 0) {
		return -2;
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			/*
			 * Qmn = (m, n).
			 * Qxy1 = (x2 - x) / (x2 - x1) * Q11 + (x - x1) / (x2
			 * - x1) * Q21
			 * Qxy2 = (x2 - x) / (x2 - x1) * Q12 + (x - x1) / (x2
			 * - x1) * Q22
			 * Qxy = (y2 - y) / (y2 - y1) * Qxy1 + (y - y1) / (y2 -
			 * y1) * Qxy2
			 */
			x = (int)(scale_x * j);
			y = (int)(scale_y * i);
			x_diff = (float)((scale_x * j) - x);
			y_diff = (float)((scale_y * i) - y);
			a = pd_canvas_get_pixel(canvas, x + rect.x + 0,
						y + rect.y + 0);
			b = pd_canvas_get_pixel(canvas, x + rect.x + 1,
						y + rect.y + 0);
			c = pd_canvas_get_pixel(canvas, x + rect.x + 0,
						y + rect.y + 1);
			d = pd_canvas_get_pixel(canvas, x + rect.x + 1,
						y + rect.y + 1);
			t_color.b = canvas_bilinear_resampling_core(
			    a.b, b.b, c.b, d.b, x_diff, y_diff);
			t_color.g = canvas_bilinear_resampling_core(
			    a.g, b.g, c.g, d.g, x_diff, y_diff);
			t_color.r = canvas_bilinear_resampling_core(
			    a.r, b.r, c.r, d.r, x_diff, y_diff);
			t_color.a = canvas_bilinear_resampling_core(
			    a.a, b.a, c.a, d.a, x_diff, y_diff);
			pd_canvas_set_pixel(buff, j, i, t_color);
		}
	}
	return 0;
}
