#include <LCUI.h>

void progress_label_init(ui_widget_t *parent)
{
        ui_widget_t *progress = ui_create_progress();
        ui_widget_t *label = ui_create_progress_label();
        ui_widget_t *value = ui_create_progress_value();

        ui_widget_add_class(progress, "progress-label-demo");
        ui_progress_set_value(progress, 56.0f);
        ui_text_set_content(label, "Upload progress");
        ui_widget_append(progress, label);
        ui_widget_append(progress, value);
        ui_widget_append(parent, progress);
}
