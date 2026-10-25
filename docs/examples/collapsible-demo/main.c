#include <LCUI.h>

static const char *collapsible_demo_css = "\
.collapsible-demo {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
  width: 350px;\
}\
.collapsible-demo .order-header {\
  display: flex;\
  align-items: center;\
  justify-content: flex-start;\
  width: 100%;\
  box-sizing: border-box;\
  gap: 16px;\
  padding: 0 16px;\
}\
.collapsible-demo collapsible-trigger {\
  display: inline-flex;\
  flex-shrink: 0;\
}\
.collapsible-demo .order-title {\
  display: block;\
  font-size: 14px;\
  font-weight: 600;\
}\
.collapsible-demo .order-row {\
  display: flex;\
  align-items: center;\
  justify-content: space-between;\
  box-sizing: border-box;\
  padding: 8px 16px;\
  border: 1px solid #e5e5e5;\
  border-radius: 6px;\
  font-size: 14px;\
}\
.collapsible-demo .order-box {\
  display: flex;\
  flex-direction: column;\
  box-sizing: border-box;\
  gap: 4px;\
  padding: 8px 16px;\
  border: 1px solid #e5e5e5;\
  border-radius: 6px;\
  font-size: 14px;\
}\
.collapsible-demo .order-label {\
  display: block;\
  font-weight: 600;\
}\
.collapsible-demo .order-copy {\
  display: block;\
  color: #666666;\
}\
";

void collapsible_demo_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *header = ui_create_widget(NULL);
        ui_widget_t *title = ui_create_widget("text");
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon("chevron-down");
        ui_widget_t *status = ui_create_widget(NULL);
        ui_widget_t *status_label = ui_create_widget("text");
        ui_widget_t *status_value = ui_create_widget("text");
        ui_widget_t *content = ui_create_collapsible_content();
        ui_widget_t *address = ui_create_widget(NULL);
        ui_widget_t *address_label = ui_create_widget("text");
        ui_widget_t *address_copy = ui_create_widget("text");
        ui_widget_t *items = ui_create_widget(NULL);
        ui_widget_t *items_label = ui_create_widget("text");
        ui_widget_t *items_copy = ui_create_widget("text");

        ui_load_css_string(collapsible_demo_css, "collapsible-demo");
        ui_widget_add_class(box, "collapsible-demo");
        ui_widget_add_class(header, "order-header");
        ui_widget_add_class(title, "order-title");
        ui_text_set_content(title, "Order #4189");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, icon);
        ui_widget_append(trigger, button);
        ui_widget_append(header, title);
        ui_widget_append(header, trigger);

        ui_widget_add_class(status, "order-row");
        ui_text_set_content(status_label, "Status");
        ui_text_set_content(status_value, "Shipped");
        ui_widget_add_class(status_label, "order-copy");
        ui_widget_append(status, status_label);
        ui_widget_append(status, status_value);

        ui_widget_add_class(address, "order-box");
        ui_text_set_content(address_label, "Shipping address");
        ui_widget_add_class(address_label, "order-label");
        ui_text_set_content(address_copy, "100 Market St, San Francisco");
        ui_widget_add_class(address_copy, "order-copy");
        ui_widget_append(address, address_label);
        ui_widget_append(address, address_copy);

        ui_widget_add_class(items, "order-box");
        ui_text_set_content(items_label, "Items");
        ui_widget_add_class(items_label, "order-label");
        ui_text_set_content(items_copy, "2x Studio Headphones");
        ui_widget_add_class(items_copy, "order-copy");
        ui_widget_append(items, items_label);
        ui_widget_append(items, items_copy);

        ui_widget_append(content, address);
        ui_widget_append(content, items);
        ui_widget_append(collapsible, header);
        ui_widget_append(collapsible, status);
        ui_widget_append(collapsible, content);
        ui_widget_append(box, collapsible);
        ui_widget_append(parent, box);
}
