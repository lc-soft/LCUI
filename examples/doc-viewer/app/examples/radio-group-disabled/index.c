#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/radio-group-disabled/main.c"

typedef struct {
        radio_group_disabled_demo_react_t base;
} radio_group_disabled_demo_t;

static void radio_group_disabled_demo_init(ui_widget_t *w)
{
        radio_group_disabled_demo_t *_that =
            ui_widget_add_data(w, radio_group_disabled_demo_proto,
                               sizeof(radio_group_disabled_demo_t));
        radio_group_disabled_demo_proto->proto->init(w);
        radio_group_disabled_demo_react_init(w);
        radio_group_disabled_init(_that->base.refs.preview);
        radio_group_disabled_demo_update(w);
}

static void radio_group_disabled_demo_destroy(ui_widget_t *w)
{
        radio_group_disabled_demo_proto->proto->destroy(w);
        radio_group_disabled_demo_react_destroy(w);
}

void radio_group_disabled_demo_update(ui_widget_t *w)
{
        radio_group_disabled_demo_react_update(w);
}

ui_widget_t *ui_create_radio_group_disabled_demo(void)
{
        return ui_create_widget_with_prototype(radio_group_disabled_demo_proto);
}

void ui_register_radio_group_disabled_demo(void)
{
        radio_group_disabled_demo_init_prototype();
        radio_group_disabled_demo_proto->init = radio_group_disabled_demo_init;
        radio_group_disabled_demo_proto->destroy =
            radio_group_disabled_demo_destroy;
}
