#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_settings_button(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_widget_set_attr(button, "data-key", "settings");
        ui_widget_set_attr(button, "data-label", "Settings");
        ui_widget_append(button, create_text("Settings"));
        return button;
}

void breadcrumb_custom_init(ui_widget_t *parent)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();

        ui_widget_add_class(breadcrumb, "breadcrumb-custom-demo");
        ui_breadcrumb_append_item(breadcrumb, "home", "Home");
        ui_breadcrumb_append_item(breadcrumb, "components", "Components");
        ui_breadcrumb_append(breadcrumb, create_settings_button());
        ui_widget_append(parent, breadcrumb);
}
