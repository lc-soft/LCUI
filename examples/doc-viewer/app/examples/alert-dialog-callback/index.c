#include "index.tsx.h"
#include "index.h"

#define on_choice alert_dialog_callback__on_choice
#include "../../../../../docs/examples/alert-dialog-callback/main.c"
#undef on_choice

typedef struct {
        alert_dialog_callback_demo_react_t base;
} alert_dialog_callback_demo_t;

void alert_dialog_callback_demo_update(ui_widget_t *w)
{
        alert_dialog_callback_demo_react_update(w);
}

static void alert_dialog_callback_demo_init(ui_widget_t *w)
{
        alert_dialog_callback_demo_t *_that =
            ui_widget_add_data(w, alert_dialog_callback_demo_proto,
                               sizeof(alert_dialog_callback_demo_t));
        alert_dialog_callback_demo_proto->proto->init(w);
        alert_dialog_callback_demo_react_init(w);
        alert_dialog_callback_init(_that->base.refs.preview);
        alert_dialog_callback_demo_update(w);
}

static void alert_dialog_callback_demo_destroy(ui_widget_t *w)
{
        alert_dialog_callback_demo_proto->proto->destroy(w);
        alert_dialog_callback_demo_react_destroy(w);
}

ui_widget_t *ui_create_alert_dialog_callback_demo(void)
{
        return ui_create_widget_with_prototype(
            alert_dialog_callback_demo_proto);
}

void ui_register_alert_dialog_callback_demo(void)
{
        alert_dialog_callback_demo_init_prototype();
        alert_dialog_callback_demo_proto->init =
            alert_dialog_callback_demo_init;
        alert_dialog_callback_demo_proto->destroy =
            alert_dialog_callback_demo_destroy;
}
