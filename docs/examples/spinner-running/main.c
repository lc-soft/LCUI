#include <LCUI.h>

static const char *spinner_running_css = "\
.spinner-running-demo {\
  display: flex;\
  flex-direction: row;\
  align-items: center;\
  gap: 24px;\
}\
";

void spinner_running_init(ui_widget_t *parent)
{
        ui_widget_t *box, *running, *stopped;

        ui_load_css_string(spinner_running_css, "spinner-running-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "spinner-running-demo");

        running = ui_create_spinner();
        ui_widget_resize(running, 40, 40);
        ui_spinner_set_running(running, true);
        ui_spinner_set_speed(running, 0.5f);

        stopped = ui_create_spinner();
        ui_widget_resize(stopped, 40, 40);
        ui_spinner_set_running(stopped, false);
        ui_spinner_set_angle(stopped, 90.0);

        ui_widget_append(box, running);
        ui_widget_append(box, stopped);
        ui_widget_append(parent, box);
}
