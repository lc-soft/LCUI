#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/dialog-no-close-button/main.c"

typedef struct {
        dialog_no_close_button_demo_react_t base;
} dialog_no_close_button_demo_t;

void dialog_no_close_button_demo_update(ui_widget_t *w)
{
        dialog_no_close_button_demo_react_update(w);
}

static void dialog_no_close_button_demo_init(ui_widget_t *w)
{
        dialog_no_close_button_demo_t *_that =
            ui_widget_add_data(w, dialog_no_close_button_demo_proto,
                               sizeof(dialog_no_close_button_demo_t));
        dialog_no_close_button_demo_proto->proto->init(w);
        dialog_no_close_button_demo_react_init(w);
        dialog_no_close_button_init(_that->base.refs.preview);
        dialog_no_close_button_demo_update(w);
}

static void dialog_no_close_button_demo_destroy(ui_widget_t *w)
{
        dialog_no_close_button_demo_proto->proto->destroy(w);
        dialog_no_close_button_demo_react_destroy(w);
}

ui_widget_t *ui_create_dialog_no_close_button_demo(void)
{
        return ui_create_widget_with_prototype(
            dialog_no_close_button_demo_proto);
}

void ui_register_dialog_no_close_button_demo(void)
{
        dialog_no_close_button_demo_init_prototype();
        dialog_no_close_button_demo_proto->init =
            dialog_no_close_button_demo_init;
        dialog_no_close_button_demo_proto->destroy =
            dialog_no_close_button_demo_destroy;
}
