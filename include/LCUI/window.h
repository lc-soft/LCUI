/*
 * include/LCUI/window.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_WINDOW_H
#define LCUI_INCLUDE_LCUI_WINDOW_H

#include <LCUI/common.h>
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>
#include <ui/types.h>
#include <ptk/types.h>

/* x and y are physical client-area pixels. Both set to
 * LCUI_WINDOW_POS_AUTO (0) means the framework centers the window. */
#define LCUI_WINDOW_POS_AUTO 0

LCUI_BEGIN_HEADER

typedef struct lcui_window lcui_window_t;

typedef struct lcui_window_options {
        const wchar_t *title;
        const char *id;
        int width, height;
        int x, y;
        bool maximized;
        bool fullscreen;
        /* Optional icon file. NULL uses the global default icon. */
        const char *icon;
} lcui_window_options_t;

LCUI_API lcui_window_t *lcui_window_create(const lcui_window_options_t *opt);
LCUI_API lcui_window_t *lcui_window_create_for_widget(
    ui_widget_t *widget, const lcui_window_options_t *opt);
LCUI_API void lcui_window_destroy(lcui_window_t *w);

LCUI_API bool lcui_window_bind_widget(lcui_window_t *w, ui_widget_t *widget);
LCUI_API ui_widget_t *lcui_window_get_widget(lcui_window_t *w);

LCUI_API void lcui_window_show(lcui_window_t *w);
LCUI_API void lcui_window_hide(lcui_window_t *w);

LCUI_API void lcui_set_quit_on_last_window(bool enable);

LCUI_API void lcui_window_get_size(lcui_window_t *w, int *cw, int *ch);
LCUI_API void lcui_window_get_position(lcui_window_t *w, int *x, int *y);

LCUI_API void lcui_window_set_size(lcui_window_t *w, int cw, int ch);
LCUI_API void lcui_window_set_position(lcui_window_t *w, int x, int y);
LCUI_API void lcui_window_set_title(lcui_window_t *w, const wchar_t *title);

/* Icon APIs must be called on the UI thread. A NULL file clears the custom
 * icon and falls back to the default icon. */
LCUI_API bool lcui_window_set_icon(lcui_window_t *w, const char *file);
/* NULL data or zero size clears the custom icon. */
LCUI_API bool lcui_window_set_icon_from_data(lcui_window_t *w, const void *data,
                                             size_t size);

/* Sets the global default window icon. NULL clears the override and falls
 * back to the built-in icon. */
LCUI_API bool lcui_set_default_window_icon(const char *file);
LCUI_API bool lcui_set_default_window_icon_from_data(const void *data,
                                                     size_t size);
LCUI_API void lcui_set_default_window_icon_enabled(bool enable);

LCUI_API void lcui_window_set_maximized(lcui_window_t *w, bool maximized);
LCUI_API void lcui_window_set_fullscreen(lcui_window_t *w, bool fullscreen);

typedef void (*lcui_window_close_cb_t)(lcui_window_t *w, void *arg);

LCUI_API void lcui_window_close(lcui_window_t *w);
LCUI_API void lcui_window_on_close(lcui_window_t *w, lcui_window_close_cb_t cb,
                                   void *arg);

LCUI_API ptk_window_t *lcui_window_get_handle(lcui_window_t *w);

LCUI_END_HEADER

#endif
