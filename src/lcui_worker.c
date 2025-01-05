/*
 * src/lcui_worker.c
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ptk.h>
#include <yutil.h>
#include <LCUI/config.h>
#include <LCUI/worker.h>

static struct lcui_worker {
        worker_t *main_worker;
        worker_t *async_worker;
} lcui_worker;

worker_task_t *lcui_worker_post_task(void *data, worker_task_cb task_cb,
                                     worker_task_cb after_task_cb)
{
        if (!lcui_worker.main_worker) {
                return NULL;
        }
        return worker_post_task(lcui_worker.main_worker, data, task_cb,
                                after_task_cb);
}

worker_task_t *lcui_worker_post_async_task(void *data, worker_task_cb task_cb,
                                           worker_task_cb after_task_cb)
{
        return worker_post_task(lcui_worker.async_worker, data, task_cb,
                                after_task_cb);
}

bool lcui_worker_cancel_async_task(worker_task_t *task)
{
        return worker_cancel_task(lcui_worker.async_worker, task);
}

bool lcui_worker_cancel_task(worker_task_t *task)
{
        return worker_cancel_task(lcui_worker.async_worker, task);
}

void lcui_worker_run(void)
{
        worker_run(lcui_worker.main_worker);
}

void lcui_worker_init(void)
{
        lcui_worker.main_worker = worker_create();
        lcui_worker.async_worker = worker_create();
        worker_run_async(lcui_worker.async_worker);
}

void lcui_worker_destroy(void)
{
        worker_destroy(lcui_worker.main_worker);
        worker_destroy(lcui_worker.async_worker);
        lcui_worker.main_worker = NULL;
        lcui_worker.async_worker = NULL;
}
