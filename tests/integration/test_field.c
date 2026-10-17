/*
 * tests/integration/test_field.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <LCUI/widgets/checkbox.h>
#include <ctest-custom.h>

static void field_orientation(void)
{
        ui_widget_t *w = ui_create_field();

        ctest_equal_str("should default to vertical",
                        ui_field_get_orientation(w), "vertical");
        ctest_equal_bool("should not have :horizontal status by default",
                         ui_widget_has_status(w, "horizontal"), false);

        ui_field_set_orientation(w, "horizontal");
        ctest_equal_str("should accept horizontal", ui_field_get_orientation(w),
                        "horizontal");
        ctest_equal_bool("should add :horizontal status",
                         ui_widget_has_status(w, "horizontal"), true);

        ui_field_set_orientation(w, "vertical");
        ctest_equal_str("should revert to vertical",
                        ui_field_get_orientation(w), "vertical");
        ctest_equal_bool("should remove :horizontal status",
                         ui_widget_has_status(w, "horizontal"), false);

        ctest_equal_int("should reject invalid value",
                        ui_field_set_orientation(w, "diagonal"), -1);
        ctest_equal_str("should stay vertical after invalid set",
                        ui_field_get_orientation(w), "vertical");

        ui_widget_destroy(w);
}

static int label_target_click_count;
static void on_label_target_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        label_target_click_count++;
}

static void field_label_forwards_click(void)
{
        ui_widget_t *target = ui_create_widget("textinput");
        ui_widget_t *label = ui_create_field_label();
        ui_event_t ev;

        ui_widget_set_id(target, "lb-target");
        ui_widget_set_attr(label, "for", "lb-target");
        ui_widget_on(target, "click", on_label_target_click, NULL);

        label_target_click_count = 0;
        ui_event_init(&ev, "click");
        ui_widget_emit_event(label, ev, NULL);
        ctest_equal_int("should forward click to target input",
                        label_target_click_count, 1);

        ui_widget_destroy(label);
        ui_widget_destroy(target);
}

static void field_composition(void)
{
        ui_widget_t *set = ui_create_field_set();
        ui_widget_t *group = ui_create_field_group();
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *desc = ui_create_field_description();
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_set_id(input, "username");
        ui_widget_set_id(label, "username-label");
        ui_widget_set_attr(label, "for", "username");
        ui_text_set_content(label, "Username");
        ui_text_set_content(desc, "Choose a unique username.");

        ui_widget_append(field, label);
        ui_widget_append(field, input);
        ui_widget_append(field, desc);
        ui_widget_append(group, field);
        ui_widget_append(set, group);

        ctest_equal_bool("field-set should have field-group child",
                         ui_widget_get_child(set, 0) == group, true);
        ctest_equal_bool("field-group should have field child",
                         ui_widget_get_child(group, 0) == field, true);
        ctest_equal_bool("field should have label as first child",
                         ui_widget_get_child(field, 0) == label, true);
        ctest_equal_bool("field should have input as middle child",
                         ui_widget_get_child(field, 1) == input, true);
        ctest_equal_bool("field should have description as last child",
                         ui_widget_get_child(field, 2) == desc, true);

        ui_widget_destroy(set);
}

static void field_has_class(void)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *content = ui_create_field_content();

        ctest_equal_bool("should not have has-field-content without child",
                         ui_widget_has_class(field, "has-field-content"),
                         false);

        ui_widget_append(field, content);
        ui_widget_append(ui_root(), field);
        ctest_equal_bool(
            "should add has-field-content when field-content linked",
            ui_widget_has_class(field, "has-field-content"), true);

        ui_widget_destroy(field);
}

static void field_label_has_class(void)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *content = ui_create_field_content();
        ui_widget_t *checkbox = ui_create_checkbox();
        ui_event_t ev;

        ctest_equal_bool("should not have has-field without child",
                         ui_widget_has_class(label, "has-field"), false);
        ctest_equal_bool("should not have has-checked without child",
                         ui_widget_has_class(label, "has-checked"), false);

        ui_widget_append(content, checkbox);
        ui_widget_append(label, content);
        ui_widget_append(field, label);
        ui_widget_append(ui_root(), field);
        ctest_equal_bool("should add has-field when field-content linked",
                         ui_widget_has_class(label, "has-field"), true);
        ctest_equal_bool("should not have has-checked when checkbox unchecked",
                         ui_widget_has_class(label, "has-checked"), false);

        ui_checkbox_set_checked(checkbox, 1);
        ui_event_init(&ev, "change");
        ui_widget_emit_event(checkbox, ev, NULL);
        ctest_equal_bool("should add has-checked when checkbox checked",
                         ui_widget_has_class(label, "has-checked"), true);

        ui_checkbox_set_checked(checkbox, 0);
        ui_event_init(&ev, "change");
        ui_widget_emit_event(checkbox, ev, NULL);
        ctest_equal_bool("should remove has-checked when checkbox unchecked",
                         ui_widget_has_class(label, "has-checked"), false);

        ui_widget_destroy(field);
}

static void field_set_legend_spacing(void)
{
        ui_widget_t *set = ui_create_field_set();
        ui_widget_t *legend = ui_create_field_legend();
        ui_widget_t *desc = ui_create_field_description();
        ui_widget_t *group = ui_create_field_group();
        float legend_desc_gap;
        float desc_group_gap;

        ui_text_set_content(legend, "Payment Method");
        ui_text_set_content(desc, "All transactions are secure and encrypted");
        ui_widget_append(set, legend);
        ui_widget_append(set, desc);
        ui_widget_append(set, group);
        ui_widget_append(ui_root(), set);
        ui_update();

        legend_desc_gap = desc->border_box.y -
                          (legend->border_box.y + legend->border_box.height);
        desc_group_gap = group->content_box.y -
                         (desc->border_box.y + desc->border_box.height);

        ctest_equal_int("legend should sit tight to the description",
                        legend_desc_gap < 1.0f && legend_desc_gap > -1.0f, 1);
        ctest_equal_int("description should keep the set gap to the group",
                        desc_group_gap > 15.0f && desc_group_gap < 17.0f, 1);

        ui_widget_destroy(set);
        ui_clear_trash();
}

void test_field(void)
{
        lcui_init();

        ctest_describe("field orientation", field_orientation);
        ctest_describe("field-label click forwarding",
                       field_label_forwards_click);
        ctest_describe("field composition", field_composition);
        ctest_describe("field has-class", field_has_class);
        ctest_describe("field-label has-class", field_label_has_class);
        ctest_describe("field-set legend spacing", field_set_legend_spacing);

        lcui_destroy();
}
