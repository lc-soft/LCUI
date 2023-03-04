#include <stdbool.h>
#include <pandagl/rect.h>
#include <ui/rect.h>
#include <ctest.h>

static inline bool ctest_euqal_pd_rect(const char *name, pd_rect_t *actual,
				       pd_rect_t *expected)
{
	return ctest_equal(name, pd_rect_to_str, actual, expected);
}

static inline bool ctest_euqal_ui_rect(const char *name, pd_rect_t *actual,
				       pd_rect_t *expected)
{
	return ctest_equal(name, ui_rect_to_str, actual, expected);
}
