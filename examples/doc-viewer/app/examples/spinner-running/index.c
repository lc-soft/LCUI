#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/spinner-running/main.c"

typedef struct {
        spinner_running_demo_react_t base;
} spinner_running_demo_t;

void spinner_running_demo_update(ui_widget_t *w)
{
        spinner_running_demo_react_update(w);
}

static void spinner_running_demo_init(ui_widget_t *w)
{
        spinner_running_demo_t *_that = ui_widget_add_data(
            w, spinner_running_demo_proto, sizeof(spinner_running_demo_t));
        spinner_running_demo_proto->proto->init(w);
        spinner_running_demo_react_init(w);
        spinner_running_init(_that->base.refs.preview);
        spinner_running_demo_update(w);
}

static void spinner_running_demo_destroy(ui_widget_t *w)
{
        spinner_running_demo_proto->proto->destroy(w);
        spinner_running_demo_react_destroy(w);
}

ui_widget_t *ui_create_spinner_running_demo(void)
{
        return ui_create_widget_with_prototype(spinner_running_demo_proto);
}

void ui_register_spinner_running_demo(void)
{
        spinner_running_demo_init_prototype();
        spinner_running_demo_proto->init = spinner_running_demo_init;
        spinner_running_demo_proto->destroy = spinner_running_demo_destroy;
}
