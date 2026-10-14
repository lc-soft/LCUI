#include <LCUI.h>

static ui_widget_t *add_item(ui_widget_t *group, const char *label,
                             const char *value)
{
        ui_widget_t *item = ui_create_select_item(value);

        ui_text_set_content(item, label);
        ui_widget_append(group, item);
        return item;
}

void select_disabled_init(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger = ui_create_select_trigger();
        ui_widget_t *content = ui_create_select_content();
        ui_widget_t *group = ui_create_select_group();
        ui_widget_t *grapes;

        ui_widget_add_class(select, "select-disabled-demo");
        ui_select_set_placeholder(select, "Select a fruit");
        add_item(group, "Apple", "apple");
        add_item(group, "Banana", "banana");
        add_item(group, "Blueberry", "blueberry");
        grapes = add_item(group, "Grapes", "grapes");
        ui_widget_set_disabled(grapes, true);
        add_item(group, "Pineapple", "pineapple");
        ui_widget_append(content, group);
        ui_widget_append(select, trigger);
        ui_widget_append(select, content);
        ui_select_build_children(select);
        ui_widget_set_disabled(select, true);
        ui_widget_append(parent, select);
}
