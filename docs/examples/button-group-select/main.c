#include <LCUI.h>

void button_group_select_init(ui_widget_t *parent)
{
        ui_widget_t *outer = ui_create_button_group();
        ui_widget_t *inner = ui_create_button_group();
        ui_widget_t *select = ui_create_select();
        ui_widget_t *input = ui_create_widget("textinput");
        ui_widget_t *send_group = ui_create_button_group();
        ui_widget_t *send = ui_create_button();

        ui_widget_add_class(outer, "button-group-select-demo");
        ui_widget_add_class(outer, "clip-safe");
        ui_widget_add_class(send_group, "clip-safe");
        ui_widget_add_class(inner, "button-group-grow");
        ui_widget_add_class(inner, "clip-safe");
        ui_select_add_item(select, "$ US Dollar", "$");
        ui_select_add_item(select, "EUR Euro", "EUR");
        ui_select_add_item(select, "GBP British Pound", "GBP");
        ui_select_set_value(select, "$");
        ui_textinput_set_placeholder(input, "10.00");
        ui_widget_append(inner, select);
        ui_widget_append(inner, input);
        ui_widget_append(outer, inner);

        ui_button_set_variant(send, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(send, UI_BUTTON_SIZE_ICON);
        ui_widget_append(send, ui_create_fluent_icon("arrow-right"));
        ui_widget_append(send_group, send);
        ui_widget_append(outer, send_group);
        ui_widget_append(parent, outer);
}
