#include "index.tsx.h"
#include "index.h"
#include "../../../../../docs/examples/anchor-basic/main.c"

typedef struct {
        anchor_basic_demo_react_t base;
        // Add additional states to your component here
        // ...
} anchor_basic_demo_t;

static void anchor_basic_demo_init(ui_widget_t *w)
{
        anchor_basic_demo_t *_that = ui_widget_add_data(
            w, anchor_basic_demo_proto, sizeof(anchor_basic_demo_t));
        anchor_basic_demo_proto->proto->init(w);
        anchor_basic_demo_react_init(w);
        anchor_basic_init(_that->base.refs.preview);
        anchor_basic_demo_update(w);
}

static anchor_basic_demo_t *anchor_basic_demo_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, anchor_basic_demo_proto);
}

static void anchor_basic_demo_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        anchor_basic_demo_react_destroy(w);
}

void anchor_basic_demo_update(ui_widget_t *w)
{
        anchor_basic_demo_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_anchor_basic_demo(void)
{
        return ui_create_widget_with_prototype(anchor_basic_demo_proto);
}

void ui_register_anchor_basic_demo(void)
{
        anchor_basic_demo_init_prototype();
        anchor_basic_demo_proto->init = anchor_basic_demo_init;
        anchor_basic_demo_proto->destroy = anchor_basic_demo_destroy;
}
