#include <LCUI.h>

static ui_widget_t *create_radio(const char *label, const char *value)
{
        ui_widget_t *item = ui_create_dropdown_menu_radio_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_set_attr(item, "value", value);
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        return item;
}

void dropdown_menu_radio_group_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *label = ui_create_dropdown_menu_label();
        ui_widget_t *group = ui_create_dropdown_menu_radio_group();

        ui_widget_add_class(content, "dropdown-menu-radio-group-demo-content");
        ui_button_set_text(button, "Open");
        ui_widget_append(trigger, button);

        ui_text_set_content(label, "Panel position");
        ui_widget_append(content, label);
        ui_widget_set_attr(group, "value", "bottom");
        ui_widget_append(group, create_radio("Top", "top"));
        ui_widget_append(group, create_radio("Bottom", "bottom"));
        ui_widget_append(group, create_radio("Right", "right"));
        ui_widget_append(content, group);

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
