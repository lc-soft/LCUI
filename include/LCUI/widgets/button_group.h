/*
 * include/LCUI/widgets/button_group.h: -- LCUI's Button Group widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_BUTTON_GROUP_H
#define LCUI_INCLUDE_WIDGETS_BUTTON_GROUP_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_button_group_orientation {
        UI_BUTTON_GROUP_HORIZONTAL = 0,
        UI_BUTTON_GROUP_VERTICAL,
} ui_button_group_orientation_t;

typedef enum ui_button_group_separator_orientation {
        UI_BUTTON_GROUP_SEPARATOR_VERTICAL = 0,
        UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL,
} ui_button_group_separator_orientation_t;

LCUI_API ui_widget_t *ui_create_button_group(void);
LCUI_API void ui_button_group_set_orientation(
    ui_widget_t *group, ui_button_group_orientation_t orientation);
LCUI_API ui_button_group_orientation_t
ui_button_group_get_orientation(ui_widget_t *group);

LCUI_API ui_widget_t *ui_create_button_group_text(void);

LCUI_API ui_widget_t *ui_create_button_group_separator(void);
LCUI_API void ui_button_group_separator_set_orientation(
    ui_widget_t *separator,
    ui_button_group_separator_orientation_t orientation);
LCUI_API ui_button_group_separator_orientation_t
ui_button_group_separator_get_orientation(ui_widget_t *separator);

LCUI_API void ui_register_button_group(void);

LCUI_END_HEADER

#endif
