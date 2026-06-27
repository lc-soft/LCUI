#include <LCUI.h>
#include "example.h"

int main(void)
{
        lcui_init();
        ui_root_append(ui_create_progress_demo());
        return lcui_main();
}
