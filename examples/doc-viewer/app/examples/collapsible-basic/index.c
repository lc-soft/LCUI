#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/collapsible-basic/main.c"

typedef struct {
        collapsible_basic_demo_react_t base;
} collapsible_basic_demo_t;

void collapsible_basic_demo_update(ui_widget_t *w)
{
        collapsible_basic_demo_react_update(w);
}

static void collapsible_basic_demo_init(ui_widget_t *w)
{
        collapsible_basic_demo_t *_that = ui_widget_add_data(
            w, collapsible_basic_demo_proto, sizeof(collapsible_basic_demo_t));
        collapsible_basic_demo_proto->proto->init(w);
        collapsible_basic_demo_react_init(w);
        collapsible_basic_init(_that->base.refs.preview);
        collapsible_basic_demo_update(w);
}

static void collapsible_basic_demo_destroy(ui_widget_t *w)
{
        collapsible_basic_demo_proto->proto->destroy(w);
        collapsible_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_collapsible_basic_demo(void)
{
        return ui_create_widget_with_prototype(collapsible_basic_demo_proto);
}

void ui_register_collapsible_basic_demo(void)
{
        collapsible_basic_demo_init_prototype();
        collapsible_basic_demo_proto->init = collapsible_basic_demo_init;
        collapsible_basic_demo_proto->destroy = collapsible_basic_demo_destroy;
}
