/*
 * lib/ui-widgets/include/ui_widgets/anchor.h: to <a> element in HTML.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_ANCHOR_H
#define LIB_UI_WIDGETS_INCLUDE_ANCHOR_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

LIBUI_WIDGETS_PUBLIC void ui_anchor_open(ui_widget_t* w);

LIBUI_WIDGETS_PUBLIC void ui_register_anchor(void);

LIBUI_WIDGETS_PUBLIC void ui_unregister_anchor(void);

LIBUI_WIDGETS_END_DECLS

#endif
