#include <LCUI.h>
#include <LCUI/widgets.h>

void text_basic_init(ui_widget_t *parent)
{
        ui_widget_t *w;

        w = ui_create_widget("text");
        ui_text_set_content(w, "Hello, LCUI!");
        ui_widget_append(parent, w);
}
