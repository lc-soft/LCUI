/*
 * include/LCUI/settings/rendering.h
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_SETTINGS_RENDERING_H
#define LCUI_INCLUDE_LCUI_SETTINGS_RENDERING_H

#include <LCUI/common.h>
#include <stdbool.h>

#define LCUI_RENDERING_FPS_CAP_MIN 30
#define LCUI_RENDERING_FPS_CAP_MAX 480
#define LCUI_RENDERING_PARALLEL_THREADS_MIN 1
#define LCUI_RENDERING_PARALLEL_THREADS_MAX 64
#define LCUI_DEFAULT_FPS_CAP 120
#define LCUI_DEFAULT_PARALLEL_THREADS 4
#define LCUI_MAX_FRAMES_PER_SEC LCUI_DEFAULT_FPS_CAP
#define LCUI_MAX_FRAME_MSEC ((int)(1000.0 / LCUI_MAX_FRAMES_PER_SEC + 0.5))

LCUI_BEGIN_HEADER

typedef struct lcui_rendering_settings {
        int fps_cap;
        int parallel_threads;
        bool paint_flashing;
} lcui_rendering_settings_t;

LCUI_API bool lcui_get_rendering(lcui_rendering_settings_t *out);
LCUI_API bool lcui_set_rendering(const lcui_rendering_settings_t *in);

LCUI_END_HEADER

#endif
