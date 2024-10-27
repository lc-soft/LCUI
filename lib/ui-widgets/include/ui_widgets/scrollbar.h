/*
 * lib/ui-widgets/include/ui_widgets/scrollbar.h: -- LCUI's scrollbar widget
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_SCROLLBAR_H
#define LIB_UI_WIDGETS_INCLUDE_SCROLLBAR_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

typedef enum {
	UI_SCROLLBAR_HORIZONTAL,
	UI_SCROLLBAR_VERTICAL
} ui_scrollbar_orientation_t;

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_update(ui_widget_t *w);

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_set_orientation(ui_widget_t* w,
					 ui_scrollbar_orientation_t orientation);

LIBUI_WIDGETS_PUBLIC ui_widget_t *ui_create_scrollbar(void);

LIBUI_WIDGETS_PUBLIC void ui_register_scrollbar(void);

LIBUI_WIDGETS_END_DECLS

#endif
