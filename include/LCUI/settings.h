/*
 * include/LCUI/settings.h
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_SETTINGS_H
#define LCUI_INCLUDE_LCUI_SETTINGS_H

#include "common.h"

#define LCUI_MAX_FRAMES_PER_SEC 120
#define LCUI_MAX_FRAME_MSEC ((int)(1000.0 / LCUI_MAX_FRAMES_PER_SEC + 0.5))

LCUI_BEGIN_HEADER

typedef struct lcui_settings {
        int frame_rate_cap;
        int parallel_rendering_threads;
        bool paint_flashing;
} lcui_settings_t;

/* Initialize settings with the current global settings. */
LCUI_API void lcui_get_settings(lcui_settings_t *settings);

/* Update global settings with the given input. */
LCUI_API void lcui_apply_settings(lcui_settings_t *settings);

/* Reset global settings to their defaults. */
LCUI_API void lcui_reset_settings(void);

#endif
