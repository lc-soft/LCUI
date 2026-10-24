#include "index.tsx.h"
#include "index.h"

#define create_image_card attachment_image__create_image_card
#include "../../../../../docs/examples/attachment-image/main.c"
#undef create_image_card

typedef struct {
        attachment_image_demo_react_t base;
} attachment_image_demo_t;

void attachment_image_demo_update(ui_widget_t *w)
{
        attachment_image_demo_react_update(w);
}

static void attachment_image_demo_init(ui_widget_t *w)
{
        attachment_image_demo_t *_that = ui_widget_add_data(
            w, attachment_image_demo_proto, sizeof(attachment_image_demo_t));
        attachment_image_demo_proto->proto->init(w);
        attachment_image_demo_react_init(w);
        attachment_image_init(_that->base.refs.preview);
        attachment_image_demo_update(w);
}

static void attachment_image_demo_destroy(ui_widget_t *w)
{
        attachment_image_demo_proto->proto->destroy(w);
        attachment_image_demo_react_destroy(w);
}

ui_widget_t *ui_create_attachment_image_demo(void)
{
        return ui_create_widget_with_prototype(attachment_image_demo_proto);
}

void ui_register_attachment_image_demo(void)
{
        attachment_image_demo_init_prototype();
        attachment_image_demo_proto->init = attachment_image_demo_init;
        attachment_image_demo_proto->destroy = attachment_image_demo_destroy;
}
