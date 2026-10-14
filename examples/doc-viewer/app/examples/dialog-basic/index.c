#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/dialog-basic/main.c"

typedef struct {
        dialog_basic_demo_react_t base;
} dialog_basic_demo_t;

void dialog_basic_demo_update(ui_widget_t *w)
{
        dialog_basic_demo_react_update(w);
}

static void dialog_basic_demo_init(ui_widget_t *w)
{
        dialog_basic_demo_t *_that = ui_widget_add_data(
            w, dialog_basic_demo_proto, sizeof(dialog_basic_demo_t));
        dialog_basic_demo_proto->proto->init(w);
        dialog_basic_demo_react_init(w);
        dialog_basic_init(_that->base.refs.preview);
        dialog_basic_demo_update(w);
}

static void dialog_basic_demo_destroy(ui_widget_t *w)
{
        dialog_basic_demo_proto->proto->destroy(w);
        dialog_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_dialog_basic_demo(void)
{
        return ui_create_widget_with_prototype(dialog_basic_demo_proto);
}

void ui_register_dialog_basic_demo(void)
{
        dialog_basic_demo_init_prototype();
        dialog_basic_demo_proto->init = dialog_basic_demo_init;
        dialog_basic_demo_proto->destroy = dialog_basic_demo_destroy;
}
