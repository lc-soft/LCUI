/*
 * docs/examples/field-choice-card/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

void field_choice_card_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *group, *set, *lbl, *desc;
        ui_widget_t *radio;
        ui_widget_t *card1, *card2;
        ui_widget_t *row1, *row2;
        ui_widget_t *content1, *content2;
        ui_widget_t *title1, *title2;
        ui_widget_t *card_desc1, *card_desc2;
        ui_widget_t *item1, *item2;

        outer = ui_create_widget(NULL);
        group = ui_create_field_group();
        set = ui_create_field_set();
        lbl = ui_create_field_label();
        desc = ui_create_field_description();
        radio = ui_create_radio_group();
        card1 = ui_create_field_label();
        card2 = ui_create_field_label();
        row1 = ui_create_field();
        row2 = ui_create_field();
        content1 = ui_create_field_content();
        content2 = ui_create_field_content();
        title1 = ui_create_field_title();
        title2 = ui_create_field_title();
        card_desc1 = ui_create_field_description();
        card_desc2 = ui_create_field_description();
        item1 = ui_create_radio_group_item();
        item2 = ui_create_radio_group_item();

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_widget_append(group, set);
        ui_label_set_for(lbl, "compute-environment");
        ui_text_set_content(lbl, "Compute Environment");
        ui_widget_append(set, lbl);
        ui_text_set_content(desc,
                            "Select the compute environment for your cluster.");
        ui_widget_append(set, desc);
        ui_widget_append(set, radio);

        ui_widget_set_attr(radio, "value", "kubernetes");

        ui_label_set_for(card1, "kubernetes-r2h");
        ui_field_set_orientation(row1, "horizontal");
        ui_widget_append(row1, content1);
        ui_text_set_content(title1, "Kubernetes");
        ui_widget_append(content1, title1);
        ui_text_set_content(card_desc1,
                            "Run GPU workloads on a K8s configured cluster.");
        ui_widget_append(content1, card_desc1);
        ui_widget_set_id(item1, "kubernetes-r2h");
        ui_widget_set_attr(item1, "value", "kubernetes");
        ui_widget_append(row1, item1);
        ui_widget_append(card1, row1);
        ui_widget_append(radio, card1);

        ui_label_set_for(card2, "vm-z4k");
        ui_field_set_orientation(row2, "horizontal");
        ui_widget_append(row2, content2);
        ui_text_set_content(title2, "Virtual Machine");
        ui_widget_append(content2, title2);
        ui_text_set_content(card_desc2,
                            "Access a VM configured cluster to run GPU "
                            "workloads.");
        ui_widget_append(content2, card_desc2);
        ui_widget_set_id(item2, "vm-z4k");
        ui_widget_set_attr(item2, "value", "vm");
        ui_widget_append(row2, item2);
        ui_widget_append(card2, row2);
        ui_widget_append(radio, card2);

        ui_widget_append(outer, group);
        ui_widget_append(parent, outer);
        ui_radio_group_update(radio);
}
