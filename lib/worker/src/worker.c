/*
 * lib/worker/src/worker.c: -- worker threading and task
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdlib.h>
#include <worker.h>
#include <yutil.h>
#include <thread.h>

struct worker_t {
        bool active;          /**< 是否处于活动状态 */
        list_t tasks;         /**< 任务队列 */
        thread_mutex_t mutex; /**< 互斥锁 */
        thread_cond_t cond;   /**< 条件变量 */
        thread_t thread;      /**< 所在的线程 */
};

static void worker_task_destroy(worker_task_t *task)
{
        if (task->destroy_arg[0] && task->arg[0]) {
                task->destroy_arg[0](task->arg[0]);
        }
        if (task->destroy_arg[1] && task->arg[1]) {
                task->destroy_arg[1](task->arg[1]);
        }
}

static int worker_task_run(worker_task_t *task)
{
        if (task && task->callback) {
                task->callback(task->arg[0], task->arg[1]);
                return 0;
        }
        return -1;
}

worker_t *worker_create(void)
{
        worker_t *worker = malloc(sizeof(worker_t));
        thread_mutex_init(&worker->mutex);
        thread_cond_init(&worker->cond);
        list_create(&worker->tasks);
        worker->active = FALSE;
        worker->thread = 0;
        return worker;
}

void worker_post_task(worker_t *worker, worker_task_t *task)
{
        worker_task_t *newtask;
        newtask = malloc(sizeof(worker_task_t));
        *newtask = *task;
        thread_mutex_lock(&worker->mutex);
        list_append(&worker->tasks, newtask);
        thread_cond_signal(&worker->cond);
        thread_mutex_unlock(&worker->mutex);
}

worker_task_t *worker_get_task(worker_t *worker)
{
        worker_task_t *task;
        list_node_t *node;

        node = list_get_node(&worker->tasks, 0);
        if (!node) {
                return NULL;
        }
        task = node->data;
        list_unlink(&worker->tasks, node);
        free(node);
        return task;
}

bool worker_run_task(worker_t *worker)
{
        worker_task_t *task;

        thread_mutex_lock(&worker->mutex);
        task = worker_get_task(worker);
        thread_mutex_unlock(&worker->mutex);
        if (!task) {
                return FALSE;
        }
        worker_task_run(task);
        worker_task_destroy(task);
        free(task);
        return TRUE;
}

static void worker_on_destroy_task(void *arg)
{
        worker_task_destroy(arg);
        free(arg);
}

static void worker_do_destroy(worker_t *worker)
{
        list_destroy(&worker->tasks, worker_on_destroy_task);
        thread_mutex_unlock(&worker->mutex);
        thread_mutex_destroy(&worker->mutex);
        thread_cond_destroy(&worker->cond);
        free(worker);
}

static void worker_thread(void *arg)
{
        worker_task_t *task;
        worker_t *worker = arg;

        thread_mutex_lock(&worker->mutex);
        while (worker->active) {
                task = worker_get_task(worker);
                if (task) {
                        thread_mutex_unlock(&worker->mutex);
                        worker_task_run(task);
                        worker_task_destroy(task);
                        free(task);
                        thread_mutex_lock(&worker->mutex);
                        continue;
                }
                if (worker->active) {
                        thread_cond_wait(&worker->cond, &worker->mutex);
                }
        }
        worker_do_destroy(worker);
        thread_exit(NULL);
}

int worker_run_async(worker_t *worker)
{
        if (worker->thread != 0) {
                return -EEXIST;
        }
        worker->active = TRUE;
        thread_create(&worker->thread, worker_thread, worker);
        return 0;
}

void worker_destroy(worker_t *worker)
{
        thread_t thread = worker->thread;

        if (worker->active) {
                thread_mutex_lock(&worker->mutex);
                worker->active = FALSE;
                thread_cond_signal(&worker->cond);
                thread_mutex_unlock(&worker->mutex);
                thread_join(thread, NULL);
                return;
        }
        worker_do_destroy(worker);
}
