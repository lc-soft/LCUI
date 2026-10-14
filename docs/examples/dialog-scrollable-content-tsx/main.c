#include <LCUI.h>
#include "example.h"

void dialog_scrollable_content_tsx_init(ui_widget_t *parent)
{
        ui_widget_append(parent, example_load());
}
