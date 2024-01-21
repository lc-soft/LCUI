/*
 * lib/ui-cursor/src/cursor.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui_cursor.h>

static struct ui_cursor_t {
	int x, y;
	bool visible;
	pd_canvas_t image;
	app_window_t *window;
} ui_cursor;

static unsigned char ui_cursor_image_data[4][12 * 19] = {
	{ 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 229, 3, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 229, 3, 3, 0,
	0, 0, 0, 0, 0, 0, 3, 255, 255, 229, 3, 3, 0, 0,
	0, 0, 0, 0, 3, 255, 255, 255, 229, 3, 3, 0, 0, 0,
	0, 0, 3, 255, 255, 255, 255, 229, 3, 3, 0, 0, 0, 0,
	3, 255, 255, 255, 255, 253, 226, 3, 3, 0, 0, 0, 3, 255,
	255, 255, 253, 251, 248, 220, 3, 3, 0, 0, 3, 255, 255, 253,
	251, 248, 245, 241, 214, 3, 3, 0, 3, 255, 253, 251, 248, 245,
	241, 238, 234, 207, 3, 3, 3, 253, 251, 248, 245, 241, 238, 234,
	230, 226, 201, 3, 3, 251, 248, 245, 217, 238, 234, 3, 3, 3,
	3, 3, 3, 248, 245, 217, 3, 164, 230, 3, 3, 0, 0, 0,
	3, 245, 217, 3, 3, 3, 226, 201, 3, 0, 0, 0, 3, 217,
	3, 3, 0, 3, 176, 219, 3, 3, 0, 0, 3, 3, 3, 0,
	0, 3, 3, 216, 192, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 192, 211, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
	3, 3, 0, 0 },
	{ 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
	6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 230, 6, 6,
	0, 0, 0, 0, 0, 0, 0, 0, 6, 255, 230, 6, 6, 0,
	0, 0, 0, 0, 0, 0, 6, 255, 255, 230, 6, 6, 0, 0,
	0, 0, 0, 0, 6, 255, 255, 255, 230, 6, 6, 0, 0, 0,
	0, 0, 6, 255, 255, 255, 255, 230, 6, 6, 0, 0, 0, 0,
	6, 255, 255, 255, 255, 253, 226, 6, 6, 0, 0, 0, 6, 255,
	255, 255, 253, 251, 248, 221, 6, 6, 0, 0, 6, 255, 255, 253,
	251, 248, 245, 241, 214, 6, 6, 0, 6, 255, 253, 251, 248, 245,
	241, 238, 234, 207, 6, 6, 6, 253, 251, 248, 245, 241, 238, 234,
	230, 226, 201, 6, 6, 251, 248, 245, 217, 238, 234, 6, 6, 6,
	6, 6, 6, 248, 245, 217, 6, 165, 230, 6, 6, 0, 0, 0,
	6, 245, 217, 6, 6, 6, 226, 201, 6, 0, 0, 0, 6, 217,
	6, 6, 0, 6, 176, 219, 6, 6, 0, 0, 6, 6, 6, 0,
	0, 6, 6, 216, 192, 6, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 192, 211, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
	6, 6, 0, 0 },
	{ 26, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 26,
	26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 232, 26, 26,
	0, 0, 0, 0, 0, 0, 0, 0, 26, 255, 232, 26, 26, 0,
	0, 0, 0, 0, 0, 0, 26, 255, 255, 232, 26, 26, 0, 0,
	0, 0, 0, 0, 26, 255, 255, 255, 232, 26, 26, 0, 0, 0,
	0, 0, 26, 255, 255, 255, 255, 232, 26, 26, 0, 0, 0, 0,
	26, 255, 255, 255, 255, 253, 228, 26, 26, 0, 0, 0, 26, 255,
	255, 255, 253, 251, 248, 223, 26, 26, 0, 0, 26, 255, 255, 253,
	251, 248, 245, 241, 216, 26, 26, 0, 26, 255, 253, 251, 248, 245,
	241, 238, 234, 209, 26, 26, 26, 253, 251, 248, 245, 241, 238, 234,
	230, 226, 203, 26, 26, 251, 248, 245, 219, 238, 234, 26, 26, 26,
	26, 26, 26, 248, 245, 219, 26, 171, 230, 26, 26, 0, 0, 0,
	26, 245, 219, 26, 26, 26, 226, 203, 26, 0, 0, 0, 26, 219,
	26, 26, 0, 26, 181, 219, 26, 26, 0, 0, 26, 26, 26, 0,
	0, 26, 26, 216, 194, 26, 0, 0, 0, 0, 0, 0, 0, 0,
	26, 194, 211, 26, 0, 0, 0, 0, 0, 0, 0, 0, 26, 26,
	26, 26, 0, 0 },
	{ 231, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 231, 189,
	55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 231, 255, 189, 55,
	0, 0, 0, 0, 0, 0, 0, 0, 231, 255, 255, 189, 55, 0,
	0, 0, 0, 0, 0, 0, 231, 255, 255, 255, 189, 55, 0, 0,
	0, 0, 0, 0, 231, 255, 255, 255, 255, 189, 55, 0, 0, 0,
	0, 0, 231, 255, 255, 255, 255, 255, 189, 55, 0, 0, 0, 0,
	231, 255, 255, 255, 255, 255, 255, 189, 55, 0, 0, 0, 231, 255,
	255, 255, 255, 255, 255, 255, 189, 55, 0, 0, 231, 255, 255, 255,
	255, 255, 255, 255, 255, 189, 55, 0, 231, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 189, 55, 231, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 189, 231, 255, 255, 255, 255, 255, 255, 189, 189, 189,
	189, 189, 231, 255, 255, 255, 244, 255, 255, 188, 77, 0, 0, 0,
	231, 255, 255, 244, 55, 211, 255, 255, 211, 0, 0, 0, 231, 255,
	244, 55, 0, 180, 255, 255, 180, 77, 0, 0, 189, 244, 55, 0,
	0, 55, 215, 255, 255, 209, 0, 0, 0, 0, 0, 0, 0, 0,
	180, 255, 255, 204, 0, 0, 0, 0, 0, 0, 0, 0, 26, 215,
	158, 49, 0, 0 }
};

static int ui_cursor_load_default_image(pd_canvas_t *buff)
{
	unsigned x, y;
	size_t i;
	pd_color_t *p;

	if (pd_canvas_is_valid(buff)) {
		pd_canvas_destroy(buff);
	}
	pd_canvas_init(buff);
	buff->color_type = PD_COLOR_TYPE_ARGB;
	if (pd_canvas_create(buff, 12, 19) != 0) {
		return -1;
	}
	for (y = 0; y < buff->height; ++y) {
		i = y * buff->width;
		p = buff->argb + i;
		for (x = 0; x < buff->width; ++x, ++i) {
			p->r = ui_cursor_image_data[0][i];
			p->g = ui_cursor_image_data[1][i];
			p->b = ui_cursor_image_data[2][i];
			p->a = ui_cursor_image_data[3][i];
		}
	}
	return 0;
}

static void ui_cursor_on_mouse_event(app_event_t *e, void *arg)
{
	// TODO: 事件中的 xy 是否需要转换为相对于窗口客户区？
	ui_cursor.x = e->mouse.x;
	ui_cursor.y = e->mouse.y;
	if (ui_cursor.window != e->window) {
		ui_cursor_refresh();
	}
	ui_cursor.window = e->window;
	ui_cursor_refresh();
}

static void ui_cursor_get_rect(pd_rect_t *rect)
{
	rect->x = ui_cursor.x;
	rect->y = ui_cursor.y;
	rect->width = ui_cursor.image.width;
	rect->height = ui_cursor.image.height;
}

void ui_cursor_refresh(void)
{
	app_event_t e = { 0 };

	if (!ui_cursor.visible || !ui_cursor.window) {
		return;
	}
	e.type = APP_EVENT_PAINT;
	e.window = ui_cursor.window;
	ui_cursor_get_rect(&e.paint.rect);
	app_post_event(&e);
}

bool ui_cursor_is_visible(void)
{
	return ui_cursor.visible;
}

void ui_cursor_show(void)
{
	ui_cursor.visible = TRUE;
	ui_cursor_refresh();
}

void ui_cursor_hide(void)
{
	ui_cursor_refresh();
	ui_cursor.visible = FALSE;
}

void ui_cursor_set_position(int x, int y)
{
	ui_cursor_refresh();
	ui_cursor.x = x;
	ui_cursor.y = y;
	ui_cursor_refresh();
}

int ui_cursor_set_image(pd_canvas_t *image)
{
	if (pd_canvas_is_valid(image)) {
		ui_cursor_refresh();
		if (pd_canvas_is_valid(&ui_cursor.image)) {
			pd_canvas_destroy(&ui_cursor.image);
		}
		pd_canvas_copy(&ui_cursor.image, image);
		ui_cursor_refresh();
		return 0;
	}
	return -1;
}

void ui_cursor_get_position(int *x, int *y)
{
	*x = ui_cursor.x;
	*y = ui_cursor.y;
}

int ui_cursor_paint(app_window_t *w, app_window_paint_t* paint)
{
	int x, y;
	if (!ui_cursor.visible) {
		return 0;
	}
	// TODO: 转换为窗口客户区内的坐标
	x = ui_cursor.x - paint->rect.x;
	y = ui_cursor.y - paint->rect.y;
	return pd_canvas_mix(&paint->canvas, &ui_cursor.image, x, y, FALSE);
}

void ui_cursor_init(void)
{
	pd_canvas_t image;

	pd_canvas_init(&image);
	pd_canvas_init(&ui_cursor.image);
	/* 载入自带的游标的图形数据 */
	ui_cursor_load_default_image(&image);
	// TODO: 移除 app 依赖
	app_on_event(APP_EVENT_MOUSEMOVE, ui_cursor_on_mouse_event, NULL);
	ui_cursor_set_image(&image);
	pd_canvas_destroy(&image);
}

void ui_cursor_destroy(void)
{
	pd_canvas_destroy(&ui_cursor.image);
}
