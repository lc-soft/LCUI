#include "index.tsx.h"
#include "index.h"

#define on_preview_click attachment_trigger__on_preview_click
#define create_action attachment_trigger__create_action
#include "../../../../../docs/examples/attachment-trigger/main.c"
#undef on_preview_click
#undef create_action

typedef struct {
        attachment_trigger_demo_react_t base;
} attachment_trigger_demo_t;

void attachment_trigger_demo_update(ui_widget_t *w)
{
        attachment_trigger_demo_react_update(w);
}

static void attachment_trigger_demo_init(ui_widget_t *w)
{
        attachment_trigger_demo_t *_that =
            ui_widget_add_data(w, attachment_trigger_demo_proto,
                               sizeof(attachment_trigger_demo_t));
        attachment_trigger_demo_proto->proto->init(w);
        attachment_trigger_demo_react_init(w);
        attachment_trigger_init(_that->base.refs.preview);
        attachment_trigger_demo_update(w);
}

static void attachment_trigger_demo_destroy(ui_widget_t *w)
{
        attachment_trigger_demo_proto->proto->destroy(w);
        attachment_trigger_demo_react_destroy(w);
}

ui_widget_t *ui_create_attachment_trigger_demo(void)
{
        return ui_create_widget_with_prototype(attachment_trigger_demo_proto);
}

void ui_register_attachment_trigger_demo(void)
{
        attachment_trigger_demo_init_prototype();
        attachment_trigger_demo_proto->init = attachment_trigger_demo_init;
        attachment_trigger_demo_proto->destroy =
            attachment_trigger_demo_destroy;
}
