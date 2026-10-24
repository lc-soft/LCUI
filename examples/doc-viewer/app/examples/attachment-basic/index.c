#include "index.tsx.h"
#include "index.h"

#define create_image_attachment attachment_basic__create_image_attachment
#define create_remove_action attachment_basic__create_remove_action
#include "../../../../../docs/examples/attachment-basic/main.c"
#undef create_image_attachment
#undef create_remove_action

typedef struct {
        attachment_basic_demo_react_t base;
} attachment_basic_demo_t;

void attachment_basic_demo_update(ui_widget_t *w)
{
        attachment_basic_demo_react_update(w);
}

static void attachment_basic_demo_init(ui_widget_t *w)
{
        attachment_basic_demo_t *_that = ui_widget_add_data(
            w, attachment_basic_demo_proto, sizeof(attachment_basic_demo_t));
        attachment_basic_demo_proto->proto->init(w);
        attachment_basic_demo_react_init(w);
        attachment_basic_init(_that->base.refs.preview);
        attachment_basic_demo_update(w);
}

static void attachment_basic_demo_destroy(ui_widget_t *w)
{
        attachment_basic_demo_proto->proto->destroy(w);
        attachment_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_attachment_basic_demo(void)
{
        return ui_create_widget_with_prototype(attachment_basic_demo_proto);
}

void ui_register_attachment_basic_demo(void)
{
        attachment_basic_demo_init_prototype();
        attachment_basic_demo_proto->init = attachment_basic_demo_init;
        attachment_basic_demo_proto->destroy = attachment_basic_demo_destroy;
}
