#include <LCUI.h>

static ui_widget_t *create_icon_button(const char *icon)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon(icon));
        return button;
}

void button_group_orientation_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();

        ui_widget_add_class(group, "button-group-orientation-demo");
        ui_button_group_set_orientation(group, UI_BUTTON_GROUP_VERTICAL);
        ui_widget_append(group, create_icon_button("add"));
        ui_widget_append(group, create_icon_button("subtract"));
        ui_widget_append(parent, group);
}
