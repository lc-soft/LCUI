#include <LCUI.h>

void input_group_inline_end_init(ui_widget_t *parent)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *description = ui_create_field_description();
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();

        ui_widget_add_class(field, "input-group-inline-end");
        ui_text_set_content(label, "Input");
        ui_label_set_for(label, "inline-end-input");
        ui_textinput_set_placeholder(input, "Enter password");
        ui_widget_set_id(input, "inline-end-input");
        ui_widget_set_attr(input, "type", "password");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(addon, ui_create_fluent_icon("eye-off"));
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_text_set_content(description, "Icon positioned at the end.");
        ui_widget_append(field, label);
        ui_widget_append(field, group);
        ui_widget_append(field, description);
        ui_widget_append(parent, field);
}
