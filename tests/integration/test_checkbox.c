/*
 * tests/integration/test_checkbox.c
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

static int g_change_count;

static void on_checkbox_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_change_count;
}

static void test_checked_roundtrip(void)
{
        ui_widget_t *cb = ui_create_checkbox();

        ctest_equal_int("should default to unchecked",
                        ui_checkbox_get_checked(cb), 0);

        ui_checkbox_set_checked(cb, 1);
        ctest_equal_int("should return 1 after set_checked(1)",
                        ui_checkbox_get_checked(cb), 1);

        ui_checkbox_set_checked(cb, 0);
        ctest_equal_int("should return 0 after set_checked(0)",
                        ui_checkbox_get_checked(cb), 0);

        ui_widget_destroy(cb);
}

static void test_click_toggles(void)
{
        ui_widget_t *cb = ui_create_checkbox();
        ui_event_t ev;

        ui_widget_append(ui_root(), cb);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(cb, ev, NULL);
        ctest_equal_int("first click should check", ui_checkbox_get_checked(cb),
                        1);

        ui_widget_emit_event(cb, ev, NULL);
        ctest_equal_int("second click should uncheck",
                        ui_checkbox_get_checked(cb), 0);

        ui_widget_destroy(cb);
}

static void test_click_emits_change(void)
{
        ui_widget_t *cb = ui_create_checkbox();
        ui_event_t ev;

        g_change_count = 0;
        ui_widget_on(cb, "change", on_checkbox_change, NULL);

        ui_widget_append(ui_root(), cb);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(cb, ev, NULL);

        ctest_equal_int("should emit change once after click", g_change_count,
                        1);

        ui_widget_destroy(cb);
}

static void test_disabled_blocks_toggle(void)
{
        ui_widget_t *cb = ui_create_checkbox();
        ui_event_t ev;

        ui_widget_append(ui_root(), cb);
        ui_widget_set_disabled(cb, true);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(cb, ev, NULL);

        ctest_equal_int("disabled checkbox should stay unchecked",
                        ui_checkbox_get_checked(cb), 0);

        ui_widget_destroy(cb);
}

static void test_indeterminate_roundtrip(void)
{
        ui_widget_t *cb = ui_create_checkbox();

        ctest_equal_int("should default to not indeterminate",
                        ui_checkbox_get_indeterminate(cb), 0);

        ui_checkbox_set_indeterminate(cb, 1);
        ctest_equal_int("should return 1 after set_indeterminate(1)",
                        ui_checkbox_get_indeterminate(cb), 1);

        ui_checkbox_set_indeterminate(cb, 0);
        ctest_equal_int("should return 0 after set_indeterminate(0)",
                        ui_checkbox_get_indeterminate(cb), 0);

        ui_widget_destroy(cb);
}

static void test_indeterminate_click_transitions_to_checked(void)
{
        ui_widget_t *cb = ui_create_checkbox();
        ui_event_t ev;

        ui_widget_append(ui_root(), cb);
        ui_checkbox_set_indeterminate(cb, 1);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(cb, ev, NULL);

        ctest_equal_int("indeterminate click should set checked",
                        ui_checkbox_get_checked(cb), 1);
        ctest_equal_int("indeterminate click should clear indeterminate",
                        ui_checkbox_get_indeterminate(cb), 0);

        ui_widget_destroy(cb);
}

static void test_status_sync(void)
{
        ui_widget_t *cb = ui_create_checkbox();

        ctest_equal_int("should not have checked status by default",
                        ui_widget_has_status(cb, "checked"), 0);

        ui_checkbox_set_checked(cb, 1);
        ctest_equal_int("checked status should be set after set_checked(1)",
                        ui_widget_has_status(cb, "checked"), 1);

        ui_checkbox_set_checked(cb, 0);
        ctest_equal_int("checked status should be cleared after set_checked(0)",
                        ui_widget_has_status(cb, "checked"), 0);

        ui_checkbox_set_indeterminate(cb, 1);
        ctest_equal_int(
            "indeterminate status should be set after set_indeterminate(1)",
            ui_widget_has_status(cb, "indeterminate"), 1);

        ui_widget_destroy(cb);
}

void test_checkbox(void)
{
        lcui_init();

        ctest_describe("checkbox checked roundtrip", test_checked_roundtrip);
        ctest_describe("checkbox click toggles", test_click_toggles);
        ctest_describe("checkbox click emits change", test_click_emits_change);
        ctest_describe("checkbox disabled blocks toggle",
                       test_disabled_blocks_toggle);
        ctest_describe("checkbox indeterminate roundtrip",
                       test_indeterminate_roundtrip);
        ctest_describe("checkbox indeterminate click transitions to checked",
                       test_indeterminate_click_transitions_to_checked);
        ctest_describe("checkbox status sync", test_status_sync);

        lcui_destroy();
}
