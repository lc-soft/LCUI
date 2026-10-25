#include <LCUI.h>

static ui_widget_t *create_item(const char *label, bool disabled)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        if (disabled) {
                ui_widget_set_disabled(item, true);
        }
        return item;
}

void dropdown_menu_basic_demo_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();
        ui_widget_t *label = ui_create_dropdown_menu_label();

        ui_widget_add_class(content, "dropdown-menu-basic-demo-content");
        ui_button_set_text(button, "Open");
        ui_widget_append(trigger, button);

        ui_text_set_content(label, "My Account");
        ui_widget_append(group, label);
        ui_widget_append(group, create_item("Profile", false));
        ui_widget_append(group, create_item("Billing", false));
        ui_widget_append(group, create_item("Settings", false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, create_item("GitHub", false));
        ui_widget_append(content, create_item("Support", false));
        ui_widget_append(content, create_item("API", true));

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
