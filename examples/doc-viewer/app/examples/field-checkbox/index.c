#include "index.tsx.h"
#include "index.h"

#define make_check_row field_checkbox__make_check_row
#include "../../../../../docs/examples/field-checkbox/main.c"
#undef make_check_row

typedef struct {
        field_checkbox_demo_react_t base;
} field_checkbox_demo_t;

static void field_checkbox_demo_init(ui_widget_t *w)
{
        field_checkbox_demo_t *_that = ui_widget_add_data(
            w, field_checkbox_demo_proto, sizeof(field_checkbox_demo_t));
        field_checkbox_demo_proto->proto->init(w);
        field_checkbox_demo_react_init(w);
        field_checkbox_init(_that->base.refs.preview);
        field_checkbox_demo_update(w);
}

static void field_checkbox_demo_destroy(ui_widget_t *w)
{
        field_checkbox_demo_proto->proto->destroy(w);
        field_checkbox_demo_react_destroy(w);
}

static void field_checkbox_demo_update(ui_widget_t *w)
{
        field_checkbox_demo_react_update(w);
}

ui_widget_t *ui_create_field_checkbox_demo(void)
{
        return ui_create_widget_with_prototype(field_checkbox_demo_proto);
}

void ui_register_field_checkbox_demo(void)
{
        field_checkbox_demo_init_prototype();
        field_checkbox_demo_proto->init = field_checkbox_demo_init;
        field_checkbox_demo_proto->destroy = field_checkbox_demo_destroy;
}
