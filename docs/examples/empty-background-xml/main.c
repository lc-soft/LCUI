#include <LCUI.h>

void empty_background_xml_init(ui_widget_t *parent)
{
        ui_widget_append(parent, ui_create_widget(NULL));
}
