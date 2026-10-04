#include <LCUI.h>

void alert_destructive_init(ui_widget_t *parent)
{
        ui_widget_t *alert = ui_create_alert();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *content = ui_create_alert_content();
        ui_widget_t *title = ui_create_alert_title();
        ui_widget_t *description = ui_create_alert_description();

        ui_widget_add_class(alert, "alert-destructive-demo");
        ui_alert_set_variant(alert, UI_ALERT_VARIANT_DESTRUCTIVE);
        ui_icon_set_name(icon, "error-circle");
        ui_text_set_content(title, "Payment failed");
        ui_text_set_content(description,
                            "Your payment could not be processed. Please check "
                            "your payment method and try again.");
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(alert, icon);
        ui_widget_append(alert, content);
        ui_widget_append(parent, alert);
}
