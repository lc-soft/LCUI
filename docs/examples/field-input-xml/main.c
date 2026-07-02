/*
 * docs/examples/field-input-xml/main.c: -- Field widget demo (XML variant)
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

int main(void)
{
        lcui_init();
        ui_load_xml_file("ui.xml");
        return lcui_main();
}
