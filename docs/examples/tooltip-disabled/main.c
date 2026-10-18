#include <LCUI.h>

void tooltip_disabled_init(ui_widget_t *parent)
{
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, "Disabled");
        ui_widget_set_disabled(button, true);
        ui_widget_append(trigger, button);
        ui_tooltip_attach(trigger, "This feature is currently unavailable");
        ui_widget_append(parent, trigger);
}
