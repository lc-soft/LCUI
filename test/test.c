#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"
#include "libtest.h"

int tests_count = 0;

int main(void)
{
	int ret = 0;
	Logger_SetLevel(LOGGER_LEVEL_OFF);
	ret += test_charset();
	ret += test_linkedlist();
	ret += test_string();
	ret += test_strpool();
	ret += test_object();
	ret += test_thread();
	ret += test_font_load();
	ret += test_image_reader();
	ret += test_xml_parser();
	ret += test_widget_event();
	ret += test_widget_opacity();
	ret += test_textview_resize();
	ret += test_textedit();
	describe("test mainloop", test_mainloop);
	describe("test css parser", test_css_parser);
	describe("test block layout", test_block_layout);
	describe("test flex layout", test_flex_layout);
	describe("test widget rect", test_widget_rect);
	PRINT_TEST_RESULT(ret);
	return ret - print_test_result();
}
