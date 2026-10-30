#include "index.tsx.h"
#include "index.h"

#define create_text breadcrumb_demo__create_text
#define create_link_item breadcrumb_demo__create_link_item
#define create_page_item breadcrumb_demo__create_page_item
#define create_menu_item breadcrumb_demo__create_menu_item
#define create_ellipsis_item breadcrumb_demo__create_ellipsis_item
#include "../../../../../docs/examples/breadcrumb-demo/main.c"
#undef create_text
#undef create_link_item
#undef create_page_item
#undef create_menu_item
#undef create_ellipsis_item

typedef struct {
        breadcrumb_demo_demo_react_t base;
} breadcrumb_demo_demo_t;

void breadcrumb_demo_demo_update(ui_widget_t *w)
{
        breadcrumb_demo_demo_react_update(w);
}

static void breadcrumb_demo_demo_init(ui_widget_t *w)
{
        breadcrumb_demo_demo_t *_that = ui_widget_add_data(
            w, breadcrumb_demo_demo_proto, sizeof(breadcrumb_demo_demo_t));
        breadcrumb_demo_demo_proto->proto->init(w);
        breadcrumb_demo_demo_react_init(w);
        breadcrumb_demo_init(_that->base.refs.preview);
        breadcrumb_demo_demo_update(w);
}

static void breadcrumb_demo_demo_destroy(ui_widget_t *w)
{
        breadcrumb_demo_demo_proto->proto->destroy(w);
        breadcrumb_demo_demo_react_destroy(w);
}

ui_widget_t *ui_create_breadcrumb_demo_demo(void)
{
        return ui_create_widget_with_prototype(breadcrumb_demo_demo_proto);
}

void ui_register_breadcrumb_demo_demo(void)
{
        breadcrumb_demo_demo_init_prototype();
        breadcrumb_demo_demo_proto->init = breadcrumb_demo_demo_init;
        breadcrumb_demo_demo_proto->destroy = breadcrumb_demo_demo_destroy;
}
