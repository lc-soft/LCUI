#include <LCUI.h>

void radio_group_basic_init(ui_widget_t *parent)
{
        ui_widget_t *group;
        ui_widget_t *rows[3];
        ui_widget_t *items[3];
        ui_widget_t *labels[3];
        const char *ids[3] = { "r1", "r2", "r3" };
        const char *values[3] = { "default", "comfortable", "compact" };
        const char *texts[3] = { "Default", "Comfortable", "Compact" };
        int i;

        group = ui_create_radio_group();
        ui_widget_set_attr(group, "value", "comfortable");

        for (i = 0; i < 3; ++i) {
                rows[i] = ui_create_widget(NULL);
                ui_widget_set_style_string(rows[i], "display", "flex");
                ui_widget_set_style_string(rows[i], "align-items", "center");
                ui_widget_set_style_string(rows[i], "gap", "8px");

                items[i] = ui_create_radio_group_item();
                ui_widget_set_id(items[i], ids[i]);
                ui_widget_set_attr(items[i], "value", values[i]);
                if (i == 1) {
                        ui_widget_set_attr(items[i], "checked", "true");
                }
                ui_widget_append(rows[i], items[i]);

                labels[i] = ui_create_label();
                ui_label_set_for(labels[i], ids[i]);
                ui_text_set_content(labels[i], texts[i]);
                ui_widget_append(rows[i], labels[i]);

                ui_widget_append(group, rows[i]);
        }
        ui_widget_append(parent, group);
}
