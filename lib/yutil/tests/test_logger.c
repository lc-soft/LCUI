#include <wchar.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/logger.h"

#ifdef _WIN32
#include <windows.h>
static void win32_logger_log(const char *str)
{
	OutputDebugStringA(str);
}

static void win32_logger_log_w(const wchar_t *wcs)
{
	OutputDebugStringW(wcs);
}

#endif

static void test_logger_set_level(void)
{
	logger_set_level(LOGGER_LEVEL_OFF);
	logger_set_level(LOGGER_LEVEL_ERROR);
	logger_set_level(LOGGER_LEVEL_WARNING);
	logger_set_level(LOGGER_LEVEL_INFO);
	logger_set_level(LOGGER_LEVEL_DEBUG);
	logger_set_level(LOGGER_LEVEL_ALL);
	it_b("logger_set_level() should work", 1, true);
}

static void test_logger_log(void)
{
	int len = 0;
	len = logger_log(LOGGER_LEVEL_ALL, "test\n");
	// logger_info("test i\n");
	// logger_debug("test d\n");
	// logger_warning("test w\n");
	// logger_error("test e\n");
	it_b("logger_log() should work", len == 5, true);
}
static void test_logger_log_w(void)
{
	int len = 0;
	len = logger_log_w(LOGGER_LEVEL_ALL, L"test\n");
	logger_info_w(L"test i\n");
	logger_debug_w(L"test d\n");
	logger_warning_w(L"test w\n");
	logger_error_w(L"test e\n");
	it_b("logger_log_w() should work", len == 5, true);
}

void test_logger(void)
{
	test_logger_set_level();
	test_logger_log();
#ifdef _WIN32
	logger_set_handler(win32_logger_log);
	it_b("logger_set_handler() should work", 1, true);

	logger_set_handler_w(win32_logger_log_w);
	it_b("logger_set_handler_w() should work", 1, true);
#endif
	test_logger_log_w();
}