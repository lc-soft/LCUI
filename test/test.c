#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

#ifdef _WIN32
#define COLOR_NONE
#define COLOR_RED
#define COLOR_GREEN
#else
#define COLOR_NONE "\e[0m"
#define COLOR_RED "\e[0;31m"
#define COLOR_GREEN "\e[0;32m"
#endif
#define RED(TEXT) COLOR_RED TEXT COLOR_NONE
#define GREEN(TEXT) COLOR_GREEN TEXT COLOR_NONE

static size_t tests_passed = 0;
static size_t tests_total = 0;

#define describe(NAME, FUNC)    \
	printf("\n%s\n", NAME); \
	FUNC();

void it_i(const char *name, int actual, int expected)
{
	tests_total++;
	if (actual == expected) {
		printf(GREEN("  √ ") "%s == %d\n", name, expected);
		tests_passed++;
		return;
	}
	printf(RED("  × %s == %d\n"), name, expected);
	printf(RED("    AssertionError: %d == %d\n"), actual, expected);
	printf(GREEN("    + expected ") RED("- actual\n\n"));
	printf(RED("    - %d\n"), actual);
	printf(RED("    + %d\n\n"), expected);
}

void it_b(const char *name, int actual, int expected)
{
	const char *actual_str = actual ? "true" : "false";
	const char *expected_str = expected ? "true" : "false";

	tests_total++;
	if (!actual == !expected) {
		printf(GREEN("  √ ") "%s\n", name);
		tests_passed++;
		return;
	}
	printf(RED("  × %s\n"), name);
	printf(RED("    AssertionError: %s == %s\n"), actual_str, expected_str);
	printf(GREEN("    + expected ") RED("- actual\n\n"));
	printf(RED("    - %s\n"), actual_str);
	printf(GREEN("    + %s\n\n"), expected_str);
}

void it_s(const char *name, const char *actual, const char *expected)
{
	tests_total++;
	if ((actual && expected && strcmp(actual, expected) == 0) ||
	    actual == expected) {
		printf(GREEN("  √ ") "%s == '%s'\n", name, expected);
		tests_passed++;
		return;
	}
	printf(RED("  × %s == '%s'\n"), name, expected);
	if (expected) {
		printf(RED("    AssertionError: '%s' == '%s'\n"), actual,
		       expected);
	} else {
		printf(RED("    AssertionError: '%s' == null\n"), actual);
	}
	printf(GREEN("    + expected ") RED("- actual\n\n"));
	printf(RED("    - %s\n"), actual);
	printf(GREEN("    + %s\n\n"), expected);
}


int tests_count = 0;

int main(void)
{
	int ret = 0;
	Logger_SetLevel(LOGGER_LEVEL_WARNING);
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
	ret += test_widget_event();
	ret += test_widget_opacity();
	ret += test_widget_rect();
	ret += test_textview_resize();
	ret += test_textedit();
	PRINT_TEST_RESULT(ret);
	return ret;
}
