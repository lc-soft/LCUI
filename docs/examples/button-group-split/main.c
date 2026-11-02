#include <LCUI.h>

void button_group_split_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *icon_button = ui_create_button();
        ui_widget_t *separator = ui_create_button_group_separator();

        ui_widget_add_class(group, "button-group-split-demo");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_SECONDARY);
        ui_text_set_content(text, "Button");
        ui_widget_append(button, text);

        ui_button_set_variant(icon_button, UI_BUTTON_VARIANT_SECONDARY);
        ui_button_set_size(icon_button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(icon_button, ui_create_fluent_icon("add"));

        ui_widget_append(group, button);
        ui_widget_append(group, separator);
        ui_widget_append(group, icon_button);
        ui_widget_append(parent, group);
}
