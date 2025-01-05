/*
 * lib/pandagl/src/font/bitmap.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pandagl.h>
#include "bitmap.h"

void pd_font_bitmap_init(pd_font_bitmap_t *bitmap)
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

void pd_font_bitmap_destroy(pd_font_bitmap_t *bitmap)
{
	if (bitmap->buffer) {
		free(bitmap->buffer);
	}
	pd_font_bitmap_init(bitmap);
}

static void pd_canvas_mix_font_bitmap_argb(pd_canvas_t *graph,
					   pd_rect_t *write_rect,
					   const pd_font_bitmap_t *bmp,
					   pd_color_t color,
					   pd_rect_t *read_rect)
{
	int x, y;
	pd_color_t c;
	pd_color_t *px, *px_row_des;
	uint8_t *byte_ptr, *byte_row_ptr;

	byte_row_ptr = bmp->buffer + read_rect->y * bmp->width;
	px_row_des = graph->argb + write_rect->y * graph->width;
	byte_row_ptr += read_rect->x;
	px_row_des += write_rect->x;
	for (y = 0; y < read_rect->height; ++y) {
		px = px_row_des;
		byte_ptr = byte_row_ptr;
		for (x = 0; x < read_rect->width; ++x, ++byte_ptr, ++px) {
			c = color;
			c.alpha = (uint8_t)(*byte_ptr * color.alpha / 255.0);
			pd_over_pixel(px, &c, 1.0);
		}
		px_row_des += graph->width;
		byte_row_ptr += bmp->width;
	}
}

static void pd_canvas_mix_font_bitmap_rgb(pd_canvas_t *graph,
					  pd_rect_t *write_rect,
					  const pd_font_bitmap_t *bmp,
					  pd_color_t color,
					  pd_rect_t *read_rect)
{
	int x, y;
	uint8_t *byte_src, *byte_row_src, *byte_row_des, *byte_des, alpha;
	byte_row_src = bmp->buffer + read_rect->y * bmp->width + read_rect->x;
	byte_row_des = graph->bytes + write_rect->y * graph->bytes_per_row;
	byte_row_des += write_rect->x * graph->bytes_per_pixel;
	for (y = 0; y < read_rect->height; ++y) {
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for (x = 0; x < read_rect->width; ++x) {
			alpha = (uint8_t)(*byte_src * color.alpha / 255);
			pd_alpha_blend(*byte_des, color.b, alpha);
			++byte_des;
			pd_alpha_blend(*byte_des, color.g, alpha);
			++byte_des;
			pd_alpha_blend(*byte_des, color.r, alpha);
			++byte_des;
			++byte_src;
		}
		byte_row_des += graph->bytes_per_row;
		byte_row_src += bmp->width;
	}
}

int pd_canvas_mix_font_bitmap(pd_canvas_t *graph, pd_pos_t pos,
			      const pd_font_bitmap_t *bmp, pd_color_t color)
{
	pd_canvas_t write_slot;
	pd_rect_t r_rect, w_rect;

	if (pos.x > (int)graph->width || pos.y > (int)graph->height) {
		return -2;
	}
	/* 获取写入区域 */
	w_rect.x = pos.x;
	w_rect.y = pos.y;
	w_rect.width = bmp->width;
	w_rect.height = bmp->rows;
	/* 获取需要裁剪的区域 */
	r_rect = pd_rect_crop(&w_rect, graph->width, graph->height);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	pd_canvas_quote(&write_slot, graph, &w_rect);
	pd_canvas_get_quote_rect(&write_slot, &w_rect);
	/* 获取背景图引用的源图形 */
	graph = pd_canvas_get_quote_source(graph);
	if (graph->color_type == PD_COLOR_TYPE_ARGB) {
		pd_canvas_mix_font_bitmap_argb(graph, &w_rect, bmp, color,
					       &r_rect);
	} else {
		pd_canvas_mix_font_bitmap_rgb(graph, &w_rect, bmp, color,
					      &r_rect);
	}
	return 0;
}
