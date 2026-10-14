/*
 * tests/integration/test_spinner_perf.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <time.h>
#include <LCUI.h>
#include <ctest-custom.h>
#include "../scenes/spinner_perf_scene.h"

/* The scene is deliberately heavy (200 spinners plus two 400x400 text
 * spinners, all supersampled), so a single timed frame per scroll offset keeps
 * the test bearable. The printed ms/frame is the point: it is the rendering
 * cost the preview shows as stutter. */
#define PERF_FRAMES 1
#define PERF_OFFSETS 3

static spinner_perf_scene_t scene;
static pd_canvas_t canvas;

static size_t render_box(void)
{
        pd_rect_t rect = { 0, 0, (int)SPINNER_PERF_BOX_W,
                           (int)SPINNER_PERF_BOX_H };
        pd_context_t paint;

        pd_canvas_fill(&canvas, pd_rgb(255, 255, 255));
        pd_canvas_init(&paint.canvas);
        paint.rect = rect;
        paint.with_alpha = true;
        pd_canvas_quote(&paint.canvas, &canvas, &rect);
        return ui_widget_render(scene.box, &paint);
}

/* Count pixels the scene actually drew, to prove the frame is non-empty. */
static int count_ink(void)
{
        int x, y, n = 0;

        for (y = 0; y < (int)SPINNER_PERF_BOX_H; ++y) {
                for (x = 0; x < (int)SPINNER_PERF_BOX_W; ++x) {
                        pd_color_t p = pd_canvas_get_pixel(&canvas, (unsigned)x,
                                                           (unsigned)y);
                        if (p.r < 248 || p.g < 248 || p.b < 248) {
                                n += 1;
                        }
                }
        }
        return n;
}

/* How many widgets the frame actually rendered. This is the real measure of
 * how much work the scroll offset removes: offscreen spinners are culled. */
/* (counted via the return value of ui_widget_render below) */

/* Time one animated frame at the given scroll offset. */
static double measure_frame_ms(float scroll_top, int *ink, size_t *widgets)
{
        clock_t t0, t1;
        size_t rendered = 0;
        int f;

        ui_scrollarea_set_scroll_top(scene.scrollarea, scroll_top);
        ui_update();

        t0 = clock();
        for (f = 0; f < PERF_FRAMES; ++f) {
                spinner_perf_scene_advance(&scene, 37.0);
                ui_update();
                rendered = render_box();
        }
        t1 = clock();
        *ink = count_ink();
        *widgets = rendered;
        return (double)(t1 - t0) * 1000.0 /
               ((double)CLOCKS_PER_SEC * PERF_FRAMES);
}

void test_spinner_perf(void)
{
        float max_scroll, offsets[PERF_OFFSETS];
        int i;

        lcui_init();
        spinner_perf_scene_build(&scene);
        /* Drive the animation from the test instead of a live frame clock. */
        for (i = 0; i < scene.count; ++i) {
                ui_spinner_set_running(scene.spinners[i], false);
        }
        for (i = 0; i < SPINNER_PERF_BIG_COUNT; ++i) {
                ui_spinner_set_running(scene.big[i], false);
        }
        ui_widget_request_refresh_style(scene.box);
        ui_update();
        ui_update();

        pd_canvas_init(&canvas);
        canvas.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&canvas, (unsigned)SPINNER_PERF_BOX_W,
                         (unsigned)SPINNER_PERF_BOX_H);

        max_scroll = ui_scrollarea_get_scroll_height(scene.scrollarea) -
                     SPINNER_PERF_VIEW_H;
        if (max_scroll < 0.0f) {
                max_scroll = 0.0f;
        }
        offsets[0] = 0.0f;
        offsets[1] = max_scroll * 0.5f;
        offsets[2] = max_scroll;

        /* One untimed warm-up frame builds every spinner's cached layer. */
        render_box();

        printf("  [spinner perf] %d small x 40x40, %d big x 400x400 text, "
               "%d frame(s)/offset, scroll range %.0f px\n",
               SPINNER_PERF_COUNT, SPINNER_PERF_BIG_COUNT, PERF_FRAMES,
               max_scroll);
        printf("    scroll_top  rendered  ink_px  ms/frame\n");
        for (i = 0; i < PERF_OFFSETS; ++i) {
                int ink = 0;
                size_t widgets = 0;
                double ms = measure_frame_ms(offsets[i], &ink, &widgets);

                printf("    %10.1f  %8lu  %6d  %8.1f\n", offsets[i],
                       (unsigned long)widgets, ink, ms);
                ctest_equal_bool(
                    "spinner perf: every offset should render something",
                    ink > 0, true);
        }

        pd_canvas_destroy(&canvas);
        ui_widget_destroy(scene.box);
        lcui_destroy();
}
