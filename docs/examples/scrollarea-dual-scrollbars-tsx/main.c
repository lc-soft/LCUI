#include <LCUI.h>
#include <LCUI/widgets.h>
#include "example.h"

int main(void)
{
        lcui_init();
        example_load();
        return lcui_main();
}
