#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/card-small/main.c"

typedef struct {
        card_small_demo_react_t base;
} card_small_demo_t;

void card_small_demo_update(ui_widget_t *w)
{
        card_small_demo_react_update(w);
}

static void card_small_demo_init(ui_widget_t *w)
{
        card_small_demo_t *_that = ui_widget_add_data(
            w, card_small_demo_proto, sizeof(card_small_demo_t));
        card_small_demo_proto->proto->init(w);
        card_small_demo_react_init(w);
        card_small_init(_that->base.refs.preview);
        card_small_demo_update(w);
}

static void card_small_demo_destroy(ui_widget_t *w)
{
        card_small_demo_proto->proto->destroy(w);
        card_small_demo_react_destroy(w);
}

ui_widget_t *ui_create_card_small_demo(void)
{
        return ui_create_widget_with_prototype(card_small_demo_proto);
}

void ui_register_card_small_demo(void)
{
        card_small_demo_init_prototype();
        card_small_demo_proto->init = card_small_demo_init;
        card_small_demo_proto->destroy = card_small_demo_destroy;
}
