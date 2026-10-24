#include <LCUI.h>

static ui_widget_t *create_input(const char *id, const char *placeholder,
                                 bool password)
{
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_set_id(input, id);
        ui_textinput_set_placeholder(input, placeholder);
        if (password) {
                ui_textinput_set_password_char(input, L'*');
        }
        return input;
}

void card_basic_init(ui_widget_t *parent)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *header_action = ui_create_card_action();
        ui_widget_t *sign_up = ui_create_button();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *form = ui_create_widget(NULL);
        ui_widget_t *email_field = ui_create_field();
        ui_widget_t *email_label = ui_create_field_label();
        ui_widget_t *email_input =
            create_input("card-email", "m@example.com", false);
        ui_widget_t *password_field = ui_create_field();
        ui_widget_t *password_row = ui_create_widget(NULL);
        ui_widget_t *password_label = ui_create_field_label();
        ui_widget_t *forgot = ui_create_button();
        ui_widget_t *password_input = create_input("card-password", "", true);
        ui_widget_t *footer = ui_create_card_footer();
        ui_widget_t *login = ui_create_button();
        ui_widget_t *google = ui_create_button();

        ui_widget_add_class(card, "card-basic-demo");
        ui_text_set_content(title, "Login to your account");
        ui_text_set_content(description,
                            "Enter your email below to login to your account");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_button_set_text(sign_up, "Sign Up");
        ui_button_set_variant(sign_up, UI_BUTTON_VARIANT_LINK);
        ui_widget_append(header_action, sign_up);
        ui_widget_append(header, header_action);
        ui_widget_add_class(content, "clip-safe");

        ui_widget_add_class(form, "card-form");
        ui_widget_add_class(form, "clip-safe");
        ui_widget_add_class(email_field, "card-field");
        ui_field_set_orientation(email_field, "vertical");
        ui_label_set_for(email_label, "card-email");
        ui_text_set_content(email_label, "Email");
        ui_widget_append(email_field, email_label);
        ui_widget_append(email_field, email_input);
        ui_widget_append(form, email_field);

        ui_widget_add_class(password_field, "card-field");
        ui_field_set_orientation(password_field, "vertical");
        ui_widget_add_class(password_row, "card-label-row");
        ui_label_set_for(password_label, "card-password");
        ui_text_set_content(password_label, "Password");
        ui_button_set_text(forgot, "Forgot your password?");
        ui_button_set_variant(forgot, UI_BUTTON_VARIANT_LINK);
        ui_button_set_size(forgot, UI_BUTTON_SIZE_SM);
        ui_widget_add_class(forgot, "card-forgot");
        ui_widget_append(password_row, password_label);
        ui_widget_append(password_row, forgot);
        ui_widget_append(password_field, password_row);
        ui_widget_append(password_field, password_input);
        ui_widget_append(form, password_field);
        ui_widget_append(content, form);

        ui_widget_add_class(footer, "card-footer-stack");
        ui_button_set_text(login, "Login");
        ui_button_set_variant(login, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_text(google, "Login with Google");
        ui_button_set_variant(google, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_append(footer, login);
        ui_widget_append(footer, google);

        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(card, footer);
        ui_widget_append(parent, card);
}
