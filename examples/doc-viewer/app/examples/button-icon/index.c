#include "index.tsx.h"
#include "index.h"

#define create_icon_button button_icon__create_icon_button
#include "../../../../../docs/examples/button-icon/main.c"
#undef create_icon_button

typedef struct {
        button_icon_demo_react_t base;
} button_icon_demo_t;

void button_icon_demo_update(ui_widget_t *w)
{
        button_icon_demo_react_update(w);
}

static void button_icon_demo_init(ui_widget_t *w)
{
        button_icon_demo_t *_that = ui_widget_add_data(
            w, button_icon_demo_proto, sizeof(button_icon_demo_t));
        button_icon_demo_proto->proto->init(w);
        button_icon_demo_react_init(w);
        button_icon_init(_that->base.refs.preview);
        button_icon_demo_update(w);
}

static void button_icon_demo_destroy(ui_widget_t *w)
{
        button_icon_demo_proto->proto->destroy(w);
        button_icon_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_icon_demo(void)
{
        return ui_create_widget_with_prototype(button_icon_demo_proto);
}

void ui_register_button_icon_demo(void)
{
        button_icon_demo_init_prototype();
        button_icon_demo_proto->init = button_icon_demo_init;
        button_icon_demo_proto->destroy = button_icon_demo_destroy;
}
