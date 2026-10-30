/*
 * tests/integration/test_worker.c: -- LCUI worker task tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <yutil.h>
#include <ctest-custom.h>

static int main_task_count;
static int after_task_count;
static int async_task_count;

static void on_main_task(void *arg)
{
        main_task_count++;
}

static void on_after_task(void *arg)
{
        after_task_count++;
}

static void on_async_task(void *arg)
{
        async_task_count++;
}

void test_worker(void)
{
        int i;
        lcui_worker_task_t *task;

        lcui_worker_init();
        main_task_count = 0;
        after_task_count = 0;
        async_task_count = 0;

        task = lcui_worker_post_task(NULL, on_main_task, on_after_task);
        ctest_equal_bool("should return a main task handle", task != NULL,
                         true);
        lcui_worker_run();
        ctest_equal_int("should run the posted main task", main_task_count, 1);
        ctest_equal_int("should run the after-task callback", after_task_count,
                        1);

        task = lcui_worker_post_task(NULL, on_main_task, NULL);
        ctest_equal_bool("should cancel a queued main task",
                         lcui_worker_cancel_task(task), true);
        lcui_worker_run();
        ctest_equal_int("should not run a canceled main task", main_task_count,
                        1);

        task = lcui_worker_post_async_task(NULL, on_async_task, NULL);
        ctest_equal_bool("should return an async task handle", task != NULL,
                         true);
        for (i = 0; i < 200 && async_task_count == 0; ++i) {
                y_sleep(10);
        }
        ctest_equal_int("should run the posted async task", async_task_count,
                        1);

        task = lcui_worker_post_task(NULL, on_main_task, on_after_task);
        ctest_equal_bool("should queue a task before destroy", task != NULL,
                         true);
        lcui_worker_destroy();
        ctest_equal_int("should discard pending tasks on destroy",
                        main_task_count, 1);
        ctest_equal_int("should run after-task callbacks for discarded tasks",
                        after_task_count, 2);
}
