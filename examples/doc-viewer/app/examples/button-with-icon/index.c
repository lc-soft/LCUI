#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/button-with-icon/main.c"

typedef struct {
        button_with_icon_demo_react_t base;
} button_with_icon_demo_t;

void button_with_icon_demo_update(ui_widget_t *w)
{
        button_with_icon_demo_react_update(w);
}

static void button_with_icon_demo_init(ui_widget_t *w)
{
        button_with_icon_demo_t *_that = ui_widget_add_data(
            w, button_with_icon_demo_proto, sizeof(button_with_icon_demo_t));
        button_with_icon_demo_proto->proto->init(w);
        button_with_icon_demo_react_init(w);
        button_with_icon_init(_that->base.refs.preview);
        button_with_icon_demo_update(w);
}

static void button_with_icon_demo_destroy(ui_widget_t *w)
{
        button_with_icon_demo_proto->proto->destroy(w);
        button_with_icon_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_with_icon_demo(void)
{
        return ui_create_widget_with_prototype(button_with_icon_demo_proto);
}

void ui_register_button_with_icon_demo(void)
{
        button_with_icon_demo_init_prototype();
        button_with_icon_demo_proto->init = button_with_icon_demo_init;
        button_with_icon_demo_proto->destroy = button_with_icon_demo_destroy;
}
