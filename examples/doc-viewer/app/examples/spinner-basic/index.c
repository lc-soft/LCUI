#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/spinner-basic/main.c"

typedef struct {
        spinner_basic_demo_react_t base;
} spinner_basic_demo_t;

void spinner_basic_demo_update(ui_widget_t *w)
{
        spinner_basic_demo_react_update(w);
}

static void spinner_basic_demo_init(ui_widget_t *w)
{
        spinner_basic_demo_t *_that = ui_widget_add_data(
            w, spinner_basic_demo_proto, sizeof(spinner_basic_demo_t));
        spinner_basic_demo_proto->proto->init(w);
        spinner_basic_demo_react_init(w);
        spinner_basic_init(_that->base.refs.preview);
        spinner_basic_demo_update(w);
}

static void spinner_basic_demo_destroy(ui_widget_t *w)
{
        spinner_basic_demo_proto->proto->destroy(w);
        spinner_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_spinner_basic_demo(void)
{
        return ui_create_widget_with_prototype(spinner_basic_demo_proto);
}

void ui_register_spinner_basic_demo(void)
{
        spinner_basic_demo_init_prototype();
        spinner_basic_demo_proto->init = spinner_basic_demo_init;
        spinner_basic_demo_proto->destroy = spinner_basic_demo_destroy;
}
