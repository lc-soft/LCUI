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
	pd_rect_correct(&quote_rect, source->width, source->height);
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

void pd_canvas_get_quote_rect(const pd_canvas_t *canvas, pd_rect_t *rect)
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

	byte_row_src = pd_canvas_pixel_at(src, src_x, src_y);
	byte_row_des = pd_canvas_pixel_at(src, des_rect.x, des_rect.y);
	for (y = 0; y < des_rect.height; ++y) {
		pd_format_pixels(byte_row_src, src->color_type, byte_row_des,
				 des->color_type, des_rect.width);
		byte_row_src += src->bytes_per_row;
		byte_row_des += des->bytes_per_row;
	}
}

int pd_canvas_begin_writing(pd_canvas_t **canvas, pd_rect_t *rect)
{
	pd_canvas_t tmp;

	if (!pd_canvas_is_valid(*canvas)) {
		return -1;
	}
	pd_canvas_quote(&tmp, *canvas, rect);
	pd_canvas_get_quote_rect(&tmp, rect);
	if (rect->width < 1 || rect->height < 1) {
		return -1;
	}
	*canvas = pd_canvas_get_quote_source(&tmp);
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
	if (pd_canvas_begin_writing(&back, &write_rect) != 0) {
		return -1;
	}
	pd_canvas_get_quote_rect(fore, &read_rect);
	if (read_rect.width <= 0 || read_rect.height <= 0) {
		return -2;
	}
	left = read_rect.x;
	top = read_rect.y;
	fore = pd_canvas_get_quote_source_readonly(fore);
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
	pd_rect_correct(&rect, canvas->width, canvas->height);
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
	canvas = pd_canvas_get_quote_source_readonly(src);
	des->color_type = canvas->color_type;
	/* 创建合适尺寸的Graph */
	pd_canvas_create(des, src->width, src->height);
	pd_canvas_replace(des, src, 0, 0);
	des->opacity = src->opacity;
}

/* FIXME: improve alpha blending method
 * Existing alpha blending methods are inefficient and need to be optimized
 */

static void pd_canvas_mix_argb_with_alpha(pd_canvas_t *des, pd_rect_t des_rect,
					  const pd_canvas_t *src, int src_x,
					  int src_y)
{
	int x, y;
	pd_color_t *px_src, *px_des;
	pd_color_t *px_row_src, *px_row_des;

	px_row_src = pd_canvas_pixel_at(src, src_x, src_y);
	px_row_des = pd_canvas_pixel_at(des, des_rect.x, des_rect.y);
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_des = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			pd_over_pixel(px_des, px_src, src->opacity);
			++px_src;
			++px_des;
		}
		px_row_des += des->width;
		px_row_src += src->width;
	}
}

static void pd_canvas_mix_argb(pd_canvas_t *dest, pd_rect_t des_rect,
			       const pd_canvas_t *src, int src_x, int src_y)
{
	int x, y;
	uchar_t a;
	pd_color_t *px_src, *px_dest;
	pd_color_t *px_row_src, *px_row_des;

	px_row_src = pd_canvas_pixel_at(src, src_x, src_y);
	px_row_des = pd_canvas_pixel_at(dest, des_rect.x, des_rect.y);
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dest = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			pd_blend_pixel(px_dest, px_src, px_src->a);
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
			pd_blend_pixel(px_dest, px_src, a);
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
	pd_color_t *px, *px_row;
	uchar_t *rowbytep, *bytep;

	/* 计算并保存第一行的首个像素的位置 */
	px_row = pd_canvas_pixel_at(src, src_x, src_y);
	rowbytep = pd_canvas_pixel_at(des, des_rect.x, des_rect.y);
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
	r_rect = pd_rect_crop(&w_rect, back->width, back->height);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	pd_canvas_quote(&w_slot, back, &w_rect);
	/* 获取实际操作区域 */
	pd_canvas_get_quote_rect(&w_slot, &w_rect);
	pd_canvas_get_quote_rect(fore, &r_rect);
	if (w_rect.width <= 0 || w_rect.height <= 0 || r_rect.width <= 0 ||
	    r_rect.height <= 0) {
		return -2;
	}
	top = r_rect.y;
	left = r_rect.x;
	/* 获取引用的源图像 */
	fore = pd_canvas_get_quote_source_readonly(fore);
	back = pd_canvas_get_quote_source(back);
	switch (fore->color_type) {
	case PD_COLOR_TYPE_RGB888:
		pd_canvas_direct_replace(back, w_rect, fore, left, top);
		return 0;
	case PD_COLOR_TYPE_ARGB8888:
		if (back->color_type == PD_COLOR_TYPE_RGB888) {
			pd_canvas_mix_argb2rgb(back, w_rect, fore, left, top);
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
	uchar_t *p;

	if (pd_canvas_begin_writing(&canvas, &rect) != 0) {
		return -1;
	}

	PD_CANVAS_ROW_WRITING_BEGIN(canvas, rect)

	PD_CANVAS_ARGB_PIXEL_BEGIN(canvas, rect, p)
	*(pd_color_t *)p = color;
	PD_CANVAS_ARGB_PIXEL_END

	PD_CANVAS_RGB_PIXEL_BEGIN(canvas, rect, p)
	p[0] = color.b;
	p[1] = color.g;
	p[2] = color.r;
	PD_CANVAS_RGB_PIXEL_END

	PD_CANVAS_ROW_WRITING_END(canvas)
	return 0;
}
