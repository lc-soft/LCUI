#include <LCUI.h>
#include <ctest-custom.h>

static void check_widget_attribute(void)
{
	ui_widget_t* w;

	w = ui_get_widget("test-attr-disabled-1");
	ctest_equal_bool("check element-1 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-2");
	ctest_equal_bool("check element-2 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-3");
	ctest_equal_bool("check element-3 is disabled", w->disabled, TRUE);
	w = ui_get_widget("test-attr-disabled-4");
	ctest_equal_bool("check element-4 is normal", w->disabled, FALSE);
}

static void check_widget_loaded_from_nested_xml(void)
{
	ui_widget_t* w;

	w = ui_get_widget("test-nested-1");
	ctest_equal_bool("check test-nested-1 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-2");
	ctest_equal_bool("check test-nested-2 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-3");
	ctest_equal_bool("check test-nested-3 should exist", w != NULL, TRUE);
	w = ui_get_widget("test-nested-4");
	ctest_equal_bool("check test-nested-4 should exist", w != NULL, TRUE);
}

void test_xml_parser(void)
{
	ui_widget_t *pack;

	ui_init();
	ctest_equal_bool("load XML file",
	     (pack = ui_load_xml_file("test_xml_parser.xml")) != NULL,
	     TRUE);
	if (!pack) {
		lcui_destroy();
		return;
	}
	ui_root_append(pack);
	ui_widget_unwrap(pack);
	ui_update();
	ctest_describe("check widget attribute", check_widget_attribute);
	ctest_describe("check widget loaded from nested xml",
		 check_widget_loaded_from_nested_xml);
	ui_destroy();
}
