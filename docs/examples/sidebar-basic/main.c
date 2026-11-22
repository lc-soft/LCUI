#include <LCUI.h>

static ui_widget_t *create_profile_button(const char *icon_name,
                                          const char *name, const char *meta,
                                          const char *extra_class,
                                          const char *logo_class)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *logo = ui_create_widget(NULL);
        ui_widget_t *icon = ui_create_fluent_icon(icon_name);
        ui_widget_t *text_box = ui_create_widget(NULL);
        ui_widget_t *name_text = ui_create_widget("text");
        ui_widget_t *meta_text = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-up-down");

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(button, extra_class);
        ui_widget_set_attr(button, "data-sidebar-tooltip", name);
        ui_widget_add_class(logo, logo_class);
        ui_widget_add_class(text_box, "sidebar-menu-text");
        ui_widget_add_class(name_text, "sidebar-menu-name");
        ui_widget_add_class(meta_text, "sidebar-menu-meta");
        ui_widget_add_class(chevron, "sidebar-menu-chevron");
        ui_text_set_content(name_text, name);
        ui_text_set_content(meta_text, meta);
        ui_widget_append(logo, icon);
        ui_widget_append(text_box, name_text);
        ui_widget_append(text_box, meta_text);
        ui_widget_append(button, logo);
        ui_widget_append(button, text_box);
        ui_widget_append(button, chevron);
        return button;
}

static ui_widget_t *create_menu_item(const char *icon_name, const char *label,
                                     bool destructive)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        if (destructive) {
                ui_dropdown_menu_item_set_variant(
                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);
        }
        return item;
}

static ui_widget_t *create_team_menu(void)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *button =
            create_profile_button("people-team", "Acme Inc", "Enterprise",
                                  "sidebar-team", "sidebar-team-logo");
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *label = ui_create_dropdown_menu_label();

        ui_widget_add_class(menu, "sidebar-team-menu");
        ui_widget_add_class(content, "sidebar-team-menu-content");
        ui_widget_set_attr(button, "data-dropdown-menu-trigger", "true");
        ui_widget_set_attr(content, "side", "right");
        ui_widget_set_attr(content, "align", "start");
        ui_text_set_content(label, "Teams");
        ui_widget_append(content, label);
        ui_widget_append(content,
                         create_menu_item("people-team", "Acme Inc", false));
        ui_widget_append(content,
                         create_menu_item("building", "Acme Corp", false));
        ui_widget_append(content,
                         create_menu_item("shield", "Evil Corp", false));
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, create_menu_item("add", "Add team", false));
        ui_widget_append(menu, button);
        ui_widget_append(menu, content);
        return menu;
}

static ui_widget_t *create_user_menu(void)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *button =
            create_profile_button("person-circle", "shadcn", "m@example.com",
                                  "sidebar-user", "sidebar-avatar");
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *label = ui_create_dropdown_menu_label();
        ui_widget_t *email = ui_create_widget("text");

        ui_widget_add_class(menu, "sidebar-user-menu");
        ui_widget_add_class(content, "sidebar-user-menu-content");
        ui_widget_set_attr(button, "data-dropdown-menu-trigger", "true");
        ui_widget_set_attr(content, "side", "right");
        ui_widget_set_attr(content, "align", "end");
        ui_text_set_content(label, "shadcn");
        ui_widget_append(content, label);
        ui_text_set_content(email, "m@example.com");
        ui_widget_add_class(email, "sidebar-user-menu-email");
        ui_widget_append(content, email);
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content,
                         create_menu_item("premium", "Upgrade to Pro", false));
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, create_menu_item("person", "Account", false));
        ui_widget_append(content,
                         create_menu_item("payment", "Billing", false));
        ui_widget_append(content,
                         create_menu_item("alert", "Notifications", false));
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content,
                         create_menu_item("sign-out", "Sign Out", false));
        ui_widget_append(menu, button);
        ui_widget_append(menu, content);
        return menu;
}

