﻿/*
 * tests/test_paint_border.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <pandagl.h>

int paint_background(pd_context_t* ctx, pd_rect_t* box)
{
	pd_canvas_t image;
	pd_color_t green = pd_rgb(102, 204, 0);
	pd_background_t bg = { 0 };

	pd_canvas_init(&image);
	// 读取背景图片
	if (pd_read_image_from_file("test_image_reader.png", &image) != 0) {
		return -1;
	}
	// 设置背景色
	bg.color = green;
	// 设置背景图
	bg.image = &image;
	bg.width = image.width;
	bg.height = image.height;
	// 让背景图居中
	bg.x = (box->width - image.width) / 2;
	bg.y = (box->height - image.height) / 2;
	// 绘制背景
	pd_paint_background(ctx, &bg, box);
	pd_canvas_destroy(&image);
	return 0;
}

void paint_border(pd_context_t* ctx, pd_rect_t* box)
{
	pd_border_t border = { 0 };
	pd_color_t black = pd_rgb(0, 0, 0);

	border.top.color = black;
	border.top.style = CSS_KEYWORD_SOLID;
	border.top.width = 4;
	border.right.color = black;
	border.right.style = CSS_KEYWORD_SOLID;
	border.right.width = 4;
	border.bottom.color = black;
	border.bottom.style = CSS_KEYWORD_SOLID;
	border.bottom.width = 4;
	border.left.color = black;
	border.left.style = CSS_KEYWORD_SOLID;
	border.left.width = 4;
	border.top_left_radius = 32;
	border.top_right_radius = 32;
	border.bottom_left_radius = 32;
	border.bottom_right_radius = 32;
	pd_paint_border(ctx, &border, box);
}

int main(void)
{
	int border_size = 4;

	pd_canvas_t canvas;
	pd_canvas_t layer;
	pd_color_t gray = pd_rgb(240, 240, 240);
	pd_rect_t border_box = { 0, 0, 400, 300 };
	pd_rect_t bg_box = { border_box.x + border_size,
			     border_box.y + border_size,
			     border_box.width - border_size * 2,
			     border_box.height - border_size * 2 };
	pd_rect_t layer_rect = { 0, 0, border_box.width, border_box.height };
	pd_context_t* paint;

	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, 800, 600);
	pd_canvas_fill(&canvas, gray);

	pd_canvas_init(&layer);
	layer.color_type = PD_COLOR_TYPE_ARGB;
	pd_canvas_create(&layer, layer_rect.width, layer_rect.height);

	// 创建绘制上下文
	paint = pd_context_create(&layer, &layer_rect);
	paint->with_alpha = true;
	paint_background(paint, &bg_box);
	paint_border(paint, &border_box);
	pd_canvas_mix(&canvas, &layer, (canvas.width - layer_rect.width) / 2,
		      (canvas.height - layer_rect.height) / 2, false);
	pd_write_png_file("test_paint_border.png", &canvas);
	pd_canvas_destroy(&canvas);
	return 0;
}
