#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/alert-dialog-small/main.c"

typedef struct {
        alert_dialog_small_demo_react_t base;
} alert_dialog_small_demo_t;

void alert_dialog_small_demo_update(ui_widget_t *w)
{
        alert_dialog_small_demo_react_update(w);
}

static void alert_dialog_small_demo_init(ui_widget_t *w)
{
        alert_dialog_small_demo_t *_that =
            ui_widget_add_data(w, alert_dialog_small_demo_proto,
                               sizeof(alert_dialog_small_demo_t));
        alert_dialog_small_demo_proto->proto->init(w);
        alert_dialog_small_demo_react_init(w);
        alert_dialog_small_init(_that->base.refs.preview);
        alert_dialog_small_demo_update(w);
}

static void alert_dialog_small_demo_destroy(ui_widget_t *w)
{
        alert_dialog_small_demo_proto->proto->destroy(w);
        alert_dialog_small_demo_react_destroy(w);
}

ui_widget_t *ui_create_alert_dialog_small_demo(void)
{
        return ui_create_widget_with_prototype(alert_dialog_small_demo_proto);
}

void ui_register_alert_dialog_small_demo(void)
{
        alert_dialog_small_demo_init_prototype();
        alert_dialog_small_demo_proto->init = alert_dialog_small_demo_init;
        alert_dialog_small_demo_proto->destroy =
            alert_dialog_small_demo_destroy;
}
