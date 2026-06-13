/*
 * include/LCUI/app.h
 *
 * Copyright (c) 2023-2026, Liu Chao
 * <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *

 * * This file is part of LCUI, distributed under the MIT License found in the

 * * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_APP_H
#define LCUI_INCLUDE_LCUI_APP_H

#include "common.h"
#include <stdbool.h>
#include <ptk/types.h>

LCUI_BEGIN_HEADER

/*
 * Set the application identifier used for config file resolution.
 *
 * The id is used as a subdirectory name under the platform config root
 * (e.g., %APPDATA%/<id>/settings.ini on Windows,
 *       $XDG_CONFIG_HOME/<id>/settings.ini or ~/.config/<id>/settings.ini on
 * Linux).
 *
 * Allowed characters: alphanumeric, '.', '_', '-'. Must be non-empty.
 * Recommended format: reverse-domain notation, e.g. "com.example.myapp".
 *
 * Returns true on success, false if app_id is invalid.
 */
LCUI_API bool lcui_set_app_id(const char *app_id);

LCUI_API const char *lcui_get_app_id(void);

LCUI_API uint32_t lcui_get_fps(void);
LCUI_API void lcui_set_fps_cap(unsigned fps_cap);

typedef void (*lcui_frame_cb_t)(uint64_t timestamp, void *data);

LCUI_API int lcui_request_frame(lcui_frame_cb_t callback, void *data);
LCUI_API void lcui_cancel_frame(int request_id);

LCUI_END_HEADER

#endif
