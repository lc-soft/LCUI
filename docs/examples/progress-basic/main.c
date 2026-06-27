#include <LCUI.h>

static int timer_id = 0;
static float progress_value = 20.0f;

static void progress_basic_destroy(void);

static void on_timer(void *arg)
{
        ui_widget_t *progress = arg;

        progress_value += 10.0f;
        if (progress_value > 80.0f) {
                progress_basic_destroy();
                return;
        }
        ui_progress_set_value(progress, progress_value);
}

void progress_basic_init(ui_widget_t *parent)
{
        ui_widget_t *progress;

        progress = ui_create_progress();
        ui_progress_set_value(progress, progress_value);
        ui_widget_append(parent, progress);
        timer_id = ptk_set_interval(500, on_timer, progress);
        progress_value = 20.0f;
}

void progress_basic_destroy(void)
{
        if (timer_id) {
                ptk_clear_timer(timer_id);
                timer_id = 0;
        }
}
