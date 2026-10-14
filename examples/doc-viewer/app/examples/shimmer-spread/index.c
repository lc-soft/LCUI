#include "index.tsx.h"
#include "index.h"

#define caption shimmer_spread__caption
#include "../../../../../docs/examples/shimmer-spread/main.c"
#undef caption

typedef struct {
        shimmer_spread_demo_react_t base;
} shimmer_spread_demo_t;

void shimmer_spread_demo_update(ui_widget_t *w)
{
        shimmer_spread_demo_react_update(w);
}

static void shimmer_spread_demo_init(ui_widget_t *w)
{
        shimmer_spread_demo_t *_that = ui_widget_add_data(
            w, shimmer_spread_demo_proto, sizeof(shimmer_spread_demo_t));
        shimmer_spread_demo_proto->proto->init(w);
        shimmer_spread_demo_react_init(w);
        shimmer_spread_init(_that->base.refs.preview);
        shimmer_spread_demo_update(w);
}

static void shimmer_spread_demo_destroy(ui_widget_t *w)
{
        shimmer_spread_demo_proto->proto->destroy(w);
        shimmer_spread_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_spread_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_spread_demo_proto);
}

void ui_register_shimmer_spread_demo(void)
{
        shimmer_spread_demo_init_prototype();
        shimmer_spread_demo_proto->init = shimmer_spread_demo_init;
        shimmer_spread_demo_proto->destroy = shimmer_spread_demo_destroy;
}