static ui_widget_t *create_nav_item(const char *icon_name, const char *title,
                                    bool active, bool open,
                                    const char **sub_titles, int sub_count)
{
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *item = ui_create_sidebar_menu_item();
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon(icon_name);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-right");
        ui_widget_t *content = ui_create_collapsible_content();
        ui_widget_t *sub = ui_create_sidebar_menu_sub();
        int i;

        ui_widget_add_class(button, "sidebar-menu-button");
        if (active) {
                ui_widget_add_class(button, "active");
        }
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_widget_add_class(chevron, "sidebar-menu-chevron");
        ui_widget_set_attr(button, "data-sidebar-tooltip", title);
        ui_text_set_content(label, title);
        ui_widget_append(button, icon);
        ui_widget_append(button, label);
        ui_widget_append(button, chevron);
        ui_widget_append(trigger, button);
        ui_widget_append(item, trigger);

        for (i = 0; i < sub_count; ++i) {
                ui_widget_t *sub_item = ui_create_sidebar_menu_sub_item();
                ui_widget_t *sub_button = ui_create_button();
                ui_widget_t *sub_label = ui_create_widget("text");

                ui_widget_add_class(sub_button, "sidebar-menu-button");
                ui_widget_add_class(sub_button, "sidebar-sub-button");
                ui_widget_add_class(sub_label, "sidebar-menu-label");
                ui_text_set_content(sub_label, sub_titles[i]);
                ui_widget_append(sub_button, sub_label);
                ui_widget_append(sub_item, sub_button);
                ui_widget_append(sub, sub_item);
        }
        ui_widget_append(content, sub);
        ui_widget_append(item, content);
        ui_widget_append(collapsible, item);
        if (open) {
                ui_collapsible_set_open(collapsible, true);
        }
        return collapsible;
}

static ui_widget_t *create_project_item(const char *icon_name,
                                        const char *title)
{
        ui_widget_t *item = ui_create_sidebar_menu_item();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon(icon_name);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *action = ui_create_sidebar_menu_action();
        ui_widget_t *more = ui_create_fluent_icon("more-horizontal");
        ui_widget_t *content = ui_create_dropdown_menu_content();

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_text_set_content(label, title);
        ui_widget_append(button, icon);
        ui_widget_append(button, label);

        ui_widget_add_class(menu, "sidebar-menu-action-menu");
        ui_widget_add_class(action, "sidebar-menu-action-trigger");
        ui_widget_set_attr(action, "data-dropdown-menu-trigger", "true");
        ui_widget_append(action, more);
        ui_widget_set_attr(content, "side", "right");
        ui_widget_set_attr(content, "align", "start");
        ui_widget_append(content,
                         create_menu_item("folder", "View Project", false));
        ui_widget_append(
            content, create_menu_item("arrow-right", "Share Project", false));
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content,
                         create_menu_item("delete", "Delete Project", true));
        ui_widget_append(menu, action);
        ui_widget_append(menu, content);

        ui_widget_append(item, button);
        ui_widget_append(item, menu);
        return item;
}

