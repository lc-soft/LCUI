#include "../include/pandagl.h"

int pd_canvas_veri_flip(const pd_canvas_t *canvas, pd_canvas_t *buff)
{
	int y;
	pd_rect_t rect;
	uchar_t *byte_src, *byte_des;

	if (!pd_canvas_is_valid(canvas)) {
		return -1;
	}
	pd_canvas_get_quote_rect(canvas, &rect);
	canvas = pd_canvas_get_quote_source_readonly(canvas);
	buff->opacity = canvas->opacity;
	buff->color_type = canvas->color_type;
	if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
		return -2;
	}
	byte_src = pd_canvas_pixel_at(canvas, rect.x, rect.y + rect.height - 1);
	byte_des = buff->bytes;
	for (y = 0; y < rect.height; ++y) {
		memcpy(byte_des, byte_src, buff->bytes_per_row);
		byte_src -= canvas->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

int canvas_horiz_flip(const pd_canvas_t *canvas, pd_canvas_t *buff)
{
	int x, y;
	pd_rect_t rect;
	uchar_t *src, *dest;

	if (!pd_canvas_is_valid(canvas)) {
		return -1;
	}
	pd_canvas_get_quote_rect(canvas, &rect);
	canvas = pd_canvas_get_quote_source_readonly(canvas);
	buff->opacity = canvas->opacity;
	buff->color_type = canvas->color_type;
	if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
		return -2;
	}
	for (y = 0; y < rect.height; ++y) {
		dest = pd_canvas_pixel_at(buff, y, 0);
		src = pd_canvas_pixel_at(canvas, rect.y + y,
					 rect.x + rect.width - 1);
		if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
			for (x = 0; x < rect.width; ++x) {
				*(pd_color_t *)dest = *(pd_color_t *)src;
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
