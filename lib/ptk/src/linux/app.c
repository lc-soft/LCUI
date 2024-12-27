/*
 * lib/ptk/src/linux/app.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <yutil.h>
#include "ptk.h"
#include "mouse.h"
#include "keyboard.h"
#include "fbapp.h"
#include "x11app.h"
#include "x11clipboard.h"

#ifdef PTK_LINUX

static struct app {
        bool active;
        ptk_window_driver_t window;
        ptk_app_driver_t app;
        ptk_app_id_t id;
} linux_app;

int ptk_process_native_events(ptk_process_events_option_t option)
{
        return linux_app.app.process_events(option);
}

int ptk_add_native_event_listener(int type, ptk_native_event_handler_t handler,
                                  void *data)
{
        return linux_app.app.on_event(type, handler, data);
}

int ptk_remove_native_event_listener(int type,
                                     ptk_native_event_handler_t handler)
{
        return linux_app.app.off_event(type, handler);
}

PTK_PUBLIC int ptk_screen_get_width(void)
{
        return linux_app.app.get_screen_width();
}

PTK_PUBLIC int ptk_screen_get_height(void)
{
        return linux_app.app.get_screen_height();
}

ptk_window_t *ptk_find_window(void *handle)
{
        return linux_app.app.get_window(handle);
}

void ptk_window_close(ptk_window_t *wnd)
{
        return linux_app.window.close(wnd);
}

void ptk_window_destroy(ptk_window_t *wnd)
{
        return linux_app.window.destroy(wnd);
}

void ptk_window_set_fullscreen(ptk_window_t *wnd, bool fullscreen)
{
        // TODO:
}

void ptk_window_activate(ptk_window_t *wnd)
{
        return linux_app.window.activate(wnd);
}

void ptk_window_set_title(ptk_window_t *wnd, const wchar_t *title)
{
        return linux_app.window.set_title(wnd, title);
}

void ptk_window_set_size(ptk_window_t *wnd, int width, int height)
{
        return linux_app.window.set_size(wnd, width, height);
}

void ptk_window_set_position(ptk_window_t *wnd, int x, int y)
{
        return linux_app.window.set_position(wnd, x, y);
}

void *ptk_window_get_handle(ptk_window_t *wnd)
{
        return linux_app.window.get_handle(wnd);
}

unsigned ptk_window_get_dpi(ptk_window_t *wnd)
{
        // TODO: get dpi from x11 window or framebuffer?
        return 96;
}

int ptk_window_get_width(ptk_window_t *wnd)
{
        return linux_app.window.get_width(wnd);
}

int ptk_window_get_height(ptk_window_t *wnd)
{
        return linux_app.window.get_height(wnd);
}

void ptk_window_set_min_width(ptk_window_t *wnd, int min_width)
{
        linux_app.window.set_max_width(wnd, min_width);
}

void ptk_window_set_min_height(ptk_window_t *wnd, int min_height)
{
        linux_app.window.set_max_width(wnd, min_height);
}

void ptk_window_set_max_width(ptk_window_t *wnd, int max_width)
{
        linux_app.window.set_max_width(wnd, max_width);
}

void ptk_window_set_max_height(ptk_window_t *wnd, int max_height)
{
        linux_app.window.set_max_height(wnd, max_height);
}

void ptk_window_show(ptk_window_t *wnd)
{
        linux_app.window.show(wnd);
}

void ptk_window_hide(ptk_window_t *wnd)
{
        linux_app.window.hide(wnd);
}

ptk_window_paint_t *ptk_window_begin_paint(ptk_window_t *wnd, pd_rect_t *rect)
{
        return linux_app.window.begin_paint(wnd, rect);
}

void ptk_window_end_paint(ptk_window_t *wnd, ptk_window_paint_t *paint)
{
        linux_app.window.end_paint(wnd, paint);
}

void ptk_window_present(ptk_window_t *wnd)
{
        linux_app.window.present(wnd);
}

ptk_window_t *ptk_window_create(const wchar_t *title, int x, int y, int width,
                                int height, ptk_window_t *parent)
{
        return linux_app.app.create_window(title, x, y, width, height, parent);
}

void ptk_app_present(void)
{
        linux_app.app.present();
}

void ptk_app_exit(int exit_code)
{
        linux_app.app.exit(exit_code);
}

ptk_app_id_t ptk_get_app_id(void)
{
        return linux_app.id;
}

int ptk_app_init(const wchar_t *name)
{
        linux_app.id = PTK_APP_ID_LINUX;
#ifdef PTK_HAS_LIBX11
        ptk_x11app_driver_init(&linux_app.app);
        ptk_x11window_driver_init(&linux_app.window);
        if (linux_app.app.init(name) == 0) {
                logger_debug("[app] use engine: x11app\n");
                linux_app.id = PTK_APP_ID_LINUX_X11;
                linux_app.active = true;
                return 0;
        }
        memset(&linux_app.app, 0, sizeof(linux_app.app));
        memset(&linux_app.window, 0, sizeof(linux_app.window));
#endif
        logger_debug("[app] use engine: fbapp\n");
        ptk_fbapp_driver_init(&linux_app.app);
        ptk_fbwindow_driver_init(&linux_app.window);
        if (linux_app.app.init(name) == 0) {
                linux_app.active = true;
                ptk_linux_mouse_init();
                ptk_linux_keyboard_init();
                return 0;
        }
        return -1;
}

int ptk_app_destroy(void)
{
        if (!linux_app.active) {
                return -1;
        }
        linux_app.active = false;
        if (linux_app.id != PTK_APP_ID_LINUX_X11) {
                ptk_linux_mouse_destroy();
                ptk_linux_keyboard_destroy();
        }
        return linux_app.app.destroy();
}

#endif
