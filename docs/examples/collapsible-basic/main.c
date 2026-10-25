#include <LCUI.h>

static const char *collapsible_basic_css = "\
.collapsible-basic-demo {\
  width: 384px;\
}\
.collapsible-basic-demo card {\
  width: 100%;\
}\
.collapsible-basic-demo collapsible:open {\
  background: #f5f5f5;\
  border-radius: 8px;\
}\
.collapsible-basic-demo collapsible-trigger button {\
  width: 100%;\
  border: 0;\
  background: transparent;\
  justify-content: space-between;\
}\
.collapsible-basic-demo collapsible-content {\
  padding: 0 10px 10px 10px;\
}\
.collapsible-basic-demo .details-copy {\
  display: block;\
  margin-bottom: 10px;\
  font-size: 14px;\
  color: #666666;\
}\
";

void collapsible_basic_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *card = ui_create_card();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-down");
        ui_widget_t *details = ui_create_collapsible_content();
        ui_widget_t *copy = ui_create_widget("text");
        ui_widget_t *learn_more = ui_create_button();

        ui_load_css_string(collapsible_basic_css, "collapsible-basic-demo");
        ui_widget_add_class(box, "collapsible-basic-demo");

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_text_set_content(label, "Product details");
        ui_widget_append(button, label);
        ui_widget_append(button, chevron);
        ui_widget_append(trigger, button);
        ui_widget_append(collapsible, trigger);

        ui_widget_add_class(copy, "details-copy");
        ui_text_set_content(
            copy,
            "This panel can be expanded or collapsed to reveal additional "
            "content.");
        ui_button_set_text(learn_more, "Learn More");
        ui_button_set_variant(learn_more, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(learn_more, UI_BUTTON_SIZE_XS);
        ui_widget_append(details, copy);
        ui_widget_append(details, learn_more);
        ui_widget_append(collapsible, details);

        ui_widget_append(content, collapsible);
        ui_widget_append(card, content);
        ui_widget_append(box, card);
        ui_widget_append(parent, box);
}
