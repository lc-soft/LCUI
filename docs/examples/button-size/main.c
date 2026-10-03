#include <LCUI.h>

static ui_widget_t *create_size_button(const char *text, ui_button_size_t size)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_size(button, size);
        ui_button_set_text(button, text);
        return button;
}

static ui_widget_t *create_icon_size_button(ui_button_size_t size)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_button_set_size(button, size);
        ui_icon_set_name(icon, "arrow-up-right");
        ui_widget_append(button, icon);
        return button;
}

static ui_widget_t *create_size_pair(const char *text, ui_button_size_t size,
                                     ui_button_size_t icon_size)
{
        ui_widget_t *group = ui_create_widget(NULL);

        ui_widget_add_class(group, "button-size-group");
        ui_widget_append(group, create_size_button(text, size));
        ui_widget_append(group, create_icon_size_button(icon_size));
        return group;
}

void button_size_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);

        ui_widget_add_class(box, "button-size-demo");
        ui_widget_append(box, create_size_pair("Extra Small", UI_BUTTON_SIZE_XS,
                                               UI_BUTTON_SIZE_ICON_XS));
        ui_widget_append(box, create_size_pair("Small", UI_BUTTON_SIZE_SM,
                                               UI_BUTTON_SIZE_ICON_SM));
        ui_widget_append(box,
                         create_size_pair("Default", UI_BUTTON_SIZE_DEFAULT,
                                          UI_BUTTON_SIZE_ICON));
        ui_widget_append(box, create_size_pair("Large", UI_BUTTON_SIZE_LG,
                                               UI_BUTTON_SIZE_ICON_LG));
        ui_widget_append(parent, box);
}
