/*
 * lib/ui-server/include/ui_server.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_SERVER_INCLUDE_UI_SERVER_H
#define LIB_UI_SERVER_INCLUDE_UI_SERVER_H

#include "ui_server/common.h"
#include <ui.h>
#include <platform.h>

LIBUI_SERVER_BEGIN_DECLS

LIBUI_SERVER_PUBLIC ui_widget_t *ui_server_get_widget(app_window_t *window);
LIBUI_SERVER_PUBLIC app_window_t *ui_server_get_window(ui_widget_t *widget);
LIBUI_SERVER_PUBLIC int ui_server_disconnect(ui_widget_t *widget, app_window_t *window);
LIBUI_SERVER_PUBLIC void ui_server_connect(ui_widget_t *widget, app_window_t *window);
LIBUI_SERVER_PUBLIC size_t ui_server_render(void);
LIBUI_SERVER_PUBLIC void ui_server_present(void);
LIBUI_SERVER_PUBLIC void ui_server_init(void);
LIBUI_SERVER_PUBLIC void ui_server_set_threads(int threads);
LIBUI_SERVER_PUBLIC void ui_server_set_paint_flashing_enabled(bool enabled);
LIBUI_SERVER_PUBLIC void ui_server_destroy(void);

LIBUI_SERVER_END_DECLS

#endif
