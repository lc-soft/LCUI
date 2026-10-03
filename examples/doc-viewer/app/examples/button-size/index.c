#include "index.tsx.h"
#include "index.h"

#define create_size_button button_size__create_size_button
#define create_icon_size_button button_size__create_icon_size_button
#define create_size_pair button_size__create_size_pair
#include "../../../../../docs/examples/button-size/main.c"
#undef create_size_button
#undef create_icon_size_button
#undef create_size_pair

typedef struct {
        button_size_demo_react_t base;
} button_size_demo_t;

void button_size_demo_update(ui_widget_t *w)
{
        button_size_demo_react_update(w);
}

static void button_size_demo_init(ui_widget_t *w)
{
        button_size_demo_t *_that = ui_widget_add_data(
            w, button_size_demo_proto, sizeof(button_size_demo_t));
        button_size_demo_proto->proto->init(w);
        button_size_demo_react_init(w);
        button_size_init(_that->base.refs.preview);
        button_size_demo_update(w);
}

static void button_size_demo_destroy(ui_widget_t *w)
{
        button_size_demo_proto->proto->destroy(w);
        button_size_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_size_demo(void)
{
        return ui_create_widget_with_prototype(button_size_demo_proto);
}

void ui_register_button_size_demo(void)
{
        button_size_demo_init_prototype();
        button_size_demo_proto->init = button_size_demo_init;
        button_size_demo_proto->destroy = button_size_demo_destroy;
}
