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

void accordion_multiple_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(accordion, "accordion-multiple-demo");
        ui_accordion_set_multiple(accordion, true);
        ui_widget_set_attr(accordion, "default-value", "notifications");
        add_item(accordion, "notifications", "Notification Settings",
                 "Manage how you receive notifications. You can enable email "
                 "alerts for updates or push notifications for mobile "
                 "devices.");
        add_item(accordion, "privacy", "Privacy & Security",
                 "Control your privacy settings and security preferences. "
                 "Enable two-factor authentication, manage connected "
                 "devices, review active sessions, and configure data "
                 "sharing preferences. You can also download your data or "
                 "delete your account.");
        add_item(accordion, "billing", "Billing & Subscription",
                 "View your current plan, payment history, and upcoming "
                 "invoices. Update your payment method, change your "
                 "subscription tier, or cancel your subscription.");
        ui_widget_append(parent, accordion);
}
