#include <LCUI.h>

typedef struct item_spec {
        const char *icon;
        const char *label;
} item_spec_t;

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_item(const char *icon_name, const char *label)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();

        ui_widget_append(item, ui_create_fluent_icon(icon_name));
        ui_widget_append(item, create_text(label));
        return item;
}

void button_group_dropdown_init(ui_widget_t *parent)
{
        static const item_spec_t items[] = {
                { "speaker-off", "Mute Conversation" },
                { "checkmark", "Mark as Read" },
                { "warning", "Report Conversation" },
                { "person-delete", "Block User" },
                { "share", "Share Conversation" },
                { "copy", "Copy Conversation" },
        };
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *follow = ui_create_button();
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *menu_group = ui_create_dropdown_menu_group();
        ui_widget_t *delete_group = ui_create_dropdown_menu_group();
        ui_widget_t *delete_item;
        size_t i;

        ui_widget_add_class(group, "button-group-dropdown-demo");
        ui_button_set_variant(follow, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_append(follow, create_text("Follow"));
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon("chevron-down"));
        ui_widget_append(trigger, button);
        for (i = 0; i < sizeof(items) / sizeof(items[0]); ++i) {
                ui_widget_append(menu_group,
                                 create_item(items[i].icon, items[i].label));
        }
        delete_item = create_item("delete", "Delete Conversation");
        ui_dropdown_menu_item_set_variant(delete_item,
                                          UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);
        ui_widget_append(delete_group, delete_item);
        ui_widget_set_attr(content, "align", "end");
        ui_widget_append(content, menu_group);
        ui_widget_append(content, ui_create_dropdown_menu_separator());
        ui_widget_append(content, delete_group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(group, follow);
        ui_widget_append(group, menu);
        ui_widget_append(parent, group);
}
