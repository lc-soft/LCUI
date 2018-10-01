#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include "test.h"

static int check_widget_attribute(void)
{
	int ret = 0;
	LCUI_Widget w;

	w = LCUIWidget_GetById("test-attr-disabled-1");
	CHECK_WITH_TEXT("check element-1 is disabled", w->disabled);
	w = LCUIWidget_GetById("test-attr-disabled-2");
	CHECK_WITH_TEXT("check element-2 is disabled", w->disabled);
	w = LCUIWidget_GetById("test-attr-disabled-3");
	CHECK_WITH_TEXT("check element-3 is disabled", w->disabled);
	w = LCUIWidget_GetById("test-attr-disabled-4");
	CHECK_WITH_TEXT("check element-4 is normal", !w->disabled);

	return ret;
}

static int check_widget_loaded_from_nested_xml(void)
{
	int ret = 0;
	LCUI_Widget w;

	w = LCUIWidget_GetById("test-nested-1");
	CHECK_WITH_TEXT("check test-nested-1 should exist", w);
	w = LCUIWidget_GetById("test-nested-2");
	CHECK_WITH_TEXT("check test-nested-2 should exist", w);
	w = LCUIWidget_GetById("test-nested-3");
	CHECK_WITH_TEXT("check test-nested-3 should exist", w);
	w = LCUIWidget_GetById("test-nested-4");
	CHECK_WITH_TEXT("check test-nested-4 should exist", w);

	return ret;
}

int test_xml_parser(void)
{
	int ret = 0;
	LCUI_Widget root, pack;

	LCUI_Init();
	TEST_LOG("test widget layout\n");
	LCUIDisplay_SetSize(960, 680);
	root = LCUIWidget_GetRoot();
	CHECK(pack = LCUIBuilder_LoadFile("test_xml_parser.xml"));
	if (!pack) {
		LCUI_Destroy();
		return ret;
	}
	Widget_UpdateStyle(root, TRUE);
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	LCUIWidget_Update();
	ret += check_widget_attribute();
	ret += check_widget_loaded_from_nested_xml();
	LCUI_Destroy();
	return ret;
}
