#include "index.tsx.h"
#include "index.h"

#define create_variant_button button_variants__create_variant_button
#include "../../../../../docs/examples/button-variants/main.c"
#undef create_variant_button

typedef struct {
        button_variants_demo_react_t base;
} button_variants_demo_t;

void button_variants_demo_update(ui_widget_t *w)
{
        button_variants_demo_react_update(w);
}

static void button_variants_demo_init(ui_widget_t *w)
{
        button_variants_demo_t *_that = ui_widget_add_data(
            w, button_variants_demo_proto, sizeof(button_variants_demo_t));
        button_variants_demo_proto->proto->init(w);
        button_variants_demo_react_init(w);
        button_variants_init(_that->base.refs.preview);
        button_variants_demo_update(w);
}

static void button_variants_demo_destroy(ui_widget_t *w)
{
        button_variants_demo_proto->proto->destroy(w);
        button_variants_demo_react_destroy(w);
}

ui_widget_t *ui_create_button_variants_demo(void)
{
        return ui_create_widget_with_prototype(button_variants_demo_proto);
}

void ui_register_button_variants_demo(void)
{
        button_variants_demo_init_prototype();
        button_variants_demo_proto->init = button_variants_demo_init;
        button_variants_demo_proto->destroy = button_variants_demo_destroy;
}
