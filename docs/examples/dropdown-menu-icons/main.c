#include <LCUI.h>

static ui_widget_t *create_item(const char *icon_name, const char *label,
                                bool destructive)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        if (destructive) {
                ui_dropdown_menu_item_set_variant(
                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);
        }
        return item;
}

void dropdown_menu_icons_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();

        ui_widget_add_class(content, "dropdown-menu-icons-demo-content");
        ui_button_set_text(button, "Open");
        ui_widget_append(trigger, button);

        ui_widget_append(content, create_item("person", "Profile", false));
        ui_widget_append(content, create_item("payment", "Billing", false));
        ui_widget_append(content, create_item("settings", "Settings", false));
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, create_item("sign-out", "Log out", true));

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
