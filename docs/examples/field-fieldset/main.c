/*
 * docs/examples/field-fieldset/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

void field_fieldset_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *set, *legend, *desc, *group;
        ui_widget_t *street_field, *street_lbl, *street_input;
        ui_widget_t *city_field, *city_lbl, *city_input;
        ui_widget_t *zip_field, *zip_lbl, *zip_input;

        outer = ui_create_widget(NULL);
        set = ui_create_field_set();
        legend = ui_create_field_legend();
        desc = ui_create_field_description();
        group = ui_create_field_group();
        street_field = ui_create_field();
        street_lbl = ui_create_field_label();
        street_input = ui_create_widget("textinput");
        city_field = ui_create_field();
        city_lbl = ui_create_field_label();
        city_input = ui_create_widget("textinput");
        zip_field = ui_create_field();
        zip_lbl = ui_create_field_label();
        zip_input = ui_create_widget("textinput");

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_text_set_content(legend, "Address Information");
        ui_widget_append(set, legend);
        ui_text_set_content(desc,
                            "We need your address to deliver your order.");
        ui_widget_append(set, desc);
        ui_widget_append(set, group);

        ui_label_set_for(street_lbl, "street");
        ui_text_set_content(street_lbl, "Street Address");
        ui_widget_append(street_field, street_lbl);
        ui_textinput_set_placeholder(street_input, "123 Main St");
        ui_widget_set_id(street_input, "street");
        ui_widget_append(street_field, street_input);
        ui_widget_append(group, street_field);

        ui_label_set_for(city_lbl, "city");
        ui_text_set_content(city_lbl, "City");
        ui_widget_append(city_field, city_lbl);
        ui_textinput_set_placeholder(city_input, "New York");
        ui_widget_set_id(city_input, "city");
        ui_widget_append(city_field, city_input);
        ui_widget_append(group, city_field);

        ui_label_set_for(zip_lbl, "postal-code");
        ui_text_set_content(zip_lbl, "Postal Code");
        ui_widget_append(zip_field, zip_lbl);
        ui_textinput_set_placeholder(zip_input, "90502");
        ui_widget_set_id(zip_input, "postal-code");
        ui_widget_append(zip_field, zip_input);
        ui_widget_append(group, zip_field);

        ui_widget_append(outer, set);
        ui_widget_append(parent, outer);
}
