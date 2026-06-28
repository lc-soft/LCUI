#include <LCUI.h>

void checkbox_disabled_init(ui_widget_t *parent)
{
        ui_widget_t *box;
        ui_widget_t *rows[2];
        ui_widget_t *boxes[2];
        ui_widget_t *labels[2];
        const char *ids[2] = { "d1", "d2" };
        const char *texts[2] = { "Disabled (pre-checked)", "Normal" };
        int i;

        box = ui_create_widget(NULL);
        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "gap", "8px");

        for (i = 0; i < 2; ++i) {
                rows[i] = ui_create_widget(NULL);
                ui_widget_set_style_string(rows[i], "display", "flex");
                ui_widget_set_style_string(rows[i], "align-items", "center");
                ui_widget_set_style_string(rows[i], "gap", "8px");

                boxes[i] = ui_create_checkbox();
                ui_widget_set_id(boxes[i], ids[i]);
                ui_widget_set_attr(boxes[i], "checked", "true");
                if (i == 0) {
                        ui_widget_set_disabled(boxes[i], true);
                }
                ui_widget_append(rows[i], boxes[i]);

                labels[i] = ui_create_label();
                ui_label_set_for(labels[i], ids[i]);
                ui_text_set_content(labels[i], texts[i]);
                ui_widget_append(rows[i], labels[i]);

                ui_widget_append(box, rows[i]);
        }
        ui_widget_append(parent, box);
}