void sidebar_basic_init(ui_widget_t *parent)
{
        const char *playground[] = { "History", "Starred", "Settings" };
        const char *models[] = { "Genesis", "Explorer", "Quantum" };
        const char *documentation[] = { "Introduction", "Get Started",
                                        "Tutorials", "Changelog" };
        const char *settings[] = { "General", "Team", "Billing", "Limits" };
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *header = ui_create_sidebar_header();
        ui_widget_t *content = ui_create_sidebar_content();
        ui_widget_t *scroll = ui_create_scrollarea();
        ui_widget_t *scroll_content = ui_create_scrollarea_content();
        ui_widget_t *scrollbar = ui_create_scrollbar();
        ui_widget_t *group = ui_create_sidebar_group();
        ui_widget_t *group_label = ui_create_sidebar_group_label();
        ui_widget_t *group_label_text = ui_create_widget("text");
        ui_widget_t *group_content = ui_create_sidebar_group_content();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *projects = ui_create_sidebar_group();
        ui_widget_t *projects_label = ui_create_sidebar_group_label();
        ui_widget_t *projects_label_text = ui_create_widget("text");
        ui_widget_t *projects_content = ui_create_sidebar_group_content();
        ui_widget_t *projects_menu = ui_create_sidebar_menu();
        ui_widget_t *footer = ui_create_sidebar_footer();
        ui_widget_t *footer_menu = ui_create_sidebar_menu();
        ui_widget_t *footer_item = ui_create_sidebar_menu_item();
        ui_widget_t *main = ui_create_widget(NULL);
        ui_widget_t *trigger = ui_create_sidebar_trigger();
        ui_widget_t *rail;

        ui_widget_add_class(provider, "sidebar-basic-demo");
        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_add_class(scroll, "sidebar-scroll");
        ui_scrollarea_set_wheel_scroll_direction(scroll,
                                                 UI_SCROLLAREA_VERTICAL);

        ui_widget_append(header, create_team_menu());
        ui_text_set_content(group_label_text, "Platform");
        ui_widget_append(group_label, group_label_text);
        ui_widget_append(menu, create_nav_item("code-block", "Playground",
                                               false, true, playground, 3));
        ui_widget_append(menu, create_nav_item("document-text", "Models", false,
                                               false, models, 3));
        ui_widget_append(menu, create_nav_item("book-open", "Documentation",
                                               false, false, documentation, 4));
        ui_widget_append(menu, create_nav_item("calendar-settings", "Settings",
                                               false, false, settings, 4));
        ui_widget_append(group_content, menu);
        ui_widget_append(group, group_label);
        ui_widget_append(group, group_content);

        ui_widget_add_class(projects, "sidebar-projects");
        ui_text_set_content(projects_label_text, "Projects");
        ui_widget_append(projects_label, projects_label_text);
        ui_widget_append(
            projects_menu,
            create_project_item("code-block", "Design Engineering"));
        ui_widget_append(projects_menu,
                         create_project_item("book-open", "Sales & Marketing"));
        ui_widget_append(projects_menu,
                         create_project_item("document-text", "Travel"));

        {
                ui_widget_t *more_item = ui_create_sidebar_menu_item();
                ui_widget_t *more_button = ui_create_button();
                ui_widget_t *more_icon =
                    ui_create_fluent_icon("more-horizontal");
                ui_widget_t *more_label = ui_create_widget("text");

                ui_widget_add_class(more_button, "sidebar-menu-button");
                ui_widget_add_class(more_button, "sidebar-more");
                ui_widget_add_class(more_label, "sidebar-menu-label");
                ui_text_set_content(more_label, "More");
                ui_widget_append(more_button, more_icon);
                ui_widget_append(more_button, more_label);
                ui_widget_append(more_item, more_button);
                ui_widget_append(projects_menu, more_item);
        }
        ui_widget_append(projects_content, projects_menu);
        ui_widget_append(projects, projects_label);
        ui_widget_append(projects, projects_content);

        ui_widget_append(scroll_content, group);
        ui_widget_append(scroll_content, projects);
        ui_widget_append(scroll, scroll_content);
        ui_widget_append(scroll, scrollbar);
        ui_widget_append(content, scroll);

        ui_widget_append(footer_item, create_user_menu());
        ui_widget_append(footer_menu, footer_item);
        ui_widget_append(footer, footer_menu);

        ui_widget_add_class(main, "sidebar-main");
        ui_widget_append(main, trigger);
        ui_widget_append(sidebar, header);
        ui_widget_append(sidebar, content);
        ui_widget_append(sidebar, footer);
        rail = ui_create_sidebar_rail();
        ui_widget_append(sidebar, rail);
        ui_widget_append(provider, sidebar);
        ui_widget_append(provider, main);
        ui_widget_append(parent, provider);
}
