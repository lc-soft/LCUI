#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/badge-variants/main.c"

typedef struct {
        badge_variants_demo_react_t base;
} badge_variants_demo_t;

void badge_variants_demo_update(ui_widget_t *w)
{
        badge_variants_demo_react_update(w);
}

static void badge_variants_demo_init(ui_widget_t *w)
{
        badge_variants_demo_t *_that = ui_widget_add_data(
            w, badge_variants_demo_proto, sizeof(badge_variants_demo_t));
        badge_variants_demo_proto->proto->init(w);
        badge_variants_demo_react_init(w);
        badge_variants_init(_that->base.refs.preview);
        badge_variants_demo_update(w);
}

static void badge_variants_demo_destroy(ui_widget_t *w)
{
        badge_variants_demo_proto->proto->destroy(w);
        badge_variants_demo_react_destroy(w);
}

ui_widget_t *ui_create_badge_variants_demo(void)
{
        return ui_create_widget_with_prototype(badge_variants_demo_proto);
}

void ui_register_badge_variants_demo(void)
{
        badge_variants_demo_init_prototype();
        badge_variants_demo_proto->init = badge_variants_demo_init;
        badge_variants_demo_proto->destroy = badge_variants_demo_destroy;
}
