#include <LCUI.h>
#include <LCUI/widgets.h>

/* example.tsx is compiled to example.h by @lcui/cli */
#include "example.h"

int main(void)
{
        lcui_init();
        example_load();
        return lcui_main();
}
