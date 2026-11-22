#include <LCUI.h>

void input_group_button_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group;
        ui_widget_t *input;
        ui_widget_t *addon;
        ui_widget_t *button;

        ui_widget_add_class(box, "input-group-button-demo");
        ui_widget_add_class(box, "clip-safe");

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "https://example.com/username");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        button = ui_create_input_group_button();
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_append(button, ui_create_fluent_icon("copy"));
        ui_widget_append(addon, button);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Type to search...");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        button = ui_create_input_group_button();
        ui_button_set_variant(button, UI_BUTTON_VARIANT_SECONDARY);
        ui_button_set_text(button, "Search");
        ui_widget_append(addon, button);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "https://");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        button = ui_create_input_group_button();
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_append(button, ui_create_fluent_icon("star"));
        ui_widget_append(addon, button);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        ui_widget_append(parent, box);
}
