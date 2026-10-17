#include "index.tsx.h"
#include "index.h"

#define add_card radio_group_choice_card__add_card
#include "../../../../../docs/examples/radio-group-choice-card/main.c"
#undef add_card

typedef struct {
        radio_group_choice_card_demo_react_t base;
} radio_group_choice_card_demo_t;

void radio_group_choice_card_demo_update(ui_widget_t *w)
{
        radio_group_choice_card_demo_react_update(w);
}

static void radio_group_choice_card_demo_init(ui_widget_t *w)
{
        radio_group_choice_card_demo_t *_that =
            ui_widget_add_data(w, radio_group_choice_card_demo_proto,
                               sizeof(radio_group_choice_card_demo_t));
        radio_group_choice_card_demo_proto->proto->init(w);
        radio_group_choice_card_demo_react_init(w);
        radio_group_choice_card_init(_that->base.refs.preview);
        radio_group_choice_card_demo_update(w);
}

static void radio_group_choice_card_demo_destroy(ui_widget_t *w)
{
        radio_group_choice_card_demo_proto->proto->destroy(w);
        radio_group_choice_card_demo_react_destroy(w);
}

ui_widget_t *ui_create_radio_group_choice_card_demo(void)
{
        return ui_create_widget_with_prototype(
            radio_group_choice_card_demo_proto);
}

void ui_register_radio_group_choice_card_demo(void)
{
        radio_group_choice_card_demo_init_prototype();
        radio_group_choice_card_demo_proto->init =
            radio_group_choice_card_demo_init;
        radio_group_choice_card_demo_proto->destroy =
            radio_group_choice_card_demo_destroy;
}
