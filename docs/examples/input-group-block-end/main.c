#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void input_group_block_end_init(ui_widget_t *parent)
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
        ui_widget_t *counter = ui_create_input_group_text();
        ui_widget_t *post = ui_create_input_group_button();

        ui_widget_add_class(fields, "input-group-block-end");
        ui_text_set_content(label, "Input");
        ui_label_set_for(label, "block-end-input");
        ui_textinput_set_placeholder(input, "Enter amount");
        ui_widget_set_id(input, "block-end-input");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_widget_append(addon_text, create_text("USD"));
        ui_widget_append(addon, addon_text);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_text_set_content(description, "Footer positioned below the input.");
        ui_widget_append(field, label);
        ui_widget_append(field, group);
        ui_widget_append(field, description);

        ui_text_set_content(label2, "Textarea");
        ui_label_set_for(label2, "block-end-textarea");
        ui_textinput_set_placeholder(textarea, "Write a comment...");
        ui_widget_set_id(textarea, "block-end-textarea");
        ui_input_group_addon_set_align(addon2, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_widget_append(counter, create_text("0/280"));
        ui_button_set_variant(post, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(post, UI_BUTTON_SIZE_SM);
        ui_button_set_text(post, "Post");
        ui_widget_add_class(post, "input-group-block-end-action");
        ui_widget_append(addon2, counter);
        ui_widget_append(addon2, post);
        ui_widget_append(group2, textarea);
        ui_widget_append(group2, addon2);
        ui_text_set_content(description2,
                            "Footer positioned below the textarea.");
        ui_widget_append(field2, label2);
        ui_widget_append(field2, group2);
        ui_widget_append(field2, description2);

        ui_widget_append(fields, field);
        ui_widget_append(fields, field2);
        ui_widget_append(parent, fields);
}
