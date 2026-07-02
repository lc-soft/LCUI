/*
 * lib/pandagl/tests/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctest.h>

extern void test_pandagl_canvas_mix(void);
extern void test_pandagl_image_reader(void);
extern void test_pandagl_font_load(void);
extern void test_pandagl_text_edit(void);
extern void test_pandagl_font_cache(void);
extern void test_pandagl_text_baseline(void);
extern void test_pandagl_text_style_tag(void);

static const ctest_suite_t suites[] = {
        { "pandagl/canvas mix", test_pandagl_canvas_mix },
        { "pandagl/image reader", test_pandagl_image_reader },
        { "pandagl/font load", test_pandagl_font_load },
        { "pandagl/text edit", test_pandagl_text_edit },
        { "pandagl/font cache", test_pandagl_font_cache },
        { "pandagl/text baseline", test_pandagl_text_baseline },
        { "pandagl/text style tag", test_pandagl_text_style_tag },
        { NULL, NULL }
};

CTEST_MAIN(suites)
