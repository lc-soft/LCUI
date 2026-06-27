/*
 * lib/ctest/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>

extern void test_str_funcs(void);
extern void test_grep_funcs(void);

int main(void)
{
        test_str_funcs();
        test_grep_funcs();
        printf("ctest-tests: all assertions passed\n");
        return 0;
}
