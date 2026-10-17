#include "index.tsx.h"
#include "index.h"

#define add_row radio_group_description__add_row
#include "../../../../../docs/examples/radio-group-description/main.c"
#undef add_row

typedef struct {
        radio_group_description_demo_react_t base;
} radio_group_description_demo_t;

void radio_group_description_demo_update(ui_widget_t *w)
{
        radio_group_description_demo_react_update(w);
}

static void radio_group_description_demo_init(ui_widget_t *w)
{
        radio_group_description_demo_t *_that =
            ui_widget_add_data(w, radio_group_description_demo_proto,
                               sizeof(radio_group_description_demo_t));
        radio_group_description_demo_proto->proto->init(w);
        radio_group_description_demo_react_init(w);
        radio_group_description_init(_that->base.refs.preview);
        radio_group_description_demo_update(w);
}

static void radio_group_description_demo_destroy(ui_widget_t *w)
{
        radio_group_description_demo_proto->proto->destroy(w);
        radio_group_description_demo_react_destroy(w);
}

ui_widget_t *ui_create_radio_group_description_demo(void)
{
        return ui_create_widget_with_prototype(
            radio_group_description_demo_proto);
}

void ui_register_radio_group_description_demo(void)
{
        radio_group_description_demo_init_prototype();
        radio_group_description_demo_proto->init =
            radio_group_description_demo_init;
        radio_group_description_demo_proto->destroy =
            radio_group_description_demo_destroy;
}
