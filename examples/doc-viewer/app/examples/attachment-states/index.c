#include "index.tsx.h"
#include "index.h"

#define create_action attachment_states__create_action
#define create_state_attachment attachment_states__create_state_attachment
#include "../../../../../docs/examples/attachment-states/main.c"
#undef create_action
#undef create_state_attachment

typedef struct {
        attachment_states_demo_react_t base;
} attachment_states_demo_t;

void attachment_states_demo_update(ui_widget_t *w)
{
        attachment_states_demo_react_update(w);
}

static void attachment_states_demo_init(ui_widget_t *w)
{
        attachment_states_demo_t *_that = ui_widget_add_data(
            w, attachment_states_demo_proto, sizeof(attachment_states_demo_t));
        attachment_states_demo_proto->proto->init(w);
        attachment_states_demo_react_init(w);
        attachment_states_init(_that->base.refs.preview);
        attachment_states_demo_update(w);
}

static void attachment_states_demo_destroy(ui_widget_t *w)
{
        attachment_states_demo_proto->proto->destroy(w);
        attachment_states_demo_react_destroy(w);
}

ui_widget_t *ui_create_attachment_states_demo(void)
{
        return ui_create_widget_with_prototype(attachment_states_demo_proto);
}

void ui_register_attachment_states_demo(void)
{
        attachment_states_demo_init_prototype();
        attachment_states_demo_proto->init = attachment_states_demo_init;
        attachment_states_demo_proto->destroy = attachment_states_demo_destroy;
}
