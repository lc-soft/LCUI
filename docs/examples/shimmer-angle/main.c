#include <LCUI.h>

/* The angle tilts the highlight band. A flat band (0 degrees) crosses each row
 * at the same column; a 45 degree band shears across the caption so the glint
 * reaches the top before the bottom. */
static const char *shimmer_angle_css = "\
.shimmer-angle-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
.shimmer-angle-demo shimmer {\
  width: 220px;\
  height: 36px;\
}\
.shimmer-angle-demo shimmer-content {\
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

void shimmer_angle_init(ui_widget_t *parent)
{
        ui_widget_t *box, *flat, *tilted;

        ui_load_css_string(shimmer_angle_css, "shimmer-angle-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "shimmer-angle-demo");

        flat = caption("Flat band");
        ui_shimmer_set_angle(flat, 0.0f);

        tilted = caption("Tilted band");
        ui_shimmer_set_angle(tilted, 45.0f);

        ui_widget_append(box, flat);
        ui_widget_append(box, tilted);
        ui_widget_append(parent, box);
}
