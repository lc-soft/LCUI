#include "index.tsx.h"
#include "index.h"

#define caption shimmer_duration__caption
#include "../../../../../docs/examples/shimmer-duration/main.c"
#undef caption

typedef struct {
        shimmer_duration_demo_react_t base;
} shimmer_duration_demo_t;

void shimmer_duration_demo_update(ui_widget_t *w)
{
        shimmer_duration_demo_react_update(w);
}

static void shimmer_duration_demo_init(ui_widget_t *w)
{
        shimmer_duration_demo_t *_that = ui_widget_add_data(
            w, shimmer_duration_demo_proto, sizeof(shimmer_duration_demo_t));
        shimmer_duration_demo_proto->proto->init(w);
        shimmer_duration_demo_react_init(w);
        shimmer_duration_init(_that->base.refs.preview);
        shimmer_duration_demo_update(w);
}

static void shimmer_duration_demo_destroy(ui_widget_t *w)
{
        shimmer_duration_demo_proto->proto->destroy(w);
        shimmer_duration_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_duration_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_duration_demo_proto);
}

void ui_register_shimmer_duration_demo(void)
{
        shimmer_duration_demo_init_prototype();
        shimmer_duration_demo_proto->init = shimmer_duration_demo_init;
        shimmer_duration_demo_proto->destroy = shimmer_duration_demo_destroy;
}
