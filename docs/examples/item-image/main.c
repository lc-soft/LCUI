#include <LCUI.h>

static ui_widget_t *create_song(const char *thumb_class, const char *title,
                                const char *artist, const char *duration)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *item_title = ui_create_item_title();
        ui_widget_t *item_desc = ui_create_item_description();
        ui_widget_t *actions = ui_create_item_actions();
        ui_widget_t *time = ui_create_widget("text");

        ui_item_set_variant(item, UI_ITEM_VARIANT_OUTLINE);
        ui_item_set_interactive(item, true);
        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_IMAGE);
        ui_widget_add_class(media, thumb_class);
        ui_text_set_content(item_title, title);
        ui_text_set_content(item_desc, artist);
        ui_widget_add_class(time, "item-duration");
        ui_text_set_content(time, duration);
        ui_widget_append(content, item_title);
        ui_widget_append(content, item_desc);
        ui_widget_append(actions, time);
        ui_widget_append(item, media);
        ui_widget_append(item, content);
        ui_widget_append(item, actions);
        return item;
}

void item_image_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "item-image-demo");
        ui_widget_append(box,
                         create_song("bg-violet-400",
                                     "Midnight City Lights - Electric Nights",
                                     "Neon Dreams", "3:45"));
        ui_widget_append(
            box, create_song("bg-sky-400",
                             "Coffee Shop Conversations - Urban Stories",
                             "The Morning Brew", "4:05"));
        ui_widget_append(
            box, create_song("bg-emerald-400", "Digital Rain - Binary Beats",
                             "Cyber Symphony", "3:30"));
        ui_widget_append(parent, box);
}
