/*
 * lib/ui-widgets/include/ui_widgets/scrollarea.h
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_SCROLLAREA_H
#define LIB_UI_WIDGETS_INCLUDE_SCROLLAREA_H

#include <ui.h>
#include "ui_widgets/common.h"

typedef enum {
        UI_SCROLLAREA_AUTO,
        UI_SCROLLAREA_VERTICAL,
        UI_SCROLLAREA_HORIZONTAL,
} ui_scrollarea_direction_t;

LIBUI_WIDGETS_BEGIN_DECLS

LIBUI_WIDGETS_PUBLIC void ui_scrollarea_set_wheel_scroll_direction(
    ui_widget_t *w, ui_scrollarea_direction_t direction);

LIBUI_WIDGETS_PUBLIC void ui_scrollarea_update(ui_widget_t *w);

LIBUI_WIDGETS_PUBLIC void ui_register_scrollarea(void);

LIBUI_WIDGETS_PUBLIC ui_widget_t *ui_create_scrollarea(void);

LIBUI_WIDGETS_PUBLIC ui_widget_t *ui_create_scrollarea_content(void);

LIBUI_WIDGETS_PUBLIC void ui_scrollarea_set_scroll_top(ui_widget_t *w,
                                                       float value);

LIBUI_WIDGETS_PUBLIC void ui_scrollarea_set_scroll_left(ui_widget_t *w,
                                                        float value);

LIBUI_WIDGETS_PUBLIC float ui_scrollarea_get_scroll_top(ui_widget_t *w);

LIBUI_WIDGETS_PUBLIC float ui_scrollarea_get_scroll_left(ui_widget_t *w);

LIBUI_WIDGETS_PUBLIC float ui_scrollarea_get_scroll_width(ui_widget_t *w);

LIBUI_WIDGETS_PUBLIC float ui_scrollarea_get_scroll_height(ui_widget_t *w);

LIBUI_WIDGETS_END_DECLS

#endif
