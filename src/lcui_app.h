/*
 * src/lcui_app.h
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_LCUI_APP_H
#define LCUI_SRC_LCUI_APP_H

#include <ptk/types.h>

void lcui_app_init(void);
void lcui_app_destroy(void);
int lcui_app_process_events(ptk_process_events_option_t option);
void lcui_window_flush_show(void);
void lcui_window_apply_pending_states(void);
void lcui_window_destroy_all(void);
void lcui_app_forget_window(ptk_window_t *window);

#endif
