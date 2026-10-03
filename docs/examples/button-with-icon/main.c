#include <LCUI.h>

static ui_widget_t *create_icon_button(const char *glyph, const char *label,
                                       bool icon_first)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *text = ui_create_widget("text");

        ui_icon_set_name(icon, glyph);
        ui_text_set_content(text, label);
        if (icon_first) {
                ui_widget_append(button, icon);
                ui_widget_append(button, text);
        } else {
                ui_widget_append(button, text);
                ui_widget_append(button, icon);
        }
        return button;
}

void button_with_icon_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "button-with-icon-demo");
        ui_widget_append(box, create_icon_button("branch", "New Branch", true));
        ui_widget_append(box, create_icon_button("branch-fork", "Fork", false));
        ui_widget_append(parent, box);
}
