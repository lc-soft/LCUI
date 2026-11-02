#include <LCUI.h>

static ui_widget_t *create_button(const char *label)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *text = ui_create_widget("text");

        ui_button_set_variant(button, UI_BUTTON_VARIANT_SECONDARY);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_text_set_content(text, label);
        ui_widget_append(button, text);
        return button;
}

void button_group_separator_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *separator = ui_create_button_group_separator();

        ui_widget_add_class(group, "button-group-separator-demo");
        ui_widget_append(group, create_button("Copy"));
        ui_widget_append(group, separator);
        ui_widget_append(group, create_button("Paste"));
        ui_widget_append(parent, group);
}
