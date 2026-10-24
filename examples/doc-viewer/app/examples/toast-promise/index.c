#include "index.tsx.h"
#include "index.h"

#define on_pending_destroy toast_promise__on_pending_destroy
#define finish_promise toast_promise__finish_promise
#define show_promise toast_promise__show_promise
#include "../../../../../docs/examples/toast-promise/main.c"
#undef on_pending_destroy
#undef finish_promise
#undef show_promise

typedef struct {
        toast_promise_demo_react_t base;
} toast_promise_demo_t;

void toast_promise_demo_update(ui_widget_t *w)
{
        toast_promise_demo_react_update(w);
}

static void toast_promise_demo_init(ui_widget_t *w)
{
        toast_promise_demo_t *_that = ui_widget_add_data(
            w, toast_promise_demo_proto, sizeof(toast_promise_demo_t));
        toast_promise_demo_proto->proto->init(w);
        toast_promise_demo_react_init(w);
        toast_promise_init(_that->base.refs.preview);
        toast_promise_demo_update(w);
}

static void toast_promise_demo_destroy(ui_widget_t *w)
{
        toast_promise_demo_proto->proto->destroy(w);
        toast_promise_demo_react_destroy(w);
}

ui_widget_t *ui_create_toast_promise_demo(void)
{
        return ui_create_widget_with_prototype(toast_promise_demo_proto);
}

void ui_register_toast_promise_demo(void)
{
        toast_promise_demo_init_prototype();
        toast_promise_demo_proto->init = toast_promise_demo_init;
        toast_promise_demo_proto->destroy = toast_promise_demo_destroy;
}
