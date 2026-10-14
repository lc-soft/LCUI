#include <LCUI.h>

/* The spread sets the highlight band's width in logical pixels. A narrow band
 * reads as a tight glint; a wide one washes across the whole caption at once.
 * The peak strength is unchanged, only how far it reaches. */
static const char *shimmer_spread_css = "\
.shimmer-spread-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
.shimmer-spread-demo shimmer {\
  width: 220px;\
  height: 28px;\
}\
.shimmer-spread-demo shimmer-content {\
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

void shimmer_spread_init(ui_widget_t *parent)
{
        ui_widget_t *box, *narrow, *wide;

        ui_load_css_string(shimmer_spread_css, "shimmer-spread-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-spread-demo");

        narrow = caption("Narrow spread");
        ui_shimmer_set_spread(narrow, 40.0f);

        wide = caption("Wide spread");
        ui_shimmer_set_spread(wide, 160.0f);

        ui_widget_append(box, narrow);
        ui_widget_append(box, wide);
        ui_widget_append(parent, box);
}
