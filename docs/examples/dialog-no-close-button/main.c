#include <LCUI.h>

void dialog_no_close_button_init(ui_widget_t *parent)
{
        ui_widget_t *dialog = ui_create_dialog();

        /* Hide the default top-right close control before building. */
        ui_dialog_show_close_button(dialog, false);
        ui_dialog_build_children(dialog);
        ui_dialog_set_trigger_text(dialog, "No Close Button");
        ui_dialog_set_title(dialog, "No Close Button");
        ui_dialog_set_description(
            dialog,
            "This dialog doesn't have a close button in the top-right corner.");
        ui_widget_remove(ui_dialog_get_footer_button(dialog, false)->parent);
        ui_widget_append(parent, dialog);
}
