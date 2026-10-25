#include "index.tsx.h"
#include "index.h"

#define create_item dropdown_menu_demo__create_item
#define create_group dropdown_menu_demo__create_group
#define create_sub_trigger dropdown_menu_demo__create_sub_trigger
#include "../../../../../docs/examples/dropdown-menu-demo/main.c"
#undef create_item
#undef create_group
#undef create_sub_trigger

typedef struct {
        dropdown_menu_demo_demo_react_t base;
} dropdown_menu_demo_demo_t;

void dropdown_menu_demo_demo_update(ui_widget_t *w)
{
        dropdown_menu_demo_demo_react_update(w);
}

static void dropdown_menu_demo_demo_init(ui_widget_t *w)
{
        dropdown_menu_demo_demo_t *_that =
            ui_widget_add_data(w, dropdown_menu_demo_demo_proto,
                               sizeof(dropdown_menu_demo_demo_t));
        dropdown_menu_demo_demo_proto->proto->init(w);
        dropdown_menu_demo_demo_react_init(w);
        dropdown_menu_demo_init(_that->base.refs.preview);
        dropdown_menu_demo_demo_update(w);
}

static void dropdown_menu_demo_demo_destroy(ui_widget_t *w)
{
        dropdown_menu_demo_demo_proto->proto->destroy(w);
        dropdown_menu_demo_demo_react_destroy(w);
}

ui_widget_t *ui_create_dropdown_menu_demo_demo(void)
{
        return ui_create_widget_with_prototype(dropdown_menu_demo_demo_proto);
}

void ui_register_dropdown_menu_demo_demo(void)
{
        dropdown_menu_demo_demo_init_prototype();
        dropdown_menu_demo_demo_proto->init = dropdown_menu_demo_demo_init;
        dropdown_menu_demo_demo_proto->destroy =
            dropdown_menu_demo_demo_destroy;
}
