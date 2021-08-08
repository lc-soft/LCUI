#include <LCUI.h>
#include <LCUI/ui.h>
#include <LCUI/gui/builder.h>
#include "ctest.h"

static void check_widget_attribute(void)
{
	ui_widget_t* w;

	w = ui_get_widget("test-attr-disabled-1");
	it_b("check element-1 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-2");
	it_b("check element-2 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-3");
	it_b("check element-3 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-4");
	it_b("check element-4 is normal", w->disabled, FALSE);
}

static void check_widget_loaded_from_nested_xml(void)
{
	ui_widget_t* w;

	w = ui_get_widget("test-nested-1");
	it_b("check test-nested-1 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-2");
	it_b("check test-nested-2 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-3");
	it_b("check test-nested-3 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-4");
	it_b("check test-nested-4 should exist", w != NULL, TRUE);
}

void test_xml_parser(void)
{
	ui_widget_t *pack;

	lcui_init();
	it_b("load XML file",
	     (pack = LCUIBuilder_LoadFile("test_xml_parser.xml")) != NULL,
	     TRUE);
	if (!pack) {
		lcui_destroy();
		return;
	}
	ui_root_append(pack);
	ui_widget_unwrap(pack);
	ui_update();
	describe("check widget attribute", check_widget_attribute);
	describe("check widget loaded from nested xml",
		 check_widget_loaded_from_nested_xml);
	lcui_destroy();
}
