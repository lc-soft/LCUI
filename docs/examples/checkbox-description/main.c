#include <LCUI.h>

void checkbox_description_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_field_group();
        ui_widget_t *field = ui_create_field();
        ui_widget_t *cb = ui_create_checkbox();
        ui_widget_t *content = ui_create_field_content();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *description = ui_create_field_description();

        ui_widget_add_class(group, "checkbox-description-demo");
        ui_field_set_orientation(field, "horizontal");
        ui_widget_set_id(cb, "terms-checkbox-desc");
        ui_widget_set_attr(cb, "checked", "true");
        ui_label_set_for(label, "terms-checkbox-desc");
        ui_text_set_content(label, "Accept terms and conditions");
        ui_text_set_content(description, "By clicking this checkbox, you agree "
                                         "to the terms and conditions.");
        ui_widget_append(field, cb);
        ui_widget_append(content, label);
        ui_widget_append(content, description);
        ui_widget_append(field, content);
        ui_widget_append(group, field);
        ui_widget_append(parent, group);
}
