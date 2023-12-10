/*
 * tests/include/ctest-custom.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdbool.h>
#include <pandagl/rect.h>
#include <ui/rect.h>
#include <ctest.h>

static inline bool ctest_equal_pd_rect(const char *name, pd_rect_t *actual,
				       pd_rect_t *expected)
{
	return ctest_equal(name, (ctest_to_str_func_t)pd_rect_to_str, actual, expected);
}

static inline bool ctest_equal_ui_rect(const char *name, ui_rect_t *actual,
				       ui_rect_t *expected)
{
	return ctest_equal(name, (ctest_to_str_func_t)ui_rect_to_str, actual, expected);
}
