/*
 * tests/integration/test_select.c
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
#include <stdio.h>
#include <wchar.h>

static void test_select_builds_owner_children(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *content;

        ui_widget_append(ui_root(), select);
        ui_select_set_placeholder(select, "Choose");
        ui_select_build_children(select);
        trigger = ui_widget_get_child(select, 0);
        content = ui_widget_get_child(select, 1);
        button = ui_widget_get_child(trigger, 0);
        ui_update();

        ctest_equal_int("select should own trigger", trigger->parent == select,
                        1);
        ctest_equal_int("trigger should contain a real button",
                        button && button->proto != NULL, 1);
        ctest_equal_int(
            "trigger should have a non-zero layout",
            trigger->border_box.width > 0 && trigger->border_box.height > 0, 1);
        ctest_equal_int("select should own content before opening",
                        content && content->parent == select, 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_transfers_content_on_open(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *content;
        ui_widget_t *portal;
        ui_widget_t *trigger;
        ui_widget_t *button;

        ui_widget_append(ui_root(), select);
        ui_select_build_children(select);
        ui_select_set_align_item_with_trigger(select, false);
        trigger = ui_widget_get_child(select, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_select_add_item(select, "Light", "light");
        ui_update();
        ui_select_open(select);
        portal = ui_widget_get_child(select, 1);
        content = ui_portal_get_content(portal);
        ui_update();

        ctest_equal_int("open should mount content in portal root",
                        content->parent == ui_get_portal_root(), 1);
        ctest_equal_int("portal should manage content",
                        ui_portal_get_content(portal) == content, 1);
        ctest_equal_float("content should align with trigger",
                          content->border_box.x, button->border_box.x);
        ctest_equal_float("content should appear below trigger",
                          content->border_box.y,
                          button->border_box.y + button->border_box.height);
        ui_select_close(select);
        ui_widget_remove(select);
        ui_clear_trash();
}

static void test_select_manages_items(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *item;
        ui_widget_t *trigger;
        ui_widget_t *value;
        wchar_t text[32];

        item = ui_select_add_item(select, "Light", "light");
        ctest_equal_str("added item should retain value",
                        ui_widget_get_attr(item, "value"), "light");
        ui_select_set_value(select, "light");
        ctest_equal_str("select should retain selected value",
                        ui_select_get_value(select), "light");
        trigger = ui_widget_get_child(select, 0);
        value = ui_widget_get_child(ui_widget_get_child(trigger, 0), 0);
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("value should show selected label",
                        wcscmp(text, L"Light") == 0, 1);
        ui_select_set_value(select, NULL);
        ctest_equal_str("select should clear selected value",
                        ui_select_get_value(select), NULL);
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("value should show placeholder when cleared",
                        wcscmp(text, L"") == 0, 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_item_labels_and_missing_values(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *with_value;
        ui_widget_t *without_value;
        wchar_t text[32];

        with_value = ui_select_add_item(select, "Light", "light");
        without_value = ui_select_add_item(select, "System", NULL);
        ctest_equal_str("item should retain value",
                        ui_widget_get_attr(with_value, "value"), "light");
        ui_text_get_content_w(without_value, text,
                              sizeof(text) / sizeof(*text));
        ctest_equal_int("item without value should retain label",
                        wcscmp(text, L"System") == 0, 1);
        ctest_equal_int("item without value should not gain value",
                        ui_widget_get_attr(without_value, "value") == NULL, 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_binds_appended_descendants(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *wrapper = ui_create_widget(NULL);
        ui_widget_t *trigger = ui_create_select_trigger();
        ui_widget_t *content = ui_create_select_content();

        ui_widget_append(wrapper, trigger);
        ui_widget_append(wrapper, content);
        ui_widget_append(select, wrapper);
        ui_select_build_children(select);

        ctest_equal_int("appended trigger should stay under select",
                        trigger->parent == wrapper, 1);
        ctest_equal_int("appended content should stay under select",
                        content->parent == wrapper, 1);
        ui_select_open(select);
        ctest_equal_int("opened content should leave owner tree",
                        content->parent == ui_get_portal_root(), 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_reopens_after_outside_click(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger;
        ui_event_t event;

        ui_widget_append(ui_root(), select);
        ui_select_build_children(select);
        trigger = ui_widget_get_child(ui_widget_get_child(select, 0), 0);
        ui_select_open(select);
        ui_event_init(&event, "click");
        event.target = ui_root();
        ui_widget_emit_event(ui_root(), event, NULL);
        ui_widget_emit_event(trigger, event, NULL);
        ctest_equal_int(
            "trigger should reopen after outside close",
            ui_portal_get_content(ui_widget_get_child(select, 1))->parent ==
                ui_get_portal_root(),
            1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_binds_plain_data_widgets(void)
{
        ui_widget_t *trigger = ui_create_widget("button");
        ui_widget_t *value = ui_create_widget("button");
        ui_widget_t *content = ui_create_select_content();
        ui_widget_t *item = ui_create_widget("button");
        ui_widget_t *fallback_item = ui_create_widget("button");
        ui_event_t event;
        wchar_t text[32];

        ui_widget_set_attr(trigger, "data-select-trigger", "plain");
        ui_widget_set_attr(trigger, "align-item-with-trigger", "false");
        ui_widget_set_attr(value, "data-select-value", "plain");
        ui_widget_set_attr(value, "data-select-placeholder", "Choose");
        ui_widget_set_attr(content, "data-select-target", "plain");
        ui_widget_set_attr(item, "data-select-item", "true");
        ui_widget_set_attr(item, "value", "dark");
        ui_widget_set_attr(item, "title", "Dark theme");
        ui_widget_set_attr(fallback_item, "data-select-item", "true");
        ui_widget_set_attr(fallback_item, "value", "system");
        ui_widget_append(ui_root(), trigger);
        ui_widget_append(ui_root(), value);
        ui_widget_append(ui_root(), content);
        ui_widget_append(content, item);
        ui_widget_append(content, fallback_item);
        ui_update();
        ctest_equal_int("plain setup", 1, 1);
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("plain value should use placeholder fallback",
                        wcscmp(text, L"Choose") == 0, 1);

        ui_event_init(&event, "click");
        event.target = trigger;
        ui_widget_emit_event(trigger, event, NULL);
        ctest_equal_int("plain trigger emit", 1, 1);
        ui_update();
        ctest_equal_int("plain trigger should open its target",
                        content->parent == ui_get_portal_root(), 1);
        ctest_equal_float("plain target should align with trigger",
                          content->border_box.x, trigger->border_box.x);
        ctest_equal_float("plain target should appear below trigger",
                          content->border_box.y,
                          trigger->border_box.y + trigger->border_box.height);

        event.target = item;
        ui_widget_emit_event(content, event, NULL);
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("plain value should use item title",
                        wcscmp(text, L"Dark theme") == 0, 1);

        event.target = fallback_item;
        ui_widget_emit_event(content, event, NULL);
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("plain value should fall back to item value",
                        wcscmp(text, L"system") == 0, 1);
        ctest_equal_str("plain select should write the selected value",
                        ui_widget_get_attr(ui_root(), "value"), "system");

        /* Root owns these widgets; let lcui_destroy tear down the tree. */
}

