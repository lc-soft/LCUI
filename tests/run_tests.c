#include <stdio.h>
#include <wchar.h>
#include <LCUI.h>
#include <platform/main.h>
#include <ctest-custom.h>
#include "run_tests.h"

int main(int argc, char **argv)
{
	logger_set_level(LOGGER_LEVEL_ERROR);
	ctest_describe("test settings", test_settings);
	ctest_describe("test thread", test_thread);
	ctest_describe("test font load", test_font_load);
	ctest_describe("test image reader", test_image_reader);
	ctest_describe("test xml parser", test_xml_parser);
	ctest_describe("test mainloop", test_mainloop);
	ctest_describe("test clipboard", test_clipboard);
	ctest_describe("test widget event", test_widget_event);
	ctest_describe("test widget opacity", test_widget_opacity);
	ctest_describe("test textview resize", test_textview_resize);
	ctest_describe("test textedit", test_textedit);
	ctest_describe("test scrollbar", test_scrollbar);
	ctest_describe("test widget rect", test_widget_rect);
	ctest_describe("test block layout", test_block_layout);
	ctest_describe("test flex layout", test_flex_layout);
	return ctest_finish();
}
