#include "index.tsx.h"
#include "index.h"

#define add_item select_groups__add_item
#define add_label select_groups__add_label
#include "../../../../../docs/examples/select-groups/main.c"
#undef add_item
#undef add_label

typedef struct {
        select_groups_demo_react_t base;
} select_groups_demo_t;

void select_groups_demo_update(ui_widget_t *w)
{
        select_groups_demo_react_update(w);
}

static void select_groups_demo_init(ui_widget_t *w)
{
        select_groups_demo_t *_that = ui_widget_add_data(
            w, select_groups_demo_proto, sizeof(select_groups_demo_t));
        select_groups_demo_proto->proto->init(w);
        select_groups_demo_react_init(w);
        select_groups_init(_that->base.refs.preview);
        select_groups_demo_update(w);
}

static void select_groups_demo_destroy(ui_widget_t *w)
{
        select_groups_demo_proto->proto->destroy(w);
        select_groups_demo_react_destroy(w);
}

ui_widget_t *ui_create_select_groups_demo(void)
{
        return ui_create_widget_with_prototype(select_groups_demo_proto);
}

void ui_register_select_groups_demo(void)
{
        select_groups_demo_init_prototype();
        select_groups_demo_proto->init = select_groups_demo_init;
        select_groups_demo_proto->destroy = select_groups_demo_destroy;
}
