/*
 * lib/ui-xml/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>

extern void test_ui_xml_parser(void);
extern void test_ui_xml_resource_node(void);

static const ctest_suite_t suites[] = { { "ui-xml/parser", test_ui_xml_parser },
                                        { "ui-xml/resource node",
                                          test_ui_xml_resource_node },
                                        { NULL, NULL } };

CTEST_MAIN(suites)
