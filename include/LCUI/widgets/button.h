/*
 * include/LCUI/widgets/button.h: -- LCUI‘s Button widget
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_BUTTON_H
#define LCUI_INCLUDE_WIDGETS_BUTTON_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

LCUI_API void ui_button_set_text_w(ui_widget_t* w, const wchar_t *wstr);

LCUI_API void ui_button_set_text(ui_widget_t* w, const char *str);

LCUI_API void ui_register_button(void);

LCUI_END_HEADER

#endif
