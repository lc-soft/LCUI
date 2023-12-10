/*
 * tests/test_paint_background.c
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

int test_paint_background_color(void)
{
	pd_canvas_t canvas;
	pd_color_t gray = pd_rgb(240, 240, 240);
	pd_color_t green = pd_rgb(102, 204, 0);
	pd_rect_t rect = { 200, 100, 400, 300 };
	pd_background_t bg = { 0 };
	pd_context_t* ctx;

	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, 800, 600);
	pd_canvas_fill(&canvas, gray);
	// 设置背景色
	bg.color = green;
	// 创建绘制上下文
	ctx = pd_context_create(&canvas, &rect);
	// 绘制背景
	pd_paint_background(ctx, &bg, &rect);
	pd_write_png_file("test_paint_background_color.png", &canvas);
	pd_context_destroy(ctx);
	pd_canvas_destroy(&canvas);
	return 0;
}

int test_paint_background_image(void)
{
	pd_canvas_t canvas;
	pd_canvas_t image;
	pd_color_t gray = pd_rgb(240, 240, 240);
	pd_color_t green = pd_rgb(102, 204, 0);
	pd_rect_t rect = { 200, 100, 400, 300 };
	pd_background_t bg = { 0 };
	pd_context_t* ctx;

	pd_canvas_init(&canvas);
	pd_canvas_init(&image);
	pd_canvas_create(&canvas, 800, 600);
	pd_canvas_fill(&canvas, gray);
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
	// 创建绘制上下文
	ctx = pd_context_create(&canvas, &rect);
	// 绘制背景
	pd_paint_background(ctx, &bg, &rect);
	pd_write_png_file("test_paint_background_image.png", &canvas);
	pd_context_destroy(ctx);
	pd_canvas_destroy(&image);
	pd_canvas_destroy(&canvas);
	return 0;
}

int test_paint_background_image_with_size(void)
{
	pd_canvas_t canvas;
	pd_canvas_t image;
	pd_color_t gray = pd_rgb(240, 240, 240);
	pd_color_t green = pd_rgb(102, 204, 0);
	pd_rect_t rect = { 200, 100, 400, 300 };
	pd_background_t bg = { 0 };
	pd_context_t* ctx;

	pd_canvas_init(&canvas);
	pd_canvas_init(&image);
	pd_canvas_create(&canvas, 800, 600);
	pd_canvas_fill(&canvas, gray);
	// 读取背景图片
	if (pd_read_image_from_file("test_image_reader.png", &image) != 0) {
		return -1;
	}
	// 设置背景色
	bg.color = green;
	// 设置背景图
	bg.image = &image;
	// 将背景图设置成与背景区域相同的尺寸
	bg.width = rect.width;
	bg.height = rect.height;
	// 创建绘制上下文
	ctx = pd_context_create(&canvas, &rect);
	// 绘制背景
	pd_paint_background(ctx, &bg, &rect);
	pd_write_png_file("test_paint_background_image_with_size.png", &canvas);
	pd_context_destroy(ctx);
	pd_canvas_destroy(&image);
	pd_canvas_destroy(&canvas);
	return 0;
}

int test_paint_background_image_with_position(void)
{
	pd_canvas_t canvas;
	pd_canvas_t image;
	pd_color_t gray = pd_rgb(240, 240, 240);
	pd_color_t green = pd_rgb(102, 204, 0);
	pd_rect_t rect = { 200, 100, 400, 300 };
	pd_background_t bg = { 0 };
	pd_context_t* ctx;

	pd_canvas_init(&canvas);
	pd_canvas_init(&image);
	pd_canvas_create(&canvas, 800, 600);
	pd_canvas_fill(&canvas, gray);
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
	bg.x = (rect.width - image.width) / 2;
	bg.y = (rect.height - image.height) / 2;
	// 创建绘制上下文
	ctx = pd_context_create(&canvas, &rect);
	// 绘制背景
	pd_paint_background(ctx, &bg, &rect);
	pd_write_png_file("test_paint_background_image_with_position.png",
			  &canvas);
	pd_context_destroy(ctx);
	pd_canvas_destroy(&image);
	pd_canvas_destroy(&canvas);
	return 0;
}

int main(void)
{
	test_paint_background_color();
	test_paint_background_image();
	test_paint_background_image_with_size();
	test_paint_background_image_with_position();
	return 0;
}
