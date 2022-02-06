#include "../include/pandagl.h"

void pd_canvas_init(pd_canvas_t *canvas)
{
	canvas->quote.is_valid = FALSE;
	canvas->quote.source = NULL;
	canvas->quote.top = 0;
	canvas->quote.left = 0;
	canvas->color_type = PD_COLOR_TYPE_RGB;
	canvas->bytes = NULL;
	canvas->opacity = 1.0;
	canvas->mem_size = 0;
	canvas->width = 0;
	canvas->height = 0;
	canvas->bytes_per_pixel = 3;
	canvas->bytes_per_row = 0;
}

pd_bool pd_canvas_is_valid(const pd_canvas_t *canvas)
{
	if (!canvas) {
		return FALSE;
	}
	if (canvas->quote.is_valid) {
		return canvas->quote.source &&
		       canvas->quote.source->width > 0 &&
		       canvas->quote.source->height > 0;
	}
	return canvas->bytes && canvas->height > 0 && canvas->width > 0;
}

int pd_canvas_quote(pd_canvas_t *self, pd_canvas_t *source,
		    const pd_rect_t *rect)
{
	pd_rect_t quote_rect;
	if (!rect) {
		quote_rect.x = 0;
		quote_rect.y = 0;
		quote_rect.width = source->width;
		quote_rect.height = source->height;
	} else {
		quote_rect = *rect;
	}
	LCUIRect_ValidateArea(&quote_rect, source->width, source->height);
	/* 如果引用源本身已经引用了另一个源 */
	if (source->quote.is_valid) {
		quote_rect.x += source->quote.left;
		quote_rect.y += source->quote.top;
		source = source->quote.source;
	}
	if (quote_rect.width <= 0 || quote_rect.height <= 0) {
		self->width = 0;
		self->height = 0;
		self->opacity = 1.0;
		self->quote.left = 0;
		self->quote.top = 0;
		self->bytes = NULL;
		self->quote.source = NULL;
		self->quote.is_valid = FALSE;
		return -EINVAL;
	}
	self->opacity = 1.0;
	self->bytes = NULL;
	self->mem_size = 0;
	self->width = quote_rect.width;
	self->height = quote_rect.height;
	self->color_type = source->color_type;
	self->bytes_per_pixel = source->bytes_per_pixel;
	self->bytes_per_row = source->bytes_per_row;
	self->quote.is_valid = TRUE;
	self->quote.source = source;
	self->quote.left = quote_rect.x;
	self->quote.top = quote_rect.y;
	return 0;
}

void pd_canvas_get_valid_rect(const pd_canvas_t *canvas, pd_rect_t *rect)
{
	if (canvas->quote.is_valid) {
		rect->x = canvas->quote.left;
		rect->y = canvas->quote.top;
	} else {
		rect->x = 0;
		rect->y = 0;
	}
	rect->width = canvas->width;
	rect->height = canvas->height;
}

void pd_canvas_destroy(pd_canvas_t *canvas)
{
	/* 解除引用 */
	if (canvas && canvas->quote.is_valid) {
		canvas->quote.source = NULL;
		canvas->quote.is_valid = FALSE;
		return;
	}
	if (canvas->bytes) {
		free(canvas->bytes);
		canvas->bytes = NULL;
	}
	canvas->width = 0;
	canvas->height = 0;
	canvas->mem_size = 0;
}

int pd_canvas_create(pd_canvas_t *canvas, unsigned width, unsigned height)
{
	size_t size;
	if (width > 10000 || height > 10000) {
		logger_error("canvas size is too large!");
		abort();
	}
	if (width < 1 || height < 1) {
		pd_canvas_destroy(canvas);
		return -1;
	}
	canvas->bytes_per_pixel = pd_get_pixel_size(canvas->color_type);
	canvas->bytes_per_row =
	    pd_get_pixel_row_size(canvas->color_type, width);
	size = canvas->bytes_per_row * height;
	if (pd_canvas_is_valid(canvas)) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if (canvas->mem_size >= size) {
			memset(canvas->bytes, 0, canvas->mem_size);
			canvas->width = width;
			canvas->height = height;
			return 0;
		}
		pd_canvas_destroy(canvas);
	}
	canvas->mem_size = size;
	canvas->bytes = calloc(1, size);
	if (!canvas->bytes) {
		canvas->width = 0;
		canvas->height = 0;
		return -2;
	}
	canvas->width = width;
	canvas->height = height;
	return 0;
}

