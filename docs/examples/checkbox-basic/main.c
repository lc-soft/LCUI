#include <LCUI.h>

void checkbox_basic_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_field_group();
        ui_widget_t *card;
        ui_widget_t *fields[4];
        ui_widget_t *boxes[4];
        ui_widget_t *contents[2];
        ui_widget_t *labels[3];
        const char *ids[4] = { "terms-checkbox", "terms-checkbox-2",
                               "toggle-checkbox", "toggle-checkbox-2" };
        int i;

        ui_widget_add_class(group, "checkbox-basic-demo");
        for (i = 0; i < 4; ++i) {
                fields[i] = ui_create_field();
                ui_field_set_orientation(fields[i], "horizontal");
                boxes[i] = ui_create_checkbox();
                ui_widget_set_id(boxes[i], ids[i]);
                ui_widget_append(fields[i], boxes[i]);
        }
        ui_widget_set_attr(boxes[1], "checked", "true");
        ui_widget_set_disabled(boxes[2], true);
        ui_widget_set_disabled(fields[2], true);

        labels[0] = ui_create_label();
        ui_label_set_for(labels[0], ids[0]);
        ui_text_set_content(labels[0], "Accept terms and conditions");
        ui_widget_append(fields[0], labels[0]);
        ui_widget_append(group, fields[0]);

        contents[0] = ui_create_field_content();
        labels[1] = ui_create_field_label();
        ui_label_set_for(labels[1], ids[1]);
        ui_text_set_content(labels[1], "Accept terms and conditions");
        ui_widget_append(contents[0], labels[1]);
        ui_widget_append(contents[0], ui_create_field_description());
        ui_text_set_content(
            ui_widget_get_child(contents[0], 1),
            "By clicking this checkbox, you agree to the terms.");
        ui_widget_append(fields[1], contents[0]);
        ui_widget_append(group, fields[1]);

        labels[2] = ui_create_field_label();
        ui_label_set_for(labels[2], ids[2]);
        ui_text_set_content(labels[2], "Enable notifications");
        ui_widget_append(fields[2], labels[2]);
        ui_widget_append(group, fields[2]);

        contents[1] = ui_create_field_content();
        ui_widget_append(contents[1], ui_create_field_title());
        ui_text_set_content(ui_widget_get_child(contents[1], 0),
                            "Enable notifications");
        ui_widget_append(contents[1], ui_create_field_description());
        ui_text_set_content(
            ui_widget_get_child(contents[1], 1),
            "You can enable and disable notifications at any time.");
        ui_widget_append(fields[3], contents[1]);

        /* The whole card is clickable: FieldLabel wraps the Field and targets
         * the checkbox through `for`. */
        card = ui_create_field_label();
        ui_label_set_for(card, ids[3]);
        ui_widget_append(card, fields[3]);
        ui_widget_append(group, card);

        ui_widget_append(parent, group);
}
