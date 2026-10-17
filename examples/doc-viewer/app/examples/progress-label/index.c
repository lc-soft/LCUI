#include "index.tsx.h"
#include "index.h"

#include "../../../../../docs/examples/progress-label/main.c"

typedef struct {
        progress_label_demo_react_t base;
} progress_label_demo_t;

void progress_label_demo_update(ui_widget_t *w)
{
        progress_label_demo_react_update(w);
}

static void progress_label_demo_init(ui_widget_t *w)
{
        progress_label_demo_t *_that = ui_widget_add_data(
            w, progress_label_demo_proto, sizeof(progress_label_demo_t));
        progress_label_demo_proto->proto->init(w);
        progress_label_demo_react_init(w);
        progress_label_init(_that->base.refs.preview);
        progress_label_demo_update(w);
}

static void progress_label_demo_destroy(ui_widget_t *w)
{
        progress_label_demo_proto->proto->destroy(w);
        progress_label_demo_react_destroy(w);
}

ui_widget_t *ui_create_progress_label_demo(void)
{
        return ui_create_widget_with_prototype(progress_label_demo_proto);
}

void ui_register_progress_label_demo(void)
{
        progress_label_demo_init_prototype();
        progress_label_demo_proto->init = progress_label_demo_init;
        progress_label_demo_proto->destroy = progress_label_demo_destroy;
}
