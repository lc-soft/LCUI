/*
 * lib/platform/src/linux/linux_app.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include "../app.h"

#ifdef LIBPLAT_LINUX

static struct app_t {
	bool active;
	app_window_driver_t window;
	app_driver_t app;
	app_id_t id;
} linux_app;

int app_process_native_events(app_process_events_option_t option)
{
	return linux_app.app.process_events(option);
}

int app_add_native_event_listener(int type, app_native_event_handler_t handler, void *data)
{
	return linux_app.app.on_event(type, handler, data);
}

int app_remove_native_event_listener(int type, app_native_event_handler_t handler)
{
	return linux_app.app.off_event(type, handler);
}

LIBPLAT_PUBLIC int app_get_screen_width(void)
{
	return linux_app.app.get_screen_width();
}

LIBPLAT_PUBLIC int app_get_screen_height(void)
{
	return linux_app.app.get_screen_height();
}

app_window_t *app_get_window_by_handle(void *handle)
{
	return linux_app.app.get_window(handle);
}

void app_window_close(app_window_t *wnd)
{
	return linux_app.window.close(wnd);
}

void app_window_destroy(app_window_t *wnd)
{
	return linux_app.window.destroy(wnd);
}

void app_window_activate(app_window_t *wnd)
{
	return linux_app.window.activate(wnd);
}

void app_window_set_title(app_window_t *wnd, const wchar_t *title)
{
	return linux_app.window.set_title(wnd, title);
}

void app_window_set_size(app_window_t *wnd, int width, int height)
{
	return linux_app.window.set_size(wnd, width, height);
}

void app_window_set_position(app_window_t *wnd, int x, int y)
{
	return linux_app.window.set_position(wnd, x, y);
}

void *app_window_get_handle(app_window_t *wnd)
{
	return linux_app.window.get_handle(wnd);
}

int app_window_get_width(app_window_t *wnd)
{
	return linux_app.window.get_width(wnd);
}

int app_window_get_height(app_window_t *wnd)
{
	return linux_app.window.get_height(wnd);
}

void app_window_set_min_width(app_window_t *wnd, int min_width)
{
	linux_app.window.set_max_width(wnd, min_width);
}

void app_window_set_min_height(app_window_t *wnd, int min_height)
{
	linux_app.window.set_max_width(wnd, min_height);
}

void app_window_set_max_width(app_window_t *wnd, int max_width)
{
	linux_app.window.set_max_width(wnd, max_width);
}

void app_window_set_max_height(app_window_t *wnd, int max_height)
{
	linux_app.window.set_max_height(wnd, max_height);
}

void app_window_show(app_window_t *wnd)
{
	linux_app.window.show(wnd);
}

void app_window_hide(app_window_t *wnd)
{
	linux_app.window.hide(wnd);
}

app_window_paint_t *app_window_begin_paint(app_window_t *wnd, pd_rect_t *rect)
{
	return linux_app.window.begin_paint(wnd, rect);
}

void app_window_end_paint(app_window_t *wnd, app_window_paint_t *paint)
{
	linux_app.window.end_paint(wnd, paint);
}

void app_window_present(app_window_t *wnd)
{
	linux_app.window.present(wnd);
}

app_window_t *app_window_create(const wchar_t *title, int x, int y, int width,
				int height, app_window_t *parent)
{
	return linux_app.app.create_window(title, x, y, width, height, parent);
}

void app_present(void)
{
	linux_app.app.present();
}

void app_exit(int exit_code)
{
	linux_app.app.exit(exit_code);
}

app_id_t app_get_id(void)
{
	return linux_app.id;
}

int app_init_engine(const wchar_t *name)
{
	linux_app.id = APP_ID_LINUX;
#ifdef LIBPLAT_HAS_LIBX11
	x11_app_driver_init(&linux_app.app);
	x11_app_window_driver_init(&linux_app.window);
	if (linux_app.app.init(name) == 0) {
		logger_debug("[app] use engine: x11app\n");
		linux_app.id = APP_ID_LINUX_X11;
		linux_app.active = true;
		return 0;
	}
	memset(&linux_app.app, 0, sizeof(linux_app.app));
	memset(&linux_app.window, 0, sizeof(linux_app.window));
#endif
	logger_debug("[app] use engine: fbapp\n");
	fb_app_driver_init(&linux_app.app);
	fb_app_window_driver_init(&linux_app.window);
	if (linux_app.app.init(name) == 0) {
		linux_app.active = true;
		linux_mouse_init();
		linux_keyboard_init();
		return 0;
	}
	return -1;
}

int app_destroy_engine(void)
{
	if (!linux_app.active) {
		return -1;
	}
	linux_app.active = false;
	if (linux_app.id != APP_ID_LINUX_X11) {
		linux_mouse_destroy();
		linux_keyboard_destroy();
	}
	return linux_app.app.destroy();
}

#endif
