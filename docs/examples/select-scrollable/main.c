#include <LCUI.h>

static void add_item(ui_widget_t *group, const char *label, const char *value)
{
        ui_widget_t *item = ui_create_select_item(value);

        ui_text_set_content(item, label);
        ui_widget_append(group, item);
}

static void add_label(ui_widget_t *group, const char *text)
{
        ui_widget_t *label = ui_create_select_label();

        ui_text_set_content(label, text);
        ui_widget_append(group, label);
}

void select_scrollable_init(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *group;

        ui_widget_add_class(select, "select-scrollable-demo");
        ui_select_set_placeholder(select, "Select a timezone");
        ui_widget_append(select, ui_create_select_trigger());
        ui_widget_append(select, ui_create_select_content());

        group = ui_create_select_group();
        add_label(group, "North America");
        add_item(group, "Eastern Standard Time", "est");
        add_item(group, "Central Standard Time", "cst");
        add_item(group, "Mountain Standard Time", "mst");
        add_item(group, "Pacific Standard Time", "pst");
        add_item(group, "Alaska Standard Time", "akst");
        add_item(group, "Hawaii Standard Time", "hst");
        ui_select_append_item(select, group);

        group = ui_create_select_group();
        add_label(group, "Europe & Africa");
        add_item(group, "Greenwich Mean Time", "gmt");
        add_item(group, "Central European Time", "cet");
        add_item(group, "Eastern European Time", "eet");
        add_item(group, "Western European Summer Time", "west");
        add_item(group, "Central Africa Time", "cat");
        add_item(group, "East Africa Time", "eat");
        ui_select_append_item(select, group);

        ui_select_append_item(select, ui_create_select_separator());

        group = ui_create_select_group();
        add_label(group, "Asia");
        add_item(group, "Moscow Time", "msk");
        add_item(group, "India Standard Time", "ist");
        add_item(group, "China Standard Time", "cst_china");
        add_item(group, "Japan Standard Time", "jst");
        add_item(group, "Korea Standard Time", "kst");
        add_item(group, "Indonesia Central Standard Time", "ist_indonesia");
        ui_select_append_item(select, group);

        ui_widget_append(parent, select);
}
