#include <LCUI.h>

void button_group_input_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *input = ui_create_widget("textinput");
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(group, "button-group-input-demo");
        ui_widget_add_class(group, "clip-safe");
        ui_textinput_set_placeholder(input, "Search...");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon("search"));
        ui_widget_append(group, input);
        ui_widget_append(group, button);
        ui_widget_append(parent, group);
}
