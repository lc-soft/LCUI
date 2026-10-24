#include <LCUI.h>

static ui_widget_t *pending_toast;
static int pending_timer;

static void on_pending_destroy(ui_widget_t *toast, ui_event_t *event, void *arg)
{
        if (toast != pending_toast) {
                return;
        }
        pending_toast = NULL;
        if (pending_timer) {
                ptk_clear_timer(pending_timer);
                pending_timer = 0;
        }
}

static void finish_promise(void *arg)
{
        ui_widget_t *toast = pending_toast;

        pending_timer = 0;
        pending_toast = NULL;
        if (!toast) {
                return;
        }
        ui_toast_set_type(toast, UI_TOAST_TYPE_SUCCESS);
        ui_toast_set_title(toast, "Event created");
        ui_toast_set_description(toast, "Your event has been created.");
        ui_toast_set_duration(toast, 5000);
}

static void show_promise(ui_widget_t *button, ui_event_t *event, void *arg)
{
        ui_toast_options_t options = { 0 };
        ui_widget_t *toast;

        if (pending_toast) {
                ui_widget_t *old = pending_toast;

                pending_toast = NULL;
                ui_toast_close(old);
        }
        if (pending_timer) {
                ptk_clear_timer(pending_timer);
                pending_timer = 0;
        }
        options.type = UI_TOAST_TYPE_LOADING;
        options.title = "Creating event...";
        options.description = "This may take a moment.";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        pending_toast = toast;
        ui_widget_on(toast, "destroy", on_pending_destroy, NULL);
        pending_timer = ptk_set_timeout(2000, finish_promise, NULL);
}

void toast_promise_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(row, "toast-demo-row");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, "Create event");
        ui_widget_on(button, "click", show_promise, NULL);
        ui_widget_append(row, button);
        ui_widget_append(parent, row);
}
