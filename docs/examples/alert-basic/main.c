#include <LCUI.h>

static ui_widget_t *create_alert(const char *icon_name, const char *title,
                                 const char *description)
{
        ui_widget_t *alert = ui_create_alert();

        ui_alert_set_icon(alert, icon_name);
        ui_alert_set_title(alert, title);
        ui_alert_set_description(alert, description);
        ui_widget_add_class(alert, "mb-6");
        return alert;
}

void alert_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "alert-basic-demo");
        ui_widget_append(
            box,
            create_alert("checkmark-circle", "Payment successful",
                         "Your payment of $29.99 has been processed. A receipt "
                         "has been sent to your email address."));
        ui_widget_append(
            box, create_alert("info", "New feature available",
                              "We've added dark mode support. You can enable "
                              "it in your account settings."));
        ui_widget_append(parent, box);
}
