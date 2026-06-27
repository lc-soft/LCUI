#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/progress-basic/main.c"

typedef struct {
        progress_basic_demo_react_t base;
} progress_basic_demo_t;

static void progress_basic_demo_init(ui_widget_t *w)
{
        progress_basic_demo_t *_that = ui_widget_add_data(
            w, progress_basic_demo_proto, sizeof(progress_basic_demo_t));
        progress_basic_demo_proto->proto->init(w);
        progress_basic_demo_react_init(w);
        progress_basic_init(_that->base.refs.preview);
        progress_basic_demo_update(w);
}

static void progress_basic_demo_destroy(ui_widget_t *w)
{
        progress_basic_destroy();
        progress_basic_demo_proto->proto->destroy(w);
        progress_basic_demo_react_destroy(w);
}

void progress_basic_demo_update(ui_widget_t *w)
{
        progress_basic_demo_react_update(w);
}

ui_widget_t *ui_create_progress_basic_demo(void)
{
        return ui_create_widget_with_prototype(progress_basic_demo_proto);
}

void ui_register_progress_basic_demo(void)
{
        progress_basic_demo_init_prototype();
        progress_basic_demo_proto->init = progress_basic_demo_init;
        progress_basic_demo_proto->destroy = progress_basic_demo_destroy;
}
