#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/empty-background/main.c"

typedef struct {
        empty_background_demo_react_t base;
} empty_background_demo_t;

void empty_background_demo_update(ui_widget_t *w)
{
        empty_background_demo_react_update(w);
}

static void empty_background_demo_init(ui_widget_t *w)
{
        empty_background_demo_t *_that = ui_widget_add_data(
            w, empty_background_demo_proto, sizeof(empty_background_demo_t));
        empty_background_demo_proto->proto->init(w);
        empty_background_demo_react_init(w);
        empty_background_init(_that->base.refs.preview);
        empty_background_demo_update(w);
}

static void empty_background_demo_destroy(ui_widget_t *w)
{
        empty_background_demo_proto->proto->destroy(w);
        empty_background_demo_react_destroy(w);
}

ui_widget_t *ui_create_empty_background_demo(void)
{
        return ui_create_widget_with_prototype(empty_background_demo_proto);
}

void ui_register_empty_background_demo(void)
{
        empty_background_demo_init_prototype();
        empty_background_demo_proto->init = empty_background_demo_init;
        empty_background_demo_proto->destroy = empty_background_demo_destroy;
}
