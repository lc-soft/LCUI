#include "test.h"
#include "ctest.h"

int main()
{
	int ret = 0;

	logger_set_level(LOGGER_LEVEL_ALL);
	describe("test_css_keywords", test_css_keywords);
	describe("test_css_value", test_css_value);
	describe("test_css_computed", test_css_computed);
	return ret - print_test_result();
}
