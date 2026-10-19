#include "index.tsx.h"
#include "index.h"

#define add_popover popover_align__add_popover
#include "../../../../../docs/examples/popover-align/main.c"
#undef add_popover

typedef struct {
        popover_align_demo_react_t base;
} popover_align_demo_t;

void popover_align_demo_update(ui_widget_t *w)
{
        popover_align_demo_react_update(w);
}

static void popover_align_demo_init(ui_widget_t *w)
{
        popover_align_demo_t *_that = ui_widget_add_data(
            w, popover_align_demo_proto, sizeof(popover_align_demo_t));
        popover_align_demo_proto->proto->init(w);
        popover_align_demo_react_init(w);
        popover_align_init(_that->base.refs.preview);
        popover_align_demo_update(w);
}

static void popover_align_demo_destroy(ui_widget_t *w)
{
        popover_align_demo_proto->proto->destroy(w);
        popover_align_demo_react_destroy(w);
}

ui_widget_t *ui_create_popover_align_demo(void)
{
        return ui_create_widget_with_prototype(popover_align_demo_proto);
}

void ui_register_popover_align_demo(void)
{
        popover_align_demo_init_prototype();
        popover_align_demo_proto->init = popover_align_demo_init;
        popover_align_demo_proto->destroy = popover_align_demo_destroy;
}
