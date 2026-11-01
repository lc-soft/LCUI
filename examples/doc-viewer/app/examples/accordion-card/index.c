#include "index.tsx.h"
#include "index.h"

#define create_text accordion_card__create_text
#define add_item accordion_card__add_item
#include "../../../../../docs/examples/accordion-card/main.c"
#undef create_text
#undef add_item

typedef struct {
        accordion_card_demo_react_t base;
} accordion_card_demo_t;

void accordion_card_demo_update(ui_widget_t *w)
{
        accordion_card_demo_react_update(w);
}

static void accordion_card_demo_init(ui_widget_t *w)
{
        accordion_card_demo_t *_that = ui_widget_add_data(
            w, accordion_card_demo_proto, sizeof(accordion_card_demo_t));
        accordion_card_demo_proto->proto->init(w);
        accordion_card_demo_react_init(w);
        accordion_card_init(_that->base.refs.preview);
        accordion_card_demo_update(w);
}

static void accordion_card_demo_destroy(ui_widget_t *w)
{
        accordion_card_demo_proto->proto->destroy(w);
        accordion_card_demo_react_destroy(w);
}

ui_widget_t *ui_create_accordion_card_demo(void)
{
        return ui_create_widget_with_prototype(accordion_card_demo_proto);
}

void ui_register_accordion_card_demo(void)
{
        accordion_card_demo_init_prototype();
        accordion_card_demo_proto->init = accordion_card_demo_init;
        accordion_card_demo_proto->destroy = accordion_card_demo_destroy;
}
