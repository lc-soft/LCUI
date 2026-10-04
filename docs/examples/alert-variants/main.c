#include <LCUI.h>

static ui_widget_t *create_alert(ui_alert_variant_t variant,
                                 const char *icon_name, const char *title,
                                 const char *description)
{
        ui_widget_t *alert = ui_create_alert();

        ui_alert_set_variant(alert, variant);
        ui_alert_set_icon(alert, icon_name);
        ui_alert_set_title(alert, title);
        ui_alert_set_description(alert, description);
        ui_widget_add_class(alert, "mb-6");
        return alert;
}

void alert_variants_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "alert-variants-demo");
        ui_widget_append(box, create_alert(UI_ALERT_VARIANT_INFO, "info",
                                           "New feature available",
                                           "Try the updated dashboard."));
        ui_widget_append(
            box, create_alert(UI_ALERT_VARIANT_SUCCESS, "checkmark-circle",
                              "Payment successful",
                              "Your payment has been processed."));
        ui_widget_append(box, create_alert(UI_ALERT_VARIANT_WARNING, "warning",
                                           "Storage almost full",
                                           "Free up space to continue."));
        ui_widget_append(
            box, create_alert(UI_ALERT_VARIANT_DESTRUCTIVE, "error-circle",
                              "Unable to save changes", "Please try again."));
        ui_widget_append(parent, box);
}
