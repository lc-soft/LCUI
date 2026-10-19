#include <LCUI.h>

void popover_simple_init(ui_widget_t *parent)
{
        ui_widget_t *popover = ui_create_popover();

        ui_popover_build_children(popover);
        ui_popover_set_title(popover, "Dimensions");
        ui_popover_set_description(popover,
                                   "Set the dimensions for the layer.");
        ui_widget_append(parent, popover);
}
