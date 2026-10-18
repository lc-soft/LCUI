/*
 * include/LCUI/widgets/tooltip.h: -- LCUI's Tooltip widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_TOOLTIP_H
#define LCUI_INCLUDE_WIDGETS_TOOLTIP_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_tooltip_side {
        UI_TOOLTIP_SIDE_TOP = 0,
        UI_TOOLTIP_SIDE_RIGHT,
        UI_TOOLTIP_SIDE_BOTTOM,
        UI_TOOLTIP_SIDE_LEFT,
} ui_tooltip_side_t;

typedef enum ui_tooltip_align {
        UI_TOOLTIP_ALIGN_CENTER = 0,
        UI_TOOLTIP_ALIGN_START,
        UI_TOOLTIP_ALIGN_END,
} ui_tooltip_align_t;

typedef struct ui_tooltip_options {
        const char *content;
        int delay;
        ui_tooltip_side_t side;
        ui_tooltip_align_t align;
        int side_offset;
        int align_offset;
} ui_tooltip_options_t;

LCUI_API void ui_tooltip_options_init(ui_tooltip_options_t *options);
LCUI_API void ui_tooltip_attach(ui_widget_t *target, const char *text);
LCUI_API void ui_tooltip_attach_ex(ui_widget_t *target,
                                   const ui_tooltip_options_t *options);

LCUI_API ui_widget_t *ui_create_tooltip(void);
LCUI_API ui_widget_t *ui_create_tooltip_trigger(void);
LCUI_API ui_widget_t *ui_create_tooltip_content(void);
LCUI_API void ui_tooltip_set_delay(ui_widget_t *w, int delay_ms);
LCUI_API int ui_tooltip_get_delay(ui_widget_t *w);
LCUI_API void ui_tooltip_show(ui_widget_t *w);
LCUI_API void ui_tooltip_hide(ui_widget_t *w);
LCUI_API void ui_tooltip_content_set_side(ui_widget_t *w,
                                          ui_tooltip_side_t side);
LCUI_API void ui_tooltip_content_set_align(ui_widget_t *w,
                                           ui_tooltip_align_t align);
LCUI_API void ui_tooltip_content_set_side_offset(ui_widget_t *w, int offset);
LCUI_API void ui_tooltip_content_set_align_offset(ui_widget_t *w, int offset);
LCUI_API bool ui_tooltip_is_open(ui_widget_t *w);
LCUI_API void ui_register_tooltip(void);
LCUI_API void ui_unregister_tooltip(void);

LCUI_END_HEADER

#endif
