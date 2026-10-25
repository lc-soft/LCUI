#include <LCUI.h>

static ui_widget_t *create_radio(const char *icon_name, const char *label,
                                 const char *value)
{
        ui_widget_t *item = ui_create_dropdown_menu_radio_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_set_attr(item, "value", value);
        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        return item;
}

void dropdown_menu_radio_icons_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();
        ui_widget_t *label = ui_create_dropdown_menu_label();
        ui_widget_t *radios = ui_create_dropdown_menu_radio_group();

        ui_widget_add_class(content, "dropdown-menu-radio-icons-demo-content");
        ui_button_set_text(button, "Payment Method");
        ui_widget_append(trigger, button);

        ui_text_set_content(label, "Select Payment Method");
        ui_widget_append(group, label);
        ui_widget_set_attr(radios, "value", "card");
        ui_widget_append(radios,
                         create_radio("payment", "Credit Card", "card"));
        ui_widget_append(radios, create_radio("wallet", "PayPal", "paypal"));
        ui_widget_append(
            radios, create_radio("building-bank", "Bank Transfer", "bank"));
        ui_widget_append(group, radios);
        ui_widget_append(content, group);

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
