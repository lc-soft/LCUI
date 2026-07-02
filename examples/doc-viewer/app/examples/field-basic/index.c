#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/field-basic/main.c"

typedef struct {
        field_basic_demo_react_t base;
} field_basic_demo_t;

static void field_basic_demo_init(ui_widget_t *w)
{
        field_basic_demo_t *_that = ui_widget_add_data(
            w, field_basic_demo_proto, sizeof(field_basic_demo_t));
        field_basic_demo_proto->proto->init(w);
        field_basic_demo_react_init(w);
        field_basic_init(_that->base.refs.preview);
        field_basic_demo_update(w);
}

static void field_basic_demo_destroy(ui_widget_t *w)
{
        field_basic_demo_proto->proto->destroy(w);
        field_basic_demo_react_destroy(w);
}

static void field_basic_demo_update(ui_widget_t *w)
{
        field_basic_demo_react_update(w);
}

ui_widget_t *ui_create_field_basic_demo(void)
{
        return ui_create_widget_with_prototype(field_basic_demo_proto);
}

void ui_register_field_basic_demo(void)
{
        field_basic_demo_init_prototype();
        field_basic_demo_proto->init = field_basic_demo_init;
        field_basic_demo_proto->destroy = field_basic_demo_destroy;
}
