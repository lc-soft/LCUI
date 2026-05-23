/*
 * lib/thread/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>

extern void test_thread_worker(void);

static const ctest_suite_t suites[] = { { "thread/worker", test_thread_worker },
                                        { NULL, NULL } };

CTEST_MAIN(suites)
