#include "index.tsx.h"
#include "index.h"

#define caption shimmer_color__caption
#include "../../../../../docs/examples/shimmer-color/main.c"
#undef caption

typedef struct {
        shimmer_color_demo_react_t base;
} shimmer_color_demo_t;

void shimmer_color_demo_update(ui_widget_t *w)
{
        shimmer_color_demo_react_update(w);
}

static void shimmer_color_demo_init(ui_widget_t *w)
{
        shimmer_color_demo_t *_that = ui_widget_add_data(
            w, shimmer_color_demo_proto, sizeof(shimmer_color_demo_t));
        shimmer_color_demo_proto->proto->init(w);
        shimmer_color_demo_react_init(w);
        shimmer_color_init(_that->base.refs.preview);
        shimmer_color_demo_update(w);
}

static void shimmer_color_demo_destroy(ui_widget_t *w)
{
        shimmer_color_demo_proto->proto->destroy(w);
        shimmer_color_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_color_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_color_demo_proto);
}

void ui_register_shimmer_color_demo(void)
{
        shimmer_color_demo_init_prototype();
        shimmer_color_demo_proto->init = shimmer_color_demo_init;
        shimmer_color_demo_proto->destroy = shimmer_color_demo_destroy;
}
