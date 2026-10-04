#include <LCUI.h>

void alert_action_init(ui_widget_t *parent)
{
        ui_widget_t *alert = ui_create_alert();
        ui_widget_t *content = ui_create_alert_content();
        ui_widget_t *title = ui_create_alert_title();
        ui_widget_t *description = ui_create_alert_description();
        ui_widget_t *action = ui_create_alert_action();
        ui_widget_t *button = ui_create_widget("button");

        ui_widget_add_class(alert, "alert-action-demo");
        ui_text_set_content(title, "Dark mode is now available");
        ui_text_set_content(description,
                            "Enable it under your profile settings to get "
                            "started.");
        ui_button_set_text(button, "Enable");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(action, button);
        ui_widget_append(alert, content);
        ui_widget_append(alert, action);
        ui_widget_append(parent, alert);
}
