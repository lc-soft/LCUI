/*
 * lib/pandagl/src/pixel.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <memory.h>
#include <pandagl/pixel.h>

unsigned pd_get_pixel_size(pd_color_type_t color_type)
{
	switch (color_type) {
	case PD_COLOR_TYPE_INDEX8:
	case PD_COLOR_TYPE_GRAY8:
	case PD_COLOR_TYPE_RGB323:
	case PD_COLOR_TYPE_ARGB2222:
		return 1;
	case PD_COLOR_TYPE_RGB555:
	case PD_COLOR_TYPE_RGB565:
		return 2;
	case PD_COLOR_TYPE_RGB888:
		return 3;
	case PD_COLOR_TYPE_ARGB8888:
	default:
		break;
	}
	return 4;
}

unsigned pd_get_pixel_row_size(pd_color_type_t color_type, size_t len)
{
	return (unsigned)(ceil(pd_get_pixel_size(color_type) * len / 4.0)) * 4;
}

static void pd_format_pixels_argb2rgb(const pd_color_t *in_pixels,
				      uint8_t *out_pixels, size_t count)
{
	const pd_color_t *p_px, *p_end_px;
	uint8_t *p_out_byte;

	p_px = in_pixels;
	p_out_byte = out_pixels;
	/* 遍历到倒数第二个像素为止 */
	p_end_px = p_px + count - 1;
	for (; p_px < p_end_px; ++p_px) {
		*((pd_color_t*)p_out_byte) = *p_px;
		p_out_byte += 3;
	}
	/* 最后一个像素，以逐个字节的形式写数据 */
	*p_out_byte++ = p_px->blue;
	*p_out_byte++ = p_px->green;
	*p_out_byte++ = p_px->red;
}

static void pd_format_pixels_rgb2argb(const uint8_t *in_bytes,
				      pd_color_t *out_pixels, size_t count)
{
	pd_color_t *p_px, *p_end_px;
	const uint8_t *p_in_byte;

	p_in_byte = in_bytes;
	p_px = (pd_color_t *)out_pixels;
	p_end_px = p_px + count;
	while (p_px < p_end_px) {
		p_px->blue = *p_in_byte++;
		p_px->green = *p_in_byte++;
		p_px->red = *p_in_byte++;
		p_px->alpha = 255;
		++p_px;
	}
}

int pd_format_pixels(const uint8_t *in_pixels, pd_color_type_t in_color_type,
		     uint8_t *out_pixels, pd_color_type_t out_color_type,
		     size_t count)
{
	if (in_color_type == out_color_type) {
		memcpy(out_pixels, in_pixels, pd_get_pixel_row_size(in_color_type, count));
		return 0;
	}
	switch (in_color_type) {
	case PD_COLOR_TYPE_ARGB8888:
		if (out_color_type == PD_COLOR_TYPE_RGB888) {
			pd_format_pixels_argb2rgb((pd_color_t*)in_pixels, out_pixels, count);
			return 0;
		}
		break;
	case PD_COLOR_TYPE_RGB888:
		if (out_color_type == PD_COLOR_TYPE_ARGB8888) {
			pd_format_pixels_rgb2argb(in_pixels, (pd_color_t*)out_pixels, count);
			return 0;
		}
		break;
	default:
		break;
	}
	return -1;
}
