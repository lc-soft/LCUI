#include <LCUI.h>

void button_group_nested_init(ui_widget_t *parent)
{
        ui_widget_t *outer = ui_create_button_group();
        ui_widget_t *inner;
        ui_widget_t *button;
        ui_widget_t *input_group;
        ui_widget_t *input;
        ui_widget_t *addon;
        ui_widget_t *voice;

        ui_widget_add_class(outer, "button-group-nested-demo");
        ui_widget_add_class(outer, "clip-safe");
        inner = ui_create_button_group();
        button = ui_create_button();
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(button, ui_create_fluent_icon("add"));
        ui_widget_append(inner, button);
        ui_widget_append(outer, inner);

        inner = ui_create_button_group();
        ui_widget_add_class(inner, "button-group-grow");
        ui_widget_add_class(inner, "clip-safe");
        input_group = ui_create_input_group();
        input = ui_create_input_group_input();
        addon = ui_create_input_group_addon();
        ui_textinput_set_placeholder(input, "Send a message...");
        ui_input_group_addon_set_align(addon, UI_INPUT_GROUP_ADDON_INLINE_END);
        voice = ui_create_button();
        ui_button_set_variant(voice, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(voice, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_append(voice, ui_create_fluent_icon("device-eq"));
        ui_tooltip_attach(voice, "Voice Mode");
        ui_widget_append(addon, voice);
        ui_widget_append(input_group, input);
        ui_widget_append(input_group, addon);
        ui_widget_append(inner, input_group);
        ui_widget_append(outer, inner);
        ui_widget_append(parent, outer);
}
