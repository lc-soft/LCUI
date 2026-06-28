/*
 * lib/ptk/src/timer_scheduler.c -- Internal timer scheduler.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <stdint.h>
#include <yutil.h>
#include "timer_scheduler.h"

typedef enum { TIMER_STATE_ACTIVE, TIMER_STATE_DELETED } timer_state_t;

typedef struct {
        timer_state_t state;
        long int id;
        bool reuse;

        int64_t start_time;
        long int total_ms;

        ptk_timer_cb callback;
        void *arg;

        list_node_t node;
} timer_entry_t;

struct timer_scheduler {
        int id_count;
        list_t timers;
};

static void timer_scheduler_reorder(timer_scheduler_t *scheduler,
                                    list_node_t *node)
{
        timer_entry_t *entry;
        int64_t remaining, cur_remaining;
        list_node_t *cur;

        entry = (timer_entry_t *)node->data;
        remaining = entry->total_ms - (y_gettime() - entry->start_time);
        list_for_each(cur, &scheduler->timers)
        {
                entry = (timer_entry_t *)cur->data;
                cur_remaining =
                    entry->total_ms - (y_gettime() - entry->start_time);
                if (remaining <= cur_remaining) {
                        list_link(&scheduler->timers, cur->prev, node);
                        return;
                }
        }
        list_append_node(&scheduler->timers, node);
}

static timer_entry_t *timer_find(timer_scheduler_t *scheduler, int timer_id)
{
        timer_entry_t *entry;
        list_node_t *node;
        list_for_each(node, &scheduler->timers)
        {
                entry = (timer_entry_t *)node->data;
                if (entry && entry->id == timer_id) {
                        return entry;
                }
        }
        return NULL;
}

static int timer_scheduler_add(timer_scheduler_t *scheduler, long int n_ms,
                               ptk_timer_cb callback, void *arg, bool reuse)
{
        timer_entry_t *entry;

        entry = (timer_entry_t *)malloc(sizeof(timer_entry_t));
        if (entry == NULL) {
                return -1;
        }
        entry->arg = arg;
        entry->callback = callback;
        entry->reuse = reuse;
        entry->total_ms = n_ms;
        entry->state = TIMER_STATE_ACTIVE;
        entry->id = ++scheduler->id_count;
        entry->start_time = y_gettime();
        entry->node.next = NULL;
        entry->node.prev = NULL;
        entry->node.data = entry;
        timer_scheduler_reorder(scheduler, &entry->node);

        return entry->id;
}

timer_scheduler_t *timer_scheduler_create(void)
{
        timer_scheduler_t *scheduler =
            (timer_scheduler_t *)malloc(sizeof(timer_scheduler_t));
        if (!scheduler) {
                return NULL;
        }
        scheduler->id_count = 0;
        list_create(&scheduler->timers);
        return scheduler;
}

void timer_scheduler_destroy(timer_scheduler_t *scheduler)
{
        list_destroy_without_node(&scheduler->timers, free);
        free(scheduler);
}

int timer_scheduler_remove(timer_scheduler_t *scheduler, int timer_id)
{
        timer_entry_t *entry = timer_find(scheduler, timer_id);
        if (entry) {
                entry->state = TIMER_STATE_DELETED;
                return 0;
        }
        return -1;
}

int timer_scheduler_reset(timer_scheduler_t *scheduler, int timer_id,
                          long int n_ms)
{
        timer_entry_t *entry = timer_find(scheduler, timer_id);
        if (entry) {
                entry->total_ms = n_ms;
                entry->start_time = y_gettime();
        }
        return entry ? 0 : -1;
}

int timer_scheduler_add_timeout(timer_scheduler_t *scheduler, long int n_ms,
                                ptk_timer_cb callback, void *arg)
{
        return timer_scheduler_add(scheduler, n_ms, callback, arg, false);
}

int timer_scheduler_add_interval(timer_scheduler_t *scheduler, long int n_ms,
                                 ptk_timer_cb callback, void *arg)
{
        return timer_scheduler_add(scheduler, n_ms, callback, arg, true);
}

size_t timer_scheduler_process(timer_scheduler_t *scheduler)
{
        size_t count = 0;
        long elapsed_ms;
        timer_entry_t *entry = NULL;
        list_node_t *node, *prev_node;

        while (scheduler) {
                list_for_each(node, &scheduler->timers)
                {
                        entry = (timer_entry_t *)node->data;
                        if (entry) {
                                if (entry->state == TIMER_STATE_DELETED) {
                                        prev_node = node->prev;
                                        list_unlink(&scheduler->timers, node);
                                        free(entry);
                                        node = prev_node;
                                        continue;
                                }
                                if (entry->state == TIMER_STATE_ACTIVE) {
                                        break;
                                }
                        }
                }
                if (!node) {
                        break;
                }
                count += 1;
                elapsed_ms = (long)(y_gettime() - entry->start_time);
                if (elapsed_ms < entry->total_ms) {
                        break;
                }
                entry->callback(entry->arg);
                list_unlink(&scheduler->timers, node);
                if (entry->reuse) {
                        entry->start_time = y_gettime();
                        timer_scheduler_reorder(scheduler, node);
                } else {
                        free(entry);
                }
        }
        return count;
}
