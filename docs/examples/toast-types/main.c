#include <LCUI.h>

static void show_type(ui_toast_type_t type)
{
        ui_toast_options_t options = { 0 };

        options.type = type;
        options.title = "Event notification";
        options.description = "This is a toast type example.";
        ui_toast_show(&options);
}

static void show_success(ui_widget_t *button, ui_event_t *event, void *arg)
{
        show_type(UI_TOAST_TYPE_SUCCESS);
}

static void show_info(ui_widget_t *button, ui_event_t *event, void *arg)
{
        show_type(UI_TOAST_TYPE_INFO);
}

static void show_warning(ui_widget_t *button, ui_event_t *event, void *arg)
{
        show_type(UI_TOAST_TYPE_WARNING);
}

static void show_error(ui_widget_t *button, ui_event_t *event, void *arg)
{
        show_type(UI_TOAST_TYPE_ERROR);
}

static void show_loading(ui_widget_t *button, ui_event_t *event, void *arg)
{
        ui_toast_options_t options = { 0 };

        options.type = UI_TOAST_TYPE_LOADING;
        options.title = "Creating event...";
        options.description = "This may take a moment.";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        ui_toast_show(&options);
}

typedef void (*toast_click_handler_t)(ui_widget_t *, ui_event_t *, void *);

static ui_widget_t *create_type_button(const char *label,
                                       toast_click_handler_t handler)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, label);
        ui_widget_on(button, "click", handler, NULL);
        return button;
}

void toast_types_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *buttons[] = { NULL, NULL, NULL, NULL, NULL };
        const char *labels[] = { "Success", "Info", "Warning", "Error",
                                 "Loading" };
        toast_click_handler_t handlers[] = {
                show_success, show_info, show_warning, show_error, show_loading,
        };
        size_t i;

        ui_widget_add_class(row, "toast-demo-row");
        for (i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i) {
                buttons[i] = create_type_button(labels[i], handlers[i]);
                ui_widget_append(row, buttons[i]);
        }
        ui_widget_append(parent, row);
}
