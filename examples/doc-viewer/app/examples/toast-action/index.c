#include "index.tsx.h"
#include "index.h"

#define on_undo toast_action__on_undo
#define show_action toast_action__show_action
#include "../../../../../docs/examples/toast-action/main.c"
#undef on_undo
#undef show_action

typedef struct {
        toast_action_demo_react_t base;
} toast_action_demo_t;

void toast_action_demo_update(ui_widget_t *w)
{
        toast_action_demo_react_update(w);
}

static void toast_action_demo_init(ui_widget_t *w)
{
        toast_action_demo_t *_that = ui_widget_add_data(
            w, toast_action_demo_proto, sizeof(toast_action_demo_t));
        toast_action_demo_proto->proto->init(w);
        toast_action_demo_react_init(w);
        toast_action_init(_that->base.refs.preview);
        toast_action_demo_update(w);
}

static void toast_action_demo_destroy(ui_widget_t *w)
{
        toast_action_demo_proto->proto->destroy(w);
        toast_action_demo_react_destroy(w);
}

ui_widget_t *ui_create_toast_action_demo(void)
{
        return ui_create_widget_with_prototype(toast_action_demo_proto);
}

void ui_register_toast_action_demo(void)
{
        toast_action_demo_init_prototype();
        toast_action_demo_proto->init = toast_action_demo_init;
        toast_action_demo_proto->destroy = toast_action_demo_destroy;
}
