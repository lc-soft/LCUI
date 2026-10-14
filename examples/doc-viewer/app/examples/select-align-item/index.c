#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/select-align-item/main.c"

typedef struct {
        select_align_item_demo_react_t base;
} select_align_item_demo_t;

void select_align_item_demo_update(ui_widget_t *w)
{
        select_align_item_demo_react_update(w);
}

static void select_align_item_demo_init(ui_widget_t *w)
{
        select_align_item_demo_t *_that = ui_widget_add_data(
            w, select_align_item_demo_proto, sizeof(select_align_item_demo_t));
        select_align_item_demo_proto->proto->init(w);
        select_align_item_demo_react_init(w);
        select_align_item_init(_that->base.refs.preview);
        select_align_item_demo_update(w);
}

static void select_align_item_demo_destroy(ui_widget_t *w)
{
        select_align_item_demo_proto->proto->destroy(w);
        select_align_item_demo_react_destroy(w);
}

ui_widget_t *ui_create_select_align_item_demo(void)
{
        return ui_create_widget_with_prototype(select_align_item_demo_proto);
}

void ui_register_select_align_item_demo(void)
{
        select_align_item_demo_init_prototype();
        select_align_item_demo_proto->init = select_align_item_demo_init;
        select_align_item_demo_proto->destroy = select_align_item_demo_destroy;
}
