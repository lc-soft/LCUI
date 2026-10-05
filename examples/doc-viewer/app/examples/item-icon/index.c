#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/item-icon/main.c"

typedef struct {
        item_icon_demo_react_t base;
} item_icon_demo_t;

void item_icon_demo_update(ui_widget_t *w)
{
        item_icon_demo_react_update(w);
}

static void item_icon_demo_init(ui_widget_t *w)
{
        item_icon_demo_t *_that = ui_widget_add_data(w, item_icon_demo_proto,
                                                     sizeof(item_icon_demo_t));
        item_icon_demo_proto->proto->init(w);
        item_icon_demo_react_init(w);
        item_icon_init(_that->base.refs.preview);
        item_icon_demo_update(w);
}

static void item_icon_demo_destroy(ui_widget_t *w)
{
        item_icon_demo_proto->proto->destroy(w);
        item_icon_demo_react_destroy(w);
}

ui_widget_t *ui_create_item_icon_demo(void)
{
        return ui_create_widget_with_prototype(item_icon_demo_proto);
}

void ui_register_item_icon_demo(void)
{
        item_icon_demo_init_prototype();
        item_icon_demo_proto->init = item_icon_demo_init;
        item_icon_demo_proto->destroy = item_icon_demo_destroy;
}
