/*
 * tests/integration/test_empty.c: -- Empty widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_text_setters_reuse_children(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *title = ui_create_empty_title();

        ui_widget_append(empty, title);
        ui_empty_set_title(empty, "Nothing here");
        ui_empty_set_description(empty, "Try changing your search.");

        ctest_equal_bool("should retain the explicit title",
                         ui_widget_get_child(empty, 0) == title, true);
        ctest_equal_bool("should append a description",
                         ui_check_widget_type(ui_widget_get_child(empty, 1),
                                              "empty-description"),
                         true);

        ui_widget_destroy(empty);
}

static void test_action_replaces_slot_content(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *first = ui_create_widget("button");
        ui_widget_t *second = ui_create_widget("button");
        ui_widget_t *slot;

        slot = ui_empty_set_action(empty, first);
        ctest_equal_bool("should append the first action",
                         ui_widget_get_child(slot, 0) == first, true);

        ui_empty_set_action(empty, second);
        ctest_equal_bool("should replace the previous action",
                         ui_widget_get_child(slot, 0) == second, true);

        ui_widget_destroy(empty);
}

static void test_setters_create_nested_structure(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *action = ui_create_widget("button");
        ui_widget_t *header;
        ui_widget_t *content;

        ui_empty_set_media(empty, "Empty state");
        ui_empty_set_title(empty, "No data");
        ui_empty_set_description(empty, "No data found");
        ui_empty_set_action(empty, action);

        header = ui_widget_get_child(empty, 0);
        content = ui_widget_get_child(empty, 1);
        ctest_equal_bool("should create an empty header",
                         ui_check_widget_type(header, "empty-header"), true);
        ctest_equal_bool(
            "should put media in the header",
            ui_check_widget_type(ui_widget_get_child(header, 0), "empty-media"),
            true);
        ctest_equal_bool(
            "should put the title in the header",
            ui_check_widget_type(ui_widget_get_child(header, 1), "empty-title"),
            true);
        ctest_equal_bool("should put the description in the header",
                         ui_check_widget_type(ui_widget_get_child(header, 2),
                                              "empty-description"),
                         true);
        ctest_equal_bool("should create empty content",
                         ui_check_widget_type(content, "empty-content"), true);
        ctest_equal_bool("should put the action in empty content",
                         ui_check_widget_type(ui_widget_get_child(content, 0),
                                              "empty-action"),
                         true);

        ui_widget_destroy(empty);
}

static void test_media_icon_variant(void)
{
        ui_widget_t *media = ui_create_empty_media();

        ui_empty_media_set_variant(media, UI_EMPTY_MEDIA_VARIANT_ICON);
        ctest_equal_bool("should set the icon status",
                         ui_widget_has_status(media, "icon"), true);

        ui_widget_destroy(media);
}

void test_empty(void)
{
        lcui_init();
        ctest_describe("empty text setters", test_text_setters_reuse_children);
        ctest_describe("empty action setter",
                       test_action_replaces_slot_content);
        ctest_describe("empty nested structure",
                       test_setters_create_nested_structure);
        ctest_describe("empty media icon variant", test_media_icon_variant);
        lcui_destroy();
}
