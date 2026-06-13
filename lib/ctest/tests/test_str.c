/*
 * lib/ctest/tests/test_str.c
 *
 * Copyright (c) 2026, Liu Chao
 * <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *

 * * This file is part of LCUI, distributed under the MIT License found in the

 * * LICENSE.TXT file in the root directory of this source tree.
 */

#include <assert.h>
#include <string.h>
#include <ctest.h>

static void test_int_to_str(void)
{
        char buf[256];
        int val = 42;
        ctest_int_to_str(&val, buf, 255);
        assert(strcmp(buf, "42") == 0);

        val = -1;
        ctest_int_to_str(&val, buf, 255);
        assert(strcmp(buf, "-1") == 0);
}

static void test_uint_to_str(void)
{
        char buf[256];
        unsigned val = 123u;
        ctest_uint_to_str(&val, buf, 255);
        assert(strcmp(buf, "123") == 0);
}

static void test_float_to_str(void)
{
        char buf[256];
        float val = 3.14f;
        ctest_float_to_str(&val, buf, 255);
        assert(strncmp(buf, "3.14", 4) == 0);
}

static void test_str_to_str(void)
{
        char buf[256];
        const char *val = "hello";
        ctest_str_to_str((void *)val, buf, 255);
        assert(strcmp(buf, "\"hello\"") == 0);
}

static void test_bool_to_str(void)
{
        char buf[256];
        bool val_true = true;
        bool val_false = false;
        ctest_bool_to_str(&val_true, buf, 255);
        assert(strcmp(buf, "true") == 0);
        ctest_bool_to_str(&val_false, buf, 255);
        assert(strcmp(buf, "false") == 0);
}

void test_str_funcs(void)
{
        test_int_to_str();
        test_uint_to_str();
        test_float_to_str();
        test_str_to_str();
        test_bool_to_str();
}
