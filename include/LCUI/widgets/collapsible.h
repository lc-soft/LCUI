/*
 * include/LCUI/widgets/collapsible.h: -- LCUI's Collapsible widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_COLLAPSIBLE_H
#define LCUI_INCLUDE_WIDGETS_COLLAPSIBLE_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef struct ui_collapsible ui_collapsible_t;

LCUI_API ui_widget_t *ui_create_collapsible(void);
LCUI_API ui_widget_t *ui_create_collapsible_trigger(void);
LCUI_API ui_widget_t *ui_create_collapsible_content(void);
LCUI_API void ui_collapsible_set_open(ui_widget_t *collapsible, bool open);
LCUI_API bool ui_collapsible_get_open(ui_widget_t *collapsible);
LCUI_API void ui_register_collapsible(void);

LCUI_END_HEADER

#endif
