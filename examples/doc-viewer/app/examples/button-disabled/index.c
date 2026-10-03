#include "index.tsx.h"
#include "index.h"

#define create_disabled_button button_disabled__create_disabled_button
#include "../../../../../docs/examples/button-disabled/main.c"
#undef create_disabled_button

typedef struct {
        button_disabled_demo_react_t base;
} button_disabled_demo_t;

void button_disabled_demo_update(ui_widget_t *w)
{
        button_disabled_demo_react_update(w);
}

static void button_disabled_demo_init(ui_widget_t *w)
{
        button_disabled_demo_t *_that = ui_widget_add_data(
            w, button_disabled_demo_proto, sizeof(button_disabled_demo_t));
        button_disabled_demo_proto->proto->init(w);
        button_disabled_demo_react_init(w);
        button_disabled_init(_that->base.refs.preview);
        button_disabled_demo_update(w);
}

static void button_disabled_demo_destroy(ui_widget_t *w)
{
        button_disabled_demo_proto->proto->destroy(w);
        button_disabled_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_disabled_demo(void)
{
        return ui_create_widget_with_prototype(button_disabled_demo_proto);
}

void ui_register_button_disabled_demo(void)
{
        button_disabled_demo_init_prototype();
        button_disabled_demo_proto->init = button_disabled_demo_init;
        button_disabled_demo_proto->destroy = button_disabled_demo_destroy;
}
