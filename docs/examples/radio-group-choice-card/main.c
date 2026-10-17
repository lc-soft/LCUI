#include <LCUI.h>

static void add_card(ui_widget_t *group, const char *id, const char *value,
                     const char *title, const char *desc, int checked)
{
        ui_widget_t *card = ui_create_field_label();
        ui_widget_t *row = ui_create_field();
        ui_widget_t *content = ui_create_field_content();
        ui_widget_t *title_w = ui_create_field_title();
        ui_widget_t *desc_w = ui_create_field_description();
        ui_widget_t *item = ui_create_radio_group_item();

        ui_label_set_for(card, id);
        ui_field_set_orientation(row, "horizontal");
        ui_text_set_content(title_w, title);
        ui_text_set_content(desc_w, desc);
        ui_widget_set_id(item, id);
        ui_widget_set_attr(item, "value", value);
        if (checked) {
                ui_widget_set_attr(item, "checked", "true");
        }
        ui_widget_append(content, title_w);
        ui_widget_append(content, desc_w);
        ui_widget_append(row, content);
        ui_widget_append(row, item);
        ui_widget_append(card, row);
        ui_widget_append(group, card);
}

void radio_group_choice_card_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_radio_group();

        ui_widget_add_class(group, "radio-group-demo");
        ui_widget_set_attr(group, "value", "plus");
        add_card(group, "plus-plan", "plus", "Plus",
                 "For individuals and small teams.", 1);
        add_card(group, "pro-plan", "pro", "Pro", "For growing businesses.", 0);
        add_card(group, "enterprise-plan", "enterprise", "Enterprise",
                 "For large teams and enterprises.", 0);
        ui_widget_append(parent, group);
        ui_radio_group_update(group);
}
