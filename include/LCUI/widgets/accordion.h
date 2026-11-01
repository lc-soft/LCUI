/*
 * include/LCUI/widgets/accordion.h: -- LCUI's Accordion widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ACCORDION_H
#define LCUI_INCLUDE_WIDGETS_ACCORDION_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_accordion(void);
LCUI_API ui_widget_t *ui_create_accordion_item(void);
LCUI_API ui_widget_t *ui_create_accordion_trigger(void);
LCUI_API ui_widget_t *ui_create_accordion_content(void);

LCUI_API void ui_accordion_set_multiple(ui_widget_t *accordion, bool multiple);
LCUI_API bool ui_accordion_get_multiple(ui_widget_t *accordion);
LCUI_API void ui_accordion_set_value(ui_widget_t *accordion,
                                     const char *values);
LCUI_API const char *ui_accordion_get_value(ui_widget_t *accordion);
LCUI_API bool ui_accordion_is_open(ui_widget_t *accordion, const char *value);

LCUI_API void ui_accordion_item_set_value(ui_widget_t *item, const char *value);
LCUI_API void ui_accordion_item_set_open(ui_widget_t *item, bool open);
LCUI_API bool ui_accordion_item_get_open(ui_widget_t *item);

LCUI_API void ui_register_accordion(void);

LCUI_END_HEADER

#endif
