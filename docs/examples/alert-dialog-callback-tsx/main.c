#include <LCUI.h>
#include "example.h"

void alert_dialog_callback_tsx_init(ui_widget_t *parent)
{
        ui_widget_append(parent, example_load());
}
