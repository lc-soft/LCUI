#include <LCUI.h>

void badge_variants_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *badge = ui_create_badge();
        ui_widget_t *secondary = ui_create_badge();
        ui_widget_t *destructive = ui_create_badge();
        ui_widget_t *outline = ui_create_badge();
        ui_widget_t *ghost = ui_create_badge();

        ui_widget_add_class(box, "badge-variants-demo");
        ui_text_set_content(badge, "Default");
        ui_text_set_content(secondary, "Secondary");
        ui_badge_set_variant(secondary, UI_BADGE_VARIANT_SECONDARY);
        ui_text_set_content(destructive, "Destructive");
        ui_badge_set_variant(destructive, UI_BADGE_VARIANT_DESTRUCTIVE);
        ui_text_set_content(outline, "Outline");
        ui_badge_set_variant(outline, UI_BADGE_VARIANT_OUTLINE);
        ui_text_set_content(ghost, "Ghost");
        ui_badge_set_variant(ghost, UI_BADGE_VARIANT_GHOST);
        ui_widget_append(box, badge);
        ui_widget_append(box, secondary);
        ui_widget_append(box, destructive);
        ui_widget_append(box, outline);
        ui_widget_append(box, ghost);
        ui_widget_append(parent, box);
}
