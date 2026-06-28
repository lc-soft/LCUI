#ifndef UTIL_TEST_H
#define UTIL_TEST_H

void describe(const char *name, void (*func)());

void it_i(const char *name, int actual, int expected);

void it_b(const char *name, int actual, int expected);

void it_s(const char *name, const char *actual, const char *expected);

void test_begin(void);

void test_end(void);

int test_result(void);

int print_test_result(void);

#endif
