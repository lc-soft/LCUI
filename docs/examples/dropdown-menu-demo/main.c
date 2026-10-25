#include <LCUI.h>

static ui_widget_t *create_item(const char *label, const char *shortcut,
                                bool disabled)
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
        if (disabled) {
                ui_widget_set_disabled(item, true);
        }
        return item;
}

static ui_widget_t *create_group(const char *label)
{
        ui_widget_t *group = ui_create_dropdown_menu_group();
        ui_widget_t *title = ui_create_dropdown_menu_label();

        ui_text_set_content(title, label);
        ui_widget_append(group, title);
        return group;
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

void dropdown_menu_demo_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group;
        ui_widget_t *sub;
        ui_widget_t *sub_content;

        ui_widget_add_class(menu, "dropdown-menu-demo");
        ui_widget_add_class(content, "dropdown-menu-demo-content");
        ui_widget_set_attr(content, "align", "start");

        ui_button_set_text(button, "Open");
        ui_widget_append(trigger, button);

        group = create_group("My Account");
        ui_widget_append(group, create_item("Profile", "Shift+P", false));
        ui_widget_append(group, create_item("Billing", "Ctrl+B", false));
        ui_widget_append(group, create_item("Settings", "Ctrl+S", false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("Team", NULL, false));

        sub = ui_create_dropdown_menu_sub();
        sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub, create_sub_trigger("Invite users"));
        ui_widget_append(sub_content, create_item("Email", NULL, false));
        ui_widget_append(sub_content, create_item("Message", NULL, false));
        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());
        ui_widget_append(sub_content, create_item("More...", NULL, false));
        ui_widget_append(sub, sub_content);
        ui_widget_append(group, sub);
        ui_widget_append(group, create_item("New Team", "Ctrl+T", false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("GitHub", NULL, false));
        ui_widget_append(group, create_item("Support", NULL, false));
        ui_widget_append(group, create_item("API", NULL, true));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("Log out", "Shift+Q", false));
        ui_widget_append(content, group);

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
