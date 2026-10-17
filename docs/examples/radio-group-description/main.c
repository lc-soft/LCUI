#include <LCUI.h>

static void add_row(ui_widget_t *group, const char *id, const char *value,
                    const char *title, const char *desc, int checked)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *item = ui_create_radio_group_item();
        ui_widget_t *content = ui_create_field_content();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *description = ui_create_field_description();

        ui_field_set_orientation(field, "horizontal");
        ui_widget_set_id(item, id);
        ui_widget_set_attr(item, "value", value);
        if (checked) {
                ui_widget_set_attr(item, "checked", "true");
        }
        ui_label_set_for(label, id);
        ui_text_set_content(label, title);
        ui_text_set_content(description, desc);
        ui_widget_append(content, label);
        ui_widget_append(content, description);
        ui_widget_append(field, item);
        ui_widget_append(field, content);
        ui_widget_append(group, field);
}

void radio_group_description_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_radio_group();

        ui_widget_add_class(group, "radio-group-demo");
        ui_widget_set_attr(group, "value", "comfortable");
        add_row(group, "desc-r1", "default", "Default",
                "Standard spacing for most use cases.", 0);
        add_row(group, "desc-r2", "comfortable", "Comfortable",
                "More space between elements.", 1);
        add_row(group, "desc-r3", "compact", "Compact",
                "Minimal spacing for dense layouts.", 0);
        ui_widget_append(parent, group);
        ui_radio_group_update(group);
}
