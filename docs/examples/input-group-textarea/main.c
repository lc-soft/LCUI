#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void input_group_textarea_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *textarea = ui_create_input_group_textarea();
        ui_widget_t *start = ui_create_input_group_addon();
        ui_widget_t *file = ui_create_input_group_text();
        ui_widget_t *copy = ui_create_input_group_button();
        ui_widget_t *end = ui_create_input_group_addon();
        ui_widget_t *counter = ui_create_input_group_text();
        ui_widget_t *run = ui_create_input_group_button();

        ui_widget_add_class(group, "input-group-textarea-demo");
        ui_textinput_set_placeholder(textarea, "console.log('Hello, world!');");
        ui_widget_add_class(textarea, "input-group-mono");
        ui_input_group_addon_set_align(start, UI_INPUT_GROUP_ADDON_BLOCK_START);
        ui_widget_add_class(file, "input-group-mono");
        ui_widget_append(file, ui_create_fluent_icon("code"));
        ui_widget_append(file, create_text("script.js"));
        ui_button_set_size(copy, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_add_class(copy, "input-group-block-start-action");
        ui_widget_append(copy, ui_create_fluent_icon("copy"));
        ui_widget_append(start, file);
        ui_widget_append(start, copy);

        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_widget_append(counter, create_text("Line 1, Column 1"));
        ui_button_set_variant(run, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(run, UI_BUTTON_SIZE_SM);
        ui_widget_append(run, create_text("Run"));
        ui_widget_append(run, ui_create_fluent_icon("arrow-enter-left"));
        ui_widget_add_class(run, "input-group-block-end-action");
        ui_widget_append(end, counter);
        ui_widget_append(end, run);

        ui_widget_append(group, textarea);
        ui_widget_append(group, start);
        ui_widget_append(group, end);
        ui_widget_append(parent, group);
}
