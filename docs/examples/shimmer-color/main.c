#include <LCUI.h>

/* Two captions, each with a different highlight colour. The `color` sheen is
 * overlaid only on the text's opaque pixels, so a blue and a green highlight
 * tint the same grey ink toward their own hue as they sweep. */
static const char *shimmer_color_css = "\
.shimmer-color-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
.shimmer-color-demo shimmer {\
  width: 220px;\
  height: 28px;\
}\
.shimmer-color-demo shimmer-content {\
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

void shimmer_color_init(ui_widget_t *parent)
{
        ui_widget_t *box, *blue, *green;

        ui_load_css_string(shimmer_color_css, "shimmer-color-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-color-demo");

        blue = caption("Generating response...");
        ui_shimmer_set_color(blue, pd_argb(200, 55, 138, 221));

        green = caption("Sending request...");
        ui_shimmer_set_color(green, pd_argb(200, 34, 197, 94));

        ui_widget_append(box, blue);
        ui_widget_append(box, green);
        ui_widget_append(parent, box);
}
