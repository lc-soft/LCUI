#include "index.tsx.h"
#include "index.h"

#define create_menu_button sidebar_group_collapsible__create_menu_button
#include "../../../../../docs/examples/sidebar-group-collapsible/main.c"
#undef create_menu_button

typedef struct {
        sidebar_group_collapsible_demo_react_t base;
} sidebar_group_collapsible_demo_t;

void sidebar_group_collapsible_demo_update(ui_widget_t *w)
{
        sidebar_group_collapsible_demo_react_update(w);
}

static void sidebar_group_collapsible_demo_init(ui_widget_t *w)
{
        sidebar_group_collapsible_demo_t *_that =
            ui_widget_add_data(w, sidebar_group_collapsible_demo_proto,
                               sizeof(sidebar_group_collapsible_demo_t));
        sidebar_group_collapsible_demo_proto->proto->init(w);
        sidebar_group_collapsible_demo_react_init(w);
        sidebar_group_collapsible_init(_that->base.refs.preview);
        sidebar_group_collapsible_demo_update(w);
}

static void sidebar_group_collapsible_demo_destroy(ui_widget_t *w)
{
        sidebar_group_collapsible_demo_proto->proto->destroy(w);
        sidebar_group_collapsible_demo_react_destroy(w);
}

ui_widget_t *ui_create_sidebar_group_collapsible_demo(void)
{
        return ui_create_widget_with_prototype(
            sidebar_group_collapsible_demo_proto);
}

void ui_register_sidebar_group_collapsible_demo(void)
{
        sidebar_group_collapsible_demo_init_prototype();
        sidebar_group_collapsible_demo_proto->init =
            sidebar_group_collapsible_demo_init;
        sidebar_group_collapsible_demo_proto->destroy =
            sidebar_group_collapsible_demo_destroy;
}
