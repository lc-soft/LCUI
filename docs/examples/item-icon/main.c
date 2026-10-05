#include <LCUI.h>

void item_icon_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *title = ui_create_item_title();
        ui_widget_t *description = ui_create_item_description();
        ui_widget_t *actions = ui_create_item_actions();
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(box, "item-icon-demo");
        ui_item_set_variant(item, UI_ITEM_VARIANT_OUTLINE);
        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_ICON);
        ui_icon_set_name(icon, "shield-error");
        ui_text_set_content(title, "Security Alert");
        ui_text_set_content(description,
                            "New login detected from unknown device.");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_button_set_text(button, "Review");
        ui_widget_append(media, icon);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(actions, button);
        ui_widget_append(item, media);
        ui_widget_append(item, content);
        ui_widget_append(item, actions);
        ui_widget_append(box, item);
        ui_widget_append(parent, box);
}
