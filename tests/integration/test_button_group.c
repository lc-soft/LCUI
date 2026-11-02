/*
 * tests/integration/test_button_group.c: -- Button group widget tests
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

static void test_factories(void)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *text = ui_create_button_group_text();
        ui_widget_t *separator = ui_create_button_group_separator();

        ctest_equal_str("group type", group->type, "button-group");
        ctest_equal_str("text type", text->type, "button-group-text");
        ctest_equal_str("separator type", separator->type,
                        "button-group-separator");
        ctest_equal_int("group defaults to horizontal",
                        ui_button_group_get_orientation(group),
                        UI_BUTTON_GROUP_HORIZONTAL);
        ctest_equal_bool("horizontal status",
                         ui_widget_has_status(group, "horizontal"), true);
        ctest_equal_int("separator defaults to vertical",
                        ui_button_group_separator_get_orientation(separator),
                        UI_BUTTON_GROUP_SEPARATOR_VERTICAL);
        ctest_equal_bool("separator vertical status",
                         ui_widget_has_status(separator, "vertical"), true);

        ui_button_group_set_orientation(group, UI_BUTTON_GROUP_VERTICAL);
        ctest_equal_int("group orientation setter",
                        ui_button_group_get_orientation(group),
                        UI_BUTTON_GROUP_VERTICAL);
        ctest_equal_bool("vertical status after setter",
                         ui_widget_has_status(group, "vertical"), true);
        ctest_equal_bool("horizontal status removed",
                         ui_widget_has_status(group, "horizontal"), false);

        ui_button_group_separator_set_orientation(
            separator, UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL);
        ctest_equal_int("separator orientation setter",
                        ui_button_group_separator_get_orientation(separator),
                        UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL);
        ctest_equal_bool("separator horizontal status",
                         ui_widget_has_status(separator, "horizontal"), true);

        ui_widget_destroy(group);
        ui_widget_destroy(text);
        ui_widget_destroy(separator);
        ui_clear_trash();
}

static void test_marking(void)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *a = ui_create_button();
        ui_widget_t *b = ui_create_button();

        ui_widget_append(group, a);
        ui_widget_append(group, b);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("first child is marked",
                         ui_widget_has_class(a, "button-group-item"), true);
        ctest_equal_bool("first child is horizontal",
                         ui_widget_has_class(a, "button-group-item-horizontal"),
                         true);
        ctest_equal_bool("second child is marked",
                         ui_widget_has_class(b, "button-group-item"), true);

        ui_button_group_set_orientation(group, UI_BUTTON_GROUP_VERTICAL);
        ui_update();
        ctest_equal_bool("orientation switch updates child class",
                         ui_widget_has_class(a, "button-group-item-vertical"),
                         true);
        ctest_equal_bool("old orientation class removed",
                         ui_widget_has_class(a, "button-group-item-horizontal"),
                         false);

        ui_widget_unlink(b);
        ui_update();
        ctest_equal_bool("unlink removes item class",
                         ui_widget_has_class(b, "button-group-item"), false);
        ctest_equal_bool("unlink removes orientation class",
                         ui_widget_has_class(b, "button-group-item-vertical"),
                         false);

        ui_widget_destroy(group);
        ui_widget_destroy(b);
        ui_clear_trash();
}

static void test_nested_group(void)
{
        ui_widget_t *outer = ui_create_button_group();
        ui_widget_t *inner = ui_create_button_group();
        ui_widget_t *button = ui_create_button();

        ui_widget_append(inner, button);
        ui_widget_append(outer, inner);
        ui_widget_append(ui_root(), outer);
        ui_update();
        ui_update();

        ctest_equal_bool("outer detects nested group",
                         ui_widget_has_status(outer, "has-group"), true);
        ctest_equal_bool("nested group is not a geometry item",
                         ui_widget_has_class(inner, "button-group-item"),
                         false);
        ctest_equal_float("nested group has no own border",
                          inner->computed_style.border_left_width, 0);
        ctest_equal_bool(
            "inner child keeps its own horizontal class",
            ui_widget_has_class(button, "button-group-item-horizontal"), true);

        ui_widget_destroy(outer);
        ui_clear_trash();
}

static void test_geometry(void)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *first = ui_create_button();
        ui_widget_t *middle = ui_create_button();
        ui_widget_t *last = ui_create_button();

        ui_widget_append(group, first);
        ui_widget_append(group, middle);
        ui_widget_append(group, last);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_float("first button keeps left radius",
                          first->computed_style.border_top_left_radius, 8);
        ctest_equal_float("middle button has no radius",
                          middle->computed_style.border_top_left_radius, 0);
        ctest_equal_float("last button keeps right radius",
                          last->computed_style.border_top_right_radius, 8);
        ctest_equal_bool("middle button has no left border",
                         middle->computed_style.border_left_width == 0, true);

        ui_button_group_set_orientation(group, UI_BUTTON_GROUP_VERTICAL);
        ui_update();
        ui_update();
        ctest_equal_float("vertical first keeps top radius",
                          first->computed_style.border_top_left_radius, 8);
        ctest_equal_float("vertical middle has no radius",
                          middle->computed_style.border_top_left_radius, 0);
        ctest_equal_float("vertical last keeps bottom radius",
                          last->computed_style.border_bottom_left_radius, 8);
        ctest_equal_bool("vertical middle has no top border",
                         middle->computed_style.border_top_width == 0, true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_input_flex(void)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("textinput grows inside the group",
                         input->computed_style.flex_grow > 0, true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_embed_wrapper(void)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *select = ui_create_select();
        ui_widget_t *send = ui_create_button();
        ui_widget_t *trigger;
        ui_widget_t *button;

        ui_select_add_item(select, "$ US Dollar", "$");
        ui_widget_append(group, select);
        ui_widget_append(group, send);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();
        ui_update();

        trigger = ui_widget_get_child(select, 0);
        button = trigger ? ui_widget_get_child(trigger, 0) : NULL;

        ctest_equal_bool("wrapper gets embed class",
                         ui_widget_has_class(select, "button-group-embed"),
                         true);
        ctest_equal_bool("wrapper is not a geometry item",
                         ui_widget_has_class(select, "button-group-item"),
                         false);
        ctest_equal_float("wrapper has no own border",
                          select->computed_style.border_left_width, 0);
        ctest_equal_bool(
            "trigger keeps its button inside",
            button && trigger &&
                button->border_box.width <= trigger->border_box.width,
            true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

void test_button_group(void)
{
        lcui_init();
        ctest_describe("button group factories", test_factories);
        ctest_describe("button group marking", test_marking);
        ctest_describe("button group nested", test_nested_group);
        ctest_describe("button group geometry", test_geometry);
        ctest_describe("button group input flex", test_input_flex);
        ctest_describe("button group embed wrapper", test_embed_wrapper);
        lcui_destroy();
}
