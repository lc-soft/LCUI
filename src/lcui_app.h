/*
 * src/lcui_app.h
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_LCUI_APP_H
#define LCUI_SRC_LCUI_APP_H

#include <ptk/types.h>

void lcui_app_init(void);
void lcui_app_destroy(void);
int lcui_app_process_events(ptk_process_events_option_t option);

#endif