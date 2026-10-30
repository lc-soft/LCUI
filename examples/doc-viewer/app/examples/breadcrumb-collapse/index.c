#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/breadcrumb-collapse/main.c"

typedef struct {
        breadcrumb_collapse_demo_react_t base;
} breadcrumb_collapse_demo_t;

void breadcrumb_collapse_demo_update(ui_widget_t *w)
{
        breadcrumb_collapse_demo_react_update(w);
}

static void breadcrumb_collapse_demo_init(ui_widget_t *w)
{
        breadcrumb_collapse_demo_t *_that =
            ui_widget_add_data(w, breadcrumb_collapse_demo_proto,
                               sizeof(breadcrumb_collapse_demo_t));
        breadcrumb_collapse_demo_proto->proto->init(w);
        breadcrumb_collapse_demo_react_init(w);
        breadcrumb_collapse_init(_that->base.refs.preview);
        breadcrumb_collapse_demo_update(w);
}

static void breadcrumb_collapse_demo_destroy(ui_widget_t *w)
{
        breadcrumb_collapse_demo_proto->proto->destroy(w);
        breadcrumb_collapse_demo_react_destroy(w);
}

ui_widget_t *ui_create_breadcrumb_collapse_demo(void)
{
        return ui_create_widget_with_prototype(breadcrumb_collapse_demo_proto);
}

void ui_register_breadcrumb_collapse_demo(void)
{
        breadcrumb_collapse_demo_init_prototype();
        breadcrumb_collapse_demo_proto->init = breadcrumb_collapse_demo_init;
        breadcrumb_collapse_demo_proto->destroy =
            breadcrumb_collapse_demo_destroy;
}
