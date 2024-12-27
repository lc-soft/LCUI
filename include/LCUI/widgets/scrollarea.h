/*
 * include/LCUI/widgets/scrollarea.h
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SCROLLAREA_H
#define LCUI_INCLUDE_WIDGETS_SCROLLAREA_H

#include <LCUI/common.h>
#include <ui.h>

typedef enum {
        UI_SCROLLAREA_AUTO,
        UI_SCROLLAREA_VERTICAL,
        UI_SCROLLAREA_HORIZONTAL,
} ui_scrollarea_direction_t;

LCUI_BEGIN_HEADER

LCUI_API void ui_scrollarea_set_wheel_scroll_direction(
    ui_widget_t *w, ui_scrollarea_direction_t direction);

LCUI_API void ui_scrollarea_update(ui_widget_t *w);

LCUI_API void ui_register_scrollarea(void);

LCUI_API ui_widget_t *ui_create_scrollarea(void);

LCUI_API ui_widget_t *ui_create_scrollarea_content(void);

LCUI_API void ui_scrollarea_set_scroll_top(ui_widget_t *w,
                                                       float value);

LCUI_API void ui_scrollarea_set_scroll_left(ui_widget_t *w,
                                                        float value);

LCUI_API float ui_scrollarea_get_scroll_top(ui_widget_t *w);

LCUI_API float ui_scrollarea_get_scroll_left(ui_widget_t *w);

LCUI_API float ui_scrollarea_get_scroll_width(ui_widget_t *w);

LCUI_API float ui_scrollarea_get_scroll_height(ui_widget_t *w);

LCUI_END_HEADER

#endif
