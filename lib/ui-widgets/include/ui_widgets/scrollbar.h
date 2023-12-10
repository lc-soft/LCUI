/*
 * lib/ui-widgets/include/ui_widgets/scrollbar.h: -- LCUI's scrollbar widget
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
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

typedef enum ui_scrollbar_direction_t {
	UI_SCROLLBAR_HORIZONTAL,
	UI_SCROLLBAR_VERTICAL
} ui_scrollbar_direction_t;

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_bind_container(ui_widget_t* w,
					  ui_widget_t* container);

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_bind_target(ui_widget_t* w, ui_widget_t* target);

/** 获取滚动条的位置 */
LIBUI_WIDGETS_PUBLIC float ui_scrollbar_get_position(ui_widget_t* w);

/** 将与滚动条绑定的内容滚动至指定位置 */
LIBUI_WIDGETS_PUBLIC float ui_scrollbar_set_position(ui_widget_t* w, float pos);

/** 设置滚动条的方向 */
LIBUI_WIDGETS_PUBLIC void ui_scrollbar_set_direction(ui_widget_t* w,
					 ui_scrollbar_direction_t direction);

LIBUI_WIDGETS_PUBLIC void ui_register_scrollbar(void);

LIBUI_WIDGETS_END_DECLS

#endif
