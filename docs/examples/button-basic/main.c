#include <LCUI.h>

void button_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *btn = ui_create_button();
        ui_widget_t *icon_btn = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_widget_add_class(box, "button-basic-demo");
        ui_button_set_text(btn, "Button");
        ui_button_set_size(icon_btn, UI_BUTTON_SIZE_ICON);
        ui_icon_set_name(icon, "arrow-up");
        ui_widget_append(icon_btn, icon);
        ui_widget_append(box, btn);
        ui_widget_append(box, icon_btn);
        ui_widget_append(parent, box);
}
