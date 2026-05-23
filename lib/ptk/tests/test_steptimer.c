/*
 * lib/ptk/tests/test_steptimer.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include <ptk/steptimer.h>
#include <ctest.h>

typedef struct steptimer_probe {
        unsigned call_count;
        uint64_t last_elapsed_time;
} steptimer_probe_t;

static void on_steptimer_tick(ptk_steptimer_t *timer, void *data)
{
        steptimer_probe_t *probe = data;

        probe->call_count++;
        probe->last_elapsed_time = timer->elapsed_time;
}

static void test_steptimer_init_sets_last_time(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };

        ptk_steptimer_init(&timer);
        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        // After init, last_time is the wall clock, so the first tick should
        // observe a near-zero delta rather than a huge one.
        ctest_equal_uint("first tick should call handler once",
                         probe.call_count, 1);
        ctest_equal_bool("first tick elapsed_time should be small (< 100ms)",
                         probe.last_elapsed_time < 100, true);
}

static void test_steptimer_variable_step(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };

        ptk_steptimer_init(&timer);
        timer.is_fixed_time_step = false;
        // Simulate 500ms elapsed since last tick.
        timer.last_time = (uint64_t)get_time_ms() - 500;

        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        ctest_equal_uint("variable-step tick calls handler exactly once",
                         probe.call_count, 1);
}

static void test_steptimer_fixed_step_catch_up(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };
        bool count_in_range;

        ptk_steptimer_init(&timer);
        timer.is_fixed_time_step = true;
        timer.enable_catch_up = true;
        timer.target_elapsed_time = 8;
        // max_delta defaults to 1000; a 500ms delta will not be clamped.
        timer.last_time = (uint64_t)get_time_ms() - 500;

        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        // Expected: 500 / 8 = 62 catch-up calls. Allow tolerance because the
        // real wall-clock delta may drift by a few milliseconds.
        count_in_range = probe.call_count >= 58 && probe.call_count <= 66;
        ctest_equal_bool(
            "fixed-step catch-up should invoke handler about 62 times",
            count_in_range, true);
}

static void test_steptimer_fixed_step_no_catch_up(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };

        ptk_steptimer_init(&timer);
        timer.is_fixed_time_step = true;
        timer.enable_catch_up = false;
        timer.target_elapsed_time = 8;
        timer.last_time = (uint64_t)get_time_ms() - 500;

        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        ctest_equal_uint("no-catch-up should invoke handler exactly once",
                         probe.call_count, 1);
        ctest_equal_bool(
            "no-catch-up elapsed_time should reflect real delta (~500ms)",
            probe.last_elapsed_time >= 400 && probe.last_elapsed_time <= 600,
            true);
}

static void test_steptimer_reset_elapsed_time(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };

        ptk_steptimer_init(&timer);
        timer.is_fixed_time_step = true;
        timer.enable_catch_up = false;
        timer.target_elapsed_time = 16;

        // Accumulate some left-over time by ticking twice with a gap.
        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);
        sleep_ms(20);
        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        // Before reset, left_over_time should be non-zero.
        ctest_equal_bool("left_over_time is non-zero before reset",
                         timer.left_over_time > 0, true);

        ptk_steptimer_reset_elapsed_time(&timer);

        ctest_equal_bool("left_over_time is zero after reset",
                         timer.left_over_time == 0, true);
        ctest_equal_uint("frames_per_second is zero after reset",
                         timer.frames_per_second, 0);
}

static void test_steptimer_max_delta_clamp(void)
{
        ptk_steptimer_t timer;
        steptimer_probe_t probe = { 0 };

        ptk_steptimer_init(&timer);
        timer.is_fixed_time_step = false;
        timer.max_delta = 1000;
        // Pretend 10 seconds have elapsed; max_delta should clamp this.
        timer.last_time = (uint64_t)get_time_ms() - 10000;

        ptk_steptimer_tick(&timer, on_steptimer_tick, &probe);

        ctest_equal_uint("variable-step still calls handler once",
                         probe.call_count, 1);
        ctest_equal_bool(
            "elapsed_time should be clamped to <= max_delta (1000ms)",
            probe.last_elapsed_time <= 1000, true);
}

void test_ptk_steptimer(void)
{
        ctest_describe("init sets last_time to wall clock",
                       test_steptimer_init_sets_last_time);
        ctest_describe("variable-step calls handler exactly once",
                       test_steptimer_variable_step);
        ctest_describe("fixed-step catch-up replays missed updates",
                       test_steptimer_fixed_step_catch_up);
        ctest_describe("fixed-step no-catch-up fires handler once",
                       test_steptimer_fixed_step_no_catch_up);
        ctest_describe("reset_elapsed_time clears leftovers",
                       test_steptimer_reset_elapsed_time);
        ctest_describe("max_delta clamps excessive deltas",
                       test_steptimer_max_delta_clamp);
}
