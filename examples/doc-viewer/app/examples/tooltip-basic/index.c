#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/tooltip-basic/main.c"

typedef struct {
        tooltip_basic_demo_react_t base;
} tooltip_basic_demo_t;

void tooltip_basic_demo_update(ui_widget_t *w)
{
        tooltip_basic_demo_react_update(w);
}

static void tooltip_basic_demo_init(ui_widget_t *w)
{
        tooltip_basic_demo_t *_that = ui_widget_add_data(
            w, tooltip_basic_demo_proto, sizeof(tooltip_basic_demo_t));
        tooltip_basic_demo_proto->proto->init(w);
        tooltip_basic_demo_react_init(w);
        tooltip_basic_init(_that->base.refs.preview);
        tooltip_basic_demo_update(w);
}

static void tooltip_basic_demo_destroy(ui_widget_t *w)
{
        tooltip_basic_demo_proto->proto->destroy(w);
        tooltip_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_tooltip_basic_demo(void)
{
        return ui_create_widget_with_prototype(tooltip_basic_demo_proto);
}

void ui_register_tooltip_basic_demo(void)
{
        tooltip_basic_demo_init_prototype();
        tooltip_basic_demo_proto->init = tooltip_basic_demo_init;
        tooltip_basic_demo_proto->destroy = tooltip_basic_demo_destroy;
}
