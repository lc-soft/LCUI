/*
 * tests/cases/test_font_face.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

void test_font_face(void)
{
        ui_init();
        ctest_equal_int("should load CSS file containing @font-face",
                        ui_load_css_file("test_font_face.css"), 0);
        ctest_equal_bool("should resolve font registered via @font-face",
                         pd_font_library_get_font_id("icomoon", 0, 0) > 0,
                         true);
        ui_destroy();
}
