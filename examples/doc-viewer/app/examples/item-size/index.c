#include "index.tsx.h"
#include "index.h"

#define create_item item_size__create_item
#include "../../../../../docs/examples/item-size/main.c"
#undef create_item

typedef struct {
        item_size_demo_react_t base;
} item_size_demo_t;

void item_size_demo_update(ui_widget_t *w)
{
        item_size_demo_react_update(w);
}

static void item_size_demo_init(ui_widget_t *w)
{
        item_size_demo_t *_that = ui_widget_add_data(w, item_size_demo_proto,
                                                     sizeof(item_size_demo_t));
        item_size_demo_proto->proto->init(w);
        item_size_demo_react_init(w);
        item_size_init(_that->base.refs.preview);
        item_size_demo_update(w);
}

static void item_size_demo_destroy(ui_widget_t *w)
{
        item_size_demo_proto->proto->destroy(w);
        item_size_demo_react_destroy(w);
}

ui_widget_t *ui_create_item_size_demo(void)
{
        return ui_create_widget_with_prototype(item_size_demo_proto);
}

void ui_register_item_size_demo(void)
{
        item_size_demo_init_prototype();
        item_size_demo_proto->init = item_size_demo_init;
        item_size_demo_proto->destroy = item_size_demo_destroy;
}
