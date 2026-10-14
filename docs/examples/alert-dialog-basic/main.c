#include <LCUI.h>

void alert_dialog_basic_init(ui_widget_t *parent)
{
        ui_widget_t *alert = ui_create_alert_dialog();

        ui_dialog_build_children(alert);
        ui_dialog_set_trigger_text(alert, "Show Dialog");
        ui_widget_append(parent, alert);
}
