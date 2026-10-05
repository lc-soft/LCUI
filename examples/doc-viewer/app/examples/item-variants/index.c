#include "index.tsx.h"
#include "index.h"

#define create_item item_variants__create_item
#include "../../../../../docs/examples/item-variants/main.c"
#undef create_item

typedef struct {
        item_variants_demo_react_t base;
} item_variants_demo_t;

void item_variants_demo_update(ui_widget_t *w)
{
        item_variants_demo_react_update(w);
}

static void item_variants_demo_init(ui_widget_t *w)
{
        item_variants_demo_t *_that = ui_widget_add_data(
            w, item_variants_demo_proto, sizeof(item_variants_demo_t));
        item_variants_demo_proto->proto->init(w);
        item_variants_demo_react_init(w);
        item_variants_init(_that->base.refs.preview);
        item_variants_demo_update(w);
}

static void item_variants_demo_destroy(ui_widget_t *w)
{
        item_variants_demo_proto->proto->destroy(w);
        item_variants_demo_react_destroy(w);
}

ui_widget_t *ui_create_item_variants_demo(void)
{
        return ui_create_widget_with_prototype(item_variants_demo_proto);
}

void ui_register_item_variants_demo(void)
{
        item_variants_demo_init_prototype();
        item_variants_demo_proto->init = item_variants_demo_init;
        item_variants_demo_proto->destroy = item_variants_demo_destroy;
}
