#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *add_standard_item(ui_widget_t *accordion, const char *value,
                                      const char *title, const char *body)
{
        ui_widget_t *item = ui_create_accordion_item();
        ui_widget_t *trigger = ui_create_accordion_trigger();
        ui_widget_t *content = ui_create_accordion_content();

        ui_accordion_item_set_value(item, value);
        ui_widget_append(trigger, create_text(title));
        ui_widget_append(content, create_text(body));
        ui_widget_append(item, trigger);
        ui_widget_append(item, content);
        ui_widget_append(accordion, item);
        return item;
}

static ui_widget_t *add_custom_item(ui_widget_t *accordion, const char *value,
                                    const char *label, const char *body)
{
        ui_widget_t *item = ui_create_accordion_item();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_accordion_content();

        ui_accordion_item_set_value(item, value);
        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_button_set_text(button, label);
        ui_widget_set_attr(button, "data-accordion-trigger", "true");
        ui_widget_append(content, create_text(body));
        ui_widget_append(item, button);
        ui_widget_append(item, content);
        ui_widget_append(accordion, item);
        return item;
}

void accordion_custom_trigger_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(accordion, "accordion-custom-trigger-demo");
        ui_widget_set_attr(accordion, "default-value", "account");
        add_custom_item(accordion, "account", "Account settings",
                        "Update your profile, email address, and password "
                        "from the account settings panel.");
        add_standard_item(accordion, "billing", "Billing & invoices",
                          "View invoices, update your payment method, or "
                          "download receipts for your records.");
        ui_widget_append(parent, accordion);
}
