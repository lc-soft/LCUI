/*
 * src/lcui_worker.c
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <yutil.h>
#include <thread.h>
#include <LCUI/worker.h>

typedef struct lcui_worker_queue {
        bool active;
        bool threaded;
        list_t tasks;
        thread_mutex_t mutex;
        thread_cond_t cond;
        thread_t thread;
} lcui_worker_queue_t;

struct lcui_worker_task {
        lcui_worker_task_cb task_cb;
        lcui_worker_task_cb after_task_cb;
        void *data;
};

static struct lcui_worker {
        bool initialized;
        lcui_worker_queue_t main;
        lcui_worker_queue_t async;
} lcui_worker;

static void lcui_worker_task_destroy(lcui_worker_task_t *task)
{
        if (task->after_task_cb) {
                task->after_task_cb(task->data);
        }
        free(task);
}

static int lcui_worker_task_run(lcui_worker_task_t *task)
{
        if (!task || !task->task_cb) {
                return -1;
        }
        task->task_cb(task->data);
        return 0;
}

static lcui_worker_task_t *lcui_worker_task_pop(lcui_worker_queue_t *queue)
{
        lcui_worker_task_t *task;
        list_node_t *node;

        node = list_get_first_node(&queue->tasks);
        if (!node) {
                return NULL;
        }
        task = node->data;
        list_delete_node(&queue->tasks, node);
        return task;
}

static int lcui_worker_queue_init(lcui_worker_queue_t *queue, bool threaded)
{
        queue->active = false;
        queue->threaded = threaded;
        queue->thread = 0;
        list_create(&queue->tasks);
        if (thread_mutex_init(&queue->mutex) != 0) {
                return -1;
        }
        if (thread_cond_init(&queue->cond) != 0) {
                thread_mutex_destroy(&queue->mutex);
                return -1;
        }
        return 0;
}

/* The caller must hold queue->mutex. */
static lcui_worker_task_t *lcui_worker_queue_post_locked(
    lcui_worker_queue_t *queue, void *data, lcui_worker_task_cb task_cb,
    lcui_worker_task_cb after_task_cb)
{
        lcui_worker_task_t *task;

        task = malloc(sizeof(lcui_worker_task_t));
        if (!task) {
                return NULL;
        }
        task->data = data;
        task->task_cb = task_cb;
        task->after_task_cb = after_task_cb;
        list_append(&queue->tasks, task);
        return task;
}

static lcui_worker_task_t *lcui_worker_queue_post(
    lcui_worker_queue_t *queue, void *data, lcui_worker_task_cb task_cb,
    lcui_worker_task_cb after_task_cb)
{
        lcui_worker_task_t *task;

        thread_mutex_lock(&queue->mutex);
        task =
            lcui_worker_queue_post_locked(queue, data, task_cb, after_task_cb);
        if (task) {
                thread_cond_signal(&queue->cond);
        }
        thread_mutex_unlock(&queue->mutex);
        return task;
}

static bool lcui_worker_queue_run(lcui_worker_queue_t *queue)
{
        lcui_worker_task_t *task;

        thread_mutex_lock(&queue->mutex);
        task = lcui_worker_task_pop(queue);
        thread_mutex_unlock(&queue->mutex);
        if (!task) {
                return false;
        }
        lcui_worker_task_run(task);
        lcui_worker_task_destroy(task);
        return true;
}

static bool lcui_worker_queue_cancel(lcui_worker_queue_t *queue,
                                     lcui_worker_task_t *task)
{
        bool found = false;
        list_node_t *node;

        thread_mutex_lock(&queue->mutex);
        for (list_each(node, &queue->tasks)) {
                if (node->data == task) {
                        list_delete_node(&queue->tasks, node);
                        found = true;
                        break;
                }
        }
        thread_mutex_unlock(&queue->mutex);
        if (found) {
                lcui_worker_task_destroy(task);
        }
        return found;
}

