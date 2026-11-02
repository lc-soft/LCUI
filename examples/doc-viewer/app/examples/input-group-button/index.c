#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/input-group-button/main.c"

typedef struct {
        input_group_button_demo_react_t base;
} input_group_button_demo_t;

void input_group_button_demo_update(ui_widget_t *w)
{
        input_group_button_demo_react_update(w);
}

static void input_group_button_demo_init(ui_widget_t *w)
{
        input_group_button_demo_t *_that =
            ui_widget_add_data(w, input_group_button_demo_proto,
                               sizeof(input_group_button_demo_t));
        input_group_button_demo_proto->proto->init(w);
        input_group_button_demo_react_init(w);
        input_group_button_init(_that->base.refs.preview);
        input_group_button_demo_update(w);
}

static void input_group_button_demo_destroy(ui_widget_t *w)
{
        input_group_button_demo_proto->proto->destroy(w);
        input_group_button_demo_react_destroy(w);
}

ui_widget_t *ui_create_input_group_button_demo(void)
{
        return ui_create_widget_with_prototype(input_group_button_demo_proto);
}

void ui_register_input_group_button_demo(void)
{
        input_group_button_demo_init_prototype();
        input_group_button_demo_proto->init = input_group_button_demo_init;
        input_group_button_demo_proto->destroy =
            input_group_button_demo_destroy;
}
