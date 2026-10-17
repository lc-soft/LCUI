/*
 * docs/examples/field-basic/main.c: -- Field widget demo (C variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

static const char *month_items[12] = { "01", "02", "03", "04", "05", "06",
                                       "07", "08", "09", "10", "11", "12" };
static const char *year_items[6] = { "2024", "2025", "2026",
                                     "2027", "2028", "2029" };

/* Build a select and point the field label at its real trigger button. */
static ui_widget_t *create_select(const char *id, const char *placeholder,
                                  const char *const *items, int count)
{
        ui_widget_t *select = ui_create_select();
        ui_widget_t *button;
        int i;

        ui_select_set_placeholder(select, placeholder);
        for (i = 0; i < count; ++i) {
                ui_select_add_item(select, items[i], items[i]);
        }
        button = ui_widget_get_child(ui_widget_get_child(select, 0), 0);
        ui_widget_set_id(button, id);
        return select;
}

void field_basic_init(ui_widget_t *parent)
{
        ui_widget_t *outer;
        ui_widget_t *set1, *legend1, *desc1, *group1;
        ui_widget_t *name_field, *name_lbl, *name_input;
        ui_widget_t *card_field, *card_lbl, *card_input, *card_desc;
        ui_widget_t *card_row;
        ui_widget_t *month_field, *month_lbl, *month_select;
        ui_widget_t *year_field, *year_lbl, *year_select;
        ui_widget_t *cvv_field, *cvv_lbl, *cvv_input;
        ui_widget_t *sep;
        ui_widget_t *set2, *legend2, *desc2, *group2;
        ui_widget_t *same_field, *same_cb, *same_lbl;
        ui_widget_t *actions, *submit, *cancel;

        outer = ui_create_widget(NULL);
        set1 = ui_create_field_set();
        legend1 = ui_create_field_legend();
        desc1 = ui_create_field_description();
        group1 = ui_create_field_group();
        name_field = ui_create_field();
        name_lbl = ui_create_field_label();
        name_input = ui_create_widget("textinput");
        card_field = ui_create_field();
        card_lbl = ui_create_field_label();
        card_input = ui_create_widget("textinput");
        card_desc = ui_create_field_description();
        card_row = ui_create_widget(NULL);
        month_field = ui_create_field();
        month_lbl = ui_create_field_label();
        year_field = ui_create_field();
        year_lbl = ui_create_field_label();
        cvv_field = ui_create_field();
        cvv_lbl = ui_create_field_label();
        cvv_input = ui_create_widget("textinput");
        sep = ui_create_field_separator();
        set2 = ui_create_field_set();
        legend2 = ui_create_field_legend();
        desc2 = ui_create_field_description();
        group2 = ui_create_field_group();
        same_field = ui_create_field();
        same_cb = ui_create_checkbox();
        same_lbl = ui_create_field_label();
        ui_widget_add_class(same_lbl, "font-normal");
        actions = ui_create_field();
        submit = ui_create_widget("button");
        cancel = ui_create_widget("button");

        ui_widget_add_class(outer, "field-demo");
        ui_widget_add_class(outer, "field-basic-demo");

        ui_text_set_content(legend1, "Payment Method");
        ui_widget_append(set1, legend1);
        ui_text_set_content(desc1, "All transactions are secure and encrypted");
        ui_widget_append(set1, desc1);
        ui_widget_append(set1, group1);

        ui_label_set_for(name_lbl, "name-on-card");
        ui_text_set_content(name_lbl, "Name on Card");
        ui_widget_append(name_field, name_lbl);
        ui_textinput_set_placeholder(name_input, "Evil Rabbit");
        ui_widget_set_id(name_input, "name-on-card");
        ui_widget_append(name_field, name_input);
        ui_widget_append(group1, name_field);

        ui_label_set_for(card_lbl, "card-number");
        ui_text_set_content(card_lbl, "Card Number");
        ui_widget_append(card_field, card_lbl);
        ui_textinput_set_placeholder(card_input, "1234 5678 9012 3456");
        ui_widget_set_id(card_input, "card-number");
        ui_widget_append(card_field, card_input);
        ui_text_set_content(card_desc, "Enter your 16-digit card number");
        ui_widget_append(card_field, card_desc);
        ui_widget_append(group1, card_field);

        ui_widget_add_class(card_row, "field-basic-card-row");
        ui_widget_add_class(month_field, "field-basic-card-field");
        ui_widget_add_class(year_field, "field-basic-card-field");
        ui_widget_add_class(cvv_field, "field-basic-card-field");

        ui_label_set_for(month_lbl, "card-month");
        ui_text_set_content(month_lbl, "Month");
        ui_widget_append(month_field, month_lbl);
        month_select = create_select("card-month", "MM", month_items, 12);
        ui_widget_append(month_field, month_select);
        ui_widget_append(card_row, month_field);

        ui_label_set_for(year_lbl, "card-year");
        ui_text_set_content(year_lbl, "Year");
        ui_widget_append(year_field, year_lbl);
        year_select = create_select("card-year", "YYYY", year_items, 6);
        ui_widget_append(year_field, year_select);
        ui_widget_append(card_row, year_field);

        ui_label_set_for(cvv_lbl, "card-cvv");
        ui_text_set_content(cvv_lbl, "CVV");
        ui_widget_append(cvv_field, cvv_lbl);
        ui_textinput_set_placeholder(cvv_input, "123");
        ui_widget_set_id(cvv_input, "card-cvv");
        ui_widget_append(cvv_field, cvv_input);
        ui_widget_append(card_row, cvv_field);

        ui_widget_append(group1, card_row);

        ui_widget_append(outer, set1);
        ui_widget_append(outer, sep);

        ui_text_set_content(legend2, "Billing Address");
        ui_widget_append(set2, legend2);
        ui_text_set_content(
            desc2, "The billing address associated with your payment method");
        ui_widget_append(set2, desc2);
        ui_widget_append(set2, group2);

        ui_field_set_orientation(same_field, "horizontal");
        ui_widget_set_id(same_cb, "billing-same");
        ui_widget_append(same_field, same_cb);
        ui_label_set_for(same_lbl, "billing-same");
        ui_text_set_content(same_lbl, "Same as shipping address");
        ui_widget_append(same_field, same_lbl);
        ui_widget_append(group2, same_field);

        ui_widget_append(outer, set2);

        ui_field_set_orientation(actions, "horizontal");
        ui_button_set_text(submit, "Submit");
        ui_button_set_variant(submit, UI_BUTTON_VARIANT_PRIMARY);
        ui_widget_append(actions, submit);
        ui_button_set_text(cancel, "Cancel");
        ui_widget_append(actions, cancel);
        ui_widget_append(outer, actions);

        ui_widget_append(parent, outer);
}
