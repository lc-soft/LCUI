#include "index.tsx.h"
#include "index.h"

#define create_text card_basic__create_text
#include "../../../../../docs/examples/card-basic/main.c"
#undef create_text

typedef struct {
        card_basic_demo_react_t base;
} card_basic_demo_t;

void card_basic_demo_update(ui_widget_t *w)
{
        card_basic_demo_react_update(w);
}

static void card_basic_demo_init(ui_widget_t *w)
{
        card_basic_demo_t *_that = ui_widget_add_data(
            w, card_basic_demo_proto, sizeof(card_basic_demo_t));
        card_basic_demo_proto->proto->init(w);
        card_basic_demo_react_init(w);
        card_basic_init(_that->base.refs.preview);
        card_basic_demo_update(w);
}

static void card_basic_demo_destroy(ui_widget_t *w)
{
        card_basic_demo_proto->proto->destroy(w);
        card_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_card_basic_demo(void)
{
        return ui_create_widget_with_prototype(card_basic_demo_proto);
}

void ui_register_card_basic_demo(void)
{
        card_basic_demo_init_prototype();
        card_basic_demo_proto->init = card_basic_demo_init;
        card_basic_demo_proto->destroy = card_basic_demo_destroy;
}
