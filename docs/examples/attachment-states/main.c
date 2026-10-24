#include <LCUI.h>

static const char *attachment_states_css = "\
.attachment-states-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
  width: 384px;\
}\
.attachment-states-demo attachment {\
  width: 100%;\
}\
";

static ui_widget_t *create_action(const char *icon_name, const char *label)
{
        ui_widget_t *action = ui_create_attachment_action();
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, icon_name);
        ui_widget_set_attr(action, "aria-label", label);
        ui_widget_append(action, icon);
        return action;
}

static ui_widget_t *create_state_attachment(ui_attachment_state_t state,
                                            const char *icon_name,
                                            const char *title_text,
                                            const char *description_text,
                                            bool retry)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();
        ui_widget_t *actions = ui_create_attachment_actions();

        ui_attachment_set_state(attachment, state);
        if (icon_name) {
                ui_widget_t *icon = ui_create_icon();

                ui_icon_set_name(icon, icon_name);
                ui_widget_append(media, icon);
        } else {
                ui_widget_append(media, ui_create_spinner());
        }
        ui_text_set_content(title, title_text);
        ui_text_set_content(description, description_text);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        if (retry) {
                ui_widget_append(actions,
                                 create_action("arrow-sync", "Retry upload"));
        }
        ui_widget_append(actions,
                         create_action("dismiss", "Remove attachment"));
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        ui_widget_append(attachment, actions);
        return attachment;
}

void attachment_states_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_load_css_string(attachment_states_css, "attachment-states-demo");
        ui_widget_add_class(box, "attachment-states-demo");
        ui_widget_append(
            box, create_state_attachment(UI_ATTACHMENT_STATE_IDLE, "clock",
                                         "selected-file.pdf", "Ready to upload",
                                         false));
        ui_widget_append(
            box, create_state_attachment(UI_ATTACHMENT_STATE_UPLOADING, NULL,
                                         "design-system.zip", "Uploading - 64%",
                                         false));
        ui_widget_append(
            box, create_state_attachment(UI_ATTACHMENT_STATE_PROCESSING,
                                         "document-text", "market-research.pdf",
                                         "Processing document", false));
        ui_widget_append(box, create_state_attachment(
                                  UI_ATTACHMENT_STATE_ERROR, "document-error",
                                  "financial-model.xlsx",
                                  "Upload failed. Try again.", true));
        ui_widget_append(
            box, create_state_attachment(UI_ATTACHMENT_STATE_DONE, "checkmark",
                                         "uploaded-report.pdf",
                                         "Uploaded - 1.8 MB", false));
        ui_widget_append(parent, box);
}
