#include "index.tsx.h"
#include "index.h"

#define add_item select_scrollable__add_item
#define add_label select_scrollable__add_label
#include "../../../../../docs/examples/select-scrollable/main.c"
#undef add_item
#undef add_label

typedef struct {
        select_scrollable_demo_react_t base;
} select_scrollable_demo_t;

void select_scrollable_demo_update(ui_widget_t *w)
{
        select_scrollable_demo_react_update(w);
}

static void select_scrollable_demo_init(ui_widget_t *w)
{
        select_scrollable_demo_t *_that = ui_widget_add_data(
            w, select_scrollable_demo_proto, sizeof(select_scrollable_demo_t));
        select_scrollable_demo_proto->proto->init(w);
        select_scrollable_demo_react_init(w);
        select_scrollable_init(_that->base.refs.preview);
        select_scrollable_demo_update(w);
}

static void select_scrollable_demo_destroy(ui_widget_t *w)
{
        select_scrollable_demo_proto->proto->destroy(w);
        select_scrollable_demo_react_destroy(w);
}

ui_widget_t *ui_create_select_scrollable_demo(void)
{
        return ui_create_widget_with_prototype(select_scrollable_demo_proto);
}

void ui_register_select_scrollable_demo(void)
{
        select_scrollable_demo_init_prototype();
        select_scrollable_demo_proto->init = select_scrollable_demo_init;
        select_scrollable_demo_proto->destroy = select_scrollable_demo_destroy;
}
