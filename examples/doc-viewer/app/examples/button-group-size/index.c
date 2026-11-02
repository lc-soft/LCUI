#include "index.tsx.h"
#include "index.h"

#define create_text button_group_size__create_text
#define create_text_button button_group_size__create_text_button
#define create_icon_button button_group_size__create_icon_button
#define add_size_group button_group_size__add_size_group
#include "../../../../../docs/examples/button-group-size/main.c"
#undef create_text
#undef create_text_button
#undef create_icon_button
#undef add_size_group

typedef struct {
        button_group_size_demo_react_t base;
} button_group_size_demo_t;

void button_group_size_demo_update(ui_widget_t *w)
{
        button_group_size_demo_react_update(w);
}

static void button_group_size_demo_init(ui_widget_t *w)
{
        button_group_size_demo_t *_that = ui_widget_add_data(
            w, button_group_size_demo_proto, sizeof(button_group_size_demo_t));
        button_group_size_demo_proto->proto->init(w);
        button_group_size_demo_react_init(w);
        button_group_size_init(_that->base.refs.preview);
        button_group_size_demo_update(w);
}

static void button_group_size_demo_destroy(ui_widget_t *w)
{
        button_group_size_demo_proto->proto->destroy(w);
        button_group_size_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_group_size_demo(void)
{
        return ui_create_widget_with_prototype(button_group_size_demo_proto);
}

void ui_register_button_group_size_demo(void)
{
        button_group_size_demo_init_prototype();
        button_group_size_demo_proto->init = button_group_size_demo_init;
        button_group_size_demo_proto->destroy = button_group_size_demo_destroy;
}
