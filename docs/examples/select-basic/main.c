#include <LCUI.h>

void select_basic_init(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();

        ui_select_set_placeholder(select, "Choose a theme");
        ui_select_add_item(select, "Light", "light");
        ui_select_add_item(select, "Dark", "dark");
        ui_select_add_item(select, "System", "system");
        ui_widget_append(parent, select);
}
