#include <LCUI.h>

static void add_popover(ui_widget_t *parent, ui_popover_align_t align,
                        const char *label)
{
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *content;

        ui_popover_build_children(popover);
        ui_popover_set_trigger_text(popover, label);
        ui_popover_set_title(popover, label);
        ui_popover_set_description(popover,
                                   "This popover uses a different alignment.");
        content = ui_popover_get_content(popover);
        ui_popover_content_set_align(content, align);
        ui_widget_append(parent, popover);
}

void popover_align_init(ui_widget_t *parent)
{
        ui_widget_t *row = ui_create_widget(NULL);

        ui_widget_add_class(row, "popover-align-demo");
        add_popover(row, UI_ALIGN_START, "Start");
        add_popover(row, UI_ALIGN_CENTER, "Center");
        add_popover(row, UI_ALIGN_END, "End");
        ui_widget_append(parent, row);
}
