#include <LCUI.h>
#include <ui_xml.h>

int main(void)
{
        lcui_init();
        ui_load_xml_file("ui.xml");
        return lcui_main();
}
