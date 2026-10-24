#include "index.tsx.h"
#include "index.h"

#define draw_cover card_image__draw_cover
#include "../../../../../docs/examples/card-image/main.c"
#undef draw_cover

typedef struct {
        card_image_demo_react_t base;
} card_image_demo_t;

void card_image_demo_update(ui_widget_t *w)
{
        card_image_demo_react_update(w);
}

static void card_image_demo_init(ui_widget_t *w)
{
        card_image_demo_t *_that = ui_widget_add_data(
            w, card_image_demo_proto, sizeof(card_image_demo_t));
        card_image_demo_proto->proto->init(w);
        card_image_demo_react_init(w);
        card_image_init(_that->base.refs.preview);
        card_image_demo_update(w);
}

static void card_image_demo_destroy(ui_widget_t *w)
{
        card_image_demo_proto->proto->destroy(w);
        card_image_demo_react_destroy(w);
}

ui_widget_t *ui_create_card_image_demo(void)
{
        return ui_create_widget_with_prototype(card_image_demo_proto);
}

void ui_register_card_image_demo(void)
{
        card_image_demo_init_prototype();
        card_image_demo_proto->init = card_image_demo_init;
        card_image_demo_proto->destroy = card_image_demo_destroy;
}
