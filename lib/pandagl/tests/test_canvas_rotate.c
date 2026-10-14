/*
 * lib/pandagl/tests/test_canvas_rotate.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <ctest.h>
#include <pandagl.h>

void test_pandagl_canvas_rotate(void)
{
        pd_canvas_t src, dst;
        pd_color_t color;
        bool identity_ok;
        bool moved;
        bool outside_clear;
        int x, y;

        /* Build src: 40x40 ARGB, transparent, top-left 10x10 marker red */
        pd_canvas_init(&src);
        src.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&src, 40, 40);
        pd_canvas_fill(&src, pd_argb(0, 0, 0, 0));
        for (y = 0; y < 10; y++) {
                for (x = 0; x < 10; x++) {
                        pd_canvas_set_pixel(&src, x, y, pd_rgb(255, 0, 0));
                }
        }

        /* rotate 0: should be identity copy */
        pd_canvas_init(&dst);
        ctest_equal_int("rotate 0 should succeed",
                        pd_canvas_rotate(&src, &dst, 0.0), 0);

        identity_ok = true;
        for (y = 0; y < 40 && identity_ok; y++) {
                for (x = 0; x < 40 && identity_ok; x++) {
                        if (pd_canvas_get_pixel(&src, x, y).value !=
                            pd_canvas_get_pixel(&dst, x, y).value) {
                                identity_ok = false;
                        }
                }
        }
        ctest_equal_bool("rotate 0 should copy pixels", identity_ok, true);
        pd_canvas_destroy(&dst);

        /* rotate 90: marker should move to upper-right */
        pd_canvas_init(&dst);
        ctest_equal_int("rotate 90 should succeed",
                        pd_canvas_rotate(&src, &dst, M_PI / 2.0), 0);

        /* (34,6) is upper-right area - should be red (was top-left marker) */
        color = pd_canvas_get_pixel(&dst, 34, 6);
        moved = color.a > 0 && color.r == 255;
        ctest_equal_bool("rotate 90 should move marker to upper-right", moved,
                         true);

        /* Areas outside rotated marker should be transparent */
        outside_clear = pd_canvas_get_pixel(&dst, 6, 6).a == 0 &&
                        pd_canvas_get_pixel(&dst, 34, 34).a == 0;
        ctest_equal_bool(
            "rotate 90 should keep area outside source transparent",
            outside_clear, true);

        pd_canvas_destroy(&dst);

        /* rotate 45: dst corners map outside src -> true OOB transparent */
        pd_canvas_init(&dst);
        ctest_equal_int("rotate 45 should succeed",
                        pd_canvas_rotate(&src, &dst, M_PI / 4.0), 0);

        outside_clear = pd_canvas_get_pixel(&dst, 0, 0).value == 0 &&
                        pd_canvas_get_pixel(&dst, 39, 0).value == 0 &&
                        pd_canvas_get_pixel(&dst, 0, 39).value == 0 &&
                        pd_canvas_get_pixel(&dst, 39, 39).value == 0;
        ctest_equal_bool(
            "rotate 45 should keep true-out-of-bounds corners transparent",
            outside_clear, true);

        pd_canvas_destroy(&dst);

        /* Invalid canvas should return -1 */
        {
                pd_canvas_t bad;
                pd_canvas_t out;

                pd_canvas_init(&bad);
                pd_canvas_init(&out);
                ctest_equal_int("invalid src should return -1",
                                pd_canvas_rotate(&bad, &out, 0.0), -1);
        }

        pd_canvas_destroy(&src);
}
