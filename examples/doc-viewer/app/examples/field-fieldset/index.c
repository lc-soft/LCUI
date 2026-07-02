#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/field-fieldset/main.c"

typedef struct {
        field_fieldset_demo_react_t base;
} field_fieldset_demo_t;

static void field_fieldset_demo_init(ui_widget_t *w)
{
        field_fieldset_demo_t *_that = ui_widget_add_data(
            w, field_fieldset_demo_proto, sizeof(field_fieldset_demo_t));
        field_fieldset_demo_proto->proto->init(w);
        field_fieldset_demo_react_init(w);
        field_fieldset_init(_that->base.refs.preview);
        field_fieldset_demo_update(w);
}

static void field_fieldset_demo_destroy(ui_widget_t *w)
{
        field_fieldset_demo_proto->proto->destroy(w);
        field_fieldset_demo_react_destroy(w);
}

static void field_fieldset_demo_update(ui_widget_t *w)
{
        field_fieldset_demo_react_update(w);
}

ui_widget_t *ui_create_field_fieldset_demo(void)
{
        return ui_create_widget_with_prototype(field_fieldset_demo_proto);
}

void ui_register_field_fieldset_demo(void)
{
        field_fieldset_demo_init_prototype();
        field_fieldset_demo_proto->init = field_fieldset_demo_init;
        field_fieldset_demo_proto->destroy = field_fieldset_demo_destroy;
}
