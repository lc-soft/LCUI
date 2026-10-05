#include <LCUI.h>

static ui_widget_t *create_item(ui_item_variant_t variant, const char *title,
                                const char *description)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *item_title = ui_create_item_title();
        ui_widget_t *item_desc = ui_create_item_description();
        ui_widget_t *actions = ui_create_item_actions();
        ui_widget_t *button = ui_create_button();

        ui_item_set_variant(item, variant);
        ui_text_set_content(item_title, title);
        ui_text_set_content(item_desc, description);
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_button_set_text(button, "Open");
        ui_widget_append(content, item_title);
        ui_widget_append(content, item_desc);
        ui_widget_append(actions, button);
        ui_widget_append(item, content);
        ui_widget_append(item, actions);
        return item;
}

void item_variants_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "item-variants-demo");
        ui_widget_append(box,
                         create_item(UI_ITEM_VARIANT_DEFAULT, "Default Variant",
                                     "Standard styling with subtle background "
                                     "and borders."));
        ui_widget_append(box,
                         create_item(UI_ITEM_VARIANT_OUTLINE, "Outline Variant",
                                     "Outlined style with clear borders and "
                                     "transparent background."));
        ui_widget_append(box,
                         create_item(UI_ITEM_VARIANT_MUTED, "Muted Variant",
                                     "Subdued appearance with muted colors for "
                                     "secondary content."));
        ui_widget_append(parent, box);
}
