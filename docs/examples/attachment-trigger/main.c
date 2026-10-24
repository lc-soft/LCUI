#include <LCUI.h>

static const char *attachment_trigger_css = "\
.attachment-trigger-demo {\
  box-sizing: border-box;\
  width: 384px;\
  padding: 4px;\
}\
.attachment-trigger-demo attachment {\
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

static void create_preview_dialog(ui_widget_t *parent)
{
        ui_widget_t *overlay = ui_create_dialog_overlay();
        ui_widget_t *content = ui_create_dialog_content();
        ui_widget_t *header = ui_create_dialog_header();
        ui_widget_t *title = ui_create_dialog_title();
        ui_widget_t *description = ui_create_dialog_description();
        ui_widget_t *footer = ui_create_dialog_footer();
        ui_widget_t *close = ui_create_button();
        ui_widget_t *open = ui_create_button();

        ui_widget_set_attr(overlay, "data-dialog-target", "attachment-preview");
        ui_text_set_content(title, "research-summary.pdf");
        ui_text_set_content(
            description,
            "The attachment trigger opens this dialog while the actions stay "
            "independently clickable.");
        ui_button_set_text(close, "Close");
        ui_button_set_variant(close, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_set_attr(close, "data-dialog-close", "attachment-preview");
        ui_button_set_text(open, "Open");
        ui_button_set_variant(open, UI_BUTTON_VARIANT_PRIMARY);
        ui_widget_set_attr(open, "data-dialog-close", "attachment-preview");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(footer, close);
        ui_widget_append(footer, open);
        ui_widget_append(content, header);
        ui_widget_append(content, footer);
        ui_widget_append(overlay, content);
        ui_widget_append(parent, overlay);
}

void attachment_trigger_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();
        ui_widget_t *actions = ui_create_attachment_actions();
        ui_widget_t *trigger = ui_create_attachment_trigger();

        ui_load_css_string(attachment_trigger_css, "attachment-trigger-demo");
        ui_widget_add_class(box, "attachment-trigger-demo");

        ui_icon_set_name(icon, "document");
        ui_widget_append(media, icon);
        ui_text_set_content(title, "research-summary.pdf");
        ui_text_set_content(description, "Open preview dialog");
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(actions, create_action("copy", "Copy link"));
        ui_widget_append(
            actions, create_action("dismiss", "Remove research-summary.pdf"));
        ui_widget_set_attr(trigger, "data-dialog-trigger",
                           "attachment-preview");
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        ui_widget_append(attachment, actions);
        ui_widget_append(attachment, trigger);
        ui_widget_append(box, attachment);
        ui_widget_append(parent, box);
        create_preview_dialog(parent);
}
