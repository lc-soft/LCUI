#include <LCUI.h>

void card_image_init(ui_widget_t *parent)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *cover = ui_create_widget(NULL);
        ui_widget_t *overlay = ui_create_widget(NULL);
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *header_action = ui_create_card_action();
        ui_widget_t *badge = ui_create_badge();
        ui_widget_t *footer = ui_create_card_footer();
        ui_widget_t *button = ui_create_button();

        ui_widget_add_class(card, "card-image-demo");
        ui_widget_add_class(cover, "card-image-cover");
        ui_widget_add_class(overlay, "card-image-overlay");
        ui_widget_append(cover, overlay);
        ui_text_set_content(title, "Design systems meetup");
        ui_text_set_content(
            description,
            "A practical talk on component APIs, accessibility, and shipping "
            "faster.");
        ui_badge_set_variant(badge, UI_BADGE_VARIANT_SECONDARY);
        ui_text_set_content(badge, "Featured");
        ui_widget_append(header_action, badge);
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(header, header_action);
        ui_widget_add_class(footer, "card-footer-stack");
        ui_button_set_text(button, "View Event");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);
        ui_widget_append(footer, button);
        ui_widget_append(card, cover);
        ui_widget_append(card, header);
        ui_widget_append(card, footer);
        ui_widget_append(parent, card);
}
