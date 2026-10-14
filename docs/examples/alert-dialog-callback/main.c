#include <LCUI.h>

static ui_widget_t *result_label;

static void on_choice(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *name = ui_get_event_name(e->type);

        ui_text_set_content(result_label, strcmp(name, "action") == 0
                                              ? "Confirmed"
                                              : "Cancelled");
}

void alert_dialog_callback_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *alert = ui_create_alert_dialog();

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "gap", "8px");

        result_label = ui_create_widget("text");
        ui_text_set_content(result_label, "No choice yet");

        ui_dialog_build_children(alert);
        ui_dialog_set_trigger_text(alert, "Confirm Changes");
        ui_dialog_set_title(alert, "Save changes?");
        ui_dialog_set_cancel_text(alert, "Discard");
        ui_dialog_set_action_text(alert, "Save");
        ui_widget_on(alert, "action", on_choice, NULL);
        ui_widget_on(alert, "cancel", on_choice, NULL);

        ui_widget_append(box, alert);
        ui_widget_append(box, result_label);
        ui_widget_append(parent, box);
}
