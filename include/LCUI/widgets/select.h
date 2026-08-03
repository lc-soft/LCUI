/*
 * include/LCUI/widgets/select.h: -- Select widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SELECT_H
#define LCUI_INCLUDE_WIDGETS_SELECT_H

#include <LCUI/common.h>
#include <LCUI/widgets/portal.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef struct ui_select ui_select_t;

LCUI_API ui_widget_t *ui_create_select(void);
LCUI_API ui_select_t *ui_select_get_data(ui_widget_t *select);
LCUI_API void ui_select_build_children(ui_widget_t *select);
LCUI_API void ui_select_set_placeholder(ui_widget_t *select,
                                        const char *placeholder);
LCUI_API const char *ui_select_get_placeholder(ui_widget_t *select);
LCUI_API void ui_select_set_value(ui_widget_t *select, const char *value);
LCUI_API const char *ui_select_get_value(ui_widget_t *select);
LCUI_API void ui_select_open(ui_widget_t *select);
LCUI_API void ui_select_close(ui_widget_t *select);
LCUI_API ui_widget_t *ui_create_select_trigger(void);
LCUI_API ui_widget_t *ui_create_select_value(void);
LCUI_API ui_widget_t *ui_create_select_arrow(void);
LCUI_API ui_widget_t *ui_create_select_content(void);
LCUI_API ui_widget_t *ui_create_select_item(const char *value);
LCUI_API int ui_select_append_item(ui_widget_t *select, ui_widget_t *item);
LCUI_API ui_widget_t *ui_select_add_item(ui_widget_t *select, const char *label,
                                         const char *value);
LCUI_API void ui_select_clear_items(ui_widget_t *select);
LCUI_API void ui_register_select(void);
LCUI_API void ui_unregister_select();

LCUI_END_HEADER

#endif
