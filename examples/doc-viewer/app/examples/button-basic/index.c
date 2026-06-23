#include "index.tsx.h"
#include "index.h"
#include "../../../../../docs/examples/button-basic/main.c"

typedef struct {
        button_basic_demo_react_t base;
} button_basic_demo_t;

static void button_basic_demo_init(ui_widget_t *w)
{
        button_basic_demo_t *_that = ui_widget_add_data(
            w, button_basic_demo_proto, sizeof(button_basic_demo_t));
        button_basic_demo_proto->proto->init(w);
        button_basic_demo_react_init(w);
        button_basic_init(_that->base.refs.preview);
        button_basic_demo_update(w);
}

static button_basic_demo_t *button_basic_demo_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, button_basic_demo_proto);
}

static void button_basic_demo_destroy(ui_widget_t *w)
{
        button_basic_demo_proto->proto->destroy(w);
        button_basic_demo_react_destroy(w);
}

void button_basic_demo_update(ui_widget_t *w)
{
        button_basic_demo_react_update(w);
}

ui_widget_t *ui_create_button_basic_demo(void)
{
        return ui_create_widget_with_prototype(button_basic_demo_proto);
}

void ui_register_button_basic_demo(void)
{
        button_basic_demo_init_prototype();
        button_basic_demo_proto->init = button_basic_demo_init;
        button_basic_demo_proto->destroy = button_basic_demo_destroy;
}
