#include <LCUI.h>

static const char *collapsible_settings_css = "\
.collapsible-settings-demo {\
  width: 320px;\
}\
.collapsible-settings-demo card {\
  width: 100%;\
}\
.collapsible-settings-demo collapsible-trigger button {\
  width: 100%;\
  border: 0;\
  background: transparent;\
  justify-content: space-between;\
}\
.collapsible-settings-demo .settings-fields {\
  display: flex;\
  gap: 8px;\
  padding-top: 10px;\
}\
.collapsible-settings-demo .settings-field {\
  display: flex;\
  flex-direction: column;\
  gap: 4px;\
  width: 50%;\
}\
.collapsible-settings-demo .settings-field textinput {\
  width: 100%;\
}\
";

static ui_widget_t *create_radius_field(const char *id, const char *text)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_add_class(field, "settings-field");
        ui_field_set_orientation(field, "vertical");
        ui_text_set_content(label, text);
        ui_widget_set_id(input, id);
        ui_textinput_set_placeholder(input, "0");
        ui_widget_append(field, label);
        ui_widget_append(field, input);
        return field;
}

void collapsible_settings_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-down");
        ui_widget_t *details = ui_create_collapsible_content();
        ui_widget_t *fields = ui_create_widget(NULL);

        ui_load_css_string(collapsible_settings_css,
                           "collapsible-settings-demo");
        ui_widget_add_class(box, "collapsible-settings-demo");
        ui_card_set_size(card, UI_CARD_SIZE_SM);
        ui_text_set_content(title, "Radius");
        ui_text_set_content(description,
                            "Set the corner radius of the element.");

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_text_set_content(label, "Advanced settings");
        ui_widget_append(button, label);
        ui_widget_append(button, chevron);
        ui_widget_append(trigger, button);
        ui_widget_append(collapsible, trigger);

        ui_widget_add_class(fields, "settings-fields");
        ui_widget_append(fields, create_radius_field("radius-x", "Radius X"));
        ui_widget_append(fields, create_radius_field("radius-y", "Radius Y"));
        ui_widget_append(details, fields);
        ui_widget_append(collapsible, details);

        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(content, collapsible);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(box, card);
        ui_widget_append(parent, box);
}
