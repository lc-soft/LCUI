#include <LCUI.h>

void select_align_item_init(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();

        ui_widget_add_class(select, "select-align-item-demo");
        ui_select_set_placeholder(select, "Select a fruit");
        ui_select_add_item(select, "Apple", "apple");
        ui_select_add_item(select, "Banana", "banana");
        ui_select_add_item(select, "Blueberry", "blueberry");
        ui_select_add_item(select, "Grapes", "grapes");
        ui_select_add_item(select, "Pineapple", "pineapple");
        ui_select_set_value(select, "banana");
        ui_widget_append(parent, select);
}
