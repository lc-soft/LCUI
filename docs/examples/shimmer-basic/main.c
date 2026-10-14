#include <LCUI.h>

/* The shimmer sweeps a soft sheen across real text, so the caption stays
 * readable while it reads as "in progress". Scoped under a unique class; an
 * inline background-color would be overridden by the theme `*` rule. */
static const char *shimmer_basic_css = "\
.shimmer-basic-demo shimmer {\
  width: 220px;\
  height: 28px;\
}\
.shimmer-basic-demo shimmer-content {\
  display: flex;\
  align-items: center;\
  padding: 4px 8px;\
  font-size: 15px;\
  color: #46545f;\
}\
";

void shimmer_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box, *shimmer, *content, *text;

        ui_load_css_string(shimmer_basic_css, "shimmer-basic-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-basic-demo");

        shimmer = ui_create_shimmer();
        content = ui_create_widget("shimmer-content");
        text = ui_create_widget("text");
        ui_text_set_content(text, "Generating response...");
        ui_widget_append(content, text);
        ui_shimmer_set_content(shimmer, content);

        ui_widget_append(box, shimmer);
        ui_widget_append(parent, box);
}
