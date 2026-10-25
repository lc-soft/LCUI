#include <LCUI.h>

static ui_widget_t *create_item(const char *icon_name, const char *label)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        return item;
}

void dropdown_menu_avatar_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();

        ui_widget_add_class(button, "dropdown-menu-avatar-trigger");
        ui_button_set_text(button, "LR");
        ui_widget_append(trigger, button);
        ui_widget_set_attr(content, "align", "end");

        ui_widget_append(group, create_item("checkmark-circle", "Account"));
        ui_widget_append(group, create_item("payment", "Billing"));
        ui_widget_append(group, create_item("alert", "Notifications"));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, create_item("sign-out", "Sign Out"));

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
