/*
 * lib/css/tests/test.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>
#include <yutil.h>
#include "test.h"

int main()
{
        logger_set_level(LOGGER_LEVEL_ALL);
        ctest_describe("css keywords", test_css_keywords);
        ctest_describe("css value", test_css_value);
        ctest_describe("css computed", test_css_computed);
        ctest_describe("css text decoration", test_css_text_decoration);
        ctest_describe("css inheritance", test_css_inheritance);
        return ctest_finish();
}
