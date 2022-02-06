#include <stdio.h>
#include <stdlib.h>
#include <LCUI/util.h>
#include <LCUI/graph.h>
#include "internal.h"

void font_bitmap_init(font_bitmap_t *bitmap)
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

void font_bitmap_destroy(font_bitmap_t *bitmap)
{
	if (bitmap->buffer) {
		free(bitmap->buffer);
	}
	font_bitmap_init(bitmap);
}

/** 在屏幕打印以0和1表示字体位图 */
int font_bitmap_print(font_bitmap_t *font_bitmap)
{
	int x, y, m;
	for (y = 0; y < font_bitmap->rows; ++y) {
		m = y * font_bitmap->width;
		for (x = 0; x < font_bitmap->width; ++x, ++m) {
			if (font_bitmap->buffer[m] > 128) {
				printf("#");
			} else if (font_bitmap->buffer[m] > 64) {
				printf("-");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

static void font_bitmap_mix_argb(pd_canvas_t *graph, pd_rect_t *write_rect,
				 const font_bitmap_t *bmp, pd_color_t color,
				 pd_rect_t *read_rect)
{
	int x, y;
	pd_color_t c;
	pd_color_t *px, *px_row_des;
	uchar_t *byte_ptr, *byte_row_ptr;

	byte_row_ptr = bmp->buffer + read_rect->y * bmp->width;
	px_row_des = graph->pixels + write_rect->y * graph->width;
	byte_row_ptr += read_rect->x;
	px_row_des += write_rect->x;
	for (y = 0; y < read_rect->height; ++y) {
		px = px_row_des;
		byte_ptr = byte_row_ptr;
		for (x = 0; x < read_rect->width; ++x, ++byte_ptr, ++px) {
			c = color;
			c.alpha = (uchar_t)(*byte_ptr * color.alpha / 255.0);
			pd_over_pixel(px, &c);
		}
		px_row_des += graph->width;
		byte_row_ptr += bmp->width;
	}
}

static void font_bitmap_mix_rgb(pd_canvas_t *graph, pd_rect_t *write_rect,
				const font_bitmap_t *bmp, pd_color_t color,
				pd_rect_t *read_rect)
{
	int x, y;
	uchar_t *byte_src, *byte_row_src, *byte_row_des, *byte_des, alpha;
	byte_row_src = bmp->buffer + read_rect->y * bmp->width + read_rect->x;
	byte_row_des = graph->bytes + write_rect->y * graph->bytes_per_row;
	byte_row_des += write_rect->x * graph->bytes_per_pixel;
	for (y = 0; y < read_rect->height; ++y) {
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for (x = 0; x < read_rect->width; ++x) {
			alpha = (uchar_t)(*byte_src * color.alpha / 255);
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

int font_bitmap_mix(pd_canvas_t *graph, pd_pos_t pos, const font_bitmap_t *bmp,
		    pd_color_t color)
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
	pd_rect_get_cut_area(graph->width, graph->height, w_rect, &r_rect);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	pd_canvas_quote(&write_slot, graph, &w_rect);
	pd_canvas_get_valid_rect(&write_slot, &w_rect);
	/* 获取背景图引用的源图形 */
	graph = pd_canvas_get_quote(graph);
	if (graph->color_type == PD_COLOR_TYPE_ARGB) {
		font_bitmap_mix_argb(graph, &w_rect, bmp, color, &r_rect);
	} else {
		font_bitmap_mix_rgb(graph, &w_rect, bmp, color, &r_rect);
	}
	return 0;
}
