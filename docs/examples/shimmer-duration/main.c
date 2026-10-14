#include <LCUI.h>

/* Two identical sweeps at different durations: the default 2000 ms and a
 * snappier 1000 ms. Duration is only observable while the shimmer is running,
 * so both lines keep the animation on. */
static const char *shimmer_duration_css = "\
.shimmer-duration-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
.shimmer-duration-demo shimmer {\
  width: 220px;\
  height: 28px;\
}\
.shimmer-duration-demo shimmer-content {\
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
        ui_shimmer_set_running(shimmer, true);
        return shimmer;
}

void shimmer_duration_init(ui_widget_t *parent)
{
        ui_widget_t *box, *normal, *fast;

        ui_load_css_string(shimmer_duration_css, "shimmer-duration-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-duration-demo");

        normal = caption("Default duration");
        fast = caption("Faster duration");
        ui_shimmer_set_duration(fast, 1000);

        ui_widget_append(box, normal);
        ui_widget_append(box, fast);
        ui_widget_append(parent, box);
}
