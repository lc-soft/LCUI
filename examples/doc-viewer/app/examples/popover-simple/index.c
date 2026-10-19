#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/popover-simple/main.c"

typedef struct {
        popover_simple_demo_react_t base;
} popover_simple_demo_t;

void popover_simple_demo_update(ui_widget_t *w)
{
        popover_simple_demo_react_update(w);
}

static void popover_simple_demo_init(ui_widget_t *w)
{
        popover_simple_demo_t *_that = ui_widget_add_data(
            w, popover_simple_demo_proto, sizeof(popover_simple_demo_t));
        popover_simple_demo_proto->proto->init(w);
        popover_simple_demo_react_init(w);
        popover_simple_init(_that->base.refs.preview);
        popover_simple_demo_update(w);
}

static void popover_simple_demo_destroy(ui_widget_t *w)
{
        popover_simple_demo_proto->proto->destroy(w);
        popover_simple_demo_react_destroy(w);
}

ui_widget_t *ui_create_popover_simple_demo(void)
{
        return ui_create_widget_with_prototype(popover_simple_demo_proto);
}

void ui_register_popover_simple_demo(void)
{
        popover_simple_demo_init_prototype();
        popover_simple_demo_proto->init = popover_simple_demo_init;
        popover_simple_demo_proto->destroy = popover_simple_demo_destroy;
}
