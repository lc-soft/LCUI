#include "index.tsx.h"
#include "index.h"

#define make_check_field field_group__make_check_field
#include "../../../../../docs/examples/field-group/main.c"
#undef make_check_field

typedef struct {
        field_group_demo_react_t base;
} field_group_demo_t;

static void field_group_demo_init(ui_widget_t *w)
{
        field_group_demo_t *_that = ui_widget_add_data(
            w, field_group_demo_proto, sizeof(field_group_demo_t));
        field_group_demo_proto->proto->init(w);
        field_group_demo_react_init(w);
        field_group_init(_that->base.refs.preview);
        field_group_demo_update(w);
}

static void field_group_demo_destroy(ui_widget_t *w)
{
        field_group_demo_proto->proto->destroy(w);
        field_group_demo_react_destroy(w);
}

static void field_group_demo_update(ui_widget_t *w)
{
        field_group_demo_react_update(w);
}

ui_widget_t *ui_create_field_group_demo(void)
{
        return ui_create_widget_with_prototype(field_group_demo_proto);
}

void ui_register_field_group_demo(void)
{
        field_group_demo_init_prototype();
        field_group_demo_proto->init = field_group_demo_init;
        field_group_demo_proto->destroy = field_group_demo_destroy;
}
