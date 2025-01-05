/*
 * lib/pandagl/test/test.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "test.h"
#include "ctest.h"

int main()
{
	ctest_describe("test_canvas_mix", test_canvas_mix);
	return ctest_finish();
}
