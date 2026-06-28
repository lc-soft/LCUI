#include <LCUI.h>

void label_basic_init(ui_widget_t *parent)
{
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *label = ui_create_label();
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_set_id(cb, "terms");
        ui_widget_append(box, cb);
        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "gap", "8px");
        ui_label_set_for(label, "terms");
        ui_text_set_content(label, "Accept terms and conditions");
        ui_widget_append(box, label);
        ui_widget_append(parent, box);
}
