#include <LCUI.h>

static const char *attachment_image_css = "\
.attachment-image-demo {\
  width: 384px;\
}\
.attachment-image-demo attachment-group {\
  width: 100%;\
}\
";

static ui_widget_t *create_image_card(const char *path, const char *name,
                                      const char *meta)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();
        ui_widget_t *actions = ui_create_attachment_actions();
        ui_widget_t *action = ui_create_attachment_action();
        ui_widget_t *icon = ui_create_icon();

        ui_attachment_set_orientation(attachment,
                                      UI_ATTACHMENT_ORIENTATION_VERTICAL);
        ui_attachment_media_set_image_url(media, path);
        ui_text_set_content(title, name);
        ui_text_set_content(description, meta);
        ui_icon_set_name(icon, "dismiss");
        ui_widget_set_attr(action, "aria-label", name);
        ui_widget_append(action, icon);
        ui_widget_append(actions, action);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        ui_widget_append(attachment, actions);
        return attachment;
}

void attachment_image_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group = ui_create_attachment_group();

        ui_load_css_string(attachment_image_css, "attachment-image-demo");
        ui_widget_add_class(box, "attachment-image-demo");
        ui_widget_append(group,
                         create_image_card("assets/attachment/workspace.jpg",
                                           "workspace.png", "PNG - 820 KB"));
        ui_widget_append(
            group, create_image_card("assets/attachment/desk-reference.jpg",
                                     "desk-reference.jpg", "JPG - 1.1 MB"));
        ui_widget_append(
            group, create_image_card("assets/attachment/office-reference.jpg",
                                     "office-reference.jpg", "JPG - 940 KB"));
        ui_widget_append(box, group);
        ui_widget_append(parent, box);
}
