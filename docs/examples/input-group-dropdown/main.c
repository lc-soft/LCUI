#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_item(const char *label)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        return item;
}

static ui_widget_t *create_menu(const char *text, const char *icon,
                                const char *item1, const char *item2,
                                const char *item3)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_input_group_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();

        if (text) {
                ui_widget_append(button, create_text(text));
        } else {
                ui_button_set_size(button, UI_BUTTON_SIZE_ICON_XS);
        }
        ui_widget_append(button, ui_create_fluent_icon(icon));
        ui_widget_append(trigger, button);
        ui_widget_set_attr(content, "align", "end");
        ui_widget_append(group, create_item(item1));
        ui_widget_append(group, create_item(item2));
        ui_widget_append(group, create_item(item3));
        ui_widget_append(content, group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        return menu;
}

void input_group_dropdown_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *addon = ui_create_input_group_addon();

        ui_widget_add_class(box, "input-group-dropdown-demo");
        ui_widget_add_class(box, "clip-safe");
        ui_textinput_set_placeholder(input, "Enter file name");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(addon, create_menu(NULL, "more-horizontal", "Settings",
                                            "Copy path", "Open location"));
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        group = ui_create_input_group();
        input = ui_create_input_group_input();
        ui_textinput_set_placeholder(input, "Enter search query");
        addon = ui_create_input_group_addon();
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        ui_widget_append(
            addon, create_menu("Search In...", "chevron-down", "Documentation",
                               "Blog Posts", "Changelog"));
        ui_widget_append(group, input);
        ui_widget_append(group, addon);
        ui_widget_append(box, group);

        ui_widget_append(parent, box);
}
