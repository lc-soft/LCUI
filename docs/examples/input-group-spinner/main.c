#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_spinner(void)
{
        ui_widget_t *spinner = ui_create_spinner();

        ui_widget_resize(spinner, 16, 16);
        return spinner;
}

void input_group_spinner_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group;
        ui_widget_t *input;
        ui_widget_t *addon;

        ui_widget_add_class(box, "input-group-spinner-demo");
        ui_widget_add_class(box, "clip-safe");

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Searching...");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(addon, create_spinner());
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Processing...");
        addon = ui_create_input_group_addon();
        ui_widget_append(addon, create_spinner());
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Saving changes...");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(addon, create_text("Saving..."));
        ui_widget_append(addon, create_spinner());
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Refreshing data...");
        addon = ui_create_input_group_addon();
        ui_widget_append(addon, create_spinner());
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(addon, create_text("Please wait..."));
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        ui_widget_append(parent, box);
}
