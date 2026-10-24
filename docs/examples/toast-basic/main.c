#include <LCUI.h>

static void show_toast(ui_widget_t *button, ui_event_t *event, void *arg)
{
        ui_toast_options_t options = { 0 };

        options.title = "Event created";
        options.description = "Sunday, December 3 at 9:00 AM";
        ui_toast_show(&options);
}

void toast_basic_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(row, "toast-demo-row");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, "Show toast");
        ui_widget_on(button, "click", show_toast, NULL);
        ui_widget_append(row, button);
        ui_widget_append(parent, row);
}
