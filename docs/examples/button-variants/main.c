#include <LCUI.h>

static ui_widget_t *create_variant_button(const char *text,
                                          ui_button_variant_t variant)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, variant);
        ui_button_set_text(button, text);
        return button;
}

void button_variants_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *outline = ui_create_button();

        ui_widget_add_class(box, "button-variants-demo");
        ui_button_set_text(outline, "Outline");
        ui_widget_append(box, outline);
        ui_widget_append(
            box, create_variant_button("Primary", UI_BUTTON_VARIANT_PRIMARY));
        ui_widget_append(box, create_variant_button(
                                  "Secondary", UI_BUTTON_VARIANT_SECONDARY));
        ui_widget_append(
            box, create_variant_button("Ghost", UI_BUTTON_VARIANT_GHOST));
        ui_widget_append(box,
                         create_variant_button("Destructive",
                                               UI_BUTTON_VARIANT_DESTRUCTIVE));
        ui_widget_append(box,
                         create_variant_button("Link", UI_BUTTON_VARIANT_LINK));
        ui_widget_append(parent, box);
}