static void pd_canvas_direct_replace(pd_canvas_t *des, pd_rect_t des_rect,
				     const pd_canvas_t *src, int src_x,
				     int src_y)
{
	int y;
	uchar_t *byte_row_des, *byte_row_src;

	byte_row_src = src->bytes + src_y * src->bytes_per_row;
	byte_row_src += src_x * src->bytes_per_pixel;
	byte_row_des = des->bytes + des_rect.y * des->bytes_per_row;
	byte_row_des += des_rect.x * des->bytes_per_pixel;

	for (y = 0; y < des_rect.height; ++y) {
		pd_format_pixels(byte_row_src, src->color_type, byte_row_des,
				 des->color_type, des_rect.width);
		byte_row_src += src->bytes_per_row;
		byte_row_des += des->bytes_per_row;
	}
	return 0;
}

int pd_canvas_replace(pd_canvas_t *back, const pd_canvas_t *fore, int left,
		      int top)
{
	pd_rect_t read_rect, write_rect;

	if (!pd_canvas_is_valid(fore)) {
		return -1;
	}
	write_rect.x = left;
	write_rect.y = top;
	write_rect.width = fore->width;
	write_rect.height = fore->height;

	PD_CANVAS_WRITING_CONTEXT(back, write_rect);

	pd_canvas_get_valid_rect(fore, &read_rect);
	if (read_rect.width <= 0 || read_rect.height <= 0) {
		return -2;
	}
	left = read_rect.x;
	top = read_rect.y;
	fore = pd_canvas_get_quote(fore);
	switch (fore->color_type) {
	case PD_COLOR_TYPE_RGB888:
	case PD_COLOR_TYPE_ARGB8888:
		pd_canvas_direct_replace(back, write_rect, fore, left, top);
		return 0;
	default:
		break;
	}
	return -1;
}

int pd_canvas_set_color_type(pd_canvas_t *canvas, int color_type)
{
	pd_canvas_t tmp;

	if (canvas->color_type == color_type) {
		return -1;
	}
	pd_canvas_init(&tmp);
	pd_canvas_create(&tmp, canvas->width, canvas->height);
	if (pd_canvas_replace(&tmp, canvas, 0, 0) == 0) {
		pd_canvas_destroy(canvas);
		*canvas = tmp;
		return 0;
	}
	pd_canvas_destroy(&tmp);
	return -1;
}

int pd_canvas_cut(const pd_canvas_t *canvas, pd_rect_t rect,
		  pd_canvas_t *out_canvas)
{
	int y;
	uchar_t *src_row, *des_row;

	if (!pd_canvas_is_valid(canvas)) {
		return -2;
	}
	LCUIRect_ValidateArea(&rect, canvas->width, canvas->height);
	if (rect.width <= 0 || rect.height <= 0) {
		return -3;
	}
	out_canvas->color_type = canvas->color_type;
	out_canvas->opacity = canvas->opacity;
	if (0 != pd_canvas_create(out_canvas, rect.width, rect.height)) {
		return -1;
	}
	des_row = out_canvas->bytes;
	src_row = canvas->bytes + rect.y * canvas->bytes_per_row +
		  rect.x * canvas->bytes_per_pixel;
	for (y = 0; y < rect.height; ++y) {
		memcpy(des_row, src_row, out_canvas->bytes_per_row);
		des_row += out_canvas->bytes_per_row;
		src_row += canvas->bytes_per_row;
	}
	return 0;
}

