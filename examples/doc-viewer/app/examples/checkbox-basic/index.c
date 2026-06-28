#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/checkbox-basic/main.c"

typedef struct {
        checkbox_basic_demo_react_t base;
} checkbox_basic_demo_t;

void checkbox_basic_demo_update(ui_widget_t *w)
{
        checkbox_basic_demo_react_update(w);
}

static void checkbox_basic_demo_init(ui_widget_t *w)
{
        checkbox_basic_demo_t *_that = ui_widget_add_data(
            w, checkbox_basic_demo_proto, sizeof(checkbox_basic_demo_t));
        checkbox_basic_demo_proto->proto->init(w);
        checkbox_basic_demo_react_init(w);
        checkbox_basic_init(_that->base.refs.preview);
        checkbox_basic_demo_update(w);
}

static void checkbox_basic_demo_destroy(ui_widget_t *w)
{
        checkbox_basic_demo_proto->proto->destroy(w);
        checkbox_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_checkbox_basic_demo(void)
{
        return ui_create_widget_with_prototype(checkbox_basic_demo_proto);
}

void ui_register_checkbox_basic_demo(void)
{
        checkbox_basic_demo_init_prototype();
        checkbox_basic_demo_proto->init = checkbox_basic_demo_init;
        checkbox_basic_demo_proto->destroy = checkbox_basic_demo_destroy;
}
