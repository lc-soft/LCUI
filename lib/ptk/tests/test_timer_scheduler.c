/*
 * lib/ptk/tests/test_timer_scheduler.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>
#include <yutil.h>
#include "timer_scheduler.h"

static timer_scheduler_t *scheduler;
static int timeout_triggered;
static int interval_count;

static void on_timeout(void *arg)
{
        timeout_triggered = 1;
}

static void on_interval(void *arg)
{
        interval_count++;
}

static void create_scheduler(void)
{
        scheduler = timer_scheduler_create();
        ctest_equal_bool("should create scheduler", scheduler != NULL, true);
}

static void destroy_scheduler(void)
{
        timer_scheduler_destroy(scheduler);
}

static void test_timer_scheduler_lifecycle(void)
{
        ctest_describe("lifecycle", create_scheduler);
        ctest_describe("lifecycle", destroy_scheduler);
}

static void test_timeout(void)
{
        int id;

        create_scheduler();
        id = timer_scheduler_add_timeout(scheduler, 10, on_timeout, NULL);
        ctest_equal_bool("should return valid id", id != -1, true);

        timeout_triggered = 0;
        y_sleep(20L);
        timer_scheduler_process(scheduler);
        ctest_equal_int("should trigger timeout callback", timeout_triggered,
                        1);

        destroy_scheduler();
}

static void test_interval(void)
{
        int id;

        create_scheduler();
        id = timer_scheduler_add_interval(scheduler, 10, on_interval, NULL);
        ctest_equal_bool("should return valid id", id != -1, true);

        interval_count = 0;
        y_sleep(20L);
        timer_scheduler_process(scheduler);
        ctest_equal_bool("should trigger interval callback at least once",
                         interval_count > 0, true);

        timer_scheduler_remove(scheduler, id);
        interval_count = 0;
        y_sleep(20L);
        timer_scheduler_process(scheduler);
        ctest_equal_int("should stop after remove", interval_count, 0);

        destroy_scheduler();
}

static void test_remove(void)
{
        int id;

        create_scheduler();
        id = timer_scheduler_add_timeout(scheduler, 50, on_timeout, NULL);
        timer_scheduler_remove(scheduler, id);
        timeout_triggered = 0;
        y_sleep(60L);
        timer_scheduler_process(scheduler);
        ctest_equal_int("should not trigger after remove", timeout_triggered,
                        0);

        ctest_equal_int("should return -1 for unknown id",
                        timer_scheduler_remove(scheduler, 999), -1);

        destroy_scheduler();
}

void test_ptk_timer_scheduler(void)
{
        ctest_describe("lifecycle", test_timer_scheduler_lifecycle);
        ctest_describe("timeout", test_timeout);
        ctest_describe("interval", test_interval);
        ctest_describe("remove", test_remove);
}
