#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *add_item(ui_widget_t *accordion, const char *value,
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

void accordion_basic_demo_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(accordion, "accordion-basic-demo");
        ui_widget_set_attr(accordion, "default-value", "item-1");
        add_item(accordion, "item-1", "How do I reset my password?",
                 "Click on 'Forgot Password' on the login page, enter your "
                 "email address, and we'll send you a link to reset your "
                 "password. The link will expire in 24 hours.");
        add_item(accordion, "item-2", "Can I change my subscription plan?",
                 "Yes, you can upgrade or downgrade your plan at any time "
                 "from your account settings. Changes will be reflected in "
                 "your next billing cycle.");
        add_item(accordion, "item-3", "What payment methods do you accept?",
                 "We accept all major credit cards, PayPal, and bank "
                 "transfers. All payments are processed securely through our "
                 "payment partners.");
        ui_widget_append(parent, accordion);
}
