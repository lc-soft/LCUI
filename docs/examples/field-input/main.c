/*
 * docs/examples/field-input/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

void field_input_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *set, *group;
        ui_widget_t *user_field, *user_lbl, *user_input, *user_desc;
        ui_widget_t *pass_field, *pass_lbl, *pass_desc, *pass_input;

        outer = ui_create_widget(NULL);
        set = ui_create_field_set();
        group = ui_create_field_group();
        user_field = ui_create_field();
        user_lbl = ui_create_field_label();
        user_input = ui_create_widget("textinput");
        user_desc = ui_create_field_description();
        pass_field = ui_create_field();
        pass_lbl = ui_create_field_label();
        pass_desc = ui_create_field_description();
        pass_input = ui_create_widget("textinput");

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_widget_append(set, group);

        ui_label_set_for(user_lbl, "username");
        ui_text_set_content(user_lbl, "Username");
        ui_widget_append(user_field, user_lbl);
        ui_textinput_set_placeholder(user_input, "Max Leiter");
        ui_widget_set_id(user_input, "username");
        ui_widget_append(user_field, user_input);
        ui_text_set_content(user_desc,
                            "Choose a unique username for your account.");
        ui_widget_append(user_field, user_desc);
        ui_widget_append(group, user_field);

        ui_label_set_for(pass_lbl, "password");
        ui_text_set_content(pass_lbl, "Password");
        ui_widget_append(pass_field, pass_lbl);
        ui_text_set_content(pass_desc, "Must be at least 8 characters long.");
        ui_widget_append(pass_field, pass_desc);
        ui_textinput_set_placeholder(pass_input, u8"••••••••");
        ui_widget_set_id(pass_input, "password");
        ui_widget_append(pass_field, pass_input);
        ui_widget_append(group, pass_field);

        ui_widget_append(outer, set);
        ui_widget_append(parent, outer);
}
