/*
 * tests/integration/test_window_restore.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_position_round_trip(void)
{
        int x, y;
        lcui_window_options_t opt = { 0 };
        lcui_window_t *window;
        ptk_window_t *wnd;

        opt.width = 320;
        opt.height = 240;
        lcui_init();
        window = lcui_window_create(&opt);
        wnd = lcui_window_get_handle(window);
        ctest_equal_bool("should create window", wnd != NULL, true);
        ptk_window_set_position(wnd, 100, 120);
        ptk_window_get_position(wnd, &x, &y);
        ctest_equal_int("should round-trip x", x, 100);
        ctest_equal_int("should round-trip y", y, 120);
        lcui_window_destroy(window);
        lcui_destroy();
}

static void test_hidden_window_stays_hidden(void)
{
        ptk_window_t *wnd;

        lcui_init();
        wnd = ptk_window_create(L"probe", 0, 0, 320, 240, NULL);
        ctest_equal_bool("should be created hidden", ptk_window_is_visible(wnd),
                         false);
        ptk_window_set_maximized(wnd, false);
        ctest_equal_bool("should stay hidden after set_maximized",
                         ptk_window_is_visible(wnd), false);
        ptk_window_show(wnd);
        ctest_equal_bool("should be visible after show",
                         ptk_window_is_visible(wnd), true);
        ptk_window_destroy(wnd);
        lcui_destroy();
}

void test_window_restore(void)
{
        ctest_describe("window position round-trip", test_position_round_trip);
        ctest_describe("window visibility", test_hidden_window_stays_hidden);
}
