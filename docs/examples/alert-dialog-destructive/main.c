#include <LCUI.h>

void alert_dialog_destructive_init(ui_widget_t *parent)
{
        ui_widget_t *alert = ui_create_alert_dialog();
        ui_widget_t *media = ui_create_alert_dialog_media();
        ui_widget_t *icon = ui_create_widget("icon");
        ui_widget_t *action;

        ui_dialog_build_children(alert);
        ui_dialog_set_trigger_text(alert, "Delete Chat");
        ui_widget_set_attr(
            ui_widget_get_child(ui_widget_get_child(alert, 0), 0), "variant",
            "destructive");
        ui_dialog_set_title(alert, "Delete chat?");
        ui_dialog_set_description(
            alert,
            "This will permanently delete this chat conversation and any "
            "memories saved during this chat.");
        ui_dialog_set_action_text(alert, "Delete");
        action = ui_dialog_get_footer_button(alert, true);
        ui_widget_set_attr(action, "variant", "destructive");
        ui_widget_set_attr(icon, "name", "delete");
        ui_widget_append(media, icon);
        ui_alert_dialog_set_media(alert, media);
        ui_alert_dialog_set_size(alert, "sm");
        ui_widget_append(parent, alert);
}
