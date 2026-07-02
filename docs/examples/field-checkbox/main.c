/*
 * docs/examples/field-checkbox/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

static void make_check_row(ui_widget_t *parent, const char *id,
                           const char *text, int checked)
{
        ui_widget_t *row = ui_create_field();
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *lbl = ui_create_field_label();

        ui_field_set_orientation(row, "horizontal");
        ui_widget_set_id(cb, id);
        if (checked) {
                ui_widget_set_attr(cb, "checked", "true");
        }
        ui_widget_append(row, cb);
        ui_label_set_for(lbl, id);
        ui_text_set_content(lbl, text);
        ui_widget_append(row, lbl);
        ui_widget_append(parent, row);
}

void field_checkbox_init(ui_widget_t *parent)
{
        ui_widget_t *outer, *set, *legend, *desc, *group;
        ui_widget_t *sep;
        ui_widget_t *sync_field, *sync_cb, *sync_content;
        ui_widget_t *sync_lbl, *sync_desc;

        outer = ui_create_widget(NULL);
        set = ui_create_field_set();
        legend = ui_create_field_legend();
        desc = ui_create_field_description();
        group = ui_create_field_group();
        sep = ui_create_field_separator();
        sync_field = ui_create_field();
        sync_cb = ui_create_checkbox();
        sync_content = ui_create_field_content();
        sync_lbl = ui_create_field_label();
        sync_desc = ui_create_field_description();

        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "gap", "8px");
        ui_widget_set_style_string(outer, "max-width", "400px");

        ui_text_set_content(legend, "Show these items on the desktop");
        ui_widget_append(set, legend);
        ui_text_set_content(
            desc, "Select the items you want to show on the desktop.");
        ui_widget_append(set, desc);
        ui_widget_set_style_string(group, "gap", "12px");
        ui_widget_append(set, group);

        make_check_row(group, "finder-pref-9k2-hard-disks-ljj", "Hard disks",
                       1);
        make_check_row(group, "finder-pref-9k2-external-disks-1yg",
                       "External disks", 0);
        make_check_row(group, "finder-pref-9k2-cds-dvds-fzt",
                       "CDs, DVDs, and iPods", 0);
        make_check_row(group, "finder-pref-9k2-connected-servers-6l2",
                       "Connected servers", 0);

        ui_widget_append(outer, set);
        ui_widget_append(outer, sep);

        ui_field_set_orientation(sync_field, "horizontal");
        ui_widget_set_id(sync_cb, "finder-pref-9k2-sync-folders-nep");
        ui_widget_set_attr(sync_cb, "checked", "true");
        ui_widget_append(sync_field, sync_cb);
        ui_widget_append(sync_field, sync_content);
        ui_label_set_for(sync_lbl, "finder-pref-9k2-sync-folders-nep");
        ui_text_set_content(sync_lbl, "Sync Desktop & Documents folders");
        ui_widget_append(sync_content, sync_lbl);
        ui_text_set_content(
            sync_desc, "Your Desktop & Documents folders are being synced with "
                       "iCloud Drive. You can access them from other devices.");
        ui_widget_append(sync_content, sync_desc);

        ui_widget_append(outer, sync_field);
        ui_widget_append(parent, outer);
}
