/*
 * include/LCUI/worker.h
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_WORKER_H
#define LCUI_INCLUDE_LCUI_WORKER_H

#include "common.h"
#include <worker.h>

LCUI_BEGIN_HEADER

LCUI_API worker_task_t *lcui_worker_post_task(void *data,
                                              worker_task_cb task_cb,
                                              worker_task_cb after_task_cb);

LCUI_API worker_task_t *lcui_worker_post_async_task(
    void *data, worker_task_cb task_cb, worker_task_cb after_task_cb);

LCUI_API bool lcui_worker_cancel_async_task(worker_task_t *task);

LCUI_API bool lcui_worker_cancel_task(worker_task_t *task);

LCUI_API void lcui_worker_run(void);

LCUI_API void lcui_worker_init(void);

LCUI_API void lcui_worker_destroy(void);

LCUI_END_HEADER

#define lcui_post_task lcui_worker_post_task
#define lcui_post_async_task lcui_worker_post_async_task
#define lcui_cancel_task lcui_worker_cancel_task
#define lcui_cancel_async_task lcui_worker_cancel_async_task

#endif
