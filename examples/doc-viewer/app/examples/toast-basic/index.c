#include "index.tsx.h"
#include "index.h"

#define on_action toast_basic__on_action
#define show_default toast_basic__show_default
#define show_success toast_basic__show_success
#define show_action toast_basic__show_action
#define show_loading toast_basic__show_loading
#define on_cancel toast_basic__on_cancel
#define on_confirm toast_basic__on_confirm
#define show_custom_actions toast_basic__show_custom_actions
#include "../../../../../docs/examples/toast-basic/main.c"
#undef on_action
#undef show_default
#undef show_success
#undef show_action
#undef show_loading
#undef on_cancel
#undef on_confirm
#undef show_custom_actions

typedef struct {
        toast_basic_demo_react_t base;
} toast_basic_demo_t;

void toast_basic_demo_update(ui_widget_t *w)
{
        toast_basic_demo_react_update(w);
}

static void toast_basic_demo_init(ui_widget_t *w)
{
        toast_basic_demo_t *_that = ui_widget_add_data(
            w, toast_basic_demo_proto, sizeof(toast_basic_demo_t));
        toast_basic_demo_proto->proto->init(w);
        toast_basic_demo_react_init(w);
        toast_basic_init(_that->base.refs.preview);
        toast_basic_demo_update(w);
}

static void toast_basic_demo_destroy(ui_widget_t *w)
{
        toast_basic_demo_proto->proto->destroy(w);
        toast_basic_demo_react_destroy(w);
}

ui_widget_t *ui_create_toast_basic_demo(void)
{
        return ui_create_widget_with_prototype(toast_basic_demo_proto);
}

void ui_register_toast_basic_demo(void)
{
        toast_basic_demo_init_prototype();
        toast_basic_demo_proto->init = toast_basic_demo_init;
        toast_basic_demo_proto->destroy = toast_basic_demo_destroy;
}
