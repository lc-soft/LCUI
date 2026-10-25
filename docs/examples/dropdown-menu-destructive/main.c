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

void dropdown_menu_destructive_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group;

        ui_button_set_text(button, "Actions");
        ui_widget_append(trigger, button);

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("edit", "Edit", false));
        ui_widget_append(group, create_item("share", "Share", false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("delete", "Delete", true));
        ui_widget_append(content, group);

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
