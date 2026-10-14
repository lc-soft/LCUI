#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/dialog-scrollable-content/main.c"

typedef struct {
        dialog_scrollable_content_demo_react_t base;
} dialog_scrollable_content_demo_t;

void dialog_scrollable_content_demo_update(ui_widget_t *w)
{
        dialog_scrollable_content_demo_react_update(w);
}

static void dialog_scrollable_content_demo_init(ui_widget_t *w)
{
        dialog_scrollable_content_demo_t *_that =
            ui_widget_add_data(w, dialog_scrollable_content_demo_proto,
                               sizeof(dialog_scrollable_content_demo_t));
        dialog_scrollable_content_demo_proto->proto->init(w);
        dialog_scrollable_content_demo_react_init(w);
        dialog_scrollable_content_init(_that->base.refs.preview);
        dialog_scrollable_content_demo_update(w);
}

static void dialog_scrollable_content_demo_destroy(ui_widget_t *w)
{
        dialog_scrollable_content_demo_proto->proto->destroy(w);
        dialog_scrollable_content_demo_react_destroy(w);
}

ui_widget_t *ui_create_dialog_scrollable_content_demo(void)
{
        return ui_create_widget_with_prototype(
            dialog_scrollable_content_demo_proto);
}

void ui_register_dialog_scrollable_content_demo(void)
{
        dialog_scrollable_content_demo_init_prototype();
        dialog_scrollable_content_demo_proto->init =
            dialog_scrollable_content_demo_init;
        dialog_scrollable_content_demo_proto->destroy =
            dialog_scrollable_content_demo_destroy;
}
