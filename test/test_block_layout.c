#include "test.h"
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

int test_block_layout(void)
{
	LCUI_Widget root;
	LCUI_Widget wrapper;

	LCUI_Init();
	wrapper = LCUIBuilder_LoadFile("test_block_layout.xml");
	root = LCUIWidget_GetRoot();
	Widget_Append(root, wrapper);
	Widget_Unwrap(wrapper);
	return 0;
}

#ifdef PREVIEW_MODE

int main(int argc, char *argv)
{
	test_block_layout();
	return LCUI_Main();
}

#endif
