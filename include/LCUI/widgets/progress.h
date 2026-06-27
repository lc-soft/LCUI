/*
 * include/LCUI/widgets/progress.h: -- LCUI's Progress widget
 *
 * Copyright (c) 2024-2026, Liu chao <lc-soft@live.cn> All rights reserved.

 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI,
 * distributed under the MIT License found in the
 * LICENSE.TXT file in the
 * root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_PROGRESS_H
#define LCUI_INCLUDE_WIDGETS_PROGRESS_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_progress(void);

LCUI_API void ui_progress_set_value(ui_widget_t *w, float value);

LCUI_API float ui_progress_get_value(ui_widget_t *w);

LCUI_API void ui_register_progress(void);

LCUI_END_HEADER

#endif
