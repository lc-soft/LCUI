#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <LCUI/header.h>
#include <LCUI/types.h>

void describe(const char *name, void (*func)());

int test_msg(const char *fmt, ...);

void it_i(const char *name, int actual, int expected);

void it_b(const char *name, int actual, int expected);

void it_s(const char *name, const char *actual, const char *expected);

void it_rectf(const char *name, const pd_rectf_t *actual,
	      const pd_rectf_t *expected);

void it_rect(const char *name, const pd_rect_t *actual,
	     const pd_rect_t *expected);

void test_begin(void);

void test_end(void);

int test_result(void);

int print_test_result(void);

#endif
