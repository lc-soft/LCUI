/*
 * include/LCUI/ui.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_UI_H
#define LCUI_INCLUDE_UI_H

#include "common.h"

LCUI_BEGIN_HEADER

typedef enum {
        LCUI_DISPLAY_DEFAULT,
        LCUI_DISPLAY_WINDOWED,
        LCUI_DISPLAY_FULLSCREEN,
        LCUI_DISPLAY_SEAMLESS,
} lcui_display_t;

LCUI_API size_t lcui_ui_render(void);

LCUI_API void lcui_ui_update(void);

LCUI_API void lcui_ui_set_display(lcui_display_t mode);

LCUI_API void lcui_ui_init(void);

LCUI_API void lcui_ui_destroy(void);

LCUI_END_HEADER

#endif
