/*
 * lib/pandagl/tests/test_canvas_mask.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <ctest.h>
#include <pandagl.h>

static int color2str(char *str, const pd_color_t *color)
{
        return sprintf(str, "rgba(%d, %d, %d, %g)", color->r, color->g,
                       color->b, color->a / 255.f);
}

void test_pandagl_mask_composite(void)
{
        char rgba_str[64];
        pd_color_t *pixel;
        pd_color_t blue;
        pd_canvas_t dst;
        pd_canvas_t mask;

        blue = pd_argb(255, 0, 0, 255);

        /* Test 1: mask left half a=255, right half a=0 */
        pd_canvas_init(&dst);
        dst.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&dst, 100, 100);
        pd_canvas_fill(&dst, blue);

        pd_canvas_init(&mask);
        mask.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&mask, 100, 100);
        pd_canvas_fill_rect(&mask, pd_argb(255, 0, 0, 255),
                            pd_rect(0, 0, 50, 100));
        pd_canvas_fill_rect(&mask, pd_argb(0, 0, 0, 255),
                            pd_rect(50, 0, 50, 100));

        pd_canvas_mask_composite(&dst, &mask);

        /* left half: mask a=255 -> dst a stays 255, r/g/b unchanged */
        pixel = pd_canvas_pixel_at(&dst, 25, 50);
        color2str(rgba_str, pixel);
        ctest_equal_str("dst(25,50) after mask a=255", rgba_str,
                        "rgba(0, 0, 255, 1)");

        /* right half: mask a=0 -> dst a becomes 0, r/g/b unchanged */
        pixel = pd_canvas_pixel_at(&dst, 75, 50);
        color2str(rgba_str, pixel);
        ctest_equal_str("dst(75,50) after mask a=0", rgba_str,
                        "rgba(0, 0, 255, 0)");

        pd_canvas_destroy(&dst);
        pd_canvas_destroy(&mask);

        /* Test 2: mask smaller than dst, overlap area only */
        pd_canvas_init(&dst);
        dst.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&dst, 100, 100);
        pd_canvas_fill(&dst, blue);

        pd_canvas_init(&mask);
        mask.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&mask, 40, 40);
        pd_canvas_fill(&mask, pd_argb(128, 0, 0, 255));

        pd_canvas_mask_composite(&dst, &mask);

        /* inside overlap: 255 * 128 / 255 = 128 */
        pixel = pd_canvas_pixel_at(&dst, 20, 20);
        color2str(rgba_str, pixel);
        ctest_equal_str("dst(20,20) inside overlap, a=128", rgba_str,
                        "rgba(0, 0, 255, 0.501961)");

        /* outside overlap: alpha unchanged = 255 */
        pixel = pd_canvas_pixel_at(&dst, 80, 80);
        color2str(rgba_str, pixel);
        ctest_equal_str("dst(80,80) outside overlap", rgba_str,
                        "rgba(0, 0, 255, 1)");

        pd_canvas_destroy(&dst);
        pd_canvas_destroy(&mask);

        /* Test 3: RGB mask treated as a=255 (opaque), dst alpha unchanged */
        pd_canvas_init(&dst);
        dst.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&dst, 100, 100);
        pd_canvas_fill(&dst, blue);

        pd_canvas_init(&mask);
        mask.color_type = PD_COLOR_TYPE_RGB;
        pd_canvas_create(&mask, 100, 100);
        pd_canvas_fill(&mask, pd_rgb(128, 255, 0));

        pd_canvas_mask_composite(&dst, &mask);

        /* RGB mask -> a=255, so dst alpha stays 255 */
        pixel = pd_canvas_pixel_at(&dst, 50, 50);
        color2str(rgba_str, pixel);
        ctest_equal_str("dst(50,50) after RGB mask", rgba_str,
                        "rgba(0, 0, 255, 1)");

        pd_canvas_destroy(&dst);
        pd_canvas_destroy(&mask);
}
