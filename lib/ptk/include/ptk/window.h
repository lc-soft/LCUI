/*
 * lib/ptk/include/ptk/window.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_WINDOW_H
#define PTK_INCLUDE_PLATFORM_WINDOW_H

#include "types.h"
#include "common.h"

PTK_BEGIN_DECLS

PTK_PUBLIC int ptk_screen_get_width(void);
PTK_PUBLIC int ptk_screen_get_height(void);
PTK_PUBLIC void *ptk_window_get_handle(ptk_window_t *wnd);
PTK_PUBLIC unsigned ptk_window_get_dpi(ptk_window_t *wnd);

PTK_PUBLIC ptk_window_t *ptk_find_window(void *handle);
PTK_PUBLIC ptk_window_t *ptk_window_create(const wchar_t *title, int x, int y,
					 int width, int height,
					 ptk_window_t *parent);

PTK_PUBLIC void ptk_window_close(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_destroy(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_set_fullscreen(ptk_window_t *wnd, bool fullscreen);
PTK_PUBLIC void ptk_window_set_position(ptk_window_t *wnd, int x, int y);
PTK_PUBLIC void ptk_window_set_framebuffer_size(ptk_window_t *wnd, int width,
					      int height);
PTK_PUBLIC void ptk_window_set_size(ptk_window_t *wnd, int width, int height);
PTK_PUBLIC void ptk_window_show(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_hide(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_activate(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_set_title(ptk_window_t *wnd, const wchar_t *title);
PTK_PUBLIC int ptk_window_get_width(ptk_window_t *wnd);
PTK_PUBLIC int ptk_window_get_height(ptk_window_t *wnd);
PTK_PUBLIC void ptk_window_set_min_width(ptk_window_t *wnd, int min_width);
PTK_PUBLIC void ptk_window_set_min_height(ptk_window_t *wnd, int min_height);
PTK_PUBLIC void ptk_window_set_max_width(ptk_window_t *wnd, int max_width);
PTK_PUBLIC void ptk_window_set_max_height(ptk_window_t *wnd, int max_height);
PTK_PUBLIC ptk_window_paint_t *ptk_window_begin_paint(ptk_window_t *wnd,
						    pd_rect_t *rect);
PTK_PUBLIC void ptk_window_end_paint(ptk_window_t *wnd,
				   ptk_window_paint_t *paint);
PTK_PUBLIC void ptk_window_present(ptk_window_t *wnd);

PTK_END_DECLS

#endif
