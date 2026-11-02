#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void input_group_block_start_init(ui_widget_t *parent)
{
        ui_widget_t *fields = ui_create_field_group();
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *description = ui_create_field_description();
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *addon_text = ui_create_input_group_text();
        ui_widget_t *field2 = ui_create_field();
        ui_widget_t *label2 = ui_create_field_label();
        ui_widget_t *description2 = ui_create_field_description();
        ui_widget_t *group2 = ui_create_input_group();
        ui_widget_t *textarea = ui_create_input_group_textarea();
        ui_widget_t *addon2 = ui_create_input_group_addon();
        ui_widget_t *file_name = ui_create_input_group_text();
        ui_widget_t *copy = ui_create_input_group_button();

        ui_widget_add_class(fields, "input-group-block-start");
        ui_text_set_content(label, "Input");
        ui_label_set_for(label, "block-start-input");
        ui_textinput_set_placeholder(input, "Enter your name");
        ui_widget_set_id(input, "block-start-input");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_BLOCK_START);
        ui_widget_append(addon_text, create_text("Full Name"));
        ui_widget_append(addon, addon_text);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_text_set_content(description, "Header positioned above the input.");
        ui_widget_append(field, label);
        ui_widget_append(field, group);
        ui_widget_append(field, description);

        ui_text_set_content(label2, "Textarea");
        ui_label_set_for(label2, "block-start-textarea");
        ui_textinput_set_placeholder(textarea, "console.log('Hello, world!');");
        ui_widget_set_id(textarea, "block-start-textarea");
        ui_widget_add_class(textarea, "input-group-mono");
        ui_input_group_addon_set_align(addon2,
                                       UI_INPUT_GROUP_ADDON_BLOCK_START);
        ui_widget_add_class(file_name, "input-group-mono");
        ui_widget_append(file_name, ui_create_fluent_icon("code"));
        ui_widget_append(file_name, create_text("script.js"));
        ui_button_set_size(copy, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_add_class(copy, "input-group-block-start-action");
        ui_widget_append(copy, ui_create_fluent_icon("copy"));
        ui_widget_append(addon2, file_name);
        ui_widget_append(addon2, copy);
        ui_widget_append(group2, textarea);
        ui_widget_append(group2, addon2);
        ui_text_set_content(description2,
                            "Header positioned above the textarea.");
        ui_widget_append(field2, label2);
        ui_widget_append(field2, group2);
        ui_widget_append(field2, description2);

        ui_widget_append(fields, field);
        ui_widget_append(fields, field2);
        ui_widget_append(parent, fields);
}
