/*
 * tests/integration/test_collapsible.c: -- Collapsible widget tests
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

static void emit_click(ui_widget_t *target)
{
        ui_event_t e;

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void test_default_state(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();

        ctest_equal_bool("should start closed",
                         ui_collapsible_get_open(collapsible), false);
        ctest_equal_bool("should add the closed status",
                         ui_widget_has_status(collapsible, "closed"), true);
        ctest_equal_bool("should not add the open status",
                         ui_widget_has_status(collapsible, "open"), false);

        ui_widget_destroy(collapsible);
}

static void test_default_open_and_setter(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();

        ui_widget_set_attr(collapsible, "default-open", "true");
        ctest_equal_bool("default-open should open the panel",
                         ui_collapsible_get_open(collapsible), true);
        ctest_equal_bool("default-open should add the open status",
                         ui_widget_has_status(collapsible, "open"), true);

        ui_widget_set_attr(collapsible, "open", "false");
        ctest_equal_bool("explicit open should override default-open",
                         ui_collapsible_get_open(collapsible), false);
        ctest_equal_bool("explicit closed should restore the closed status",
                         ui_widget_has_status(collapsible, "closed"), true);

        ui_collapsible_set_open(collapsible, true);
        ctest_equal_bool("setter should open the panel",
                         ui_collapsible_get_open(collapsible), true);
        ctest_equal_bool("setter should replace the closed status",
                         ui_widget_has_status(collapsible, "open"), true);
        ctest_equal_bool("setter should remove the closed status",
                         ui_widget_has_status(collapsible, "closed"), false);

        ui_widget_destroy(collapsible);
}

static void test_trigger_click(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *content = ui_create_collapsible_content();
        ui_widget_t *label = ui_create_widget("text");

        ui_text_set_content(label, "Toggle");
        ui_widget_append(trigger, label);
        ui_widget_append(collapsible, trigger);
        ui_widget_append(collapsible, content);
        ui_widget_append(ui_root(), collapsible);
        ui_update();

        ctest_equal_bool("content should start hidden",
                         ui_widget_is_visible(content), false);

        emit_click(label);
        ui_update();
        ctest_equal_bool("clicking the trigger should open the panel",
                         ui_collapsible_get_open(collapsible), true);
        ctest_equal_bool("opened content should be visible",
                         ui_widget_is_visible(content), true);

        emit_click(content);
        ui_update();
        ctest_equal_bool("clicking content should not toggle the panel",
                         ui_collapsible_get_open(collapsible), true);

        emit_click(label);
        ui_update();
        ctest_equal_bool("clicking the trigger again should close the panel",
                         ui_collapsible_get_open(collapsible), false);
        ctest_equal_bool("closed content should be hidden",
                         ui_widget_is_visible(content), false);

        ui_widget_destroy(collapsible);
}

static void test_data_trigger(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_widget(NULL);
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *sibling = ui_create_widget("text");

        ui_text_set_content(label, "Toggle");
        ui_text_set_content(sibling, "Sibling");
        ui_widget_set_attr(trigger, "data-collapsible-trigger", "true");
        ui_widget_append(trigger, icon);
        ui_widget_append(trigger, label);
        ui_widget_append(collapsible, trigger);
        ui_widget_append(collapsible, sibling);
        ui_widget_append(ui_root(), collapsible);
        ui_update();

        emit_click(label);
        ui_update();
        ctest_equal_bool("a data trigger should open the panel",
                         ui_collapsible_get_open(collapsible), true);

        emit_click(icon);
        ui_update();
        ctest_equal_bool("a trigger icon should toggle the panel",
                         ui_collapsible_get_open(collapsible), false);

        emit_click(sibling);
        ui_update();
        ctest_equal_bool("a non-trigger sibling should not toggle the panel",
                         ui_collapsible_get_open(collapsible), false);

        ui_widget_destroy(collapsible);
}

static void test_nested(void)
{
        ui_widget_t *outer = ui_create_collapsible();
        ui_widget_t *inner = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_widget(NULL);
        ui_widget_t *label = ui_create_widget("text");

        ui_text_set_content(label, "Inner");
        ui_widget_set_attr(trigger, "data-collapsible-trigger", "true");
        ui_widget_append(trigger, label);
        ui_widget_append(inner, trigger);
        ui_widget_append(outer, inner);
        ui_widget_append(ui_root(), outer);
        ui_update();

        emit_click(label);
        ui_update();

        ctest_equal_bool("inner collapsible should open",
                         ui_collapsible_get_open(inner), true);
        ctest_equal_bool("outer collapsible should stay closed",
                         ui_collapsible_get_open(outer), false);

        ui_widget_destroy(outer);
}

static int open_count;
static int close_count;

static void on_open(ui_widget_t *w, ui_event_t *e, void *arg)
{
        open_count++;
}

static void on_close(ui_widget_t *w, ui_event_t *e, void *arg)
{
        close_count++;
}

static void test_events(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();

        open_count = 0;
        close_count = 0;
        ui_widget_on(collapsible, "open", on_open, NULL);
        ui_widget_on(collapsible, "close", on_close, NULL);

        ui_collapsible_set_open(collapsible, true);
        ui_collapsible_set_open(collapsible, false);

        ctest_equal_int("opening should emit one open event", open_count, 1);
        ctest_equal_int("closing should emit one close event", close_count, 1);

        ui_widget_destroy(collapsible);
}

void test_collapsible(void)
{
        lcui_init();

        ctest_describe("collapsible state", test_default_state);
        ctest_describe("collapsible default open",
                       test_default_open_and_setter);
        ctest_describe("collapsible trigger", test_trigger_click);
        ctest_describe("collapsible data trigger", test_data_trigger);
        ctest_describe("collapsible nested", test_nested);
        ctest_describe("collapsible events", test_events);

        lcui_destroy();
}
