#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include "test.h"
#include "libtest.h"

static void check_widget_attribute(void)
{
	LCUI_Widget w;

	w = LCUIWidget_GetById("test-attr-disabled-1");
	it_b("check element-1 is disabled", w->disabled, TRUE);
	w = LCUIWidget_GetById("test-attr-disabled-2");
	it_b("check element-2 is disabled", w->disabled, TRUE);
	w = LCUIWidget_GetById("test-attr-disabled-3");
	it_b("check element-3 is disabled", w->disabled, TRUE);
	w = LCUIWidget_GetById("test-attr-disabled-4");
	it_b("check element-4 is normal", w->disabled, FALSE);
}

static void check_widget_loaded_from_nested_xml(void)
{
	LCUI_Widget w;

	w = LCUIWidget_GetById("test-nested-1");
	it_b("check test-nested-1 should exist", w != NULL, TRUE);
	w = LCUIWidget_GetById("test-nested-2");
	it_b("check test-nested-2 should exist", w != NULL, TRUE);
	w = LCUIWidget_GetById("test-nested-3");
	it_b("check test-nested-3 should exist", w != NULL, TRUE);
	w = LCUIWidget_GetById("test-nested-4");
	it_b("check test-nested-4 should exist", w != NULL, TRUE);
}

void test_xml_parser(void)
{
	LCUI_Widget root, pack;

	LCUI_Init();
	LCUIDisplay_SetSize(960, 680);
	root = LCUIWidget_GetRoot();
	it_b("load XML file",
	     (pack = LCUIBuilder_LoadFile("test_xml_parser.xml")) != NULL,
	     TRUE);
	if (!pack) {
		LCUI_Destroy();
		return;
	}
	Widget_UpdateStyle(root, TRUE);
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	LCUIWidget_Update();
	describe("check widget attribute", check_widget_attribute);
	describe("check widget loaded from nested xml",
		 check_widget_loaded_from_nested_xml);
	LCUI_Destroy();
}
