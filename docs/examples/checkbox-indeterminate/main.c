#include <LCUI.h>

static void add_row(ui_widget_t *parent, const char *id, const char *text,
                    int checked, int indeterminate)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *label = ui_create_label();

        ui_widget_add_class(row, "checkbox-row");
        ui_widget_set_id(cb, id);
        if (checked) {
                ui_widget_set_attr(cb, "checked", "true");
        }
        if (indeterminate) {
                ui_widget_set_attr(cb, "indeterminate", "true");
        }
        ui_label_set_for(label, id);
        ui_text_set_content(label, text);
        ui_widget_append(row, cb);
        ui_widget_append(row, label);
        ui_widget_append(parent, row);
}

void checkbox_indeterminate_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "checkbox-indeterminate-demo");
        add_row(box, "select-all", "Select all", 0, 1);
        add_row(box, "option-a", "Option A", 1, 0);
        add_row(box, "option-b", "Option B", 0, 0);
        ui_widget_append(parent, box);
}
