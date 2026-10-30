#include "index.tsx.h"
#include "index.h"

#define create_text breadcrumb_custom__create_text
#define create_settings_button breadcrumb_custom__create_settings_button
#include "../../../../../docs/examples/breadcrumb-custom/main.c"
#undef create_text
#undef create_settings_button

typedef struct {
        breadcrumb_custom_demo_react_t base;
} breadcrumb_custom_demo_t;

void breadcrumb_custom_demo_update(ui_widget_t *w)
{
        breadcrumb_custom_demo_react_update(w);
}

static void breadcrumb_custom_demo_init(ui_widget_t *w)
{
        breadcrumb_custom_demo_t *_that = ui_widget_add_data(
            w, breadcrumb_custom_demo_proto, sizeof(breadcrumb_custom_demo_t));
        breadcrumb_custom_demo_proto->proto->init(w);
        breadcrumb_custom_demo_react_init(w);
        breadcrumb_custom_init(_that->base.refs.preview);
        breadcrumb_custom_demo_update(w);
}

static void breadcrumb_custom_demo_destroy(ui_widget_t *w)
{
        breadcrumb_custom_demo_proto->proto->destroy(w);
        breadcrumb_custom_demo_react_destroy(w);
}

ui_widget_t *ui_create_breadcrumb_custom_demo(void)
{
        return ui_create_widget_with_prototype(breadcrumb_custom_demo_proto);
}

void ui_register_breadcrumb_custom_demo(void)
{
        breadcrumb_custom_demo_init_prototype();
        breadcrumb_custom_demo_proto->init = breadcrumb_custom_demo_init;
        breadcrumb_custom_demo_proto->destroy = breadcrumb_custom_demo_destroy;
}
