#include "index.tsx.h"
#include "index.h"

#define make_scrolling_body dialog_sticky_footer__make_scrolling_body
#include "../../../../../docs/examples/dialog-sticky-footer/main.c"
#undef make_scrolling_body

typedef struct {
        dialog_sticky_footer_demo_react_t base;
} dialog_sticky_footer_demo_t;

void dialog_sticky_footer_demo_update(ui_widget_t *w)
{
        dialog_sticky_footer_demo_react_update(w);
}

static void dialog_sticky_footer_demo_init(ui_widget_t *w)
{
        dialog_sticky_footer_demo_t *_that =
            ui_widget_add_data(w, dialog_sticky_footer_demo_proto,
                               sizeof(dialog_sticky_footer_demo_t));
        dialog_sticky_footer_demo_proto->proto->init(w);
        dialog_sticky_footer_demo_react_init(w);
        dialog_sticky_footer_init(_that->base.refs.preview);
        dialog_sticky_footer_demo_update(w);
}

static void dialog_sticky_footer_demo_destroy(ui_widget_t *w)
{
        dialog_sticky_footer_demo_proto->proto->destroy(w);
        dialog_sticky_footer_demo_react_destroy(w);
}

ui_widget_t *ui_create_dialog_sticky_footer_demo(void)
{
        return ui_create_widget_with_prototype(dialog_sticky_footer_demo_proto);
}

void ui_register_dialog_sticky_footer_demo(void)
{
        dialog_sticky_footer_demo_init_prototype();
        dialog_sticky_footer_demo_proto->init = dialog_sticky_footer_demo_init;
        dialog_sticky_footer_demo_proto->destroy =
            dialog_sticky_footer_demo_destroy;
}
