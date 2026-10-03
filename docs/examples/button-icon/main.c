#include <LCUI.h>

static ui_widget_t *create_icon_button(ui_button_variant_t variant)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_button_set_variant(button, variant);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_icon_set_name(icon, "arrow-up-right");
        ui_widget_append(button, icon);
        return button;
}

void button_icon_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "button-icon-demo");
        ui_widget_append(box, create_icon_button(UI_BUTTON_VARIANT_OUTLINE));
        ui_widget_append(box, create_icon_button(UI_BUTTON_VARIANT_GHOST));
        ui_widget_append(box,
                         create_icon_button(UI_BUTTON_VARIANT_DESTRUCTIVE));
        ui_widget_append(parent, box);
}
