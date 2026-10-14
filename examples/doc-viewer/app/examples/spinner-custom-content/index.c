#include "index.tsx.h"
#include "index.h"

#define create_ring_content spinner_custom_content__create_ring_content
#include "../../../../../docs/examples/spinner-custom-content/main.c"
#undef create_ring_content

typedef struct {
        spinner_custom_content_demo_react_t base;
} spinner_custom_content_demo_t;

void spinner_custom_content_demo_update(ui_widget_t *w)
{
        spinner_custom_content_demo_react_update(w);
}

static void spinner_custom_content_demo_init(ui_widget_t *w)
{
        spinner_custom_content_demo_t *_that =
            ui_widget_add_data(w, spinner_custom_content_demo_proto,
                               sizeof(spinner_custom_content_demo_t));
        spinner_custom_content_demo_proto->proto->init(w);
        spinner_custom_content_demo_react_init(w);
        spinner_custom_content_init(_that->base.refs.preview);
        spinner_custom_content_demo_update(w);
}

static void spinner_custom_content_demo_destroy(ui_widget_t *w)
{
        spinner_custom_content_demo_proto->proto->destroy(w);
        spinner_custom_content_demo_react_destroy(w);
}

ui_widget_t *ui_create_spinner_custom_content_demo(void)
{
        return ui_create_widget_with_prototype(
            spinner_custom_content_demo_proto);
}

void ui_register_spinner_custom_content_demo(void)
{
        spinner_custom_content_demo_init_prototype();
        spinner_custom_content_demo_proto->init =
            spinner_custom_content_demo_init;
        spinner_custom_content_demo_proto->destroy =
            spinner_custom_content_demo_destroy;
}