void pd_canvas_copy(pd_canvas_t *des, const pd_canvas_t *src)
{
	const pd_canvas_t *canvas;

	if (!des || !pd_canvas_is_valid(src)) {
		return;
	}
	canvas = pd_canvas_get_quote(src);
	des->color_type = canvas->color_type;
	/* 创建合适尺寸的Graph */
	pd_canvas_create(des, src->width, src->height);
	pd_canvas_replace(des, src, 0, 0);
	des->opacity = src->opacity;
}

/* FIXME: improve alpha blending method
 * Existing alpha blending methods are inefficient and need to be optimized
 */

static void canvas_mix_argb_with_alpha(pd_canvas_t *dst, pd_rect_t des_rect,
				       const pd_canvas_t *src, int src_x,
				       int src_y)
{
	int x, y;
	pd_pixel_t *px_src, *px_dst;
	pd_pixel_t *px_row_src, *px_row_des;
	double a, out_a, out_r, out_g, out_b, src_a;

	px_row_src = src->pixels + src_y * src->width + src_x;
	px_row_des = dst->pixels + des_rect.y * dst->width + des_rect.x;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dst = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			src_a = px_src->a / 255.0;
			a = (1.0 - src_a) * px_dst->a / 255.0;
			out_r = px_dst->r * a + px_src->r * src_a;
			out_g = px_dst->g * a + px_src->g * src_a;
			out_b = px_dst->b * a + px_src->b * src_a;
			out_a = src_a + a;
			if (out_a > 0) {
				out_r /= out_a;
				out_g /= out_a;
				out_b /= out_a;
			}
			px_dst->r = (uchar_t)(out_r + 0.5);
			px_dst->g = (uchar_t)(out_g + 0.5);
			px_dst->b = (uchar_t)(out_b + 0.5);
			px_dst->a = (uchar_t)(255.0 * out_a + 0.5);
			++px_src;
			++px_dst;
		}
		px_row_des += dst->width;
		px_row_src += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dst = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			src_a = px_src->a / 255.0 * src->opacity;
			a = (1.0 - src_a) * px_dst->a / 255.0;
			out_r = px_dst->r * a + px_src->r * src_a;
			out_g = px_dst->g * a + px_src->g * src_a;
			out_b = px_dst->b * a + px_src->b * src_a;
			out_a = src_a + a;
			if (out_a > 0) {
				out_r /= out_a;
				out_g /= out_a;
				out_b /= out_a;
			}
			px_dst->r = (uchar_t)(out_r + 0.5);
			px_dst->g = (uchar_t)(out_g + 0.5);
			px_dst->b = (uchar_t)(out_b + 0.5);
			px_dst->a = (uchar_t)(255.0 * out_a + 0.5);
			++px_src;
			++px_dst;
		}
		px_row_des += dst->width;
		px_row_src += src->width;
	}
}

static void pd_canvas_mix_argb(pd_canvas_t *dest, pd_rect_t des_rect,
			       const pd_canvas_t *src, int src_x, int src_y)
{
	int x, y;
	uchar_t a;
	pd_pixel_t *px_src, *px_dest;
	pd_pixel_t *px_row_src, *px_row_des;

	px_row_src = src->pixels + src_y * src->width + src_x;
	px_row_des = dest->pixels + des_rect.y * dest->width + des_rect.x;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dest = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			pd_pixel_blend(px_dest, px_src, px_src->a);
			++px_src;
			++px_dest;
		}
		px_row_des += dest->width;
		px_row_src += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dest = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			a = (uchar_t)(px_src->a * src->opacity);
			pd_pixel_blend(px_dest, px_src, a);
			++px_src;
			++px_dest;
		}
		px_row_des += dest->width;
		px_row_src += src->width;
	}
}

