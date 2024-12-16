/*
 * include/LCUI/app.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_APP_H
#define LCUI_INCLUDE_LCUI_APP_H

#include "common.h"
#include <ptk.h>
#include <worker.h>

#define LCUI_MAX_FRAMES_PER_SEC 120
#define LCUI_MAX_FRAME_MSEC ((int)(1000.0 / LCUI_MAX_FRAMES_PER_SEC + 0.5))

LCUI_BEGIN_HEADER

// Settings

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

// Tasks

LCUI_API worker_task_t *lcui_post_task(void *data, worker_task_cb task_cb,
                                       worker_task_cb after_task_cb);
LCUI_API worker_task_t *lcui_post_async_task(void *data, worker_task_cb task_cb,
                                             worker_task_cb after_task_cb);

bool lcui_cancel_async_task(worker_task_t *task);
bool lcui_cancel_task(worker_task_t *task);

// UI

typedef enum lcui_display_mode_t {
        LCUI_DISPLAY_MODE_DEFAULT,
        LCUI_DISPLAY_MODE_WINDOWED,
        LCUI_DISPLAY_MODE_FULLSCREEN,
        LCUI_DISPLAY_MODE_SEAMLESS,
} lcui_display_mode_t;

LCUI_API void lcui_init_ui(void);
LCUI_API void lcui_destroy_ui(void);
LCUI_API void lcui_update_ui(void);
LCUI_API size_t lcui_render_ui(void);
LCUI_API void lcui_preset_ui(void);
LCUI_API void lcui_dispatch_ui_event(ptk_event_t *app_event);
LCUI_API void lcui_set_ui_display_mode(lcui_display_mode_t mode);

// Event

LCUI_API int lcui_get_event(ptk_event_t *e);
LCUI_API int lcui_process_event(ptk_event_t *e);
LCUI_API int lcui_process_events(ptk_process_events_option_t option);

INLINE int lcui_process_all_events(void)
{
        return lcui_process_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
}

// Base

LCUI_API uint32_t lcui_get_fps(void);

LCUI_API void lcui_set_frame_rate_cap(unsigned rate_cap);

LCUI_API void lcui_init_app(void);
LCUI_API void lcui_destroy_app(void);

/** 初始化 LCUI 各项功能 */
LCUI_API void lcui_init(void);

LCUI_API int lcui_main(void);

LCUI_API int lcui_run(void);

/** 获取LCUI的版本 */
LCUI_API const char *lcui_get_version(void);

/** 释放LCUI占用的资源 */
LCUI_API void lcui_destroy(void);

/** 退出LCUI，释放LCUI占用的资源 */
LCUI_API void lcui_quit(void);

/** 退出 LCUI，并设置退出码 */
LCUI_API void lcui_exit(int code);

LCUI_END_HEADER

#endif
