#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/checkbox-disabled/main.c"

typedef struct {
        checkbox_disabled_demo_react_t base;
} checkbox_disabled_demo_t;

static void checkbox_disabled_demo_init(ui_widget_t *w)
{
        checkbox_disabled_demo_t *_that = ui_widget_add_data(
            w, checkbox_disabled_demo_proto, sizeof(checkbox_disabled_demo_t));
        checkbox_disabled_demo_proto->proto->init(w);
        checkbox_disabled_demo_react_init(w);
        checkbox_disabled_init(_that->base.refs.preview);
        checkbox_disabled_demo_update(w);
}

static void checkbox_disabled_demo_destroy(ui_widget_t *w)
{
        checkbox_disabled_demo_proto->proto->destroy(w);
        checkbox_disabled_demo_react_destroy(w);
}

void checkbox_disabled_demo_update(ui_widget_t *w)
{
        checkbox_disabled_demo_react_update(w);
}

ui_widget_t *ui_create_checkbox_disabled_demo(void)
{
        return ui_create_widget_with_prototype(checkbox_disabled_demo_proto);
}

void ui_register_checkbox_disabled_demo(void)
{
        checkbox_disabled_demo_init_prototype();
        checkbox_disabled_demo_proto->init = checkbox_disabled_demo_init;
        checkbox_disabled_demo_proto->destroy = checkbox_disabled_demo_destroy;
}
