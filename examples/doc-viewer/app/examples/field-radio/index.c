#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/field-radio/main.c"

typedef struct {
        field_radio_demo_react_t base;
} field_radio_demo_t;

void field_radio_demo_update(ui_widget_t *w)
{
        field_radio_demo_react_update(w);
}

static void field_radio_demo_init(ui_widget_t *w)
{
        field_radio_demo_t *_that = ui_widget_add_data(
            w, field_radio_demo_proto, sizeof(field_radio_demo_t));
        field_radio_demo_proto->proto->init(w);
        field_radio_demo_react_init(w);
        field_radio_init(_that->base.refs.preview);
        field_radio_demo_update(w);
}

static void field_radio_demo_destroy(ui_widget_t *w)
{
        field_radio_demo_proto->proto->destroy(w);
        field_radio_demo_react_destroy(w);
}

ui_widget_t *ui_create_field_radio_demo(void)
{
        return ui_create_widget_with_prototype(field_radio_demo_proto);
}

void ui_register_field_radio_demo(void)
{
        field_radio_demo_init_prototype();
        field_radio_demo_proto->init = field_radio_demo_init;
        field_radio_demo_proto->destroy = field_radio_demo_destroy;
}
