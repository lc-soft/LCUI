#include <LCUI.h>

static const char *attachment_basic_css = "\
.attachment-basic-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 12px;\
  width: 384px;\
}\
.attachment-basic-demo attachment-group {\
  width: 100%;\
}\
.attachment-basic-demo attachment {\
  width: 100%;\
}\
.attachment-basic-demo attachment:vertical {\
  width: 120px;\
}\
";

static ui_widget_t *create_image_attachment(const char *path, const char *name,
                                            const char *meta)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();

        ui_attachment_set_orientation(attachment,
                                      UI_ATTACHMENT_ORIENTATION_VERTICAL);
        ui_attachment_media_set_image_url(media, path);
        ui_text_set_content(title, name);
        ui_text_set_content(description, meta);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        return attachment;
}

static ui_widget_t *create_remove_action(const char *name)
{
        ui_widget_t *action = ui_create_attachment_action();
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "dismiss");
        ui_widget_set_attr(action, "aria-label", name);
        ui_widget_append(action, icon);
        return action;
}

void attachment_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group = ui_create_attachment_group();
        ui_widget_t *uploading = ui_create_attachment();
        ui_widget_t *uploading_media = ui_create_attachment_media();
        ui_widget_t *spinner = ui_create_spinner();
        ui_widget_t *uploading_content = ui_create_attachment_content();
        ui_widget_t *uploading_title = ui_create_attachment_title();
        ui_widget_t *uploading_description = ui_create_attachment_description();
        ui_widget_t *uploading_actions = ui_create_attachment_actions();
        ui_widget_t *file = ui_create_attachment();
        ui_widget_t *file_media = ui_create_attachment_media();
        ui_widget_t *file_icon = ui_create_icon();
        ui_widget_t *file_content = ui_create_attachment_content();
        ui_widget_t *file_title = ui_create_attachment_title();
        ui_widget_t *file_description = ui_create_attachment_description();
        ui_widget_t *file_actions = ui_create_attachment_actions();

        ui_load_css_string(attachment_basic_css, "attachment-basic-demo");
        ui_widget_add_class(box, "attachment-basic-demo");

        ui_widget_append(
            group, create_image_attachment("assets/attachment/workspace.jpg",
                                           "workspace.png", "PNG - 820 KB"));
        ui_widget_append(group, create_image_attachment(
                                    "assets/attachment/desk-reference.jpg",
                                    "desk-reference.jpg", "JPG - 1.1 MB"));
        ui_widget_append(group, create_image_attachment(
                                    "assets/attachment/office-reference.jpg",
                                    "office-reference.jpg", "JPG - 940 KB"));

        ui_attachment_set_state(uploading, UI_ATTACHMENT_STATE_UPLOADING);
        ui_widget_append(uploading_media, spinner);
        ui_text_set_content(uploading_title, "sales-dashboard.pdf");
        ui_text_set_content(uploading_description, "Uploading - 64%");
        ui_widget_append(uploading_content, uploading_title);
        ui_widget_append(uploading_content, uploading_description);
        ui_widget_append(uploading_actions,
                         create_remove_action("Cancel upload"));
        ui_widget_append(uploading, uploading_media);
        ui_widget_append(uploading, uploading_content);
        ui_widget_append(uploading, uploading_actions);

        ui_icon_set_name(file_icon, "code");
        ui_widget_append(file_media, file_icon);
        ui_text_set_content(file_title, "message-renderer.tsx");
        ui_text_set_content(file_description, "TypeScript - 12 KB");
        ui_widget_append(file_content, file_title);
        ui_widget_append(file_content, file_description);
        ui_widget_append(file_actions,
                         create_remove_action("Remove message-renderer.tsx"));
        ui_widget_append(file, file_media);
        ui_widget_append(file, file_content);
        ui_widget_append(file, file_actions);

        ui_widget_append(box, group);
        ui_widget_append(box, uploading);
        ui_widget_append(box, file);
        ui_widget_append(parent, box);
}
