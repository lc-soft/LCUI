/*
 * tests/integration/test_label.c
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

static int g_target_click_count;

static void on_target_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)e;
        (void)arg;
        ++g_target_click_count;
}

static void test_get_for_default(void)
{
        ui_widget_t *label = ui_create_label();

        ctest_equal_bool("should return NULL when no 'for' attribute set",
                         ui_label_get_for(label) == NULL, true);

        ui_widget_destroy(label);
}

static void test_set_for_roundtrip(void)
{
        ui_widget_t *label = ui_create_label();

        ui_label_set_for(label, "my-target");
        ctest_equal_str("should return the id that was set",
                        ui_label_get_for(label), "my-target");

        ui_label_set_for(label, "other-target");
        ctest_equal_str("should overwrite previous id", ui_label_get_for(label),
                        "other-target");

        ui_widget_destroy(label);
}

static void test_click_forwards_to_target(void)
{
        ui_widget_t *label;
        ui_widget_t *target;
        ui_event_t ev;

        g_target_click_count = 0;

        target = ui_create_widget("button");
        ui_widget_set_id(target, "tgt");
        ui_widget_on(target, "click", on_target_click, NULL);

        label = ui_create_label();
        ui_label_set_for(label, "tgt");

        ui_widget_append(ui_root(), target);
        ui_widget_append(ui_root(), label);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(label, ev, NULL);

        ctest_equal_int("should fire click on target widget",
                        g_target_click_count, 1);

        ui_widget_destroy(label);
        ui_widget_destroy(target);
}

static void test_disabled_blocks_forward(void)
{
        ui_widget_t *label;
        ui_widget_t *target;
        ui_event_t ev;

        g_target_click_count = 0;

        target = ui_create_widget("button");
        ui_widget_set_id(target, "tgt2");
        ui_widget_on(target, "click", on_target_click, NULL);

        label = ui_create_label();
        ui_label_set_for(label, "tgt2");
        ui_widget_set_disabled(label, true);

        ui_widget_append(ui_root(), target);
        ui_widget_append(ui_root(), label);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(label, ev, NULL);

        ctest_equal_int("should not fire click when label is disabled",
                        g_target_click_count, 0);

        ui_widget_destroy(label);
        ui_widget_destroy(target);
}

static void test_click_without_for_is_safe(void)
{
        ui_widget_t *label;
        ui_event_t ev;

        label = ui_create_label();
        ui_widget_append(ui_root(), label);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(label, ev, NULL);

        ctest_equal_bool("should not crash when 'for' is unset", true, true);

        ui_widget_destroy(label);
}

void test_label(void)
{
        lcui_init();

        ctest_describe("label get_for default", test_get_for_default);
        ctest_describe("label set_for roundtrip", test_set_for_roundtrip);
        ctest_describe("label click forwards to target",
                       test_click_forwards_to_target);
        ctest_describe("label click without for is safe",
                       test_click_without_for_is_safe);
        ctest_describe("label disabled blocks forward",
                       test_disabled_blocks_forward);

        lcui_destroy();
}