static void pd_canvas_mix_argb2rgb(pd_canvas_t *des, pd_rect_t des_rect,
				   const pd_canvas_t *src, int src_x, int src_y)
{
	int x, y;
	uchar_t a;
	pd_pixel_t *px, *px_row;
	uchar_t *rowbytep, *bytep;

	/* 计算并保存第一行的首个像素的位置 */
	px_row = src->pixels + src_y * src->width + src_x;
	rowbytep = des->bytes + des_rect.y * des->bytes_per_row;
	rowbytep += des_rect.x * des->bytes_per_pixel;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px = px_row;
		bytep = rowbytep;
		for (x = 0; x < des_rect.width; ++x, ++px) {
			*bytep = _pd_alpha_blend(*bytep, px->b, px->a);
			++bytep;
			*bytep = _pd_alpha_blend(*bytep, px->g, px->a);
			++bytep;
			*bytep = _pd_alpha_blend(*bytep, px->r, px->a);
			++bytep;
		}
		rowbytep += des->bytes_per_row;
		px_row += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px = px_row;
		bytep = rowbytep;
		for (x = 0; x < des_rect.width; ++x, ++px) {
			a = (uchar_t)(px->a * src->opacity);
			*bytep = _pd_alpha_blend(*bytep, px->b, a);
			++bytep;
			*bytep = _pd_alpha_blend(*bytep, px->g, a);
			++bytep;
			*bytep = _pd_alpha_blend(*bytep, px->r, a);
			++bytep;
		}
		rowbytep += des->bytes_per_row;
		px_row += src->width;
	}
}

int pd_canvas_mix(pd_canvas_t *back, const pd_canvas_t *fore, int left, int top,
		  pd_bool with_alpha)
{
	pd_canvas_t w_slot;
	pd_rect_t r_rect, w_rect;

	if (!pd_canvas_is_valid(back) || !pd_canvas_is_valid(fore)) {
		return -1;
	}
	w_rect.x = left;
	w_rect.y = top;
	w_rect.width = fore->width;
	w_rect.height = fore->height;
	pd_rect_get_cut_area(back->width, back->height, w_rect, &r_rect);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	pd_canvas_quote(&w_slot, back, &w_rect);
	/* 获取实际操作区域 */
	pd_canvas_get_valid_rect(&w_slot, &w_rect);
	pd_canvas_get_valid_rect(fore, &r_rect);
	if (w_rect.width <= 0 || w_rect.height <= 0 || r_rect.width <= 0 ||
	    r_rect.height <= 0) {
		return -2;
	}
	top = r_rect.y;
	left = r_rect.x;
	/* 获取引用的源图像 */
	fore = pd_canvas_get_quote(fore);
	back = pd_canvas_get_quote(back);
	switch (fore->color_type) {
	case PD_COLOR_TYPE_RGB888:
		pd_canvas_direct_replace(back, w_rect, fore, left, top);
		return 0;
	case PD_COLOR_TYPE_ARGB8888:
		if (back->color_type == PD_COLOR_TYPE_RGB888) {
			canvas_mix_argb2rgb(back, w_rect, fore, left, top);
			return 0;
		}
		if (!with_alpha) {
			pd_canvas_mix_argb(back, w_rect, fore, left, top);
			return 0;
		}
		pd_canvas_mix_argb_with_alpha(back, w_rect, fore, left, top);
		return 0;
	default:
		break;
	}
	return -3;
}

int pd_canvas_fill_rect(pd_canvas_t *canvas, pd_color_t color, pd_rect_t rect)
{
	int x, y;
	size_t row_size;
	pd_pixel_t *pixel;

	PD_CANVAS_WRITING_CONTEXT(canvas, rect);
	row_size = rect.width * canvas->bytes_per_pixel;
	for (y = 0; y < rect.height; ++y) {
		pixel =
		    canvas->pixels + rect.y * canvas->bytes_per_row + rect.x;
		memset(pixel, color, row_size);
	}
	return 0;
}

int pd_canvas_tile(pd_canvas_t *buff, const pd_canvas_t *canvas,
		   pd_bool replace, pd_bool with_alpha)
{
	int ret = 0;
	unsigned x, y;

	if (!pd_canvas_is_valid(canvas) || !pd_canvas_is_valid(buff)) {
		return -1;
	}
	for (y = 0; y < buff->height; y += canvas->height) {
		for (x = 0; x < buff->width; x += canvas->width) {
			if (replace) {
				ret += pd_canvas_replace(buff, canvas, y, x);
				continue;
			}
			ret += pd_canvas_mix(buff, canvas, y, x, with_alpha);
		}
	}
	return ret;
}

