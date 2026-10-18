#include "index.tsx.h"
#include "index.h"

#define add_tooltip tooltip_sides__add_tooltip
#include "../../../../../docs/examples/tooltip-sides/main.c"
#undef add_tooltip

typedef struct {
        tooltip_sides_demo_react_t base;
} tooltip_sides_demo_t;

void tooltip_sides_demo_update(ui_widget_t *w)
{
        tooltip_sides_demo_react_update(w);
}

static void tooltip_sides_demo_init(ui_widget_t *w)
{
        tooltip_sides_demo_t *_that = ui_widget_add_data(
            w, tooltip_sides_demo_proto, sizeof(tooltip_sides_demo_t));
        tooltip_sides_demo_proto->proto->init(w);
        tooltip_sides_demo_react_init(w);
        tooltip_sides_init(_that->base.refs.preview);
        tooltip_sides_demo_update(w);
}

static void tooltip_sides_demo_destroy(ui_widget_t *w)
{
        tooltip_sides_demo_proto->proto->destroy(w);
        tooltip_sides_demo_react_destroy(w);
}

ui_widget_t *ui_create_tooltip_sides_demo(void)
{
        return ui_create_widget_with_prototype(tooltip_sides_demo_proto);
}

void ui_register_tooltip_sides_demo(void)
{
        tooltip_sides_demo_init_prototype();
        tooltip_sides_demo_proto->init = tooltip_sides_demo_init;
        tooltip_sides_demo_proto->destroy = tooltip_sides_demo_destroy;
}
