#include "index.tsx.h"
#include "index.h"

#define create_alert alert_variants__create_alert
#include "../../../../../docs/examples/alert-variants/main.c"
#undef create_alert

typedef struct {
        alert_variants_demo_react_t base;
} alert_variants_demo_t;

void alert_variants_demo_update(ui_widget_t *w)
{
        alert_variants_demo_react_update(w);
}

static void alert_variants_demo_init(ui_widget_t *w)
{
        alert_variants_demo_t *_that = ui_widget_add_data(
            w, alert_variants_demo_proto, sizeof(alert_variants_demo_t));
        alert_variants_demo_proto->proto->init(w);
        alert_variants_demo_react_init(w);
        alert_variants_init(_that->base.refs.preview);
        alert_variants_demo_update(w);
}

static void alert_variants_demo_destroy(ui_widget_t *w)
{
        alert_variants_demo_proto->proto->destroy(w);
        alert_variants_demo_react_destroy(w);
}

ui_widget_t *ui_create_alert_variants_demo(void)
{
        return ui_create_widget_with_prototype(alert_variants_demo_proto);
}

void ui_register_alert_variants_demo(void)
{
        alert_variants_demo_init_prototype();
        alert_variants_demo_proto->init = alert_variants_demo_init;
        alert_variants_demo_proto->destroy = alert_variants_demo_destroy;
}
