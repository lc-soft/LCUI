/*
 * include/LCUI/widgets/input_group.h: -- LCUI's Input Group widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_INPUT_GROUP_H
#define LCUI_INCLUDE_WIDGETS_INPUT_GROUP_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_input_group_addon_align {
        UI_INPUT_GROUP_ADDON_INLINE_START = 0,
        UI_INPUT_GROUP_ADDON_INLINE_END,
        UI_INPUT_GROUP_ADDON_BLOCK_START,
        UI_INPUT_GROUP_ADDON_BLOCK_END,
} ui_input_group_addon_align_t;

LCUI_API ui_widget_t *ui_create_input_group(void);
LCUI_API ui_widget_t *ui_create_input_group_addon(void);
LCUI_API void ui_input_group_addon_set_align(
    ui_widget_t *addon, ui_input_group_addon_align_t align);
LCUI_API ui_input_group_addon_align_t
ui_input_group_addon_get_align(ui_widget_t *addon);

LCUI_API ui_widget_t *ui_create_input_group_text(void);
LCUI_API ui_widget_t *ui_create_input_group_kbd(void);
LCUI_API ui_widget_t *ui_create_input_group_input(void);
LCUI_API ui_widget_t *ui_create_input_group_textarea(void);
LCUI_API ui_widget_t *ui_create_input_group_button(void);
LCUI_API void ui_input_group_mark_control(ui_widget_t *control);
LCUI_API void ui_register_input_group(void);

LCUI_END_HEADER

#endif
