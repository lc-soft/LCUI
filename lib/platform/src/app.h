﻿/*
 * lib/platform/src/app.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include <platform.h>

#ifdef LIBPLAT_WIN_DESKTOP

int ime_add_win32(void);

#endif

#ifdef LIBPLAT_LINUX

#ifdef LIBPLAT_HAS_LIBX11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>

Display *x11_app_get_display(void);
Window x11_app_get_main_window(void);

void x11_app_driver_init(app_driver_t *driver);
void x11_app_window_driver_init(app_window_driver_t *driver);

int x11_clipboard_request_text(clipboard_callback_t callback, void *arg);
int x11_clipboard_set_text(const wchar_t *text, size_t len);
void x11_clipboard_init(void);
void x11_clipboard_destroy(void);

#endif

void fb_app_driver_init(app_driver_t *driver);
void fb_app_window_driver_init(app_window_driver_t *driver);

int linux_mouse_init(void);
int linux_mouse_destroy(void);
int linux_keyboard_init(void);
int linux_keyboard_destroy(void);
int ime_add_linux(void);

#endif

int app_init_engine(const wchar_t *name);
int app_destroy_engine(void);
