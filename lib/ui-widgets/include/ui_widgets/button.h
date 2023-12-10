/*
 * lib/ui-widgets/include/ui_widgets/button.h: -- LCUI‘s Button widget
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_BUTTON_H
#define LIB_UI_WIDGETS_INCLUDE_BUTTON_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

LIBUI_WIDGETS_PUBLIC void ui_button_set_text_w(ui_widget_t* w, const wchar_t *wstr);

LIBUI_WIDGETS_PUBLIC void ui_button_set_text(ui_widget_t* w, const char *str);

LIBUI_WIDGETS_PUBLIC void ui_register_button(void);

LIBUI_WIDGETS_END_DECLS

#endif
