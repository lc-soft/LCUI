#include <LCUI.h>

void badge_colors_init(ui_widget_t *parent)
{
        const char *colors[] = { "blue", "green", "sky", "purple", "red" };
        size_t index;
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "badge-colors-demo");
        for (index = 0; index < sizeof(colors) / sizeof(colors[0]); ++index) {
                ui_widget_t *badge = ui_create_badge();

                ui_badge_set_color(badge, colors[index]);
                ui_text_set_content(badge, colors[index]);
                ui_widget_append(box, badge);
        }
        ui_widget_append(parent, box);
}
