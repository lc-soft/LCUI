#include "example.h"
#include "example.tsx.h"

typedef struct {
        progress_demo_react_t base;
        int timer_id;
} progress_demo_t;

static void on_timer(void *arg);

static void progress_demo_react_init_state(ui_widget_t *w)
{
        progress_demo_t *_that = ui_widget_get_data(w, progress_demo_proto);
        _that->state.value = 20;
}

static void on_timer(void *arg)
{
        ui_widget_t *w = arg;
        progress_demo_t *_that = ui_widget_get_data(w, progress_demo_proto);

        _that->state.value += 10;
        if (_that->state.value > 80) {
                if (_that->timer_id >= 0) {
                        ptk_clear_timer(_that->timer_id);
                        _that->timer_id = -1;
                }
                return;
        }
        progress_demo_react_update(w);
}

static void progress_demo_react_init(ui_widget_t *w)
{
        progress_demo_t *_that = ui_widget_get_data(w, progress_demo_proto);
        progress_demo_load_template(w);
        progress_demo_react_init_state(w);
        _that->timer_id = ptk_set_interval(500, on_timer, w);
        progress_demo_react_update(w);
}

static void progress_demo_init(ui_widget_t *w)
{
        progress_demo_t *_that =
            ui_widget_add_data(w, root_page_proto, sizeof(root_page_t));

        root_page_react_init(w);
}

static void progress_demo_destroy(ui_widget_t *w)
{
        progress_demo_t *_that = ui_widget_get_data(w, progress_demo_proto);
        if (_that->timer_id >= 0) {
                ptk_clear_timer(_that->timer_id);
                _that->timer_id = -1;
        }
        progress_demo_react_update(w);
}
