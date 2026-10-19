#include <LCUI.h>

static void add_tooltip(ui_widget_t *parent, ui_tooltip_side_t side,
                        const char *label)
{
        ui_widget_t *button = ui_create_button();
        ui_tooltip_options_t options;

        ui_tooltip_options_init(&options);
        options.content = "Add to library";
        options.side = side;
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, label);
        ui_tooltip_attach_ex(button, &options);
        ui_widget_append(parent, button);
}

void tooltip_sides_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);

        ui_widget_add_class(row, "tooltip-sides-demo");
        add_tooltip(row, UI_SIDE_LEFT, "Left");
        add_tooltip(row, UI_SIDE_TOP, "Top");
        add_tooltip(row, UI_SIDE_BOTTOM, "Bottom");
        add_tooltip(row, UI_SIDE_RIGHT, "Right");
        ui_widget_append(parent, row);
}
