/*
 * include/LCUI/widgets/textcaret.h: -- textcaret widget, used in textinput.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_TEXTCARET_H
#define LCUI_INCLUDE_WIDGETS_TEXTCARET_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_API void ui_textcaret_refresh(ui_widget_t* widget);

LCUI_API void ui_textcaret_set_visible(ui_widget_t* widget, bool visible);

/** 设置闪烁的时间间隔 */
LCUI_API void ui_textcaret_set_blink_time(ui_widget_t* widget, unsigned int n_ms);

LCUI_API void ui_register_textcaret(void);

#endif