static void test_select_trigger_arrow_and_content_width(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *value;
        ui_widget_t *arrow;
        ui_widget_t *portal;
        ui_widget_t *content;
        float trigger_width_before_open;

        ui_widget_append(ui_root(), select);
        ui_select_set_placeholder(select, "Select a fruit");
        ui_select_add_item(select, "Apple", "apple");
        ui_select_add_item(select, "Banana", "banana");
        ui_select_add_item(select, "Blueberry", "blueberry");
        ui_update();
        trigger = ui_widget_get_child(select, 0);
        button = ui_widget_get_child(trigger, 0);
        value = ui_widget_get_child(button, 0);
        arrow = ui_widget_get_child(button, 1);
        ctest_equal_int("select trigger should include value widget",
                        value != NULL, 1);
        ctest_equal_int("select trigger should include arrow widget",
                        arrow != NULL, 1);
        ctest_equal_int("select trigger button should keep minimum width",
                        button->border_box.width >= 176.0f, 1);
        trigger_width_before_open = trigger->border_box.width;
        ui_select_open(select);
        ui_update();
        portal = ui_widget_get_child(select, 1);
        content = ui_portal_get_content(portal);
        ctest_equal_float("select trigger width should match inner button",
                          trigger->border_box.width, button->border_box.width);
        ctest_equal_float("select content should match trigger width",
                          content->border_box.width, trigger->border_box.width);
        ctest_equal_float("select trigger width should stay stable on open",
                          trigger->border_box.width, trigger_width_before_open);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_groups_sync_value(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger;
        ui_widget_t *value;
        ui_widget_t *content;
        ui_widget_t *fruits;
        ui_widget_t *vegetables;
        ui_widget_t *apple;
        ui_widget_t *carrot;
        wchar_t text[32];

        ui_widget_append(ui_root(), select);
        ui_select_build_children(select);
        trigger = ui_widget_get_child(select, 0);
        content = ui_widget_get_child(select, 1);
        value = ui_widget_get_child(ui_widget_get_child(trigger, 0), 0);

        fruits = ui_create_select_group();
        ui_widget_append(fruits, ui_create_select_label());
        apple = ui_create_select_item("apple");
        ui_text_set_content(apple, "Apple");
        ui_widget_append(fruits, apple);

        vegetables = ui_create_select_group();
        ui_widget_append(vegetables, ui_create_select_label());
        carrot = ui_create_select_item("carrot");
        ui_text_set_content(carrot, "Carrot");
        ui_widget_append(vegetables, carrot);

        ui_widget_append(content, fruits);
        ui_widget_append(content, ui_create_select_separator());
        ui_widget_append(content, vegetables);

        ui_select_set_value(select, "carrot");
        ui_text_get_content_w(value, text, sizeof(text) / sizeof(*text));
        ctest_equal_int("select should read label from a nested group item",
                        wcscmp(text, L"Carrot") == 0, 1);
        ctest_equal_str("nested selected item should be checked",
                        ui_widget_get_attr(carrot, "checked"), "true");
        ctest_equal_str("nested unselected item should not be checked",
                        ui_widget_get_attr(apple, "checked"), "false");
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_disabled_does_not_open(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *content;
        ui_event_t event;

        ui_widget_append(ui_root(), select);
        ui_select_add_item(select, "Light", "light");
        trigger = ui_widget_get_child(select, 0);
        button = ui_widget_get_child(trigger, 0);
        content = ui_widget_get_child(select, 1);
        ui_widget_set_disabled(select, true);
        ui_update();
        ui_event_init(&event, "click");
        event.target = button;
        ui_widget_emit_event(button, event, NULL);
        ui_update();
        ctest_equal_int("disabled select should keep content in place",
                        content->parent == select, 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void test_select_disabled_item_is_ignored(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *content;
        ui_widget_t *item;
        ui_event_t event;

        ui_widget_append(ui_root(), select);
        item = ui_select_add_item(select, "Grapes", "grapes");
        content = ui_widget_get_child(select, 1);
        ui_widget_set_disabled(item, true);
        ui_update();
        ui_event_init(&event, "click");
        event.target = item;
        ui_widget_emit_event(content, event, NULL);
        ctest_equal_int("disabled item click should not set a value",
                        ui_select_get_value(select) == NULL, 1);
        ui_widget_destroy(select);
        ui_clear_trash();
}

static void add_numbered_items(ui_widget_t *select, int count)
{
        int i;
        char label[16];
        char value[16];

        for (i = 0; i < count; ++i) {
                snprintf(label, sizeof(label), "Item %d", i);
                snprintf(value, sizeof(value), "v%d", i);
                ui_select_add_item(select, label, value);
        }
}

static void settle_frames(void)
{
        int i;

        for (i = 0; i < 6; ++i) {
                ui_update();
        }
}

static void test_select_content_scrolls(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *content;

        ui_widget_resize(ui_root(), 400, 200);
        ui_widget_append(ui_root(), select);
        add_numbered_items(select, 30);
        ui_update();
        ui_select_open(select);
        settle_frames();
        content = ui_portal_get_content(ui_widget_get_child(select, 1));
        ctest_equal_int("overflowing content should be scrollable",
                        ui_scrollarea_get_scroll_height(content) >
                            content->content_box.height + 0.5f,
                        1);
        ui_scrollarea_set_scroll_top(content, 100000.0f);
        ctest_equal_int("scroll position should clamp into a positive range",
                        ui_scrollarea_get_scroll_top(content) > 0.5f, 1);
        ui_select_close(select);
        ui_widget_remove(select);
        ui_clear_trash();
        ui_widget_resize(ui_root(), 800, 600);
}

static void test_select_flips_above_near_bottom(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *spacer = ui_create_widget(NULL);
        ui_widget_t *portal;

        ui_widget_resize(ui_root(), 400, 200);
        ui_widget_set_style_string(spacer, "height", "150px");
        ui_widget_append(ui_root(), spacer);
        ui_widget_append(ui_root(), select);
        add_numbered_items(select, 20);
        ui_update();
        ui_select_open(select);
        settle_frames();
        portal = ui_widget_get_child(select, 1);
        ctest_equal_str("popup should flip above near the bottom edge",
                        ui_widget_get_attr(portal, "side"), "top");
        ui_select_close(select);
        ui_widget_remove(select);
        ui_widget_remove(spacer);
        ui_clear_trash();
        ui_widget_resize(ui_root(), 800, 600);
}

static void test_select_aligns_selected_item(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *spacer = ui_create_widget(NULL);
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *item = NULL;
        float ix, iy, ax, ay;
        float item_center, anchor_center;
        int i;
        char label[16];
        char value[16];

        ui_widget_resize(ui_root(), 400, 600);
        ui_widget_set_style_string(spacer, "height", "250px");
        ui_widget_append(ui_root(), spacer);
        ui_widget_append(ui_root(), select);
        for (i = 0; i < 8; ++i) {
                ui_widget_t *added;

                snprintf(label, sizeof(label), "Item %d", i);
                snprintf(value, sizeof(value), "v%d", i);
                added = ui_select_add_item(select, label, value);
                if (i == 4) {
                        item = added;
                }
        }
        ui_select_set_value(select, "v4");
        ui_update();
        trigger = ui_widget_get_child(select, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_select_open(select);
        settle_frames();
        ui_widget_get_offset(item, ui_root(), &ix, &iy);
        ui_widget_get_offset(button, ui_root(), &ax, &ay);
        item_center = iy + item->border_box.height / 2;
        anchor_center = ay + button->border_box.height / 2;
        ctest_equal_int("selected item should overlap the trigger",
                        item_center - anchor_center < 2.0f &&
                            anchor_center - item_center < 2.0f,
                        1);
        ui_select_close(select);
        ui_widget_remove(select);
        ui_widget_remove(spacer);
        ui_clear_trash();
        ui_widget_resize(ui_root(), 800, 600);
}

static void test_select_aligns_first_item_without_value(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *spacer = ui_create_widget(NULL);
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *first = NULL;
        float ix, iy, ax, ay;
        float item_center, anchor_center;
        int i;
        char label[16];
        char value[16];

        ui_widget_resize(ui_root(), 400, 600);
        ui_widget_set_style_string(spacer, "height", "250px");
        ui_widget_append(ui_root(), spacer);
        ui_widget_append(ui_root(), select);
        for (i = 0; i < 8; ++i) {
                ui_widget_t *added;

                snprintf(label, sizeof(label), "Item %d", i);
                snprintf(value, sizeof(value), "v%d", i);
                added = ui_select_add_item(select, label, value);
                if (i == 0) {
                        first = added;
                }
        }
        ui_update();
        trigger = ui_widget_get_child(select, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_select_open(select);
        settle_frames();
        ui_widget_get_offset(first, ui_root(), &ix, &iy);
        ui_widget_get_offset(button, ui_root(), &ax, &ay);
        item_center = iy + first->border_box.height / 2;
        anchor_center = ay + button->border_box.height / 2;
        ctest_equal_int("first item should overlap the trigger without a value",
                        item_center - anchor_center < 2.0f &&
                            anchor_center - item_center < 2.0f,
                        1);
        ui_select_close(select);
        ui_widget_remove(select);
        ui_widget_remove(spacer);
        ui_clear_trash();
        ui_widget_resize(ui_root(), 800, 600);
}

static void test_select_highlights_selected_item(void)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *item1 = NULL;
        ui_widget_t *item3 = NULL;
        ui_widget_t *indicator;
        int i;
        char label[16];
        char value[16];

        ui_widget_resize(ui_root(), 400, 600);
        ui_widget_append(ui_root(), select);
        for (i = 0; i < 5; ++i) {
                ui_widget_t *added;

                snprintf(label, sizeof(label), "Item %d", i);
                snprintf(value, sizeof(value), "v%d", i);
                added = ui_select_add_item(select, label, value);
                if (i == 1) {
                        item1 = added;
                }
                if (i == 3) {
                        item3 = added;
                }
        }
        indicator = ui_widget_get_child(item1, 0);
        ctest_equal_int(
            "select item should own a check indicator",
            indicator != NULL && ui_check_widget_type(indicator, "icon"), 1);
        ui_select_set_value(select, "v1");
        ui_update();
        ui_select_open(select);
        settle_frames();
        ctest_equal_int("selected item should be highlighted",
                        ui_widget_has_status(item1, "highlighted"), 1);
        ctest_equal_int("unselected item should not be highlighted",
                        ui_widget_has_status(item3, "highlighted"), 0);
        ctest_equal_int("selected item should be checked",
                        ui_widget_has_status(item1, "checked"), 1);
        ui_select_close(select);
        ctest_equal_int("highlight should clear on close",
                        ui_widget_has_status(item1, "highlighted"), 0);
        ui_widget_remove(select);
        ui_clear_trash();
        ui_widget_resize(ui_root(), 800, 600);
}

void test_select(void)
{
        lcui_init();
        ctest_describe("select builds owner children",
                       test_select_builds_owner_children);
        ctest_describe("select transfers content on open",
                       test_select_transfers_content_on_open);
        ctest_describe("select manages items", test_select_manages_items);
        ctest_describe("select item labels and missing values",
                       test_select_item_labels_and_missing_values);
        ctest_describe("select binds appended descendants",
                       test_select_binds_appended_descendants);
        ctest_describe("select reopens after outside click",
                       test_select_reopens_after_outside_click);
        ctest_describe("select trigger arrow and content width",
                       test_select_trigger_arrow_and_content_width);
        ctest_describe("select plain data widgets",
                       test_select_binds_plain_data_widgets);
        ctest_describe("select groups sync value",
                       test_select_groups_sync_value);
        ctest_describe("select disabled does not open",
                       test_select_disabled_does_not_open);
        ctest_describe("select disabled item is ignored",
                       test_select_disabled_item_is_ignored);
        ctest_describe("select content scrolls", test_select_content_scrolls);
        ctest_describe("select flips above near bottom",
                       test_select_flips_above_near_bottom);
        ctest_describe("select aligns selected item",
                       test_select_aligns_selected_item);
        ctest_describe("select aligns first item without value",
                       test_select_aligns_first_item_without_value);
        ctest_describe("select highlights selected item",
                       test_select_highlights_selected_item);
        lcui_destroy();
}
