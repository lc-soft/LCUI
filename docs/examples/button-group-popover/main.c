#include <LCUI.h>

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

void button_group_popover_init(ui_widget_t *parent)
{
        ui_widget_t *group = ui_create_button_group();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *trigger = ui_create_popover_trigger();
        ui_widget_t *trigger_button = ui_create_button();
        ui_widget_t *content = ui_create_popover_content();
        ui_widget_t *header = ui_create_popover_header();
        ui_widget_t *title = ui_create_popover_title();
        ui_widget_t *description = ui_create_popover_description();
        ui_widget_t *field = ui_create_field();
        ui_widget_t *label = ui_create_field_label();
        ui_widget_t *input;
        ui_widget_t *field_description = ui_create_field_description();

        ui_widget_add_class(group, "button-group-popover-demo");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_widget_append(button, ui_create_fluent_icon("bot"));
        ui_widget_append(button, create_text("Copilot"));

        ui_button_set_variant(trigger_button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(trigger_button, UI_BUTTON_SIZE_ICON);
        ui_widget_append(trigger_button, ui_create_fluent_icon("chevron-down"));
        ui_widget_append(trigger, trigger_button);
        ui_widget_append(popover, trigger);

        ui_text_set_content(title, "Start a new task with Copilot");
        ui_text_set_content(description,
                            "Describe your task in natural language.");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(content, header);

        input = ui_create_widget("textinput");
        ui_textinput_enable_multiline(input, true);
        ui_textinput_set_placeholder(input, "I need to...");
        ui_widget_add_class(input, "popover-task-input");
        ui_widget_add_class(label, "sr-only");
        ui_text_set_content(label, "Task Description");
        ui_label_set_for(label, "task");
        ui_widget_set_id(input, "task");
        ui_text_set_content(field_description,
                            "Copilot will open a pull request for review.");
        ui_widget_append(field, label);
        ui_widget_append(field, input);
        ui_widget_append(field, field_description);
        ui_widget_append(content, field);
        ui_widget_append(popover, content);
        ui_popover_build_children(popover);
        ui_popover_content_set_align(content, UI_ALIGN_END);

        ui_widget_append(group, button);
        ui_widget_append(group, popover);
        ui_widget_append(parent, group);
}
