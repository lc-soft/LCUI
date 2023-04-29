/*
 * worker.h -- worker threading and task
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
