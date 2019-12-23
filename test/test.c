#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

int tests_count = 0;

int main(void)
{
	int ret = 0;

	ret += test_charset();
	ret += test_linkedlist();
	ret += test_string();
	ret += test_strpool();
	ret += test_object();
	ret += test_thread();
	ret += test_font_load();
	ret += test_image_reader();
	ret += test_css_parser();
	ret += test_xml_parser();
	ret += test_widget_layout();
	ret += test_widget_flex_layout();
	ret += test_widget_inline_block_layout();
	ret += test_widget_event();
	ret += test_widget_opacity();
	ret += test_widget_rect();
	ret += test_textview_resize();
	ret += test_textedit();
	PRINT_TEST_RESULT(ret);
	return ret;
}
