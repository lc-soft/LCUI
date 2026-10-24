#include "index.tsx.h"
#include "index.h"

#define show_type toast_types__show_type
#define show_success toast_types__show_success
#define show_info toast_types__show_info
#define show_warning toast_types__show_warning
#define show_error toast_types__show_error
#define show_loading toast_types__show_loading
#define create_type_button toast_types__create_type_button
#include "../../../../../docs/examples/toast-types/main.c"
#undef show_type
#undef show_success
#undef show_info
#undef show_warning
#undef show_error
#undef show_loading
#undef create_type_button

typedef struct {
        toast_types_demo_react_t base;
} toast_types_demo_t;

void toast_types_demo_update(ui_widget_t *w)
{
        toast_types_demo_react_update(w);
}

static void toast_types_demo_init(ui_widget_t *w)
{
        toast_types_demo_t *_that = ui_widget_add_data(
            w, toast_types_demo_proto, sizeof(toast_types_demo_t));
        toast_types_demo_proto->proto->init(w);
        toast_types_demo_react_init(w);
        toast_types_init(_that->base.refs.preview);
        toast_types_demo_update(w);
}

static void toast_types_demo_destroy(ui_widget_t *w)
{
        toast_types_demo_proto->proto->destroy(w);
        toast_types_demo_react_destroy(w);
}

ui_widget_t *ui_create_toast_types_demo(void)
{
        return ui_create_widget_with_prototype(toast_types_demo_proto);
}

void ui_register_toast_types_demo(void)
{
        toast_types_demo_init_prototype();
        toast_types_demo_proto->init = toast_types_demo_init;
        toast_types_demo_proto->destroy = toast_types_demo_destroy;
}
