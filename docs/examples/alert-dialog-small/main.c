#include <LCUI.h>

void alert_dialog_small_init(ui_widget_t *parent)
{
        ui_widget_t *alert = ui_create_alert_dialog();

        ui_dialog_build_children(alert);
        ui_dialog_set_trigger_text(alert, "Show Dialog");
        ui_dialog_set_title(alert, "Allow accessory to connect?");
        ui_dialog_set_description(
            alert, "Do you want to allow the USB accessory to connect to this "
                   "device?");
        ui_dialog_set_cancel_text(alert, "Don't allow");
        ui_dialog_set_action_text(alert, "Allow");
        ui_alert_dialog_set_size(alert, "sm");
        ui_widget_append(parent, alert);
}
