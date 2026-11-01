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

void accordion_disabled_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *item;

        ui_widget_add_class(accordion, "accordion-disabled-demo");
        add_item(accordion, "item-1", "Can I access my account history?",
                 "Yes, you can view your complete account history including "
                 "all transactions, plan changes, and support tickets in the "
                 "Account History section of your dashboard.");
        item = add_item(accordion, "item-2", "Premium feature information",
                        "This section contains information about premium "
                        "features. Upgrade your plan to access this content.");
        ui_widget_set_disabled(item, true);
        add_item(accordion, "item-3", "How do I update my email address?",
                 "You can update your email address in your account "
                 "settings. You'll receive a verification email at your new "
                 "address to confirm the change.");
        ui_widget_append(parent, accordion);
}
