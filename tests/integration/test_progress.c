/*
 * tests/integration/test_progress.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include "../scenes/progress_scene.h"

static void test_get_set_value(void)
{
        ui_widget_t *progress = ui_create_progress();
        ui_root_append(progress);

        ui_progress_set_value(progress, 30.0f);
        ctest_equal_float("should store the value",
                          ui_progress_get_value(progress), 30.0f);

        ui_progress_set_value(progress, 75.0f);
        ctest_equal_float("should overwrite the previous value",
                          ui_progress_get_value(progress), 75.0f);
}

static void test_clamp_value(void)
{
        ui_widget_t *progress = ui_create_progress();
        ui_root_append(progress);

        ui_progress_set_value(progress, -10.0f);
        ctest_equal_float("should clamp negative to zero",
                          ui_progress_get_value(progress), 0.0f);

        ui_progress_set_value(progress, 150.0f);
        ctest_equal_float("should clamp above 100 to 100",
                          ui_progress_get_value(progress), 100.0f);

        ui_progress_set_value(progress, 0.0f);
        ctest_equal_float("should accept zero as-is",
                          ui_progress_get_value(progress), 0.0f);

        ui_progress_set_value(progress, 100.0f);
        ctest_equal_float("should accept 100 as-is",
                          ui_progress_get_value(progress), 100.0f);
}

static void test_indicator_width(void)
{
        ui_widget_t *progress;
        ui_widget_t *track;
        ui_widget_t *indicator;
        float track_width;
        float progress_height;

        progress_scene_build();
        lcui_ui_update();

        progress = ui_get_widget("main");
        track = ui_widget_get_child(progress, 0);
        indicator = ui_widget_get_child(track, 0);
        track_width = track->border_box.width;
        progress_height = progress->border_box.height;

        ui_progress_set_value(progress, 0.0f);
        lcui_ui_update();
        ctest_equal_float("value 0: indicator width should be 0",
                          indicator->border_box.width, 0.0f);

        ui_progress_set_value(progress, 50.0f);
        lcui_ui_update();
        ctest_equal_float("value 50: indicator width should be half track",
                          indicator->border_box.width, track_width * 0.5f);

        ui_progress_set_value(progress, 100.0f);
        lcui_ui_update();
        ctest_equal_float("value 100: indicator width should match track",
                          indicator->border_box.width, track_width);

        ctest_equal_float("track height should fill progress",
                          track->border_box.height, progress_height);
        ctest_equal_float("indicator height should fill track",
                          indicator->border_box.height,
                          track->border_box.height);
}

static void test_track_has_indicator(void)
{
        ui_widget_t *progress;
        ui_widget_t *track;
        ui_widget_t *child;
        int has_indicator = 0;

        progress = ui_create_progress();
        ui_root_append(progress);

        track = ui_widget_get_child(progress, 0);
        for (child = ui_widget_get_child(track, 0); child;
             child = ui_widget_next(child)) {
                if (ui_check_widget_type(child, "progress-indicator")) {
                        has_indicator = 1;
                }
        }
        ctest_equal_bool("progress should auto-create track with indicator",
                         has_indicator, true);
}

void test_progress(void)
{
        lcui_init();

        ctest_describe("get/set value", test_get_set_value);
        ctest_describe("clamp value to 0..100", test_clamp_value);
        ctest_describe("structure has track + indicator",
                       test_track_has_indicator);

        lcui_destroy();

        lcui_init();
        ctest_describe("indicator width after layout", test_indicator_width);
        lcui_destroy();
}
