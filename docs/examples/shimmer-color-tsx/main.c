#include <LCUI.h>

/* example.tsx is compiled to example.h by @lcui/cli */
#include "example.h"

int main(void)
{
        lcui_init();
        ui_root_append(ui_create_shimmer_color_demo());
        return lcui_main();
}
