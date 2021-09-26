#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/draw/border.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int ret = 0;
	LCUI_Widget root, box;

	LCUI_Init();
	box = LCUIBuilder_LoadFile("test_border.xml");
	if (!box) {
		LCUI_Destroy();
		return ret;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append(root, box);
	Widget_Unwrap(box);
	return LCUI_Main();
}
