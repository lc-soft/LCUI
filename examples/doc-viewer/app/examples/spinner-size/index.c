#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/spinner-size/main.c"

typedef struct {
        spinner_size_demo_react_t base;
} spinner_size_demo_t;

void spinner_size_demo_update(ui_widget_t *w)
{
        spinner_size_demo_react_update(w);
}

static void spinner_size_demo_init(ui_widget_t *w)
{
        spinner_size_demo_t *_that = ui_widget_add_data(
            w, spinner_size_demo_proto, sizeof(spinner_size_demo_t));
        spinner_size_demo_proto->proto->init(w);
        spinner_size_demo_react_init(w);
        spinner_size_init(_that->base.refs.preview);
        spinner_size_demo_update(w);
}

static void spinner_size_demo_destroy(ui_widget_t *w)
{
        spinner_size_demo_proto->proto->destroy(w);
        spinner_size_demo_react_destroy(w);
}

ui_widget_t *ui_create_spinner_size_demo(void)
{
        return ui_create_widget_with_prototype(spinner_size_demo_proto);
}

void ui_register_spinner_size_demo(void)
{
        spinner_size_demo_init_prototype();
        spinner_size_demo_proto->init = spinner_size_demo_init;
        spinner_size_demo_proto->destroy = spinner_size_demo_destroy;
}
