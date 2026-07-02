/*
 * docs/examples/field-radio/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

void field_radio_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *set, *lbl, *desc, *group;
        ui_widget_t *row[3];
        ui_widget_t *item[3];
        ui_widget_t *row_lbl[3];
        const char *ids[3] = { "plan-monthly", "plan-yearly", "plan-lifetime" };
        const char *values[3] = { "monthly", "yearly", "lifetime" };
        const char *texts[3] = { "Monthly ($9.99/month)",
                                 "Yearly ($99.99/year)", "Lifetime ($299.99)" };
        int i;

        outer = ui_create_widget(NULL);
        set = ui_create_field_set();
        lbl = ui_create_field_label();
        desc = ui_create_field_description();
        group = ui_create_radio_group();

        for (i = 0; i < 3; ++i) {
                row[i] = ui_create_field();
                item[i] = ui_create_radio_group_item();
                row_lbl[i] = ui_create_field_label();
        }

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_text_set_content(lbl, "Subscription Plan");
        ui_widget_append(set, lbl);
        ui_text_set_content(desc, "Yearly and lifetime plans offer significant "
                                  "savings.");
        ui_widget_append(set, desc);
        ui_widget_append(set, group);

        for (i = 0; i < 3; ++i) {
                ui_field_set_orientation(row[i], "horizontal");
                ui_widget_set_id(item[i], ids[i]);
                ui_widget_set_attr(item[i], "value", values[i]);
                ui_widget_append(row[i], item[i]);
                ui_label_set_for(row_lbl[i], ids[i]);
                ui_text_set_content(row_lbl[i], texts[i]);
                ui_widget_append(row[i], row_lbl[i]);
                ui_widget_append(group, row[i]);
        }

        ui_widget_set_attr(group, "value", "monthly");
        ui_radio_group_update(group);

        ui_widget_append(outer, set);
        ui_widget_append(parent, outer);
}
