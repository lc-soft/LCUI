#include <LCUI.h>

static ui_widget_t *add_group(ui_widget_t *parent, const char *placeholder,
                              const char *start_icon, const char *end_icon,
                              const char *end_icon2)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();

        ui_textinput_set_placeholder(input, placeholder);
        ui_widget_append(group, input);
        if (start_icon) {
                ui_widget_t *start = ui_create_input_group_addon();

                ui_widget_append(start, ui_create_fluent_icon(start_icon));
                ui_widget_append(group, start);
        }
        if (end_icon) {
                ui_widget_t *end = ui_create_input_group_addon();

                ui_input_group_addon_set_align(end,
                                               UI_INPUT_GROUP_ADDON_INLINE_END);
                ui_widget_append(end, ui_create_fluent_icon(end_icon));
                if (end_icon2) {
                        ui_widget_append(end, ui_create_fluent_icon(end_icon2));
                }
                ui_widget_append(group, end);
        }
        ui_widget_append(parent, group);
        return group;
}

void input_group_icon_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "input-group-icon-demo");
        ui_widget_add_class(box, "clip-safe");
        add_group(box, "Search...", "search", NULL, NULL);
        add_group(box, "Enter your email", "mail", NULL, NULL);
        add_group(box, "Card number", "payment", "checkmark", NULL);
        add_group(box, "Card number", NULL, "star", "info");
        ui_widget_append(parent, box);
}
