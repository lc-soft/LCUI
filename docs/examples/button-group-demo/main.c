#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_button(const char *label)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_append(button, create_text(label));
        return button;
}

static ui_widget_t *create_menu(void)
{
        const char *items[] = { "Mark as Read", "Archive", "Snooze",
                                "Add to Calendar", "Add to List" };
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();
        size_t i;

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon("more-horizontal"));
        ui_widget_append(trigger, button);
        for (i = 0; i < 5; ++i) {
                ui_widget_t *item = ui_create_dropdown_menu_item();

                ui_widget_append(item, create_text(items[i]));
                ui_widget_append(group, item);
        }
        ui_widget_append(content, group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        return menu;
}

void button_group_demo_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *inner;
        ui_widget_t *button;

        ui_widget_add_class(group, "button-group-demo");
        inner = ui_create_button_group();
        button = ui_create_button();
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon("arrow-left"));
        ui_widget_append(inner, button);
        ui_widget_append(group, inner);

        inner = ui_create_button_group();
        ui_widget_append(inner, create_button("Archive"));
        ui_widget_append(inner, create_button("Report"));
        ui_widget_append(group, inner);

        inner = ui_create_button_group();
        ui_widget_append(inner, create_button("Snooze"));
        ui_widget_append(inner, create_menu());
        ui_widget_append(group, inner);
        ui_widget_append(parent, group);
}
