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

void accordion_demo_init(ui_widget_t *parent)
{
        ui_widget_t *accordion = ui_create_accordion();

        ui_widget_add_class(accordion, "accordion-demo");
        ui_widget_set_attr(accordion, "default-value", "shipping");
        add_item(accordion, "shipping", "What are your shipping options?",
                 "We offer standard (5-7 days), express (2-3 days), and "
                 "overnight shipping. Free shipping on international orders.");
        add_item(accordion, "returns", "What is your return policy?",
                 "Returns accepted within 30 days. Items must be unused and "
                 "in original packaging. Refunds processed within 5-7 "
                 "business days.");
        add_item(accordion, "support", "How can I contact customer support?",
                 "Reach us via email, live chat, or phone. We respond within "
                 "24 hours during business days.");
        ui_widget_append(parent, accordion);
}
