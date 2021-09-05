#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/draw/border.h>
#include <LCUI/ui.h>
#include <LCUI/gui/builder.h>

int main(void)
{
	int ret = 0;
	ui_widget_t* root, box;

	LCUI_Init();
	box = LCUIBuilder_LoadFile("test_border.xml");
	if (!box) {
		LCUI_Destroy();
		return ret;
	}
	root = ui_root();
	ui_widget_append(root, box);
	ui_widget_unwrap(box);
	return LCUI_Main();
}
