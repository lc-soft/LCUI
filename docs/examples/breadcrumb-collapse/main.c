#include <LCUI.h>

void breadcrumb_collapse_init(ui_widget_t *parent)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();

        ui_widget_add_class(breadcrumb, "breadcrumb-collapse-demo");
        ui_breadcrumb_build_from_path(breadcrumb,
                                      "/workspace/projects/lcui/components/"
                                      "breadcrumb/navigation",
                                      NULL, NULL);
        ui_widget_append(parent, breadcrumb);
}
