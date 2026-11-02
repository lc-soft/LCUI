#include "index.tsx.h"
#include "index.h"

#define create_button button_group_separator__create_button
#include "../../../../../docs/examples/button-group-separator/main.c"
#undef create_button

typedef struct {
        button_group_separator_demo_react_t base;
} button_group_separator_demo_t;

void button_group_separator_demo_update(ui_widget_t *w)
{
        button_group_separator_demo_react_update(w);
}

static void button_group_separator_demo_init(ui_widget_t *w)
{
        button_group_separator_demo_t *_that =
            ui_widget_add_data(w, button_group_separator_demo_proto,
                               sizeof(button_group_separator_demo_t));
        button_group_separator_demo_proto->proto->init(w);
        button_group_separator_demo_react_init(w);
        button_group_separator_init(_that->base.refs.preview);
        button_group_separator_demo_update(w);
}

static void button_group_separator_demo_destroy(ui_widget_t *w)
{
        button_group_separator_demo_proto->proto->destroy(w);
        button_group_separator_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_group_separator_demo(void)
{
        return ui_create_widget_with_prototype(
            button_group_separator_demo_proto);
}

void ui_register_button_group_separator_demo(void)
{
        button_group_separator_demo_init_prototype();
        button_group_separator_demo_proto->init =
            button_group_separator_demo_init;
        button_group_separator_demo_proto->destroy =
            button_group_separator_demo_destroy;
}
