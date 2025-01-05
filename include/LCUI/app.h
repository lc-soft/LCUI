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
#include <ptk/types.h>

LCUI_BEGIN_HEADER

LCUI_API uint32_t lcui_app_get_fps(void);
LCUI_API void lcui_app_set_frame_rate_cap(unsigned rate_cap);
LCUI_API int lcui_app_process_events(ptk_process_events_option_t option);
LCUI_API void lcui_app_init(void);
LCUI_API void lcui_app_destroy(void);

LCUI_END_HEADER

#endif
