#include <LCUI.h>

static const char *css = ".dialog-close-button-example dialog-footer {\n"
                         "  justify-content: flex-start;\n"
                         "}\n"
                         ".dialog-close-button-example dialog-content {\n"
                         "  max-width: 448px;\n"
                         "}\n";

void dialog_close_button_init(ui_widget_t *parent)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *input = ui_create_widget("textinput");
        ui_widget_t *action;

        ui_load_css_string(css, "dialog_close_button_demo");
        ui_widget_add_class(dialog, "dialog-close-button-example");

        ui_dialog_build_children(dialog);
        ui_dialog_set_trigger_text(dialog, "Share");
        ui_dialog_set_title(dialog, "Share link");
        ui_dialog_set_description(
            dialog, "Anyone who has this link will be able to view this.");
        /* Keep a single primary Close button in the footer: drop the default
         * cancel button, then mark the action button as a close control. */
        ui_widget_remove(ui_dialog_get_footer_button(dialog, false));
        action = ui_dialog_get_footer_button(dialog, true);
        ui_widget_set_attr(action, "variant", "primary");
        ui_widget_set_attr(action, "data-dialog-close", "true");
        ui_button_set_text(action, "Close");
        ui_dialog_build_children(dialog);

        ui_textinput_set_text(input, "https://lcui.dev/docs/installation");
        ui_widget_set_attr(input, "readonly", "true");
        ui_widget_set_attr(input, "tabindex", "0");
        ui_widget_set_style_string(input, "flex", "1");
        ui_dialog_append_body(dialog, input);
        ui_widget_append(parent, dialog);
}
