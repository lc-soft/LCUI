/*
 * lib/ctest/tests/test_grep.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <assert.h>
#include <string.h>
#include <ctest.h>

static void test_should_run_no_pattern(void)
{
        ctest_set_grep(NULL);
        assert(ctest_should_run("anything") == true);
        assert(ctest_should_run("") == true);
}

static void test_should_run_with_pattern(void)
{
        ctest_set_grep("widget");
        assert(ctest_should_run("widget event") == true);
        assert(ctest_should_run("widget opacity") == true);
        assert(ctest_should_run("settings") == false);
        assert(ctest_should_run("thread") == false);
}

static void test_should_run_exact_match(void)
{
        ctest_set_grep("steptimer");
        assert(ctest_should_run("steptimer") == true);
        assert(ctest_should_run("ptk/steptimer") == true);
        assert(ctest_should_run("widget event") == false);
}

static void test_should_run_empty_pattern(void)
{
        ctest_set_grep("");
        assert(ctest_should_run("anything") == true);
}

static void test_is_interactive_default(void)
{
        ctest_set_interactive(false);
        assert(ctest_is_interactive() == false);
}

static void test_is_interactive_set(void)
{
        ctest_set_interactive(true);
        assert(ctest_is_interactive() == true);
        ctest_set_interactive(false);
        assert(ctest_is_interactive() == false);
}

void test_grep_funcs(void)
{
        test_should_run_no_pattern();
        test_should_run_with_pattern();
        test_should_run_exact_match();
        test_should_run_empty_pattern();
        test_is_interactive_default();
        test_is_interactive_set();
}
