#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/label-basic/main.c"

typedef struct {
        label_basic_demo_react_t base;
} label_basic_demo_t;

void label_basic_demo_update(ui_widget_t *w)
{
        label_basic_demo_react_update(w);
}

static void label_basic_demo_init(ui_widget_t *w)
{
        label_basic_demo_t *_that = ui_widget_add_data(
            w, label_basic_demo_proto, sizeof(label_basic_demo_t));
        label_basic_demo_proto->proto->init(w);
        label_basic_demo_react_init(w);
        label_basic_init(_that->base.refs.preview);
        label_basic_demo_update(w);
}

static void label_basic_demo_destroy(ui_widget_t *w)
{
        label_basic_demo_proto->proto->destroy(w);
        label_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_label_basic_demo(void)
{
        return ui_create_widget_with_prototype(label_basic_demo_proto);
}

void ui_register_label_basic_demo(void)
{
        label_basic_demo_init_prototype();
        label_basic_demo_proto->init = label_basic_demo_init;
        label_basic_demo_proto->destroy = label_basic_demo_destroy;
}
