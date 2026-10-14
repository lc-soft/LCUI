#include <LCUI.h>

static ui_widget_t *make_field(const char *label_text, const char *value)
{
        ui_widget_t *field = ui_create_widget("field");
        ui_widget_t *label = ui_create_widget("label");
        ui_widget_t *input = ui_create_widget("textinput");

        ui_text_set_content(label, label_text);
        ui_textinput_set_text(input, value);
        ui_widget_set_attr(input, "tabindex", "0");
        ui_widget_append(field, label);
        ui_widget_append(field, input);
        return field;
}

void dialog_basic_init(ui_widget_t *parent)
{
        ui_widget_t *dialog = ui_create_dialog();

        ui_dialog_build_children(dialog);
        ui_dialog_set_trigger_text(dialog, "Open Dialog");
        ui_dialog_set_title(dialog, "Edit profile");
        ui_dialog_set_description(
            dialog,
            "Make changes to your profile here. Click save when you're done.");
        ui_dialog_set_cancel_text(dialog, "Cancel");
        ui_dialog_set_action_text(dialog, "Save changes");
        ui_dialog_append_body(dialog, make_field("Name", "Pedro Duarte"));
        ui_dialog_append_body(dialog, make_field("Username", "@peduarte"));
        ui_widget_append(parent, dialog);
}
