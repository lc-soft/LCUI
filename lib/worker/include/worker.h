/*
 * lib/worker/include/worker.h: -- worker threading and task
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_WORKER_INCLULDE_WORKER_H
#define LIB_WORKER_INCLULDE_WORKER_H

#include <stdbool.h>
#include "worker/common.h"

LIBWORKER_BEGIN_DECLS

typedef void (*worker_callback_t)(void *, void *);

typedef struct worker_task_t {
        worker_callback_t callback;     /**< 任务处理函数 */
        void *arg[2];                   /**< 两个参数 */
        void (*destroy_arg[2])(void *); /**< 参数的销毁函数 */
} worker_task_t;

typedef struct worker_t worker_t;

LIBWORKER_PUBLIC worker_t *worker_create(void);

LIBWORKER_PUBLIC void worker_post_task(worker_t *worker, worker_task_t *task);

LIBWORKER_PUBLIC bool worker_run_task(worker_t *worker);

LIBWORKER_PUBLIC int worker_run_async(worker_t *worker);

LIBWORKER_PUBLIC void worker_destroy(worker_t *worker);

LIBWORKER_END_DECLS

#endif
