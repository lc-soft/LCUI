/*
 * lib/ui-widgets/include/ui_widgets/textcaret.h: -- textcaret widget, used in textinput.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_TEXTCARET_H
#define LIB_UI_WIDGETS_INCLUDE_TEXTCARET_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_PUBLIC void ui_textcaret_refresh(ui_widget_t* widget);

LIBUI_WIDGETS_PUBLIC void ui_textcaret_set_visible(ui_widget_t* widget, bool visible);

/** 设置闪烁的时间间隔 */
LIBUI_WIDGETS_PUBLIC void ui_textcaret_set_blink_time(ui_widget_t* widget, unsigned int n_ms);

LIBUI_WIDGETS_PUBLIC void ui_register_textcaret(void);

#endif
