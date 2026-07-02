/*
 * docs/examples/field-group/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

static void make_check_field(ui_widget_t *parent, const char *id,
                             const char *text, int checked, int disabled)
{
        ui_widget_t *row = ui_create_field();
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *lbl = ui_create_field_label();

        ui_field_set_orientation(row, "horizontal");
        ui_widget_set_id(cb, id);
        if (checked) {
                ui_widget_set_attr(cb, "checked", "true");
        }
        if (disabled) {
                ui_widget_set_attr(cb, "disabled", "true");
        }
        ui_widget_append(row, cb);
        ui_label_set_for(lbl, id);
        ui_text_set_content(lbl, text);
        ui_widget_append(row, lbl);
        ui_widget_append(parent, row);
}

void field_group_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *group;
        ui_widget_t *set1, *lbl1, *desc1, *sub1;
        ui_widget_t *set2, *lbl2, *desc2, *sub2;
        ui_widget_t *sep;

        outer = ui_create_widget(NULL);
        group = ui_create_field_group();
        set1 = ui_create_field_set();
        lbl1 = ui_create_field_label();
        desc1 = ui_create_field_description();
        sub1 = ui_create_field_group();
        set2 = ui_create_field_set();
        lbl2 = ui_create_field_label();
        desc2 = ui_create_field_description();
        sub2 = ui_create_field_group();
        sep = ui_create_field_separator();

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_widget_append(group, set1);
        ui_text_set_content(lbl1, "Responses");
        ui_widget_append(set1, lbl1);
        ui_text_set_content(
            desc1,
            "Get notified when ChatGPT responds to requests that take time, "
            "like research or image generation.");
        ui_widget_append(set1, desc1);
        ui_widget_append(set1, sub1);

        make_check_field(sub1, "push", "Push notifications", 1, 1);

        ui_widget_append(group, sep);

        ui_widget_append(group, set2);
        ui_text_set_content(lbl2, "Tasks");
        ui_widget_append(set2, lbl2);
        ui_text_set_content(desc2,
                            "Get notified when tasks you've created have "
                            "updates.");
        ui_widget_append(set2, desc2);
        ui_widget_append(set2, sub2);

        make_check_field(sub2, "push-tasks", "Push notifications", 0, 0);
        make_check_field(sub2, "email-tasks", "Email notifications", 0, 0);

        ui_widget_append(outer, group);
        ui_widget_append(parent, outer);
}
