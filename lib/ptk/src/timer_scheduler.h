/*
 * lib/ptk/src/timer_scheduler.h -- Internal timer scheduler.
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

#ifndef PTK_SRC_TIMER_SCHEDULER_H
#define PTK_SRC_TIMER_SCHEDULER_H

#include "ptk/types.h"

typedef struct timer_scheduler timer_scheduler_t;

timer_scheduler_t *timer_scheduler_create(void);
void timer_scheduler_destroy(timer_scheduler_t *scheduler);
size_t timer_scheduler_process(timer_scheduler_t *scheduler);

int timer_scheduler_add_interval(timer_scheduler_t *scheduler, long int n_ms,
                                 ptk_timer_cb callback, void *arg);
int timer_scheduler_add_timeout(timer_scheduler_t *scheduler, long int n_ms,
                                ptk_timer_cb callback, void *arg);
int timer_scheduler_remove(timer_scheduler_t *scheduler, int timer_id);
int timer_scheduler_reset(timer_scheduler_t *scheduler, int timer_id,
                          long int n_ms);

#endif
