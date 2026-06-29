/*
 * tests/integration/test_radio_group.c
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

static void on_radio_group_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)e;
        (void)arg;
        ++g_change_count;
}

static void test_value_roundtrip(void)
{
        ui_widget_t *g = ui_create_radio_group();

        ctest_equal_str("should default to empty value",
                        ui_widget_get_attr(g, "value"), NULL);

        ui_radio_group_set_value(g, "comfortable");
        ctest_equal_str("should return set value",
                        ui_widget_get_attr(g, "value"), "comfortable");

        ui_radio_group_set_value(g, NULL);
        ctest_equal_str("should clear value when NULL",
                        ui_widget_get_attr(g, "value"), NULL);

        ui_widget_destroy(g);
}

static void test_item_checked_roundtrip(void)
{
        ui_widget_t *item = ui_create_radio_group_item();

        ctest_equal_int("should default to unchecked",
                        ui_radio_group_item_get_checked(item), 0);

        ui_radio_group_item_set_checked(item, 1);
        ctest_equal_int("should return 1 after set_checked(1)",
                        ui_radio_group_item_get_checked(item), 1);

        ui_radio_group_item_set_checked(item, 0);
        ctest_equal_int("should return 0 after set_checked(0)",
                        ui_radio_group_item_get_checked(item), 0);

        ui_widget_destroy(item);
}

static void test_click_selects_item(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_event_t ev;

        ui_widget_set_attr(item, "value", "default");
        ui_widget_append(g, item);
        ui_widget_append(ui_root(), g);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(item, ev, NULL);

        ctest_equal_int("clicked item should be checked",
                        ui_radio_group_item_get_checked(item), 1);
        ctest_equal_str("group value should match clicked item",
                        ui_radio_group_get_value(g), "default");

        ui_widget_destroy(g);
}

static void test_click_emits_change(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_event_t ev;

        g_change_count = 0;
        ui_widget_on(g, "change", on_radio_group_change, NULL);

        ui_widget_append(g, item);
        ui_widget_append(ui_root(), g);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(item, ev, NULL);

        ctest_equal_int("should emit change once on group", g_change_count, 1);

        ui_widget_destroy(g);
}

static void test_click_deselects_siblings(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item1 = ui_create_radio_group_item();
        ui_widget_t *item2 = ui_create_radio_group_item();
        ui_event_t ev;

        ui_widget_set_attr(item1, "value", "default");
        ui_widget_set_attr(item2, "value", "compact");
        ui_widget_append(g, item1);
        ui_widget_append(g, item2);
        ui_widget_append(ui_root(), g);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(item1, ev, NULL);
        ctest_equal_int("item1 checked after first click",
                        ui_radio_group_item_get_checked(item1), 1);
        ctest_equal_int("item2 still unchecked",
                        ui_radio_group_item_get_checked(item2), 0);

        ui_widget_emit_event(item2, ev, NULL);
        ctest_equal_int("item1 unchecked after second click",
                        ui_radio_group_item_get_checked(item1), 0);
        ctest_equal_int("item2 checked after second click",
                        ui_radio_group_item_get_checked(item2), 1);
        ctest_equal_str("group value tracks latest selection",
                        ui_radio_group_get_value(g), "compact");

        ui_widget_destroy(g);
}

static void test_disabled_item_blocks_click(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_event_t ev;

        ui_widget_append(g, item);
        ui_widget_append(ui_root(), g);
        ui_widget_set_disabled(item, true);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(item, ev, NULL);

        ctest_equal_int("disabled item should stay unchecked",
                        ui_radio_group_item_get_checked(item), 0);
        ctest_equal_str("group value should remain unset",
                        ui_radio_group_get_value(g), NULL);

        ui_widget_destroy(g);
}

static void test_click_through_wrapper_div(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *wrap = ui_create_widget(NULL);
        ui_widget_t *item = ui_create_radio_group_item();
        ui_event_t ev;

        ui_widget_set_attr(item, "value", "comfortable");
        ui_widget_append(wrap, item);
        ui_widget_append(g, wrap);
        ui_widget_append(ui_root(), g);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(item, ev, NULL);

        ctest_equal_int("item inside wrapper div still gets selected",
                        ui_radio_group_item_get_checked(item), 1);
        ctest_equal_str("group value updated from nested item",
                        ui_radio_group_get_value(g), "comfortable");

        ui_widget_destroy(g);
}

static void test_click_on_indicator_child(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_widget_t *indicator;
        ui_event_t ev;

        ui_widget_set_attr(item, "value", "compact");
        ui_widget_append(g, item);
        ui_widget_append(ui_root(), g);

        indicator = ui_widget_get_child(item, 0);

        ui_event_init(&ev, "click");
        ui_widget_emit_event(indicator, ev, NULL);

        ctest_equal_int("clicking indicator child should select item",
                        ui_radio_group_item_get_checked(item), 1);
        ctest_equal_str("group value updated from indicator click",
                        ui_radio_group_get_value(g), "compact");

        ui_widget_destroy(g);
}

static void test_update_syncs_items_to_value(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item1 = ui_create_radio_group_item();
        ui_widget_t *item2 = ui_create_radio_group_item();

        ui_widget_set_attr(item1, "value", "default");
        ui_widget_set_attr(item2, "value", "comfortable");
        ui_widget_append(g, item1);
        ui_widget_append(g, item2);
        ui_widget_append(ui_root(), g);

        ui_radio_group_set_value(g, "comfortable");
        ctest_equal_int("item matching value should be checked",
                        ui_radio_group_item_get_checked(item2), 1);
        ctest_equal_int("item not matching value should be unchecked",
                        ui_radio_group_item_get_checked(item1), 0);

        ui_radio_group_set_value(g, "default");
        ctest_equal_int("after value change, new match should be checked",
                        ui_radio_group_item_get_checked(item1), 1);
        ctest_equal_int("after value change, old match should be unchecked",
                        ui_radio_group_item_get_checked(item2), 0);

        ui_radio_group_set_value(g, NULL);
        ctest_equal_int("NULL value clears all checks",
                        ui_radio_group_item_get_checked(item1), 0);
        ctest_equal_int("NULL value clears all checks (other)",
                        ui_radio_group_item_get_checked(item2), 0);

        ui_widget_destroy(g);
}

static void test_update_finds_items_in_nested_wrappers(void)
{
        ui_widget_t *g = ui_create_radio_group();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_widget_t *wrap = ui_create_widget(NULL);

        ui_widget_set_attr(item, "value", "opt");
        ui_widget_append(wrap, item);
        ui_widget_append(g, wrap);
        ui_widget_append(ui_root(), g);

        ui_radio_group_set_value(g, "opt");
        ctest_equal_int("item nested in wrapper div should still be checked",
                        ui_radio_group_item_get_checked(item), 1);

        ui_radio_group_set_value(g, "other");
        ctest_equal_int(
            "item nested in wrapper div should be unchecked when value differs",
            ui_radio_group_item_get_checked(item), 0);

        ui_widget_destroy(g);
}

void test_radio_group(void)
{
        lcui_init();

        ctest_describe("radio-group value roundtrip", test_value_roundtrip);
        ctest_describe("radio-group-item checked roundtrip",
                       test_item_checked_roundtrip);
        ctest_describe("radio-group click selects item",
                       test_click_selects_item);
        ctest_describe("radio-group click emits change",
                       test_click_emits_change);
        ctest_describe("radio-group click deselects siblings",
                       test_click_deselects_siblings);
        ctest_describe("radio-group disabled item blocks click",
                       test_disabled_item_blocks_click);
        ctest_describe("radio-group click through wrapper div",
                       test_click_through_wrapper_div);
        ctest_describe("radio-group click on indicator child",
                       test_click_on_indicator_child);
        ctest_describe("radio-group update syncs items to value",
                       test_update_syncs_items_to_value);
        ctest_describe("radio-group update finds items in nested wrappers",
                       test_update_finds_items_in_nested_wrappers);

        lcui_destroy();
}
