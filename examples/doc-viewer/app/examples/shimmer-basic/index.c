#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/shimmer-basic/main.c"

typedef struct {
        shimmer_basic_demo_react_t base;
} shimmer_basic_demo_t;

void shimmer_basic_demo_update(ui_widget_t *w)
{
        shimmer_basic_demo_react_update(w);
}

static void shimmer_basic_demo_init(ui_widget_t *w)
{
        shimmer_basic_demo_t *_that = ui_widget_add_data(
            w, shimmer_basic_demo_proto, sizeof(shimmer_basic_demo_t));
        shimmer_basic_demo_proto->proto->init(w);
        shimmer_basic_demo_react_init(w);
        shimmer_basic_init(_that->base.refs.preview);
        shimmer_basic_demo_update(w);
}

static void shimmer_basic_demo_destroy(ui_widget_t *w)
{
        shimmer_basic_demo_proto->proto->destroy(w);
        shimmer_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_basic_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_basic_demo_proto);
}

void ui_register_shimmer_basic_demo(void)
{
        shimmer_basic_demo_init_prototype();
        shimmer_basic_demo_proto->init = shimmer_basic_demo_init;
        shimmer_basic_demo_proto->destroy = shimmer_basic_demo_destroy;
}
