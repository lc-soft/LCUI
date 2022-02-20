#include <stdio.h>
#include <wchar.h>
#include <LCUI.h>
#include <LCUI/main.h>
#include "ctest.h"
#include "run_tests.h"

int main(int argc, char **argv)
{
	int ret = 0;
	logger_set_level(LOGGER_LEVEL_ERROR);
	describe("test settings", test_settings);
	describe("test thread", test_thread);
	describe("test font load", test_font_load);
	describe("test image reader", test_image_reader);
	describe("test xml parser", test_xml_parser);
	describe("test mainloop", test_mainloop);
	describe("test clipboard", test_clipboard);
	describe("test widget event", test_widget_event);
	describe("test widget opacity", test_widget_opacity);
	describe("test textview resize", test_textview_resize);
	describe("test textedit", test_textedit);
	describe("test scrollbar", test_scrollbar);
	describe("test widget rect", test_widget_rect);
	describe("test block layout", test_block_layout);
	describe("test flex layout", test_flex_layout);
	return ret - print_test_result();
}