int pd_canvas_veri_flip(const pd_canvas_t *canvas, pd_canvas_t *buff)
{
	int y;
	pd_rect_t rect;
	uchar_t *byte_src, *byte_des;

	if (!pd_canvas_is_valid(canvas)) {
		return -1;
	}
	pd_canvas_get_valid_rect(canvas, &rect);
	canvas = pd_canvas_get_quote(canvas);
	buff->opacity = canvas->opacity;
	buff->color_type = canvas->color_type;
	if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
		return -2;
	}
	byte_src = canvas->bytes;
	byte_src += (rect.y + rect.height - 1) * canvas->bytes_per_row;
	byte_src += rect.x * canvas->bytes_per_pixel;
	byte_des = buff->bytes;
	for (y = 0; y < rect.height; ++y) {
		memcpy(byte_des, byte_src, buff->bytes_per_row);
		byte_src -= canvas->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

static int canvas_horiz_flip(const pd_canvas_t *canvas, pd_canvas_t *buff)
{
	int x, y;
	pd_rect_t rect;
	uchar_t *src, *dest;

	if (!pd_canvas_is_valid(canvas)) {
		return -1;
	}
	pd_canvas_get_valid_rect(canvas, &rect);
	canvas = pd_canvas_get_quote(canvas);
	buff->opacity = canvas->opacity;
	buff->color_type = canvas->color_type;
	if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
		return -2;
	}
	for (y = 0; y < rect.height; ++y) {
		dest = pd_canvas_get_pixel_ptr(buff, y, 0);
		src = pd_canvas_get_pixel_ptr(canvas, rect.y + y,
					  rect.x + rect.width - 1);
		if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
			for (x = 0; x < rect.width; ++x) {
				*(pd_pixel_t *)dest = *(pd_pixel_t *)src;
				dest += canvas->bytes_per_pixel;
				src -= canvas->bytes_per_pixel;
			}
		} else {
			for (x = 0; x < rect.width; ++x) {
				dest[0] = src[0];
				dest[1] = src[1];
				dest[2] = src[2];
				dest += canvas->bytes_per_pixel;
				src -= canvas->bytes_per_pixel;
			}
		}
	}
	return 0;
}

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
	pd_canvas_get_valid_rect(canvas, &rect);
	canvas = pd_canvas_get_quote(canvas);
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
			px_row_src = canvas->pixels;
			px_row_src += (src_y + rect.y) * canvas->width + rect.x;
			px_des = buff->pixels + y * width;
			for (x = 0; x < width; ++x) {
				src_x = (int)(x * scale_x);
				px_src = px_row_src + src_x;
				*px_des++ = *px_src;
			}
		}
	} else {
		uchar_t *byte_src, *byte_des, *byte_row_src;
		for (y = 0; y < height; ++y) {
			src_y = (int)(y * scale_y);
			byte_row_src = canvas->bytes;
			byte_row_src +=
			    (src_y + rect.y) * canvas->bytes_per_row;
			byte_row_src += rect.x * canvas->bytes_per_pixel;
			byte_des = buff->bytes + y * buff->bytes_per_row;
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
	pd_pixel_t a, b, c, d, t_color;

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
	pd_canvas_get_valid_rect(canvas, &rect);
	canvas = pd_canvas_get_quote(canvas);
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
			pd_canvas_get_pixel(canvas, x + rect.x + 0,
					    y + rect.y + 0, a);
			pd_canvas_get_pixel(canvas, x + rect.x + 1,
					    y + rect.y + 0, b);
			pd_canvas_get_pixel(canvas, x + rect.x + 0,
					    y + rect.y + 1, c);
			pd_canvas_get_pixel(canvas, x + rect.x + 1,
					    y + rect.y + 1, d);
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
