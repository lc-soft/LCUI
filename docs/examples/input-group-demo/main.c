#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void input_group_demo_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *start = ui_create_input_group_addon();
        ui_widget_t *end = ui_create_input_group_addon();

        ui_widget_add_class(group, "input-group-demo");
        ui_textinput_set_placeholder(input, "Search...");
        ui_widget_append(start, ui_create_fluent_icon("search"));
        ui_widget_append(end, create_text("12 results"));
        ui_input_group_addon_set_align(end, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(group, input);
        ui_widget_append(group, start);
        ui_widget_append(group, end);
        ui_widget_append(parent, group);
}
