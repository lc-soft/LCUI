#include <LCUI.h>

void dialog_no_close_button_xml_init(ui_widget_t *parent)
{
        ui_widget_append(parent, ui_create_widget(NULL));
}
