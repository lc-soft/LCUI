#include <LCUI.h>

static ui_widget_t *create_menu_button(const char *icon_name,
                                       const char *label_text)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon(icon_name);
        ui_widget_t *label = ui_create_widget("text");

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_text_set_content(label, label_text);
        ui_widget_append(button, icon);
        ui_widget_append(button, label);
        return button;
}

void sidebar_group_collapsible_init(ui_widget_t *parent)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *content = ui_create_sidebar_content();
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *group = ui_create_sidebar_group();
        ui_widget_t *group_label = ui_create_sidebar_group_label();
        ui_widget_t *label_text = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-down");
        ui_widget_t *collapsible_content = ui_create_collapsible_content();
        ui_widget_t *group_content = ui_create_sidebar_group_content();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *main = ui_create_widget(NULL);
        ui_widget_t *main_title = ui_create_widget("text");
        ui_widget_t *main_copy = ui_create_widget("text");
        ui_widget_t *trigger = ui_create_sidebar_trigger();

        ui_widget_add_class(provider, "sidebar-group-collapsible-demo");

        ui_widget_set_attr(group_label, "data-collapsible-trigger", "true");
        ui_text_set_content(label_text, "Help");
        ui_widget_append(group_label, label_text);
        ui_widget_append(group_label, chevron);
        ui_collapsible_set_open(collapsible, true);
        ui_widget_append(menu, ui_create_sidebar_menu_item());
        ui_widget_append(ui_widget_get_child(menu, 0),
                         create_menu_button("chat-help", "Support"));
        ui_widget_append(menu, ui_create_sidebar_menu_item());
        ui_widget_append(ui_widget_get_child(menu, 1),
                         create_menu_button("calendar-mail", "Feedback"));
        ui_widget_append(group_content, menu);
        ui_widget_append(collapsible_content, group_content);
        ui_widget_append(group, group_label);
        ui_widget_append(group, collapsible_content);
        ui_widget_append(collapsible, group);

        ui_widget_add_class(main, "sidebar-main");
        ui_widget_add_class(main_title, "sidebar-main-title");
        ui_text_set_content(main_title, "Workspace");
        ui_widget_add_class(main_copy, "sidebar-main-copy");
        ui_text_set_content(main_copy,
                            "Groups can collapse without leaving the sidebar.");
        ui_widget_append(main, trigger);
        ui_widget_append(main, main_title);
        ui_widget_append(main, main_copy);

        ui_widget_append(content, collapsible);
        ui_widget_append(sidebar, content);
        ui_widget_append(provider, sidebar);
        ui_widget_append(provider, main);
        ui_widget_append(parent, provider);
}
