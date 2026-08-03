#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/select-basic/main.c"

typedef struct {
        select_basic_demo_react_t base;
} select_basic_demo_t;

static void select_basic_demo_init(ui_widget_t *w)
{
        select_basic_demo_t *_that = ui_widget_add_data(
            w, select_basic_demo_proto, sizeof(select_basic_demo_t));
        select_basic_demo_proto->proto->init(w);
        select_basic_demo_react_init(w);
        select_basic_init(_that->base.refs.preview);
        select_basic_demo_update(w);
}

static void select_basic_demo_destroy(ui_widget_t *w)
{
        select_basic_demo_proto->proto->destroy(w);
        select_basic_demo_react_destroy(w);
}

static void select_basic_demo_update(ui_widget_t *w)
{
        select_basic_demo_react_update(w);
}

ui_widget_t *ui_create_select_basic_demo(void)
{
        return ui_create_widget_with_prototype(select_basic_demo_proto);
}

void ui_register_select_basic_demo(void)
{
        select_basic_demo_init_prototype();
        select_basic_demo_proto->init = select_basic_demo_init;
        select_basic_demo_proto->destroy = select_basic_demo_destroy;
}
