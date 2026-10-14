#include "index.tsx.h"
#include "index.h"

#define caption shimmer_angle__caption
#include "../../../../../docs/examples/shimmer-angle/main.c"
#undef caption

typedef struct {
        shimmer_angle_demo_react_t base;
} shimmer_angle_demo_t;

void shimmer_angle_demo_update(ui_widget_t *w)
{
        shimmer_angle_demo_react_update(w);
}

static void shimmer_angle_demo_init(ui_widget_t *w)
{
        shimmer_angle_demo_t *_that = ui_widget_add_data(
            w, shimmer_angle_demo_proto, sizeof(shimmer_angle_demo_t));
        shimmer_angle_demo_proto->proto->init(w);
        shimmer_angle_demo_react_init(w);
        shimmer_angle_init(_that->base.refs.preview);
        shimmer_angle_demo_update(w);
}

static void shimmer_angle_demo_destroy(ui_widget_t *w)
{
        shimmer_angle_demo_proto->proto->destroy(w);
        shimmer_angle_demo_react_destroy(w);
}

ui_widget_t *ui_create_shimmer_angle_demo(void)
{
        return ui_create_widget_with_prototype(shimmer_angle_demo_proto);
}

void ui_register_shimmer_angle_demo(void)
{
        shimmer_angle_demo_init_prototype();
        shimmer_angle_demo_proto->init = shimmer_angle_demo_init;
        shimmer_angle_demo_proto->destroy = shimmer_angle_demo_destroy;
}
