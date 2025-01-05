/*
 * lib/worker/include/worker.h: -- worker threading and task
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
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

typedef void (*worker_task_cb)(void *);
typedef struct worker worker_t;
typedef struct worker_task worker_task_t;

LIBWORKER_PUBLIC worker_t *worker_create(void);

LIBWORKER_PUBLIC worker_task_t *worker_post_task(worker_t *worker, void *data,
                                                 worker_task_cb task_cb,
                                                 worker_task_cb after_task_cb);

LIBWORKER_PUBLIC bool worker_cancel_task(worker_t *worker, worker_task_t *task);

LIBWORKER_PUBLIC bool worker_run(worker_t *worker);

LIBWORKER_PUBLIC int worker_run_async(worker_t *worker);

LIBWORKER_PUBLIC void worker_destroy(worker_t *worker);

LIBWORKER_END_DECLS

#endif
