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

void accordion_card_init(ui_widget_t *parent)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *card_content = ui_create_card_content();
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(card, "accordion-card-demo");
        ui_text_set_content(title, "Subscription & Billing");
        ui_text_set_content(description,
                            "Common questions about your account, plans, "
                            "payments and cancellations.");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(card, header);
        ui_widget_append(card, card_content);

        ui_widget_set_attr(accordion, "default-value", "plans");
        add_item(accordion, "plans", "What subscription plans do you offer?",
                 "We offer three subscription tiers: Starter ($9/month), "
                 "Professional ($29/month), and Enterprise ($99/month). Each "
                 "plan includes increasing storage limits, API access, "
                 "priority support, and team collaboration features.");
        add_item(accordion, "billing", "How does billing work?",
                 "Billing occurs automatically at the start of each billing "
                 "cycle. We accept all major credit cards, PayPal, and ACH "
                 "transfers for enterprise customers. You'll receive an "
                 "invoice via email after each payment.");
        add_item(accordion, "cancel", "How do I cancel my subscription?",
                 "You can cancel your subscription anytime from your account "
                 "settings. There are no cancellation fees or penalties. "
                 "Your access will continue until the end of your current "
                 "billing period.");
        ui_widget_append(card_content, accordion);
        ui_widget_append(parent, card);
}
