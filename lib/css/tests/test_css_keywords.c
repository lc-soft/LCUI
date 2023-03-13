#include <stdio.h>
#include <css/keywords.h>
#include "test.h"
#include "ctest.h"

static void test_keyword_value(const char *name, int value)
{
	char str[256] = { 0 };

	snprintf(str, 255, "keyword('%s').value", name);
	it_i(str, css_get_keyword_key(name), value);
}

static void test_keyword_register(const char *name)
{
	char str[256] = { 0 };
	int a = css_register_keyword(name);
	int b = css_get_keyword_key(name);

	snprintf(str, 255, "keyword('%s')", name);
	it_i(str, a, b);
}

void test_css_keywords(void)
{
	css_init_keywords();

	test_keyword_value("inherit", CSS_KEYWORD_INHERIT);
	test_keyword_value("none", CSS_KEYWORD_NONE);
	test_keyword_value("auto", CSS_KEYWORD_AUTO);
	test_keyword_value("center", CSS_KEYWORD_CENTER);
	test_keyword_value("inline-block", CSS_KEYWORD_INLINE_BLOCK);
	test_keyword_register("custom-keyword");

	css_destroy_keywords();
}
