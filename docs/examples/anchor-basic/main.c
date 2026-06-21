#include <LCUI.h>

void anchor_basic_init(ui_widget_t *parent)
{
        ui_widget_t *a;

        a = ui_create_widget("a");
        ui_text_set_content(a, "LCUI Homepage");
        ui_widget_set_attr(a, "href", "https://lcui.dev");
        ui_widget_append(parent, a);
}
