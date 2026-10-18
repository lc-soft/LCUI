#include <LCUI.h>

void tooltip_basic_init(ui_widget_t *parent)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, "Hover");
        ui_tooltip_attach(button, "Add to library");
        ui_widget_append(parent, button);
}
