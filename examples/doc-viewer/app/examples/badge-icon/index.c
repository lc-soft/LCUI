#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/badge-icon/main.c"

typedef struct {
        badge_icon_demo_react_t base;
} badge_icon_demo_t;

void badge_icon_demo_update(ui_widget_t *w)
{
        badge_icon_demo_react_update(w);
}

static void badge_icon_demo_init(ui_widget_t *w)
{
        badge_icon_demo_t *_that = ui_widget_add_data(
            w, badge_icon_demo_proto, sizeof(badge_icon_demo_t));
        badge_icon_demo_proto->proto->init(w);
        badge_icon_demo_react_init(w);
        badge_icon_init(_that->base.refs.preview);
        badge_icon_demo_update(w);
}

static void badge_icon_demo_destroy(ui_widget_t *w)
{
        badge_icon_demo_proto->proto->destroy(w);
        badge_icon_demo_react_destroy(w);
}

ui_widget_t *ui_create_badge_icon_demo(void)
{
        return ui_create_widget_with_prototype(badge_icon_demo_proto);
}

void ui_register_badge_icon_demo(void)
{
        badge_icon_demo_init_prototype();
        badge_icon_demo_proto->init = badge_icon_demo_init;
        badge_icon_demo_proto->destroy = badge_icon_demo_destroy;
}
