#include <LCUI.h>

void text_input_basic_init(ui_widget_t *parent)
{
        ui_widget_t *w;

        w = ui_create_widget("textinput");
        ui_textinput_set_placeholder(w, u8"请输入...");
        ui_widget_append(parent, w);
}
