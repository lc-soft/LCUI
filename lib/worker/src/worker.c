/*
 * lib/worker/src/worker.c: -- worker threading and task
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
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

struct worker {
        bool active;
        list_t tasks;
        thread_mutex_t mutex;
        thread_cond_t cond;
        thread_t thread;
};

struct worker_task {
        worker_task_cb task_cb;
        worker_task_cb after_task_cb;
        void *data;
};

static void worker_task_destroy(worker_task_t *task)
{
        if (task->after_task_cb) {
                task->after_task_cb(task->data);
        }
        free(task);
}

static int worker_task_run(worker_task_t *task)
{
        if (task && task->task_cb) {
                task->task_cb(task->data);
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
        worker->active = false;
        worker->thread = 0;
        return worker;
}

void worker_destroy(worker_t *worker)
{
        thread_t thread = worker->thread;

        if (worker->active) {
                thread_mutex_lock(&worker->mutex);
                worker->active = false;
                thread_cond_signal(&worker->cond);
                thread_mutex_unlock(&worker->mutex);
                thread_join(thread, NULL);
        }
        list_destroy(&worker->tasks, (list_item_destructor_t)worker_task_destroy);
        thread_mutex_unlock(&worker->mutex);
        thread_mutex_destroy(&worker->mutex);
        thread_cond_destroy(&worker->cond);
        free(worker);
}

worker_task_t *worker_post_task(worker_t *worker, void *data,
                                worker_task_cb task_cb,
                                worker_task_cb after_task_cb)
{
        worker_task_t *task;

        task = malloc(sizeof(worker_task_t));
        task->data = data;
        task->task_cb = task_cb;
        task->after_task_cb = after_task_cb;
        thread_mutex_lock(&worker->mutex);
        list_append(&worker->tasks, task);
        thread_cond_signal(&worker->cond);
        thread_mutex_unlock(&worker->mutex);
        return task;
}

static worker_task_t *worker_get_task(worker_t *worker)
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

bool worker_run(worker_t *worker)
{
        worker_task_t *task;

        thread_mutex_lock(&worker->mutex);
        task = worker_get_task(worker);
        thread_mutex_unlock(&worker->mutex);
        if (!task) {
                return false;
        }
        worker_task_run(task);
        worker_task_destroy(task);
        return true;
}

bool worker_cancel_task(worker_t *worker, worker_task_t *task)
{
        list_node_t *node;

        thread_mutex_lock(&worker->mutex);
        for (list_each(node, &worker->tasks)) {
                if (node->data == task) {
                        worker_task_destroy(task);
                        list_delete_node(&worker->tasks, node);
                        thread_mutex_unlock(&worker->mutex);
                        return true;
                }
        }
        thread_mutex_unlock(&worker->mutex);
        return false;
}

static void worker_thread(void *arg)
{
        worker_task_t *task;
        worker_t *worker = arg;

        while (worker->active) {
                thread_mutex_lock(&worker->mutex);
                thread_cond_wait(&worker->cond, &worker->mutex);
                task = worker_get_task(worker);
                thread_mutex_unlock(&worker->mutex);
                if (task) {
                        worker_task_run(task);
                        worker_task_destroy(task);
                }
        }
        thread_exit(NULL);
}

int worker_run_async(worker_t *worker)
{
        if (worker->thread != 0) {
                return -EEXIST;
        }
        worker->active = true;
        thread_create(&worker->thread, worker_thread, worker);
        return 0;
}
