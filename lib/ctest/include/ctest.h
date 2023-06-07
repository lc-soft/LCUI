#ifndef LIB_CTEST_INCLUDE_CTEST_H
#define LIB_CTEST_INCLUDE_CTEST_H

#include <stdbool.h>

typedef int (*ctest_to_str_func_t)(void *, char *, unsigned);

void ctest_describe(const char *name, void (*func)());
int ctest_printf(const char *fmt, ...);
void ctest_group_begin(void);
void ctest_group_end(void);
void ctest_describe(const char *name, void (*func)());
bool ctest_equal(const char *name, ctest_to_str_func_t to_str, void *actual,
                 void *expected);
int ctest_int_to_str(void *data, char *str, unsigned max_len);
int ctest_uint_to_str(void *data, char *str, unsigned max_len);
int ctest_float_to_str(void *data, char *str, unsigned max_len);
int ctest_str_to_str(void *data, char *str, unsigned max_len);
int ctest_bool_to_str(void *data, char *str, unsigned max_len);
bool ctest_euqal_int(const char *name, int actual, int expected);
bool ctest_euqal_bool(const char *name, bool actual, bool expected);
bool ctest_euqal_uint(const char *name, unsigned actual, unsigned expected);
bool ctest_euqal_float(const char *name, float actual, float expected);
bool ctest_euqal_str(const char *name, const char *actual,
                     const char *expected);
int ctest_finish(void);

#endif
