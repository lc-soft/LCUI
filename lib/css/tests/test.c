/*
 * lib/css/tests/test.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>
#include <yutil.h>
#include "test.h"

int main()
{
	logger_set_level(LOGGER_LEVEL_ALL);
	ctest_describe("test_css_keywords", test_css_keywords);
	ctest_describe("test_css_value", test_css_value);
	ctest_describe("test_css_computed", test_css_computed);
	return ctest_finish();
}
