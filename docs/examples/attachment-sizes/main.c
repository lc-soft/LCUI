#include <LCUI.h>

static const char *attachment_sizes_css = "\
.attachment-sizes-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 12px;\
  width: 384px;\
}\
.attachment-sizes-demo attachment {\
  width: 100%;\
}\
";

static ui_widget_t *create_size_attachment(ui_attachment_size_t size,
                                           const char *title_text,
                                           const char *description_text)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();

        ui_attachment_set_size(attachment, size);
        ui_icon_set_name(icon, "document");
        ui_widget_append(media, icon);
        ui_text_set_content(title, title_text);
        ui_widget_append(content, title);
        if (description_text) {
                ui_widget_t *description = ui_create_attachment_description();
                ui_text_set_content(description, description_text);
                ui_widget_append(content, description);
        }
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        return attachment;
}

void attachment_sizes_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_load_css_string(attachment_sizes_css, "attachment-sizes-demo");
        ui_widget_add_class(box, "attachment-sizes-demo");
        ui_widget_append(
            box, create_size_attachment(UI_ATTACHMENT_SIZE_DEFAULT,
                                        "Default attachment", "PDF - 2.4 MB"));
        ui_widget_append(
            box, create_size_attachment(UI_ATTACHMENT_SIZE_SM,
                                        "Small attachment", "PDF - 2.4 MB"));
        ui_widget_append(
            box, create_size_attachment(UI_ATTACHMENT_SIZE_XS,
                                        "Extra small attachment", NULL));
        ui_widget_append(parent, box);
}
