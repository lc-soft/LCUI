#include <LCUI.h>

#define LOREM                                                                  \
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do "     \
        "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim " \
        "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "  \
        "aliquip ex ea commodo consequat."

static ui_widget_t *make_scrolling_body(void)
{
        ui_widget_t *scroll = ui_create_scrollarea();
        ui_widget_t *content = ui_create_scrollarea_content();
        ui_widget_t *vbar = ui_create_widget("scrollbar");
        ui_widget_t *box, *text;
        int i;

        ui_widget_set_style_string(scroll, "height", "200px");
        ui_widget_set_style_string(scroll, "width", "100%");
        box = ui_create_widget(NULL);
        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "gap", "16px");
        for (i = 0; i < 10; ++i) {
                text = ui_create_widget("text");
                ui_text_set_content(text, LOREM);
                ui_widget_append(box, text);
        }
        ui_widget_append(content, box);
        ui_widget_append(scroll, content);
        ui_widget_append(scroll, vbar);
        return scroll;
}

void dialog_sticky_footer_init(ui_widget_t *parent)
{
        ui_widget_t *dialog = ui_create_dialog();

        ui_dialog_build_children(dialog);
        ui_dialog_set_trigger_text(dialog, "Sticky Footer");
        ui_dialog_set_title(dialog, "Sticky Footer");
        ui_dialog_set_description(
            dialog,
            "This dialog has a sticky footer that stays visible while the "
            "content scrolls.");
        ui_widget_remove(ui_dialog_get_footer_button(dialog, true));
        ui_dialog_set_cancel_text(dialog, "Close");
        ui_dialog_append_body(dialog, make_scrolling_body());
        ui_widget_append(parent, dialog);
}
