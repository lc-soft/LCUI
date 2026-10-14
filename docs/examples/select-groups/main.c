#include <LCUI.h>

static ui_widget_t *add_item(ui_widget_t *group, const char *label,
                             const char *value)
{
        ui_widget_t *item = ui_create_select_item(value);

        ui_text_set_content(item, label);
        ui_widget_append(group, item);
        return item;
}

static ui_widget_t *add_label(ui_widget_t *group, const char *text)
{
        ui_widget_t *label = ui_create_select_label();

        ui_text_set_content(label, text);
        ui_widget_append(group, label);
        return label;
}

void select_groups_init(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *trigger = ui_create_select_trigger();
        ui_widget_t *content = ui_create_select_content();
        ui_widget_t *fruits = ui_create_select_group();
        ui_widget_t *vegetables = ui_create_select_group();

        ui_widget_add_class(select, "select-groups-demo");
        ui_select_set_placeholder(select, "Select a fruit");
        add_label(fruits, "Fruits");
        add_item(fruits, "Apple", "apple");
        add_item(fruits, "Banana", "banana");
        add_item(fruits, "Blueberry", "blueberry");
        add_label(vegetables, "Vegetables");
        add_item(vegetables, "Carrot", "carrot");
        add_item(vegetables, "Broccoli", "broccoli");
        add_item(vegetables, "Spinach", "spinach");
        ui_widget_append(content, fruits);
        ui_widget_append(content, ui_create_select_separator());
        ui_widget_append(content, vegetables);
        ui_widget_append(select, trigger);
        ui_widget_append(select, content);
        ui_select_build_children(select);
        ui_widget_append(parent, select);
}
