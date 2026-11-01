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
        ui_widget_add_class(item, "accordion-borders-item");
        ui_widget_append(trigger, create_text(title));
        ui_widget_append(content, create_text(body));
        ui_widget_append(item, trigger);
        ui_widget_append(item, content);
        ui_widget_append(accordion, item);
        return item;
}

void accordion_borders_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(accordion, "accordion-borders-demo");
        ui_widget_set_attr(accordion, "default-value", "billing");
        add_item(accordion, "billing", "How does billing work?",
                 "We offer monthly and annual subscription plans. Billing is "
                 "charged at the beginning of each cycle, and you can cancel "
                 "anytime. All plans include automatic backups, 24/7 "
                 "support, and unlimited team members.");
        add_item(accordion, "security", "Is my data secure?",
                 "Yes. We use end-to-end encryption, SOC 2 Type II "
                 "compliance, and regular third-party security audits. All "
                 "data is encrypted at rest and in transit using "
                 "industry-standard protocols.");
        add_item(accordion, "integration", "What integrations do you support?",
                 "We integrate with 500+ popular tools including Slack, "
                 "Zapier, Salesforce, HubSpot, and more. You can also build "
                 "custom integrations using our REST API and webhooks.");
        ui_widget_append(parent, accordion);
}
