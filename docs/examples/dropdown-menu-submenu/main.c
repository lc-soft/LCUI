#include <LCUI.h>

static ui_widget_t *create_item(const char *label, const char *shortcut)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *hint;

        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        if (shortcut) {
                hint = ui_create_dropdown_menu_shortcut();
                ui_text_set_content(hint, shortcut);
                ui_widget_append(item, hint);
        }
        return item;
}

static ui_widget_t *create_sub_trigger(const char *label)
{
        ui_widget_t *trigger = ui_create_dropdown_menu_sub_trigger();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-right");

        ui_text_set_content(text, label);
        ui_widget_add_class(chevron, "dropdown-menu-chevron");
        ui_widget_append(trigger, text);
        ui_widget_append(trigger, chevron);
        return trigger;
}

void dropdown_menu_submenu_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();
        ui_widget_t *sub = ui_create_dropdown_menu_sub();
        ui_widget_t *sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_t *more = ui_create_dropdown_menu_sub();
        ui_widget_t *more_content = ui_create_dropdown_menu_sub_content();

        ui_button_set_text(button, "Open");
        ui_widget_append(trigger, button);

        ui_widget_append(group, create_item("Team", NULL));
        ui_widget_append(sub, create_sub_trigger("Invite users"));
        ui_widget_append(sub_content, create_item("Email", NULL));
        ui_widget_append(sub_content, create_item("Message", NULL));
        ui_widget_append(more, create_sub_trigger("More options"));
        ui_widget_append(more_content, create_item("Calendly", NULL));
        ui_widget_append(more_content, create_item("Slack", NULL));
        ui_widget_append(more_content, ui_create_dropdown_menu_separator());
        ui_widget_append(more_content, create_item("Webhook", NULL));
        ui_widget_append(more, more_content);
        ui_widget_append(sub_content, more);
        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());
        ui_widget_append(sub_content, create_item("Advanced...", NULL));
        ui_widget_append(sub, sub_content);
        ui_widget_append(group, sub);
        ui_widget_append(group, create_item("New Team", "Ctrl+T"));

        ui_widget_append(content, group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
