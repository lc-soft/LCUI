#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *add_text_addon(ui_widget_t *group, const char *content,
                                   ui_input_group_addon_align_t align)
{
        ui_widget_t *addon = ui_create_input_group_addon();
        ui_widget_t *text = ui_create_input_group_text();

        ui_input_group_addon_set_align(addon, align);
        ui_widget_append(text, create_text(content));
        ui_widget_append(addon, text);
        ui_widget_append(group, addon);
        return addon;
}

void input_group_text_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group;
        ui_widget_t *input;
        ui_widget_t *textarea;

        ui_widget_add_class(box, "input-group-text-demo");
        ui_widget_add_class(box, "clip-safe");

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "0.00");
        add_text_addon(group, "$", UI_INPUT_GROUP_ADDON_INLINE_START);
        ui_widget_append(group, input);
        add_text_addon(group, "USD", UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "example.com");
        add_text_addon(group, "https://", UI_INPUT_GROUP_ADDON_INLINE_START);
        ui_widget_append(group, input);
        add_text_addon(group, ".com", UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Enter your username");
        ui_widget_append(group, input);
        add_text_addon(group, "@company.com", UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        textarea = ui_create_input_group_textarea();
        ui_textinput_set_placeholder(textarea, "Enter your message");
        add_text_addon(group, "120 characters left",
                       UI_INPUT_GROUP_ADDON_BLOCK_END);
        ui_widget_append(group, textarea);
        ui_widget_append(box, group);

        ui_widget_append(parent, box);
}
