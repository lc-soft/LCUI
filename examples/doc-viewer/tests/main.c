/*
 * examples/doc-viewer/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest.h>

extern void test_demo_header_layout(void);
extern void test_gallery_layout(void);

static const ctest_suite_t suites[] = {
        { "demo header layout", test_demo_header_layout },
        { "gallery layout", test_gallery_layout },
        { NULL, NULL }
};

int main(int argc, char **argv)
{
        logger_set_level(LOGGER_LEVEL_ERROR);
        if (ctest_parse_args(argc, argv) != 0) {
                return 1;
        }
        return ctest_run_suites(suites);
}
