/*
 * include/LCUI/widgets/placement.h: -- Shared placement types
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_PLACEMENT_H
#define LCUI_INCLUDE_WIDGETS_PLACEMENT_H

#include <LCUI/common.h>

LCUI_BEGIN_HEADER

typedef enum ui_side {
        UI_SIDE_TOP = 0,
        UI_SIDE_RIGHT,
        UI_SIDE_BOTTOM,
        UI_SIDE_LEFT,
} ui_side_t;

typedef enum ui_align {
        UI_ALIGN_CENTER = 0,
        UI_ALIGN_START,
        UI_ALIGN_END,
} ui_align_t;

LCUI_END_HEADER

#endif
