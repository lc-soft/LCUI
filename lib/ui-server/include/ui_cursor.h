/*
 * lib/ui-server/include/ui_cursor.h
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_SERVER_INCLUDE_UI_CURSOR_H
#define LIB_UI_SERVER_INCLUDE_UI_CURSOR_H

#include <stdbool.h>
#include <pandagl.h>
#include <ptk.h>
#include "ui_server/common.h"

LIBUI_SERVER_BEGIN_DECLS

LIBUI_SERVER_PUBLIC void ui_cursor_refresh(void);
LIBUI_SERVER_PUBLIC bool ui_cursor_is_visible(void);
LIBUI_SERVER_PUBLIC void ui_cursor_show(void);
LIBUI_SERVER_PUBLIC void ui_cursor_hide(void);
LIBUI_SERVER_PUBLIC void ui_cursor_set_position(int x, int y);
LIBUI_SERVER_PUBLIC int ui_cursor_set_image(pd_canvas_t *image);
LIBUI_SERVER_PUBLIC void ui_cursor_get_position(int *x, int *y);
LIBUI_SERVER_PUBLIC int ui_cursor_paint(ptk_window_t *w,
                                        ptk_window_paint_t *paint);
LIBUI_SERVER_PUBLIC void ui_cursor_init(void);
LIBUI_SERVER_PUBLIC void ui_cursor_destroy(void);

LIBUI_SERVER_END_DECLS

#endif
