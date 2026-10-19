#include "index.tsx.h"
#include "index.h"

#define make_field popover_basic__make_field
#include "../../../../../docs/examples/popover-basic/main.c"
#undef make_field

typedef struct {
        popover_basic_demo_react_t base;
} popover_basic_demo_t;

void popover_basic_demo_update(ui_widget_t *w)
{
        popover_basic_demo_react_update(w);
}

static void popover_basic_demo_init(ui_widget_t *w)
{
        popover_basic_demo_t *_that = ui_widget_add_data(
            w, popover_basic_demo_proto, sizeof(popover_basic_demo_t));
        popover_basic_demo_proto->proto->init(w);
        popover_basic_demo_react_init(w);
        popover_basic_init(_that->base.refs.preview);
        popover_basic_demo_update(w);
}

static void popover_basic_demo_destroy(ui_widget_t *w)
{
        popover_basic_demo_proto->proto->destroy(w);
        popover_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_popover_basic_demo(void)
{
        return ui_create_widget_with_prototype(popover_basic_demo_proto);
}

void ui_register_popover_basic_demo(void)
{
        popover_basic_demo_init_prototype();
        popover_basic_demo_proto->init = popover_basic_demo_init;
        popover_basic_demo_proto->destroy = popover_basic_demo_destroy;
}
