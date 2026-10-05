#include <LCUI.h>

static ui_widget_t *create_text_button(const char *content)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        ui_widget_append(button, text);
        return button;
}

void empty_basic_init(ui_widget_t *parent)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header = ui_create_empty_header();
        ui_widget_t *media = ui_create_empty_media();
        ui_widget_t *folder = ui_create_icon();
        ui_widget_t *title = ui_create_empty_title();
        ui_widget_t *description = ui_create_empty_description();
        ui_widget_t *content = ui_create_empty_content();
        ui_widget_t *actions = ui_create_empty_action();
        ui_widget_t *create_button = create_text_button("Create Project");
        ui_widget_t *import_button = create_text_button("Import Project");
        ui_widget_t *learn_more_button = create_text_button("Learn More");
        ui_widget_t *arrow_up_right = ui_create_icon();

        ui_widget_add_class(empty, "empty-basic-demo");
        ui_empty_media_set_variant(media, UI_EMPTY_MEDIA_VARIANT_ICON);
        ui_icon_set_name(folder, "folder");
        ui_widget_append(media, folder);
        ui_text_set_content(title, "No Projects Yet");
        ui_text_set_content(
            description,
            "You haven't created any projects yet. Get started by creating "
            "your first project.");
        ui_button_set_variant(create_button, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_variant(learn_more_button, UI_BUTTON_VARIANT_LINK);
        ui_button_set_size(learn_more_button, UI_BUTTON_SIZE_SM);
        ui_widget_add_class(learn_more_button, "text-muted-foreground");
        ui_icon_set_name(arrow_up_right, "arrow-up-right");
        ui_widget_append(learn_more_button, arrow_up_right);
        ui_widget_append(actions, create_button);
        ui_widget_append(actions, import_button);
        ui_widget_append(header, media);
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(content, actions);
        ui_widget_append(empty, header);
        ui_widget_append(empty, content);
        ui_widget_append(empty, learn_more_button);
        ui_widget_append(parent, empty);
}
