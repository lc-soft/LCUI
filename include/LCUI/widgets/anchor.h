/*
 * include/LCUI/widgets/anchor.h: to <a> element in HTML.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ANCHOR_H
#define LCUI_INCLUDE_WIDGETS_ANCHOR_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

LCUI_API void ui_anchor_open(ui_widget_t* w);

LCUI_API void ui_register_anchor(void);

LCUI_API void ui_unregister_anchor(void);

LCUI_END_HEADER

#endif
