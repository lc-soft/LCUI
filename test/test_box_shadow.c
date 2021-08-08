#include <LCUI.h>
#include <LCUI/gui/builder.h>
#include <LCUI/main.h>

int main(void)
{
	int ret = 0;
	ui_widget_t *box;

	lcui_init();
	box = LCUIBuilder_LoadFile("test_box_shadow.xml");
	if (!box) {
		lcui_destroy();
		return ret;
	}
	ui_root_append(box);
	ui_widget_unwrap(box);
	return lcui_main();
}
