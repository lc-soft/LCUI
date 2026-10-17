#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/checkbox-description/main.c"

typedef struct {
        checkbox_description_demo_react_t base;
} checkbox_description_demo_t;

void checkbox_description_demo_update(ui_widget_t *w)
{
        checkbox_description_demo_react_update(w);
}

static void checkbox_description_demo_init(ui_widget_t *w)
{
        checkbox_description_demo_t *_that =
            ui_widget_add_data(w, checkbox_description_demo_proto,
                               sizeof(checkbox_description_demo_t));
        checkbox_description_demo_proto->proto->init(w);
        checkbox_description_demo_react_init(w);
        checkbox_description_init(_that->base.refs.preview);
        checkbox_description_demo_update(w);
}

static void checkbox_description_demo_destroy(ui_widget_t *w)
{
        checkbox_description_demo_proto->proto->destroy(w);
        checkbox_description_demo_react_destroy(w);
}

ui_widget_t *ui_create_checkbox_description_demo(void)
{
        return ui_create_widget_with_prototype(checkbox_description_demo_proto);
}

void ui_register_checkbox_description_demo(void)
{
        checkbox_description_demo_init_prototype();
        checkbox_description_demo_proto->init = checkbox_description_demo_init;
        checkbox_description_demo_proto->destroy =
            checkbox_description_demo_destroy;
}
