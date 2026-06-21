#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/scrollarea-dual-scrollbars/main.c"

typedef struct {
        scrollarea_dual_scrollbars_demo_react_t base;
} scrollarea_dual_scrollbars_demo_t;

static void scrollarea_dual_scrollbars_demo_init(ui_widget_t *w)
{
        scrollarea_dual_scrollbars_demo_t *_that =
            ui_widget_add_data(w, scrollarea_dual_scrollbars_demo_proto,
                               sizeof(scrollarea_dual_scrollbars_demo_t));
        scrollarea_dual_scrollbars_demo_proto->proto->init(w);
        scrollarea_dual_scrollbars_demo_react_init(w);
        scrollarea_dual_scrollbars_init(_that->base.refs.preview);
        scrollarea_dual_scrollbars_demo_update(w);
}

static void scrollarea_dual_scrollbars_demo_destroy(ui_widget_t *w)
{
        scrollarea_dual_scrollbars_demo_proto->proto->destroy(w);
        scrollarea_dual_scrollbars_demo_react_destroy(w);
}

void scrollarea_dual_scrollbars_demo_update(ui_widget_t *w)
{
        scrollarea_dual_scrollbars_demo_react_update(w);
}

ui_widget_t *ui_create_scrollarea_dual_scrollbars_demo(void)
{
        return ui_create_widget_with_prototype(
            scrollarea_dual_scrollbars_demo_proto);
}

void ui_register_scrollarea_dual_scrollbars_demo(void)
{
        scrollarea_dual_scrollbars_demo_init_prototype();
        scrollarea_dual_scrollbars_demo_proto->init =
            scrollarea_dual_scrollbars_demo_init;
        scrollarea_dual_scrollbars_demo_proto->destroy =
            scrollarea_dual_scrollbars_demo_destroy;
}
