/*
 * include/LCUI/widgets/dropdown_menu.h: -- LCUI's DropdownMenu widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_DROPDOWN_MENU_H
#define LCUI_INCLUDE_WIDGETS_DROPDOWN_MENU_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_dropdown_menu_item_variant {
        UI_DROPDOWN_MENU_ITEM_DEFAULT = 0,
        UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE,
} ui_dropdown_menu_item_variant_t;

LCUI_API ui_widget_t *ui_create_dropdown_menu(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_trigger(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_content(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_portal(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_group(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_label(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_item(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_checkbox_item(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_radio_group(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_radio_item(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_separator(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_shortcut(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_sub(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_sub_trigger(void);
LCUI_API ui_widget_t *ui_create_dropdown_menu_sub_content(void);

LCUI_API void ui_dropdown_menu_set_open(ui_widget_t *menu, bool open);
LCUI_API bool ui_dropdown_menu_is_open(ui_widget_t *menu);
LCUI_API void ui_dropdown_menu_item_set_variant(
    ui_widget_t *item, ui_dropdown_menu_item_variant_t variant);
LCUI_API void ui_register_dropdown_menu(void);
LCUI_API void ui_unregister_dropdown_menu(void);

static inline void ui_dropdown_menu_open(ui_widget_t *menu)
{
        ui_dropdown_menu_set_open(menu, true);
}

static inline void ui_dropdown_menu_close(ui_widget_t *menu)
{
        ui_dropdown_menu_set_open(menu, false);
}

LCUI_END_HEADER

#endif
