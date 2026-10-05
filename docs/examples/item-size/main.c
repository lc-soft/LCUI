#include <LCUI.h>

static ui_widget_t *create_item(ui_item_size_t size, const char *title,
                                const char *description)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *item_title = ui_create_item_title();
        ui_widget_t *item_desc = ui_create_item_description();

        ui_item_set_variant(item, UI_ITEM_VARIANT_OUTLINE);
        ui_item_set_size(item, size);
        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_IMAGE);
        ui_widget_add_class(media, "bg-violet-400");
        ui_text_set_content(item_title, title);
        ui_text_set_content(item_desc, description);
        ui_widget_append(content, item_title);
        ui_widget_append(content, item_desc);
        ui_widget_append(item, media);
        ui_widget_append(item, content);
        return item;
}

void item_size_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "item-size-demo");
        ui_widget_append(box,
                         create_item(UI_ITEM_SIZE_DEFAULT, "Default Size",
                                     "The standard size for most use cases."));
        ui_widget_append(box, create_item(UI_ITEM_SIZE_SM, "Small Size",
                                          "A compact size for dense layouts."));
        ui_widget_append(box, create_item(UI_ITEM_SIZE_XS, "Extra Small Size",
                                          "The most compact size available."));
        ui_widget_append(parent, box);
}
