/*
 * tests/cases/test_settings.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <LCUI.h>
#include <ctest-custom.h>

static void check_settings_fps_cap(void *arg)
{
        char str[256];
        int fps_limit = *((int *)arg);
        int fps = (int)lcui_get_fps();

        sprintf(str, "should reach frame cap of %u (actual %u)", fps_limit,
                fps);
        ctest_equal_bool(str, fps <= fps_limit + 2 && fps > fps_limit / 2,
                         true);
        lcui_quit();
}

static void test_default_settings(void)
{
        lcui_settings_t settings;

        lcui_init();
        lcui_reset_settings();
        lcui_get_settings(&settings);

        ctest_equal_int("should default frame rate cap to 120",
                        settings.fps_cap, 120);
        ctest_equal_int("should default parallel rendering threads to 4",
                        settings.parallel_rendering_threads, 4);
        ctest_equal_bool("should disable paint flashing by default",
                         settings.paint_flashing, false);
        lcui_quit();
        lcui_main();
}

static void test_apply_settings(void)
{
        lcui_settings_t settings;

        settings.fps_cap = 60;
        settings.parallel_rendering_threads = 2;
        settings.paint_flashing = true;

        lcui_init();
        lcui_apply_settings(&settings);
        lcui_get_settings(&settings);
        ctest_equal_int("should apply frame rate cap", settings.fps_cap, 60);
        ctest_equal_int("should apply parallel rendering threads",
                        settings.parallel_rendering_threads, 2);
        ctest_equal_bool("should enable paint flashing",
                         settings.paint_flashing, true);

        settings.fps_cap = -1;
        settings.parallel_rendering_threads = -1;

        lcui_apply_settings(&settings);
        lcui_get_settings(&settings);

        ctest_equal_int("should clamp frame rate cap to minimum 1",
                        settings.fps_cap, 1);
        ctest_equal_int("should clamp parallel rendering threads to minimum 1",
                        settings.parallel_rendering_threads, 1);

        lcui_reset_settings();

        lcui_get_settings(&settings);
        ctest_equal_int("should reset frame rate cap to default",
                        settings.fps_cap, LCUI_MAX_FRAMES_PER_SEC);
        lcui_quit();
        lcui_main();
}

void test_settings_fps_cap(void)
{
        lcui_settings_t settings;

        lcui_init();
        lcui_get_settings(&settings);

        settings.fps_cap = 30;
        lcui_apply_settings(&settings);
        ptk_set_timeout(2000, check_settings_fps_cap, &settings.fps_cap);
        lcui_main();

        lcui_init();
        settings.fps_cap = 5;
        lcui_apply_settings(&settings);
        ptk_set_timeout(2000, check_settings_fps_cap, &settings.fps_cap);
        lcui_main();

        lcui_init();
        settings.fps_cap = 90;
        lcui_apply_settings(&settings);
        ptk_set_timeout(2000, check_settings_fps_cap, &settings.fps_cap);
        lcui_main();

        lcui_init();
        settings.fps_cap = 25;
        lcui_apply_settings(&settings);
        ptk_set_timeout(2000, check_settings_fps_cap, &settings.fps_cap);
        lcui_main();
}

void test_settings(void)
{
        ctest_describe("default settings", test_default_settings);
        ctest_describe("apply settings", test_apply_settings);
        ctest_describe("settings.fps_cap", test_settings_fps_cap);
}
