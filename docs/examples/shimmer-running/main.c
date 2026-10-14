#include <LCUI.h>

/* The first caption keeps sweeping; the second has the animation turned off,
 * so its text is composited as-is with no sheen added. `running = false` is the
 * shimmer's off state: the content renders normally. */
static const char *shimmer_running_css = "\
.shimmer-running-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
.shimmer-running-demo shimmer {\
  width: 220px;\
  height: 28px;\
}\
.shimmer-running-demo shimmer-content {\
  display: flex;\
  align-items: center;\
  padding: 4px 8px;\
  font-size: 15px;\
  color: #46545f;\
}\
";

static ui_widget_t *caption(const char *label)
{
        ui_widget_t *shimmer, *content, *text;

        shimmer = ui_create_shimmer();
        content = ui_create_widget("shimmer-content");
        text = ui_create_widget("text");
        ui_text_set_content(text, label);
        ui_widget_append(content, text);
        ui_shimmer_set_content(shimmer, content);
        return shimmer;
}

void shimmer_running_init(ui_widget_t *parent)
{
        ui_widget_t *box, *running, *stopped;

        ui_load_css_string(shimmer_running_css, "shimmer-running-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-running-demo");

        running = caption("Searching the web...");
        ui_shimmer_set_running(running, true);
        ui_shimmer_set_duration(running, 1000);

        stopped = caption("Reading 4 files");
        ui_shimmer_set_running(stopped, false);

        ui_widget_append(box, running);
        ui_widget_append(box, stopped);
        ui_widget_append(parent, box);
}
