#include <LCUI.h>

static void on_undo(ui_widget_t *toast, void *arg)
{
        ui_toast_close(toast);
}

static void show_action(ui_widget_t *button, ui_event_t *event, void *arg)
{
        ui_toast_options_t options = { 0 };

        options.title = "Event created";
        options.description = "You can undo this action.";
        options.action_text = "Undo";
        options.on_action = on_undo;
        ui_toast_show(&options);
}

void toast_action_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(row, "toast-demo-row");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, "Show toast");
        ui_widget_on(button, "click", show_action, NULL);
        ui_widget_append(row, button);
        ui_widget_append(parent, row);
}
