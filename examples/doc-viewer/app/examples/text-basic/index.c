#include "index.tsx.h"
#include "index.h"
#include "../../../../../docs/examples/text-basic/main.c"

typedef struct {
        text_basic_demo_react_t base;
} text_basic_demo_t;

static void text_basic_demo_init(ui_widget_t *w)
{
        text_basic_demo_t *_that = ui_widget_add_data(
            w, text_basic_demo_proto, sizeof(text_basic_demo_t));
        text_basic_demo_proto->proto->init(w);
        text_basic_demo_react_init(w);
        text_basic_init(_that->base.refs.preview);
        text_basic_demo_update(w);
}

static text_basic_demo_t *text_basic_demo_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, text_basic_demo_proto);
}

static void text_basic_demo_destroy(ui_widget_t *w)
{
        text_basic_demo_proto->proto->destroy(w);
        text_basic_demo_react_destroy(w);
}

void text_basic_demo_update(ui_widget_t *w)
{
        text_basic_demo_react_update(w);
}

ui_widget_t *ui_create_text_basic_demo(void)
{
        return ui_create_widget_with_prototype(text_basic_demo_proto);
}

void ui_register_text_basic_demo(void)
{
        text_basic_demo_init_prototype();
        text_basic_demo_proto->init = text_basic_demo_init;
        text_basic_demo_proto->destroy = text_basic_demo_destroy;
}
