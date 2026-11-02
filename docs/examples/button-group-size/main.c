#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *create_text_button(const char *label, ui_button_size_t size)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, size);
        ui_widget_append(button, create_text(label));
        return button;
}

static ui_widget_t *create_icon_button(ui_button_size_t size)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, size);
        ui_widget_append(button, ui_create_fluent_icon("add"));
        return button;
}

static void add_size_group(ui_widget_t *parent, ui_button_size_t text_size,
                           ui_button_size_t icon_size, const char *first)
{
        ui_widget_t *group = ui_create_button_group();

        ui_widget_append(group, create_text_button(first, text_size));
        ui_widget_append(group, create_text_button("Button", text_size));
        ui_widget_append(group, create_text_button("Group", text_size));
        ui_widget_append(group, create_icon_button(icon_size));
        ui_widget_append(parent, group);
}

void button_group_size_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "button-group-size-demo");
        add_size_group(box, UI_BUTTON_SIZE_SM, UI_BUTTON_SIZE_ICON_SM, "Small");
        add_size_group(box, UI_BUTTON_SIZE_DEFAULT, UI_BUTTON_SIZE_ICON,
                       "Default");
        add_size_group(box, UI_BUTTON_SIZE_LG, UI_BUTTON_SIZE_ICON_LG, "Large");
        ui_widget_append(parent, box);
}
