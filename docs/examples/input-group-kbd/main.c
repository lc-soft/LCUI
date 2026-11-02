#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void input_group_kbd_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *end = ui_create_input_group_addon();
        ui_widget_t *kbd = ui_create_input_group_kbd();

        ui_widget_add_class(group, "input-group-kbd-demo");
        ui_textinput_set_placeholder(input, "Search...");
        ui_widget_append(addon, ui_create_fluent_icon("search"));
        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(kbd, create_text("Ctrl K"));
        ui_widget_append(end, kbd);
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(group, end);
        ui_widget_append(parent, group);
}
