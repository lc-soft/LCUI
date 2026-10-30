#include <LCUI.h>
#include <stdio.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_link_item(const char *path, const char *label)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *link = ui_create_breadcrumb_link();

        ui_widget_set_attr(item, "data-path", path);
        ui_widget_append(link, create_text(label));
        ui_widget_append(item, link);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        return item;
}

static ui_widget_t *create_page_item(const char *path, const char *label)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *page = ui_create_breadcrumb_page();

        ui_widget_set_attr(item, "data-path", path);
        ui_widget_append(page, create_text(label));
        ui_widget_append(item, page);
        return item;
}

static ui_widget_t *create_menu_item(const char *path, const char *label)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();

        ui_widget_set_attr(item, "data-key", path);
        ui_widget_append(item, create_text(label));
        return item;
}

static ui_widget_t *create_ellipsis_item(void)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_widget_append(button, ui_create_breadcrumb_ellipsis());
        ui_widget_append(trigger, button);
        ui_widget_append(group,
                         create_menu_item("/Documentation", "Documentation"));
        ui_widget_append(group,
                         create_menu_item("/Documentation/Themes", "Themes"));
        ui_widget_append(
            group, create_menu_item("/Documentation/Themes/GitHub", "GitHub"));
        ui_widget_append(content, group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(item, menu);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        return item;
}

static void on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        char text[512];

        snprintf(text, sizeof(text), "Selected: %s",
                 arg ? (const char *)arg : "-");
        ui_text_set_content(e->data, text);
}

void breadcrumb_demo_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *status = create_text("Selected: -");

        ui_widget_add_class(box, "breadcrumb-demo");
        ui_widget_append(breadcrumb, create_link_item("/", "Home"));
        ui_widget_append(breadcrumb, create_ellipsis_item());
        ui_widget_append(
            breadcrumb,
            create_link_item("/Documentation/Themes/GitHub/Components",
                             "Components"));
        ui_widget_append(
            breadcrumb,
            create_page_item(
                "/Documentation/Themes/GitHub/Components/Breadcrumb",
                "Breadcrumb"));
        ui_widget_on(breadcrumb, "change", on_change, status);

        ui_widget_add_class(status, "breadcrumb-demo-status");
        ui_widget_append(box, breadcrumb);
        ui_widget_append(box, status);
        ui_widget_append(parent, box);
}
