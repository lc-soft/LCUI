#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/shimmer-running/main.c"

typedef struct {
        shimmer_running_demo_react_t base;
} shimmer_running_demo_t;

void shimmer_running_demo_update(ui_widget_t *w)
{
        shimmer_running_demo_react_update(w);
}

static void shimmer_running_demo_init(ui_widget_t *w)
{
        shimmer_running_demo_t *_that = ui_widget_add_data(
            w, shimmer_running_demo_proto, sizeof(shimmer_running_demo_t));
        shimmer_running_demo_proto->proto->init(w);
        shimmer_running_demo_react_init(w);
        shimmer_running_init(_that->base.refs.preview);
        shimmer_running_demo_update(w);
}

static void shimmer_running_demo_destroy(ui_widget_t *w)
{
        shimmer_running_demo_proto->proto->destroy(w);
        shimmer_running_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_running_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_running_demo_proto);
}

void ui_register_shimmer_running_demo(void)
{
        shimmer_running_demo_init_prototype();
        shimmer_running_demo_proto->init = shimmer_running_demo_init;
        shimmer_running_demo_proto->destroy = shimmer_running_demo_destroy;
}
