/*
 * include/LCUI/settings/window_state.h
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_SETTINGS_WINDOW_STATE_H
#define LCUI_INCLUDE_LCUI_SETTINGS_WINDOW_STATE_H

#include <LCUI/common.h>
#include <stdbool.h>
#include <ui/types.h>

#define LCUI_WINDOW_STATE_SIZE_MIN 1
#define LCUI_WINDOW_STATE_SIZE_MAX 32768

LCUI_BEGIN_HEADER

typedef struct lcui_window_state {
        int x;
        int y;
        int width;
        int height;
        bool maximized;
} lcui_window_state_t;

LCUI_API bool lcui_get_window_state(const char *id, lcui_window_state_t *out);
LCUI_API bool lcui_set_window_state(const char *id,
                                    const lcui_window_state_t *in);
LCUI_API bool lcui_bind_window_state(ui_widget_t *widget, const char *id);

LCUI_END_HEADER

#endif
