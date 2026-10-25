#include "index.tsx.h"
#include "index.h"

#define create_menu_button sidebar_collapsed__create_menu_button
#include "../../../../../docs/examples/sidebar-collapsed/main.c"
#undef create_menu_button

typedef struct {
        sidebar_collapsed_demo_react_t base;
} sidebar_collapsed_demo_t;

void sidebar_collapsed_demo_update(ui_widget_t *w)
{
        sidebar_collapsed_demo_react_update(w);
}

static void sidebar_collapsed_demo_init(ui_widget_t *w)
{
        sidebar_collapsed_demo_t *_that = ui_widget_add_data(
            w, sidebar_collapsed_demo_proto, sizeof(sidebar_collapsed_demo_t));
        sidebar_collapsed_demo_proto->proto->init(w);
        sidebar_collapsed_demo_react_init(w);
        sidebar_collapsed_init(_that->base.refs.preview);
        sidebar_collapsed_demo_update(w);
}

static void sidebar_collapsed_demo_destroy(ui_widget_t *w)
{
        sidebar_collapsed_demo_proto->proto->destroy(w);
        sidebar_collapsed_demo_react_destroy(w);
}

ui_widget_t *ui_create_sidebar_collapsed_demo(void)
{
        return ui_create_widget_with_prototype(sidebar_collapsed_demo_proto);
}

void ui_register_sidebar_collapsed_demo(void)
{
        sidebar_collapsed_demo_init_prototype();
        sidebar_collapsed_demo_proto->init = sidebar_collapsed_demo_init;
        sidebar_collapsed_demo_proto->destroy = sidebar_collapsed_demo_destroy;
}
