/*
 * include/LCUI/widgets/sidebar.h: -- LCUI's Sidebar widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SIDEBAR_H
#define LCUI_INCLUDE_WIDGETS_SIDEBAR_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_sidebar_collapsible {
        UI_SIDEBAR_COLLAPSIBLE_OFFCANVAS = 0,
        UI_SIDEBAR_COLLAPSIBLE_ICON,
        UI_SIDEBAR_COLLAPSIBLE_NONE,
} ui_sidebar_collapsible_t;

typedef enum ui_sidebar_side {
        UI_SIDEBAR_SIDE_LEFT = 0,
        UI_SIDEBAR_SIDE_RIGHT,
} ui_sidebar_side_t;

LCUI_API ui_widget_t *ui_create_sidebar_provider(void);
LCUI_API void ui_sidebar_provider_set_open(ui_widget_t *provider, bool open);
LCUI_API bool ui_sidebar_provider_get_open(ui_widget_t *provider);
LCUI_API void ui_sidebar_provider_toggle(ui_widget_t *provider);
LCUI_API ui_widget_t *ui_create_sidebar(void);
LCUI_API void ui_sidebar_set_collapsible(ui_widget_t *sidebar,
                                         ui_sidebar_collapsible_t mode);
LCUI_API ui_sidebar_collapsible_t
ui_sidebar_get_collapsible(ui_widget_t *sidebar);
LCUI_API void ui_sidebar_set_side(ui_widget_t *sidebar, ui_sidebar_side_t side);
LCUI_API ui_sidebar_side_t ui_sidebar_get_side(ui_widget_t *sidebar);
LCUI_API ui_widget_t *ui_create_sidebar_trigger(void);
LCUI_API ui_widget_t *ui_create_sidebar_rail(void);
LCUI_API ui_widget_t *ui_create_sidebar_header(void);
LCUI_API ui_widget_t *ui_create_sidebar_footer(void);
LCUI_API ui_widget_t *ui_create_sidebar_content(void);
LCUI_API ui_widget_t *ui_create_sidebar_group(void);
LCUI_API ui_widget_t *ui_create_sidebar_group_label(void);
LCUI_API ui_widget_t *ui_create_sidebar_group_action(void);
LCUI_API ui_widget_t *ui_create_sidebar_group_content(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu_item(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu_action(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu_sub(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu_sub_item(void);
LCUI_API ui_widget_t *ui_create_sidebar_menu_badge(void);
LCUI_API void ui_register_sidebar(void);

LCUI_END_HEADER

#endif
