#include <LCUI.h>

static ui_widget_t *make_field(const char *id, const char *label_text,
                               const char *value)
{
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *input = ui_create_widget("textinput");

        ui_field_set_orientation(field, "horizontal");
        ui_label_set_for(label, id);
        ui_widget_set_id(input, id);
        ui_text_set_content(label, label_text);
        ui_textinput_set_text(input, value);
        ui_widget_add_class(input, "popover-form-input");
        ui_widget_append(field, label);
        ui_widget_append(field, input);
        return field;
}

void popover_basic_init(ui_widget_t *parent)
{
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *content;
        ui_widget_t *form;

        ui_popover_build_children(popover);
        ui_popover_set_trigger_text(popover, "Open popover");
        ui_popover_set_title(popover, "Dimensions");
        ui_popover_set_description(popover,
                                   "Set the dimensions for the layer.");
        content = ui_popover_get_content(popover);
        ui_widget_add_class(content, "w-80");
        form = ui_create_widget(NULL);
        ui_widget_add_class(form, "popover-form-grid");
        ui_widget_add_class(form, "clip-safe");
        ui_widget_append(form, make_field("popover-width", "Width", "100%"));
        ui_widget_append(
            form, make_field("popover-max-width", "Max. width", "300px"));
        ui_widget_append(form, make_field("popover-height", "Height", "25px"));
        ui_widget_append(
            form, make_field("popover-max-height", "Max. height", "none"));
        ui_widget_append(content, form);
        ui_widget_append(parent, popover);
}
