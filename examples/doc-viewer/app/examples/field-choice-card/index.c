#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/field-choice-card/main.c"

typedef struct {
        field_choice_card_demo_react_t base;
} field_choice_card_demo_t;

void field_choice_card_demo_update(ui_widget_t *w)
{
        field_choice_card_demo_react_update(w);
}

static void field_choice_card_demo_init(ui_widget_t *w)
{
        field_choice_card_demo_t *_that = ui_widget_add_data(
            w, field_choice_card_demo_proto, sizeof(field_choice_card_demo_t));
        field_choice_card_demo_proto->proto->init(w);
        field_choice_card_demo_react_init(w);
        field_choice_card_init(_that->base.refs.preview);
        field_choice_card_demo_update(w);
}

static void field_choice_card_demo_destroy(ui_widget_t *w)
{
        field_choice_card_demo_proto->proto->destroy(w);
        field_choice_card_demo_react_destroy(w);
}

ui_widget_t *ui_create_field_choice_card_demo(void)
{
        return ui_create_widget_with_prototype(field_choice_card_demo_proto);
}

void ui_register_field_choice_card_demo(void)
{
        field_choice_card_demo_init_prototype();
        field_choice_card_demo_proto->init = field_choice_card_demo_init;
        field_choice_card_demo_proto->destroy = field_choice_card_demo_destroy;
}
