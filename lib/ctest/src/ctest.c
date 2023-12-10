/*
 * lib/ctest/src/ctest.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <yutil.h>
#include <ctest.h>

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

#if defined(CI_ENV) || defined(_WIN32)
#define CHECK_MARK GREEN("* ")
#define ERROR_MARK "x "
#else
#define CHECK_MARK GREEN("√ ")
#define ERROR_MARK "× "
#endif

static size_t tests_passed = 0;
static size_t tests_total = 0;
static int test_msg_indent = 0;
static int64_t test_start_time = 0;

int ctest_printf(const char *fmt, ...)
{
	int ret;
	char str[1024];
	va_list args;

	va_start(args, fmt);
	ret = vsnprintf(str, 1023, fmt, args);
	va_end(args);
	printf("%*s%s", test_msg_indent * 2, "", str);
	return ret;
}

void ctest_group_begin(void)
{
	if (test_start_time == 0) {
		test_start_time = get_time_ms();
	}
	test_msg_indent++;
}

void ctest_group_end(void)
{
	test_msg_indent--;
}

void ctest_describe(const char *name, void (*func)())
{
	ctest_printf("%s\n", name);
	ctest_group_begin();
	func();
	ctest_group_end();
	ctest_printf("\n");
}

bool ctest_equal(const char *name, int (*to_str)(void *, char *, unsigned),
		 void *actual, void *expected)
{
	char actual_str[256] = { 0 };
	char expected_str[256] = { 0 };

	tests_total++;
	to_str(actual, actual_str, 255);
	to_str(expected, expected_str, 255);
	if (strncmp(actual_str, expected_str, 255) == 0) {
		ctest_printf(CHECK_MARK "%s\n", name);
		tests_passed++;
		return true;
	}
	ctest_printf(RED(ERROR_MARK "%s\n"), name);
	ctest_printf(GREEN("  + expected ") RED("- actual\n\n"));
	ctest_printf(RED("  - %s\n"), actual_str);
	ctest_printf(GREEN("  + %s\n\n"), expected_str);
	return false;
}

int ctest_int_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "%d", *(int *)data);
}

int ctest_uint_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "%u", *(unsigned *)data);
}

int ctest_float_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "%f", *(float *)data);
}

int ctest_str_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "\"%s\"", (char *)data);
}

int ctest_bool_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "%s", *(bool*)data ? "true" : "false");
}

bool ctest_equal_int(const char *name, int actual, int expected)
{
	return ctest_equal(name, ctest_int_to_str, &actual, &expected);
}

bool ctest_equal_bool(const char *name, bool actual, bool expected)
{
	return ctest_equal(name, ctest_bool_to_str, &actual, &expected);
}

bool ctest_equal_uint(const char *name, unsigned actual, unsigned expected)
{
	return ctest_equal(name, ctest_uint_to_str, &actual, &expected);
}

bool ctest_equal_float(const char *name, float actual, float expected)
{
	return ctest_equal(name, ctest_float_to_str, &actual, &expected);
}

bool ctest_equal_str(const char *name, const char *actual, const char *expected)
{
	return ctest_equal(name, ctest_str_to_str, (void *)actual, (void *)expected);
}

int ctest_finish(void)
{
	printf(GREEN("  %zu passing") " (%ums)\n", tests_passed,
	       (unsigned)get_time_delta(test_start_time));
	if (tests_total > tests_passed) {
		printf(RED("  %zu faling\n\n"), tests_total - tests_passed);
		return (int)(tests_total - tests_passed);
	}
	printf("\n");
	return 0;
}
