#include "index.tsx.h"
#include "index.h"
#include "../../../../../docs/examples/scrollarea-basic/main.c"

typedef struct {
        scrollarea_basic_demo_react_t base;
} scrollarea_basic_demo_t;

static void scrollarea_basic_demo_init(ui_widget_t *w)
{
        scrollarea_basic_demo_t *_that = ui_widget_add_data(
            w, scrollarea_basic_demo_proto, sizeof(scrollarea_basic_demo_t));
        scrollarea_basic_demo_proto->proto->init(w);
        scrollarea_basic_demo_react_init(w);
        scrollarea_basic_init(_that->base.refs.preview);
        scrollarea_basic_demo_update(w);
}

static scrollarea_basic_demo_t *scrollarea_basic_demo_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, scrollarea_basic_demo_proto);
}

static void scrollarea_basic_demo_destroy(ui_widget_t *w)
{
        scrollarea_basic_demo_proto->proto->destroy(w);
        scrollarea_basic_demo_react_destroy(w);
}

void scrollarea_basic_demo_update(ui_widget_t *w)
{
        scrollarea_basic_demo_react_update(w);
}

ui_widget_t *ui_create_scrollarea_basic_demo(void)
{
        return ui_create_widget_with_prototype(scrollarea_basic_demo_proto);
}

void ui_register_scrollarea_basic_demo(void)
{
        scrollarea_basic_demo_init_prototype();
        scrollarea_basic_demo_proto->init = scrollarea_basic_demo_init;
        scrollarea_basic_demo_proto->destroy = scrollarea_basic_demo_destroy;
}
