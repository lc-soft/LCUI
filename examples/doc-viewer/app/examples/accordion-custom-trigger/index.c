#include "index.tsx.h"
#include "index.h"

#define create_text accordion_custom_trigger__create_text
#define add_standard_item accordion_custom_trigger__add_standard_item
#define add_custom_item accordion_custom_trigger__add_custom_item
#include "../../../../../docs/examples/accordion-custom-trigger/main.c"
#undef create_text
#undef add_standard_item
#undef add_custom_item

typedef struct {
        accordion_custom_trigger_demo_react_t base;
} accordion_custom_trigger_demo_t;

void accordion_custom_trigger_demo_update(ui_widget_t *w)
{
        accordion_custom_trigger_demo_react_update(w);
}

static void accordion_custom_trigger_demo_init(ui_widget_t *w)
{
        accordion_custom_trigger_demo_t *_that =
            ui_widget_add_data(w, accordion_custom_trigger_demo_proto,
                               sizeof(accordion_custom_trigger_demo_t));
        accordion_custom_trigger_demo_proto->proto->init(w);
        accordion_custom_trigger_demo_react_init(w);
        accordion_custom_trigger_init(_that->base.refs.preview);
        accordion_custom_trigger_demo_update(w);
}

static void accordion_custom_trigger_demo_destroy(ui_widget_t *w)
{
        accordion_custom_trigger_demo_proto->proto->destroy(w);
        accordion_custom_trigger_demo_react_destroy(w);
}

ui_widget_t *ui_create_accordion_custom_trigger_demo(void)
{
        return ui_create_widget_with_prototype(
            accordion_custom_trigger_demo_proto);
}

void ui_register_accordion_custom_trigger_demo(void)
{
        accordion_custom_trigger_demo_init_prototype();
        accordion_custom_trigger_demo_proto->init =
            accordion_custom_trigger_demo_init;
        accordion_custom_trigger_demo_proto->destroy =
            accordion_custom_trigger_demo_destroy;
}
