#include <LCUI.h>

static ui_widget_t *create_item(const char *icon_name, const char *label,
                                const char *shortcut, bool disabled,
                                bool destructive)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *hint;

        if (icon_name) {
                ui_widget_append(item, ui_create_fluent_icon(icon_name));
        }
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
        if (destructive) {
                ui_dropdown_menu_item_set_variant(
                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);
        }
        return item;
}

static ui_widget_t *create_checkbox(const char *icon_name, const char *label,
                                    bool checked)
{
        ui_widget_t *item = ui_create_dropdown_menu_checkbox_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        if (checked) {
                ui_widget_set_attr(item, "checked", "true");
        }
        return item;
}

static ui_widget_t *create_radio(const char *icon_name, const char *label,
                                 const char *value)
{
        ui_widget_t *item = ui_create_dropdown_menu_radio_item();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_set_attr(item, "value", value);
        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_append(item, text);
        return item;
}

static ui_widget_t *create_sub_trigger(const char *icon_name, const char *label)
{
        ui_widget_t *trigger = ui_create_dropdown_menu_sub_trigger();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-right");

        ui_widget_append(trigger, ui_create_fluent_icon(icon_name));
        ui_text_set_content(text, label);
        ui_widget_add_class(chevron, "dropdown-menu-chevron");
        ui_widget_append(trigger, text);
        ui_widget_append(trigger, chevron);
        return trigger;
}

static ui_widget_t *create_sub(ui_widget_t *trigger, ui_widget_t *content)
{
        ui_widget_t *sub = ui_create_dropdown_menu_sub();

        ui_widget_append(sub, trigger);
        ui_widget_append(sub, content);
        return sub;
}

static ui_widget_t *create_label(const char *text)
{
        ui_widget_t *label = ui_create_dropdown_menu_label();

        ui_text_set_content(label, text);
        return label;
}

void dropdown_menu_complex_init(ui_widget_t *parent)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group;
        ui_widget_t *sub_content;
        ui_widget_t *sub_sub_content;
        ui_widget_t *radios;

        ui_widget_add_class(content, "dropdown-menu-complex-demo-content");
        ui_button_set_text(button, "Complex Menu");
        ui_widget_append(trigger, button);

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_label("File"));
        ui_widget_append(
            group, create_item("document", "New File", "Ctrl+N", false, false));
        ui_widget_append(group, create_item("folder", "New Folder", "Shift+N",
                                            false, false));
        sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub_content, create_label("Recent Projects"));
        ui_widget_append(sub_content, create_item("code", "Project Alpha", NULL,
                                                  false, false));
        ui_widget_append(sub_content, create_item("code", "Project Beta", NULL,
                                                  false, false));
        sub_sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub_sub_content, create_item("code", "Project Gamma",
                                                      NULL, false, false));
        ui_widget_append(sub_sub_content, create_item("code", "Project Delta",
                                                      NULL, false, false));
        ui_widget_append(
            sub_content,
            create_sub(create_sub_trigger("more-horizontal", "More Projects"),
                       sub_sub_content));
        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());
        ui_widget_append(sub_content, create_item("search", "Browse...", NULL,
                                                  false, false));
        ui_widget_append(
            group, create_sub(create_sub_trigger("folder-open", "Open Recent"),
                              sub_content));
        ui_widget_append(group, ui_create_dropdown_menu_separator());
        ui_widget_append(group,
                         create_item("save", "Save", "Ctrl+S", false, false));
        ui_widget_append(group, create_item("arrow-download", "Export",
                                            "Shift+E", false, false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_label("View"));
        ui_widget_append(group, create_checkbox("eye", "Show Sidebar", true));
        ui_widget_append(group,
                         create_checkbox("grid", "Show Status Bar", false));
        sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub_content, create_label("Appearance"));
        radios = ui_create_dropdown_menu_radio_group();
        ui_widget_set_attr(radios, "value", "light");
        ui_widget_append(radios,
                         create_radio("weather-sunny", "Light", "light"));
        ui_widget_append(radios, create_radio("weather-moon", "Dark", "dark"));
        ui_widget_append(radios, create_radio("desktop", "System", "system"));
        ui_widget_append(sub_content, radios);
        ui_widget_append(group,
                         create_sub(create_sub_trigger("paint-brush", "Theme"),
                                    sub_content));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_label("Account"));
        ui_widget_append(
            group, create_item("person", "Profile", "Shift+P", false, false));
        ui_widget_append(group,
                         create_item("payment", "Billing", NULL, false, false));
        sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub_content, create_label("Preferences"));
        ui_widget_append(
            sub_content,
            create_item("keyboard", "Keyboard Shortcuts", NULL, false, false));
        ui_widget_append(sub_content, create_item("local-language", "Language",
                                                  NULL, false, false));
        sub_sub_content = ui_create_dropdown_menu_sub_content();
        ui_widget_append(sub_sub_content, create_label("Notification Types"));
        ui_widget_append(sub_sub_content,
                         create_checkbox("alert", "Push Notifications", true));
        ui_widget_append(sub_sub_content,
                         create_checkbox("mail", "Email Notifications", true));
        ui_widget_append(sub_content, create_sub(create_sub_trigger(
                                                     "alert", "Notifications"),
                                                 sub_sub_content));
        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());
        ui_widget_append(
            sub_content,
            create_item("shield", "Privacy & Security", NULL, false, false));
        ui_widget_append(group,
                         create_sub(create_sub_trigger("settings", "Settings"),
                                    sub_content));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(group, create_item("question-circle", "Help & Support",
                                            NULL, false, false));
        ui_widget_append(group, create_item("document-text", "Documentation",
                                            NULL, false, false));
        ui_widget_append(content, group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());

        group = ui_create_dropdown_menu_group();
        ui_widget_append(
            group, create_item("sign-out", "Sign Out", "Shift+Q", false, true));
        ui_widget_append(content, group);

        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(parent, menu);
}
