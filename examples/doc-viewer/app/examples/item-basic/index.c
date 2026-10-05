#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/item-basic/main.c"

typedef struct {
        item_basic_demo_react_t base;
} item_basic_demo_t;

void item_basic_demo_update(ui_widget_t *w)
{
        item_basic_demo_react_update(w);
}

static void item_basic_demo_init(ui_widget_t *w)
{
        item_basic_demo_t *_that = ui_widget_add_data(
            w, item_basic_demo_proto, sizeof(item_basic_demo_t));
        item_basic_demo_proto->proto->init(w);
        item_basic_demo_react_init(w);
        item_basic_init(_that->base.refs.preview);
        item_basic_demo_update(w);
}

static void item_basic_demo_destroy(ui_widget_t *w)
{
        item_basic_demo_proto->proto->destroy(w);
        item_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_item_basic_demo(void)
{
        return ui_create_widget_with_prototype(item_basic_demo_proto);
}

void ui_register_item_basic_demo(void)
{
        item_basic_demo_init_prototype();
        item_basic_demo_proto->init = item_basic_demo_init;
        item_basic_demo_proto->destroy = item_basic_demo_destroy;
}
