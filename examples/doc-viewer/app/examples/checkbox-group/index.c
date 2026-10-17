#include "index.tsx.h"
#include "index.h"

#define add_row checkbox_group__add_row
#include "../../../../../docs/examples/checkbox-group/main.c"
#undef add_row

typedef struct {
        checkbox_group_demo_react_t base;
} checkbox_group_demo_t;

void checkbox_group_demo_update(ui_widget_t *w)
{
        checkbox_group_demo_react_update(w);
}

static void checkbox_group_demo_init(ui_widget_t *w)
{
        checkbox_group_demo_t *_that = ui_widget_add_data(
            w, checkbox_group_demo_proto, sizeof(checkbox_group_demo_t));
        checkbox_group_demo_proto->proto->init(w);
        checkbox_group_demo_react_init(w);
        checkbox_group_init(_that->base.refs.preview);
        checkbox_group_demo_update(w);
}

static void checkbox_group_demo_destroy(ui_widget_t *w)
{
        checkbox_group_demo_proto->proto->destroy(w);
        checkbox_group_demo_react_destroy(w);
}

ui_widget_t *ui_create_checkbox_group_demo(void)
{
        return ui_create_widget_with_prototype(checkbox_group_demo_proto);
}

void ui_register_checkbox_group_demo(void)
{
        checkbox_group_demo_init_prototype();
        checkbox_group_demo_proto->init = checkbox_group_demo_init;
        checkbox_group_demo_proto->destroy = checkbox_group_demo_destroy;
}
