#include <LCUI.h>

void spinner_basic_init(ui_widget_t *parent)
{
        ui_widget_t *spinner;

        spinner = ui_create_spinner();
        ui_widget_resize(spinner, 40, 40);
        ui_widget_append(parent, spinner);
}
