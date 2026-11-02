/*
 * tests/integration/test_input_group.c: -- Input group widget tests
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
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *text = ui_create_input_group_text();
        ui_widget_t *kbd = ui_create_input_group_kbd();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *textarea = ui_create_input_group_textarea();
        ui_widget_t *button = ui_create_input_group_button();
        ui_widget_t *custom = ui_create_widget(NULL);

        ctest_equal_str("group type", group->type, "input-group");
        ctest_equal_str("addon type", addon->type, "input-group-addon");
        ctest_equal_str("text type", text->type, "input-group-text");
        ctest_equal_str("kbd type", kbd->type, "input-group-kbd");
        ctest_equal_str("input type", input->type, "textinput");
        ctest_equal_bool("input has control class",
                         ui_widget_has_class(input, "input-group-control"),
                         true);
        ctest_equal_bool("input has input class",
                         ui_widget_has_class(input, "input-group-input"), true);
        ctest_equal_str("textarea type", textarea->type, "textinput");
        ctest_equal_bool("textarea has control class",
                         ui_widget_has_class(textarea, "input-group-control"),
                         true);
        ctest_equal_bool("textarea has textarea class",
                         ui_widget_has_class(textarea, "input-group-textarea"),
                         true);
        ctest_equal_str("button type", button->type, "button");
        ctest_equal_int("button defaults to ghost",
                        ui_button_get_variant(button), UI_BUTTON_VARIANT_GHOST);
        ctest_equal_int("button defaults to xs", ui_button_get_size(button),
                        UI_BUTTON_SIZE_XS);
        ctest_equal_bool("button has group class",
                         ui_widget_has_class(button, "input-group-button"),
                         true);

        ui_input_group_mark_control(custom);
        ctest_equal_bool("mark_control adds the contract class",
                         ui_widget_has_class(custom, "input-group-control"),
                         true);

        ui_widget_destroy(group);
        ui_widget_destroy(addon);
        ui_widget_destroy(text);
        ui_widget_destroy(kbd);
        ui_widget_destroy(input);
        ui_widget_destroy(textarea);
        ui_widget_destroy(button);
        ui_widget_destroy(custom);
        ui_clear_trash();
}

static void test_addon_align(void)
{
        ui_widget_t *addon = ui_create_input_group_addon();

        ctest_equal_int("addon defaults to inline-start",
                        ui_input_group_addon_get_align(addon),
                        UI_INPUT_GROUP_ADDON_INLINE_START);
        ctest_equal_bool("default align status",
                         ui_widget_has_status(addon, "align-inline-start"),
                         true);

        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ctest_equal_int("setter updates align",
                        ui_input_group_addon_get_align(addon),
                        UI_INPUT_GROUP_ADDON_BLOCK_END);
        ctest_equal_bool("old align status removed",
                         ui_widget_has_status(addon, "align-inline-start"),
                         false);
        ctest_equal_bool("new align status added",
                         ui_widget_has_status(addon, "align-block-end"), true);

        ui_widget_destroy(addon);
        ui_clear_trash();
}

static void test_inline_order(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *start = ui_create_input_group_addon();
        ui_widget_t *end = ui_create_input_group_addon();

        ui_input_group_addon_set_align(start,
                                       UI_INPUT_GROUP_ADDON_INLINE_START);
        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(group, input);
        ui_widget_append(group, end);
        ui_widget_append(group, start);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("inline-start addon moves before input",
                         ui_widget_get_child(group, 0) == start, true);
        ctest_equal_bool("input stays in the middle",
                         ui_widget_get_child(group, 1) == input, true);
        ctest_equal_bool("inline-end addon moves after input",
                         ui_widget_get_child(group, 2) == end, true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_block_layout(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *start = ui_create_input_group_addon();
        ui_widget_t *end = ui_create_input_group_addon();

        ui_input_group_addon_set_align(start, UI_INPUT_GROUP_ADDON_BLOCK_START);
        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_widget_append(group, input);
        ui_widget_append(group, end);
        ui_widget_append(group, start);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("block-start addon is first",
                         ui_widget_get_child(group, 0) == start, true);
        ctest_equal_bool("block-end addon is last",
                         ui_widget_get_child(group, 2) == end, true);
        ctest_equal_bool("group gets block status",
                         ui_widget_has_status(group, "block"), true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_multiline_status(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *textarea = ui_create_input_group_textarea();

        ui_widget_append(group, textarea);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("textarea adds multiline status",
                         ui_widget_has_status(group, "multiline"), true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void emit_click(ui_widget_t *target)
{
        ui_event_t e;

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void test_focus_status(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *outside = ui_create_widget("textinput");

        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_widget_append(ui_root(), outside);
        ui_update();
        ui_update();

        ui_set_focus(input);
        ui_update();
        ctest_equal_bool("input focus adds focused status",
                         ui_widget_has_status(group, "focused"), true);

        ui_set_focus(outside);
        ui_update();
        ctest_equal_bool("focus outside removes focused status",
                         ui_widget_has_status(group, "focused"), false);

        ui_widget_destroy(group);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static void test_addon_focus_delegation(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *button = ui_create_input_group_button();
        ui_widget_t *outside = ui_create_widget("textinput");

        ui_text_set_content(label, "Search");
        ui_widget_append(addon, label);
        ui_widget_append(addon, button);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(ui_root(), group);
        ui_widget_append(ui_root(), outside);
        ui_update();
        ui_update();

        emit_click(label);
        ctest_equal_bool("clicking addon focuses the input",
                         ui_get_focus() == input, true);

        ui_set_focus(outside);
        emit_click(button);
        ctest_equal_bool("clicking an addon button keeps focus off the input",
                         ui_get_focus() == input, false);

        ui_widget_destroy(group);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static void test_disabled_status(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();

        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ui_widget_set_disabled(input, true);
        ui_update();
        ctest_equal_bool("disabled input adds has-disabled",
                         ui_widget_has_status(group, "has-disabled"), true);

        ui_widget_set_disabled(input, false);
        ui_update();
        ctest_equal_bool("re-enabled input removes has-disabled",
                         ui_widget_has_status(group, "has-disabled"), false);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_theme(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *button = ui_create_input_group_button();

        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(group, button);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("input drops its own border",
                         input->computed_style.border_top_width == 0, true);
        ctest_equal_bool("group keeps its rounded corner",
                         group->computed_style.border_top_left_radius > 0,
                         true);
        ctest_equal_float("input group button uses an 8px radius",
                          button->computed_style.border_top_left_radius, 8);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_addon_wrapper_focus(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *wrapper = ui_create_widget(NULL);
        ui_widget_t *label = ui_create_widget("text");

        ui_text_set_content(label, "Search");
        ui_widget_append(wrapper, label);
        ui_widget_append(addon, wrapper);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        emit_click(label);
        ctest_equal_bool("wrapper click focuses the input",
                         ui_get_focus() == input, true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_runtime_align(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *start = ui_create_input_group_addon();
        ui_widget_t *end = ui_create_input_group_addon();

        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(group, input);
        ui_widget_append(group, end);
        ui_widget_append(group, start);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ui_input_group_addon_set_align(start, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_update();
        ui_update();
        ctest_equal_bool("runtime align moves the addon after input",
                         ui_widget_get_child(group, 0) == input, true);
        ctest_equal_bool("runtime align keeps addon order",
                         ui_widget_get_child(group, 1) == start &&
                             ui_widget_get_child(group, 2) == end,
                         true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_mark_control(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *custom = ui_create_widget(NULL);

        custom->tab_index = 0;
        ui_input_group_mark_control(custom);
        ui_text_set_content(label, "Focus");
        ui_widget_append(addon, label);
        ui_widget_append(group, custom);
        ui_widget_append(group, addon);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();

        ctest_equal_bool("marked control grows inside the group",
                         custom->computed_style.flex_grow > 0, true);
        emit_click(label);
        ctest_equal_bool("addon click focuses the marked control",
                         ui_get_focus() == custom, true);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_root_disabled_focus(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *outside = ui_create_widget("textinput");

        ui_text_set_content(label, "Focus");
        ui_widget_append(addon, label);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_set_disabled(group, true);
        ui_widget_append(ui_root(), group);
        ui_widget_append(ui_root(), outside);
        ui_update();
        ui_update();

        ui_set_focus(outside);
        emit_click(label);
        ctest_equal_bool("disabled group addon does not steal focus",
                         ui_get_focus() == outside, true);

        ui_widget_destroy(group);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static void test_empty_group_clears_disabled(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();

        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();
        ui_widget_set_disabled(input, true);
        ui_update();
        ctest_equal_bool("disabled input marks the group",
                         ui_widget_has_status(group, "has-disabled"), true);

        ui_widget_destroy(input);
        ui_update();
        ctest_equal_bool("empty group clears has-disabled",
                         ui_widget_has_status(group, "has-disabled"), false);

        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_focus_destroyed(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();

        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_update();
        ui_update();
        ui_set_focus(input);
        ui_update();
        ctest_equal_bool("focused before destroy",
                         ui_widget_has_status(group, "focused"), true);

        ui_widget_destroy(input);
        ui_update();
        ctest_equal_bool("destroying the focused control clears focused",
                         ui_widget_has_status(group, "focused"), false);

        ui_widget_destroy(group);
        ui_clear_trash();
}

void test_input_group(void)
{
        lcui_init();
        ctest_describe("input group factories", test_factories);
        ctest_describe("input group addon align", test_addon_align);
        ctest_describe("input group inline order", test_inline_order);
        ctest_describe("input group block layout", test_block_layout);
        ctest_describe("input group multiline", test_multiline_status);
        ctest_describe("input group focus status", test_focus_status);
        ctest_describe("input group addon focus", test_addon_focus_delegation);
        ctest_describe("input group disabled", test_disabled_status);
        ctest_describe("input group wrapper focus", test_addon_wrapper_focus);
        ctest_describe("input group runtime align", test_runtime_align);
        ctest_describe("input group mark control", test_mark_control);
        ctest_describe("input group root disabled", test_root_disabled_focus);
        ctest_describe("input group empty disabled",
                       test_empty_group_clears_disabled);
        ctest_describe("input group focus destroyed", test_focus_destroyed);
        ctest_describe("input group theme", test_theme);
        lcui_destroy();
}
