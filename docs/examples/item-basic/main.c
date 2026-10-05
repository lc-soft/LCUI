#include <LCUI.h>

void item_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *basic = ui_create_item();
        ui_widget_t *basic_content = ui_create_item_content();
        ui_widget_t *basic_title = ui_create_item_title();
        ui_widget_t *basic_description = ui_create_item_description();
        ui_widget_t *basic_actions = ui_create_item_actions();
        ui_widget_t *action = ui_create_button();
        ui_widget_t *verified = ui_create_item();
        ui_widget_t *verified_media = ui_create_item_media();
        ui_widget_t *verified_icon = ui_create_icon();
        ui_widget_t *verified_content = ui_create_item_content();
        ui_widget_t *verified_title = ui_create_item_title();
        ui_widget_t *verified_actions = ui_create_item_actions();
        ui_widget_t *chevron = ui_create_icon();

        ui_widget_add_class(box, "item-basic-demo");
        ui_item_set_variant(basic, UI_ITEM_VARIANT_OUTLINE);
        ui_text_set_content(basic_title, "Basic Item");
        ui_text_set_content(basic_description,
                            "A simple item with title and description.");
        ui_button_set_variant(action, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(action, UI_BUTTON_SIZE_SM);
        ui_button_set_text(action, "Action");
        ui_widget_append(basic_content, basic_title);
        ui_widget_append(basic_content, basic_description);
        ui_widget_append(basic_actions, action);
        ui_widget_append(basic, basic_content);
        ui_widget_append(basic, basic_actions);
        ui_item_set_variant(verified, UI_ITEM_VARIANT_OUTLINE);
        ui_item_set_size(verified, UI_ITEM_SIZE_SM);
        ui_item_set_interactive(verified, true);
        ui_widget_add_class(verified_icon, "text-xl");
        ui_icon_set_name(verified_icon, "checkmark-starburst");
        ui_widget_add_class(chevron, "text-base");
        ui_icon_set_name(chevron, "chevron-right");
        ui_text_set_content(verified_title, "Your profile has been verified.");
        ui_widget_append(verified_media, verified_icon);
        ui_widget_append(verified_content, verified_title);
        ui_widget_append(verified_actions, chevron);
        ui_widget_append(verified, verified_media);
        ui_widget_append(verified, verified_content);
        ui_widget_append(verified, verified_actions);
        ui_widget_append(box, basic);
        ui_widget_append(box, verified);
        ui_widget_append(parent, box);
}
