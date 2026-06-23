#include <LCUI.h>
#include <LCUI/widgets.h>

void button_basic_init(ui_widget_t *parent)
{
        ui_widget_t *btn;

        btn = ui_create_widget("button");
        ui_button_set_text(btn, "Click me");
        ui_widget_append(parent, btn);
}
