#include <LCUI.h>

void empty_background_init(ui_widget_t *parent)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header = ui_create_empty_header();
        ui_widget_t *media = ui_create_empty_media();
        ui_widget_t *alert = ui_create_icon();
        ui_widget_t *title = ui_create_empty_title();
        ui_widget_t *description = ui_create_empty_description();
        ui_widget_t *content = ui_create_empty_content();
        ui_widget_t *action = ui_create_empty_action();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *arrow_sync = ui_create_icon();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_add_class(empty, "empty-background-demo");
        ui_empty_media_set_variant(media, UI_EMPTY_MEDIA_VARIANT_ICON);
        ui_icon_set_name(alert, "alert");
        ui_widget_append(media, alert);
        ui_text_set_content(text, "Refresh");
        ui_icon_set_name(arrow_sync, "arrow-sync");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_append(button, arrow_sync);
        ui_widget_append(button, text);
        ui_widget_append(action, button);
        ui_text_set_content(title, "No Notifications");
        ui_text_set_content(
            description,
            "You're all caught up. New notifications will appear here.");
        ui_widget_append(header, media);
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(content, action);
        ui_widget_append(empty, header);
        ui_widget_append(empty, content);
        ui_widget_append(parent, empty);
}
