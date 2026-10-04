#include <LCUI.h>

void badge_icon_init(ui_widget_t *parent)
{
        ui_widget_t *wrapper = ui_create_widget(NULL);
        ui_widget_t *verified = ui_create_badge();
        ui_widget_t *verified_icon =
            ui_create_fluent_icon("checkmark-starburst");
        ui_widget_t *verified_text = ui_create_widget("text");
        ui_widget_t *bookmark = ui_create_badge();
        ui_widget_t *bookmark_text = ui_create_widget("text");
        ui_widget_t *bookmark_icon = ui_create_fluent_icon("bookmark");

        ui_widget_add_class(wrapper, "badge-icon-demo");
        ui_badge_set_variant(verified, UI_BADGE_VARIANT_SECONDARY);
        ui_text_set_content(verified_text, "Verified");
        ui_widget_append(verified, verified_icon);
        ui_widget_append(verified, verified_text);
        ui_badge_set_variant(bookmark, UI_BADGE_VARIANT_OUTLINE);
        ui_text_set_content(bookmark_text, "Bookmark");
        ui_widget_append(bookmark, bookmark_text);
        ui_widget_append(bookmark, bookmark_icon);
        ui_widget_append(wrapper, verified);
        ui_widget_append(wrapper, bookmark);
        ui_widget_append(parent, wrapper);
}
