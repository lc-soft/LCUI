/*
 * include/LCUI/widgets/scrollbar.h: -- LCUI's scrollbar widget
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SCROLLBAR_H
#define LCUI_INCLUDE_WIDGETS_SCROLLBAR_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

typedef enum {
	UI_SCROLLBAR_HORIZONTAL,
	UI_SCROLLBAR_VERTICAL
} ui_scrollbar_orientation_t;

LCUI_API void ui_scrollbar_update(ui_widget_t *w);

LCUI_API void ui_scrollbar_set_orientation(ui_widget_t* w,
					 ui_scrollbar_orientation_t orientation);

LCUI_API ui_widget_t *ui_create_scrollbar(void);

LCUI_API void ui_register_scrollbar(void);

LCUI_END_HEADER

#endif
