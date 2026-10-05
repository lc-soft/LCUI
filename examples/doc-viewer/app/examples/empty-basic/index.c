#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/empty-basic/main.c"

typedef struct {
        empty_basic_demo_react_t base;
} empty_basic_demo_t;

void empty_basic_demo_update(ui_widget_t *w)
{
        empty_basic_demo_react_update(w);
}

static void empty_basic_demo_init(ui_widget_t *w)
{
        empty_basic_demo_t *_that = ui_widget_add_data(
            w, empty_basic_demo_proto, sizeof(empty_basic_demo_t));
        empty_basic_demo_proto->proto->init(w);
        empty_basic_demo_react_init(w);
        empty_basic_init(_that->base.refs.preview);
        empty_basic_demo_update(w);
}

static void empty_basic_demo_destroy(ui_widget_t *w)
{
        empty_basic_demo_proto->proto->destroy(w);
        empty_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_empty_basic_demo(void)
{
        return ui_create_widget_with_prototype(empty_basic_demo_proto);
}

void ui_register_empty_basic_demo(void)
{
        empty_basic_demo_init_prototype();
        empty_basic_demo_proto->init = empty_basic_demo_init;
        empty_basic_demo_proto->destroy = empty_basic_demo_destroy;
}
