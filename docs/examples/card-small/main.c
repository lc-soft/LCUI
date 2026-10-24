#include <LCUI.h>

static ui_widget_t *create_feature(const char *content)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *text = ui_create_widget("text");

        ui_widget_add_class(row, "card-small-row");
        ui_icon_set_name(icon, "chevron-right");
        ui_icon_set_size(icon, 16);
        ui_widget_add_class(icon, "card-small-chevron");
        ui_text_set_content(text, content);
        ui_widget_append(row, icon);
        ui_widget_append(row, text);
        return row;
}

void card_small_init(ui_widget_t *parent)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *list = ui_create_widget(NULL);
        ui_widget_t *footer = ui_create_card_footer();
        ui_widget_t *primary = ui_create_button();
        ui_widget_t *secondary = ui_create_button();

        ui_widget_add_class(card, "card-small-demo");
        ui_card_set_size(card, UI_CARD_SIZE_SM);
        ui_text_set_content(title, "Scheduled reports");
        ui_text_set_content(description,
                            "Weekly snapshots. No more manual exports.");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_add_class(list, "card-small-list");
        ui_widget_append(list, create_feature("Choose a schedule (daily, or "
                                              "weekly)."));
        ui_widget_append(
            list, create_feature("Send to channels or specific teammates."));
        ui_widget_append(list, create_feature("Include charts, tables, and key "
                                              "metrics."));
        ui_widget_append(content, list);
        ui_widget_add_class(footer, "card-footer-stack");
        ui_button_set_text(primary, "Set up scheduled reports");
        ui_button_set_variant(primary, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_size(primary, UI_BUTTON_SIZE_SM);
        ui_button_set_text(secondary, "See what's new");
        ui_button_set_variant(secondary, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(secondary, UI_BUTTON_SIZE_SM);
        ui_widget_append(footer, primary);
        ui_widget_append(footer, secondary);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(card, footer);
        ui_widget_append(parent, card);
}
