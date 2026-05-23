/*
 * lib/ptk/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>

extern void test_ptk_steptimer(void);

static const ctest_suite_t suites[] = { { "ptk/steptimer", test_ptk_steptimer },
                                        { NULL, NULL } };

CTEST_MAIN(suites)
