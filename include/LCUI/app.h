/*
 * include/LCUI/app.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_APP_H
#define LCUI_INCLUDE_LCUI_APP_H

#include "common.h"
#include <stdbool.h>
#include <ptk/types.h>

LCUI_BEGIN_HEADER

LCUI_API bool lcui_set_app_id(const char *app_id);
LCUI_API const char *lcui_get_app_id(void);

LCUI_API uint32_t lcui_get_fps(void);
LCUI_API void lcui_set_fps_cap(unsigned fps_cap);

typedef void (*lcui_frame_cb_t)(uint64_t timestamp, void *data);

LCUI_API int lcui_request_frame(lcui_frame_cb_t callback, void *data);
LCUI_API void lcui_cancel_frame(int request_id);

LCUI_END_HEADER

#endif
