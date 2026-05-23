/*
 * lib/ctest/include/ctest.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_CTEST_INCLUDE_CTEST_H
#define LIB_CTEST_INCLUDE_CTEST_H

#include <stdbool.h>
#include <wchar.h>

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
int ctest_wcs_to_str(void *data, char *str, unsigned max_len);
int ctest_bool_to_str(void *data, char *str, unsigned max_len);
bool ctest_equal_int(const char *name, int actual, int expected);
bool ctest_equal_bool(const char *name, bool actual, bool expected);
bool ctest_equal_uint(const char *name, unsigned actual, unsigned expected);
bool ctest_equal_float(const char *name, float actual, float expected);
bool ctest_equal_str(const char *name, const char *actual,
                     const char *expected);
bool ctest_equal_wcs(const char *name, const wchar_t *actual,
                     const wchar_t *expected);
int ctest_finish(void);

typedef struct {
        const char *name;
        void (*run)(void);
} ctest_suite_t;

void ctest_set_grep(const char *pattern);
bool ctest_should_run(const char *name);
bool ctest_is_interactive(void);
void ctest_set_interactive(bool interactive);
int ctest_parse_args(int argc, char **argv);
int ctest_run_suites(const ctest_suite_t *suites);

#define CTEST_MAIN(SUITES)                             \
        int main(int argc, char **argv)                \
        {                                              \
                if (ctest_parse_args(argc, argv) != 0) \
                        return 1;                      \
                return ctest_run_suites(SUITES);       \
        }

#endif
