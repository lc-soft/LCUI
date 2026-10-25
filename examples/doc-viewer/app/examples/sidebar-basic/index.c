#include "index.tsx.h"
#include "index.h"

#define create_menu_button sidebar_basic__create_menu_button
#include "../../../../../docs/examples/sidebar-basic/main.c"
#undef create_menu_button

typedef struct {
        sidebar_basic_demo_react_t base;
} sidebar_basic_demo_t;

void sidebar_basic_demo_update(ui_widget_t *w)
{
        sidebar_basic_demo_react_update(w);
}

static void sidebar_basic_demo_init(ui_widget_t *w)
{
        sidebar_basic_demo_t *_that = ui_widget_add_data(
            w, sidebar_basic_demo_proto, sizeof(sidebar_basic_demo_t));
        sidebar_basic_demo_proto->proto->init(w);
        sidebar_basic_demo_react_init(w);
        sidebar_basic_init(_that->base.refs.preview);
        sidebar_basic_demo_update(w);
}

static void sidebar_basic_demo_destroy(ui_widget_t *w)
{
        sidebar_basic_demo_proto->proto->destroy(w);
        sidebar_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_sidebar_basic_demo(void)
{
        return ui_create_widget_with_prototype(sidebar_basic_demo_proto);
}

void ui_register_sidebar_basic_demo(void)
{
        sidebar_basic_demo_init_prototype();
        sidebar_basic_demo_proto->init = sidebar_basic_demo_init;
        sidebar_basic_demo_proto->destroy = sidebar_basic_demo_destroy;
}
