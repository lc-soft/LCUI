#include <LCUI.h>

void input_group_custom_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_widget("textinput");
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *submit = ui_create_input_group_button();

        ui_widget_add_class(group, "input-group-custom-demo");
        ui_widget_add_class(input, "input-group-custom-control");
        ui_widget_add_class(input, "input-group-input");
        ui_input_group_mark_control(input);
        ui_textinput_set_placeholder(input, "Autoresize textarea...");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_button_set_variant(submit, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(submit, UI_BUTTON_SIZE_SM);
        ui_button_set_text(submit, "Submit");
        ui_widget_add_class(submit, "input-group-block-end-action");
        ui_widget_append(addon, submit);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(parent, group);
}
