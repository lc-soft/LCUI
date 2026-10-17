#include <LCUI.h>

static void add_row(ui_widget_t *group, const char *id, const char *text,
                    int checked)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *label = ui_create_field_label();

        ui_widget_add_class(label, "font-normal");
        ui_field_set_orientation(field, "horizontal");
        ui_widget_set_id(cb, id);
        if (checked) {
                ui_widget_set_attr(cb, "checked", "true");
        }
        ui_label_set_for(label, id);
        ui_text_set_content(label, text);
        ui_widget_append(field, cb);
        ui_widget_append(field, label);
        ui_widget_append(group, field);
}

void checkbox_group_init(ui_widget_t *parent)
{
        ui_widget_t *outer = ui_create_widget(NULL);
        ui_widget_t *set = ui_create_field_set();
        ui_widget_t *legend = ui_create_field_legend();
        ui_widget_t *description = ui_create_field_description();
        ui_widget_t *group = ui_create_field_group();

        ui_widget_add_class(outer, "checkbox-group-demo");
        ui_text_set_content(legend, "Show these items on the desktop:");
        ui_text_set_content(
            description, "Select the items you want to show on the desktop.");
        add_row(group, "finder-pref-hard-disks", "Hard disks", 1);
        add_row(group, "finder-pref-external-disks", "External disks", 1);
        add_row(group, "finder-pref-cds-dvds", "CDs, DVDs, and iPods", 0);
        add_row(group, "finder-pref-servers", "Connected servers", 0);
        ui_widget_append(set, legend);
        ui_widget_append(set, description);
        ui_widget_append(set, group);
        ui_widget_append(outer, set);
        ui_widget_append(parent, outer);
}
