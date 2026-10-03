/*
 * include/LCUI/widgets/icon.h: -- LCUI's Icon widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT LICENSE file in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ICON_H
#define LCUI_INCLUDE_WIDGETS_ICON_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_icon_library {
        UI_ICON_LIBRARY_FLUENT,
        UI_ICON_LIBRARY_LUCIDE,
} ui_icon_library_t;

LCUI_API ui_widget_t *ui_create_icon(void);
LCUI_API ui_widget_t *ui_create_fluent_icon(const char *name);
LCUI_API void ui_icon_set_library(ui_widget_t *icon, ui_icon_library_t library);
LCUI_API void ui_icon_set_name(ui_widget_t *icon, const char *name);
LCUI_API void ui_icon_set_size(ui_widget_t *icon, int size);
LCUI_API void ui_icon_set_filled(ui_widget_t *icon, bool filled);
LCUI_API void ui_register_icon(void);

LCUI_END_HEADER

#endif
