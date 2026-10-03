#include <LCUI.h>

static ui_widget_t *create_disabled_button(const char *text,
                                           ui_button_variant_t variant)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, variant);
        ui_button_set_text(button, text);
        ui_widget_set_disabled(button, true);
        return button;
}

void button_disabled_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *outline = ui_create_button();

        ui_widget_add_class(box, "button-disabled-demo");
        ui_button_set_text(outline, "Outline");
        ui_widget_set_disabled(outline, true);
        ui_widget_append(box, outline);
        ui_widget_append(
            box, create_disabled_button("Primary", UI_BUTTON_VARIANT_PRIMARY));
        ui_widget_append(box, create_disabled_button(
                                  "Secondary", UI_BUTTON_VARIANT_SECONDARY));
        ui_widget_append(
            box, create_disabled_button("Ghost", UI_BUTTON_VARIANT_GHOST));
        ui_widget_append(box,
                         create_disabled_button("Destructive",
                                                UI_BUTTON_VARIANT_DESTRUCTIVE));
        ui_widget_append(
            box, create_disabled_button("Link", UI_BUTTON_VARIANT_LINK));
        ui_widget_append(parent, box);
}