static void lcui_worker_queue_thread(void *arg)
{
        lcui_worker_queue_t *queue = arg;
        lcui_worker_task_t *task;

        for (;;) {
                thread_mutex_lock(&queue->mutex);
                while (queue->active && queue->tasks.length == 0) {
                        thread_cond_wait(&queue->cond, &queue->mutex);
                }
                if (!queue->active) {
                        thread_mutex_unlock(&queue->mutex);
                        break;
                }
                task = lcui_worker_task_pop(queue);
                thread_mutex_unlock(&queue->mutex);
                if (task) {
                        lcui_worker_task_run(task);
                        lcui_worker_task_destroy(task);
                }
        }
        thread_exit(NULL);
}

static int lcui_worker_queue_start(lcui_worker_queue_t *queue)
{
        int ret;

        if (!queue->threaded || queue->thread != 0) {
                return 0;
        }
        thread_mutex_lock(&queue->mutex);
        queue->active = true;
        thread_mutex_unlock(&queue->mutex);
        ret = thread_create(&queue->thread, lcui_worker_queue_thread, queue);
        if (ret != 0) {
                thread_mutex_lock(&queue->mutex);
                queue->active = false;
                thread_mutex_unlock(&queue->mutex);
        }
        return ret;
}

static void lcui_worker_queue_destroy(lcui_worker_queue_t *queue)
{
        if (queue->threaded && queue->thread != 0) {
                thread_mutex_lock(&queue->mutex);
                queue->active = false;
                thread_cond_signal(&queue->cond);
                thread_mutex_unlock(&queue->mutex);
                thread_join(queue->thread, NULL);
                queue->thread = 0;
        }
        list_destroy(&queue->tasks,
                     (list_item_destructor_t)lcui_worker_task_destroy);
        thread_cond_destroy(&queue->cond);
        thread_mutex_destroy(&queue->mutex);
}

lcui_worker_task_t *lcui_worker_post_task(void *data,
                                          lcui_worker_task_cb task_cb,
                                          lcui_worker_task_cb after_task_cb)
{
        if (!lcui_worker.initialized) {
                return NULL;
        }
        return lcui_worker_queue_post(&lcui_worker.main, data, task_cb,
                                      after_task_cb);
}

lcui_worker_task_t *lcui_worker_post_async_task(
    void *data, lcui_worker_task_cb task_cb, lcui_worker_task_cb after_task_cb)
{
        if (!lcui_worker.initialized) {
                return NULL;
        }
        return lcui_worker_queue_post(&lcui_worker.async, data, task_cb,
                                      after_task_cb);
}

bool lcui_worker_cancel_async_task(lcui_worker_task_t *task)
{
        if (!lcui_worker.initialized) {
                return false;
        }
        return lcui_worker_queue_cancel(&lcui_worker.async, task);
}

bool lcui_worker_cancel_task(lcui_worker_task_t *task)
{
        if (!lcui_worker.initialized) {
                return false;
        }
        return lcui_worker_queue_cancel(&lcui_worker.main, task);
}

void lcui_worker_run(void)
{
        if (!lcui_worker.initialized) {
                return;
        }
        lcui_worker_queue_run(&lcui_worker.main);
}

void lcui_worker_init(void)
{
        if (lcui_worker.initialized) {
                return;
        }
        if (lcui_worker_queue_init(&lcui_worker.main, false) != 0) {
                return;
        }
        if (lcui_worker_queue_init(&lcui_worker.async, true) != 0) {
                lcui_worker_queue_destroy(&lcui_worker.main);
                return;
        }
        if (lcui_worker_queue_start(&lcui_worker.async) != 0) {
                lcui_worker_queue_destroy(&lcui_worker.async);
                lcui_worker_queue_destroy(&lcui_worker.main);
                return;
        }
        lcui_worker.initialized = true;
}

void lcui_worker_destroy(void)
{
        if (!lcui_worker.initialized) {
                return;
        }
        lcui_worker.initialized = false;
        lcui_worker_queue_destroy(&lcui_worker.async);
        lcui_worker_queue_destroy(&lcui_worker.main);
}
