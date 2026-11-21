/*
 * tests/integration/test_spinner.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <ctest-custom.h>
#include "../../src/widgets/draw_backend.h"
#include "../../src/widgets/spinner_priv.h"
#include "../scenes/spinner_scene.h"

#define SPIN 40
#define RING 16
#define BASE pd_rgb(255, 255, 255)

/* The default ring stroke color (#6b7f8f). */
#define RING_R 107
#define RING_G 127
#define RING_B 143

#define RED_R 255
#define RED_G 0
#define RED_B 0
#define GREEN_R 0
#define GREEN_G 255
#define GREEN_B 0
#define BLUE_R 0
#define BLUE_G 0
#define BLUE_B 255

static spinner_scene_t scene;

static void make_argb(pd_canvas_t *canvas, int width, int height)
{
        pd_canvas_init(canvas);
        canvas->color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(canvas, (unsigned)width, (unsigned)height);
}

static int is_color(pd_color_t c, int r, int g, int b)
{
        return c.a > 200 && abs(c.r - r) < 24 && abs(c.g - g) < 24 &&
               abs(c.b - b) < 24;
}

static int is_base(pd_color_t c)
{
        return abs(c.r - 255) < 8 && abs(c.g - 255) < 8 && abs(c.b - 255) < 8;
}

/* A pixel carrying the default ring stroke color. */
static int is_ring(pd_color_t c)
{
        return c.a > 180 && abs(c.r - RING_R) < 30 && abs(c.g - RING_G) < 30 &&
               abs(c.b - RING_B) < 40;
}

/* A pixel carrying the ring stroke when `color` is set to red. */
static int is_redish(pd_color_t c)
{
        return c.a > 200 && c.r > 180 && c.g < 90 && c.b < 90;
}

/* Count ring-stroke pixels inside the half-open box [x0, x1) x [y0, y1). The
 * inset ring and supersampled rotation move exact sample points frame to
 * frame, so the ring assertions count regions rather than pin single pixels.
 */
static int count_ring(pd_canvas_t *out, int x0, int y0, int x1, int y1)
{
        int x, y, n = 0;

        for (y = y0; y < y1; ++y) {
                for (x = x0; x < x1; ++x) {
                        if (is_ring(pd_canvas_get_pixel(out, x, y))) {
                                n += 1;
                        }
                }
        }
        return n;
}

/* Mean x of every ring-stroke pixel; -1 when no stroke is present. Used to
 * detect the horizontal direction the ring centroid shifts under rotation. */
static double ring_centroid_x(pd_canvas_t *out, int w, int h)
{
        int x, y, n = 0;
        double sx = 0.0;

        for (y = 0; y < h; ++y) {
                for (x = 0; x < w; ++x) {
                        if (is_ring(pd_canvas_get_pixel(out, x, y))) {
                                sx += x;
                                n += 1;
                        }
                }
        }
        return n > 0 ? sx / (double)n : -1.0;
}

/* Distance from the nearest box edge to the closest ring-stroke column, taken
 * over the full height so the top gap cannot short-circuit the measurement. */
static int ring_left_margin(pd_canvas_t *out, int w, int h)
{
        int x, y;

        for (x = 0; x < w; ++x) {
                for (y = 0; y < h; ++y) {
                        if (is_ring(pd_canvas_get_pixel(out, x, y))) {
                                return x;
                        }
                }
        }
        return w;
}

static int ring_right_margin(pd_canvas_t *out, int w, int h)
{
        int x, y;

        for (x = w - 1; x >= 0; --x) {
                for (y = 0; y < h; ++y) {
                        if (is_ring(pd_canvas_get_pixel(out, x, y))) {
                                return (w - 1) - x;
                        }
                }
        }
        return w;
}

static void refresh_style_each(ui_widget_t *w, void *arg)
{
        ui_widget_request_refresh_style(w);
}

/* Prepare a spinner for a deterministic single-frame render: stop the
 * animation so set_angle is authoritative, resize the box, then refresh the
 * whole subtree and reflow so the content (and its descendants) are measured
 * at the current scale before we render. Three updates absorb the one-pass
 * layout lag from the resize callback that sizes the offscreen content. */
static void prime_spinner(ui_widget_t *sp, float width, float height,
                          double angle)
{
        ui_spinner_set_running(sp, false);
        ui_spinner_set_angle(sp, angle);
        ui_widget_resize(sp, width, height);
        ui_widget_request_refresh_style(sp);
        ui_widget_each(sp, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
}

static void render_into(ui_widget_t *sp, pd_canvas_t *out, int cw, int ch)
{
        pd_rect_t rect = { 0, 0, cw, ch };
        pd_context_t paint;

        make_argb(out, cw, ch);
        pd_canvas_fill(out, BASE);
        paint.with_alpha = true;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, out, &rect);
        ui_widget_render(sp, &paint);
}

/* 1. Identity: at angle 0 the spinner's composited center equals a direct
 * offscreen capture of its content, which also proves the content appears only
 * once (nothing painted at the offscreen -100000px location). */
static void check_identity(void)
{
        pd_canvas_t out, cap;
        ui_draw_surface_t *cs;
        pd_rect_t cr = { 0, 0, SPIN, SPIN };
        pd_context_t ctx;
        pd_color_t a, b;

        ui_metrics.dpi = 96;
        prime_spinner(scene.spinner, SPIN, SPIN, 0);

        cs = ui_draw_surface_create();
        ui_draw_surface_resize(cs, SPIN, SPIN);
        ui_draw_capture(scene.content, cs);
        make_argb(&cap, SPIN, SPIN);
        pd_canvas_init(&ctx.canvas);
        ctx.rect = cr;
        ctx.with_alpha = true;
        pd_canvas_quote(&ctx.canvas, &cap, &cr);
        ui_draw_blend_surface(&ctx, cs, 0, 0, true, 1.0f);
        ui_draw_surface_destroy(cs);
        render_into(scene.spinner, &out, SPIN, SPIN);

        a = pd_canvas_get_pixel(&out, SPIN / 2, SPIN / 2);
        b = pd_canvas_get_pixel(&cap, SPIN / 2, SPIN / 2);
        ctest_equal_bool("identity: composited center should match capture",
                         is_color(a, RED_R, RED_G, RED_B) &&
                             abs(a.r - b.r) < 4 && abs(a.g - b.g) < 4 &&
                             abs(a.b - b.b) < 4,
                         true);

        pd_canvas_destroy(&out);
        pd_canvas_destroy(&cap);
}

/* 2. Rotation reaches the widget layer: the top-left mark moves to the
 * top-right under a 90 degree turn (direction verified in the draw backend). */
static void check_rotate90(void)
{
        pd_canvas_t out;

        ui_metrics.dpi = 96;
        prime_spinner(scene.spinner, SPIN, SPIN, 90);
        render_into(scene.spinner, &out, SPIN, SPIN);

        ctest_equal_bool(
            "rotate: mark should move to the top-right",
            is_color(pd_canvas_get_pixel(&out, SPIN - SPIN / 8, SPIN / 8),
                     GREEN_R, GREEN_G, GREEN_B),
            true);
        ctest_equal_bool(
            "rotate: original mark corner should show the background",
            is_color(pd_canvas_get_pixel(&out, SPIN / 8, SPIN / 8), RED_R,
                     RED_G, RED_B),
            true);

        pd_canvas_destroy(&out);
}

/* 2b. Declarative attrs route through proto->setattr: an `angle` attribute must
 * rotate the composited content, and `running`/`speed` must be accepted without
 * corrupting the render. */
static void check_setattr(void)
{
        pd_canvas_t out;

        ui_metrics.dpi = 96;
        prime_spinner(scene.spinner, SPIN, SPIN, 0);

        ui_widget_set_attr(scene.spinner, "running", "false");
        ui_widget_set_attr(scene.spinner, "speed", "2.0");
        ui_widget_set_attr(scene.spinner, "angle", "90");
        render_into(scene.spinner, &out, SPIN, SPIN);

        ctest_equal_bool(
            "setattr: angle attribute should rotate the mark to the top-right",
            is_color(pd_canvas_get_pixel(&out, SPIN - SPIN / 8, SPIN / 8),
                     GREEN_R, GREEN_G, GREEN_B),
            true);
        ctest_equal_bool(
            "setattr: running/speed attrs should leave the mark corner clear",
            is_color(pd_canvas_get_pixel(&out, SPIN / 8, SPIN / 8), RED_R,
                     RED_G, RED_B),
            true);

        pd_canvas_destroy(&out);
}

/* 3. set_content swaps the captured subtree: the center feature changes color.
 */
static void check_set_content(void)
{
        pd_canvas_t out;
        ui_widget_t *alt, *child;

        ui_metrics.dpi = 96;
        prime_spinner(scene.spinner, SPIN, SPIN, 0);

        alt = ui_create_widget("spinner-content");
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "mark-alt");
        ui_widget_append(alt, child);
        ui_spinner_set_content(scene.spinner, alt);
        prime_spinner(scene.spinner, SPIN, SPIN, 0);

        render_into(scene.spinner, &out, SPIN, SPIN);
        ctest_equal_bool(
            "set_content: swapped content should recolor the center",
            is_color(pd_canvas_get_pixel(&out, SPIN / 2, SPIN / 2), BLUE_R,
                     BLUE_G, BLUE_B),
            true);

        pd_canvas_destroy(&out);
}

/* 4. Clip: a content block larger than the spinner box is clipped to the box,
 * so the target area outside the box keeps the base color. */
static void check_clip(void)
{
        pd_canvas_t out;
        ui_widget_t *bleed, *child;

        ui_metrics.dpi = 96;
        bleed = ui_create_widget("spinner-content");
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "bleed");
        ui_widget_append(bleed, child);
        ui_spinner_set_content(scene.spinner, bleed);
        prime_spinner(scene.spinner, 24, 24, 0);

        render_into(scene.spinner, &out, 64, 64);
        ctest_equal_bool(
            "clip: inside the box should show the feature",
            is_color(pd_canvas_get_pixel(&out, 12, 12), RED_R, RED_G, RED_B),
            true);
        ctest_equal_bool("clip: outside the box should keep the base color",
                         is_base(pd_canvas_get_pixel(&out, 50, 50)), true);

        pd_canvas_destroy(&out);
}

/* 5. Resize parity: after growing the box the buffers rebuild to the new size
 * with no leftover from the previous smaller capture. */
static void check_resize_parity(void)
{
        pd_canvas_t small, big;
        ui_widget_t *content, *child;

        ui_metrics.dpi = 96;
        content = ui_create_widget("spinner-content");
        ui_widget_add_class(content, "content-bg");
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "mark");
        ui_widget_append(content, child);
        ui_spinner_set_content(scene.spinner, content);

        prime_spinner(scene.spinner, 16, 16, 0);
        render_into(scene.spinner, &small, 16, 16);

        prime_spinner(scene.spinner, 48, 48, 0);
        render_into(scene.spinner, &big, 48, 48);

        ctest_equal_bool(
            "resize: small buffer center should be the background",
            is_color(pd_canvas_get_pixel(&small, 8, 12), RED_R, RED_G, RED_B),
            true);
        ctest_equal_bool(
            "resize: growing the box should reveal fresh content at 48",
            is_color(pd_canvas_get_pixel(&big, 44, 44), RED_R, RED_G, RED_B),
            true);
        ctest_equal_bool("resize: the mark should scale with the new buffer",
                         is_color(pd_canvas_get_pixel(&big, 4, 4), GREEN_R,
                                  GREEN_G, GREEN_B),
                         true);

        pd_canvas_destroy(&small);
        pd_canvas_destroy(&big);
}

/* 6. DPI parity: the spinner rebuilds its capture buffer at the device scale.
 * Both renders target the same 80x80 canvas. With the box at 40 logical px the
 * buffer is 40 at 96 dpi but 80 at 192 dpi, so a corner pixel is base color at
 * 96 (outside the small buffer) and the background color at 192 (inside the
 * enlarged buffer), proving the offscreen capture tracks the DPI. */
static void check_dpi_parity(void)
{
        pd_canvas_t c96, c192;
        ui_widget_t *content;

        ui_metrics.dpi = 96;
        content = ui_create_widget("spinner-content");
        ui_widget_add_class(content, "content-bg");
        ui_spinner_set_content(scene.spinner, content);

        prime_spinner(scene.spinner, SPIN, SPIN, 0);
        render_into(scene.spinner, &c96, SPIN * 2, SPIN * 2);

        ui_metrics.dpi = 192;
        prime_spinner(scene.spinner, SPIN, SPIN, 0);
        render_into(scene.spinner, &c192, SPIN * 2, SPIN * 2);

        ctest_equal_bool("dpi: near-corner is background color at 96",
                         is_color(pd_canvas_get_pixel(&c96, SPIN / 4, SPIN / 4),
                                  RED_R, RED_G, RED_B),
                         true);
        ctest_equal_bool(
            "dpi: near-corner is background color at 192",
            is_color(pd_canvas_get_pixel(&c192, SPIN / 4, SPIN / 4), RED_R,
                     RED_G, RED_B),
            true);
        /* (78,78) is inside the 80 buffer but outside the 40 buffer. */
        ctest_equal_bool(
            "dpi: 96 buffer stays 40 (corner is base color)",
            is_base(pd_canvas_get_pixel(&c96, SPIN * 2 - 2, SPIN * 2 - 2)),
            true);
        ctest_equal_bool(
            "dpi: 192 buffer grows to 80 (corner shows the background)",
            is_color(pd_canvas_get_pixel(&c192, SPIN * 2 - 2, SPIN * 2 - 2),
                     RED_R, RED_G, RED_B),
            true);

        ui_metrics.dpi = 96;
        pd_canvas_destroy(&c96);
        pd_canvas_destroy(&c192);
}

/* 7. Lifecycle: the factory + default content path works and toggling running
 * or destroying the widget is safe. */
static void check_lifecycle(void)
{
        ui_widget_t *sp, *probe_content;
        pd_canvas_t out;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        ctest_equal_bool("lifecycle: factory should return a widget",
                         sp != NULL, true);

        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, SPIN, SPIN, 0);

        render_into(sp, &out, SPIN, SPIN);
        /* The default content is a ring: assert its presence by counting stroke
         * pixels across the box instead of pinning one sample point, so the
         * check holds for the scaled geometry at a 40px box. */
        {
                int x, y, ring_count = 0;
                for (y = 0; y < SPIN; ++y) {
                        for (x = 0; x < SPIN; ++x) {
                                if (is_ring(pd_canvas_get_pixel(&out, x, y))) {
                                        ring_count += 1;
                                }
                        }
                }
                ctest_equal_bool(
                    "lifecycle: default content should render a visible ring",
                    ring_count >= 24, true);
        }
        pd_canvas_destroy(&out);

        ui_spinner_set_running(sp, true);
        ui_spinner_set_running(sp, false);
        ui_spinner_set_running(sp, true);

        probe_content = ui_create_widget("spinner-content");
        ui_spinner_set_content(sp, probe_content);
        ui_widget_destroy(sp);
}

/* 8. Default ring (regression lock): a bare spinner has an intrinsic size and
 * renders an inset ring whose stroke is present on the left, right and bottom
 * but broken by a transparent gap at the top center, with a hollow middle. The
 * ring is now inset from the box edge and drawn through the supersampled
 * rotation path, so exact pixels drift; assert over counted regions instead. */
static void check_default_ring(void)
{
        ui_widget_t *sp;
        pd_canvas_t out;
        int left, right, bottom, top_center, center, bottom_center, total;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        ui_widget_append(ui_root(), sp);
        /* No explicit resize: prove the default CSS width/height give the bare
         * spinner a non-zero box so it renders at all. */
        ui_spinner_set_running(sp, false);
        ui_spinner_set_angle(sp, 0);
        ui_widget_request_refresh_style(sp);
        ui_widget_each(sp, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
        ctest_equal_bool(
            "default ring: a bare spinner should have intrinsic "
            "size",
            sp->content_box.width >= 8.0f && sp->content_box.height >= 8.0f,
            true);

        render_into(sp, &out, RING, RING);
        total = count_ring(&out, 0, 0, RING, RING);
        left = count_ring(&out, 0, 0, RING / 2, RING);
        right = count_ring(&out, RING / 2, 0, RING, RING);
        bottom = count_ring(&out, 0, RING / 2, RING, RING);
        /* The gap is the top quarter of the ring; sample a center-width band of
         * the upper half so the (present) left/right arcs do not pollute it. */
        top_center = count_ring(&out, RING / 2 - 3, 0, RING / 2 + 3, RING / 2);
        center = count_ring(&out, RING / 2 - 2, RING / 2 - 2, RING / 2 + 2,
                            RING / 2 + 2);
        bottom_center =
            count_ring(&out, RING / 2 - 3, RING / 2, RING / 2 + 3, RING);

        ctest_equal_bool("default ring: the stroke should be visible",
                         total >= 16, true);
        ctest_equal_bool("default ring: the left half should carry the stroke",
                         left > 0, true);
        ctest_equal_bool("default ring: the right half should carry the stroke",
                         right > 0, true);
        ctest_equal_bool("default ring: the bottom should carry the stroke",
                         bottom > 0, true);
        /* Left and right halves are mirror arcs, so their counts stay balanced:
         * a rotation off the border center would skew one side. */
        ctest_equal_bool("default ring: the stroke should be roughly symmetric",
                         abs(left - right) <= 6, true);
        /* Top center is the gap: the base shows through, so it carries no
         * stroke, while the opposite (bottom center) does. */
        ctest_equal_bool("default ring: the top center should be the gap",
                         top_center == 0, true);
        ctest_equal_bool("default ring: the bottom center should carry the "
                         "stroke",
                         bottom_center > top_center, true);
        /* The center of a ring is empty, never the stroke. */
        ctest_equal_bool("default ring: the center should stay hollow",
                         center == 0, true);

        ui_widget_destroy(sp);
        pd_canvas_destroy(&out);
}

/* 8b. Ring centering: at angle 0 the ring is inset and drawn with no rotation,
 * so its stroke bounding box must sit equidistant from the left and right
 * edges. This is only true when the rotation pivot (now the pixel center)
 * coincides with the border engine's rounded-corner center; a half-pixel or
 * geometric-center mismatch would shift the whole ring toward one edge. The
 * non-zero margins also prove the inset leaves headroom for the AA so the ring
 * is not clipped by the capture bounds. Measured at a 40px box: left margin 3,
 * right margin 4. */
static void check_ring_centered(void)
{
        ui_widget_t *sp;
        pd_canvas_t out;
        int left_margin, right_margin;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, SPIN, SPIN, 0);
        render_into(sp, &out, SPIN, SPIN);

        left_margin = ring_left_margin(&out, SPIN, SPIN);
        right_margin = ring_right_margin(&out, SPIN, SPIN);
        ctest_equal_bool("centered: the ring should not touch an edge (inset)",
                         left_margin >= 1 && right_margin >= 1, true);
        ctest_equal_bool(
            "centered: the ring should be equidistant from the left and right",
            abs(left_margin - right_margin) <= 2, true);

        ui_widget_destroy(sp);
        pd_canvas_destroy(&out);
}

/* 8c. Pivot correctness via a rotation-invariant probe. A full (gap-free) ring
 * drawn by the border engine is unchanged by rotation about its drawn centre,
 * so its ink centroid must be IDENTICAL at every angle iff the rotation pivot
 * equals the border-drawn centre (the pixel midpoint (w-1)/2). If the pivot is
 * off -- e.g. the geometric box centre w/2 -- the ring orbits and the centroid
 * sweeps by ~twice the offset. This is what the earlier "spread <= 2px" check
 * was too loose to catch. Content is a custom full ring so the gap's asymmetry
 * is not a factor.
 */
static void check_pivot_invariant(void)
{
        static const int angles[] = { 0, 45, 90, 135, 180, 225, 270, 315 };
        ui_widget_t *sp, *content, *ring;
        float cxs[8], cys[8], bboxcx[8], bboxcy[8];
        int i, x, y;
        pd_canvas_t out;

        ui_metrics.dpi = 96;
        ui_load_css_string(
            ".pivot-full-ring { position: absolute; box-sizing: border-box; "
            "left: 15%; top: 15%; width: 70%; height: 70%; "
            "border-style: solid; border-width: 3px; border-color: #e5484d; "
            "border-radius: 9999px; }",
            "pivot-full-ring");
        sp = ui_create_spinner();
        content = ui_create_widget("spinner-content");
        ring = ui_create_widget(NULL);
        ui_widget_add_class(ring, "pivot-full-ring");
        ui_widget_append(content, ring);
        ui_spinner_set_content(sp, content);
        ui_widget_append(ui_root(), sp);

        for (i = 0; i < 8; ++i) {
                double sx = 0, sy = 0;
                int cnt = 0;
                int bx0 = 999, bx1 = -1, by0 = 999, by1 = -1;
                prime_spinner(sp, SPIN, SPIN, angles[i]);
                render_into(sp, &out, SPIN, SPIN);
                for (y = 0; y < SPIN; ++y) {
                        for (x = 0; x < SPIN; ++x) {
                                pd_color_t c = pd_canvas_get_pixel(&out, x, y);
                                if (c.a > 200 && c.r > 180 && c.g < 90 &&
                                    c.b < 90) {
                                        sx += x;
                                        sy += y;
                                        cnt += 1;
                                        if (x < bx0)
                                                bx0 = x;
                                        if (x > bx1)
                                                bx1 = x;
                                        if (y < by0)
                                                by0 = y;
                                        if (y > by1)
                                                by1 = y;
                                }
                        }
                }
                pd_canvas_destroy(&out);
                if (cnt < 1) {
                        cnt = 1;
                }
                cxs[i] = (float)(sx / cnt);
                cys[i] = (float)(sy / cnt);
                /* ink bounding-box centre: the "does the ring look centred"
                 * measure -- each edge's outer ink, not the averaged centroid.
                 */
                bboxcx[i] = (float)(bx0 + bx1) / 2.0f;
                bboxcy[i] = (float)(by0 + by1) / 2.0f;
        }
        {
                double mx = 0, my = 0;
                float ex = (float)(SPIN - 1) / 2.0f;
                for (i = 0; i < 8; ++i) {
                        mx += cxs[i];
                        my += cys[i];
                }
                mx /= 8.0;
                my /= 8.0;
                /* Averaging over a full rotation cancels the orbit radius and
                 * leaves the pivot's offset from the ring's drawn centre, which
                 * is what "is the ring centred?" really asks. The old per-angle
                 * spread check (<=2px) was far too loose to see this. */
                ctest_equal_bool(
                    "pivot: mean ring centroid must sit on the drawn centre",
                    (float)fabs(mx - ex) <= 0.25f &&
                        (float)fabs(my - ex) <= 0.25f,
                    true);
        }
        {
                float ex = (float)(SPIN - 1) / 2.0f;
                float nminx = bboxcx[0], nmaxx = bboxcx[0];
                float nminy = bboxcy[0], nmaxy = bboxcy[0];
                for (i = 1; i < 8; ++i) {
                        if (bboxcx[i] < nminx)
                                nminx = bboxcx[i];
                        if (bboxcx[i] > nmaxx)
                                nmaxx = bboxcx[i];
                        if (bboxcy[i] < nminy)
                                nminy = bboxcy[i];
                        if (bboxcy[i] > nmaxy)
                                nmaxy = bboxcy[i];
                }
                /* The rendered ring's outer silhouette (its bounding-box
                 * centre) must be equidistant from all four edges -- i.e. on
                 * (w-1)/2 -- and identical at every angle. This is the
                 * pixel-level stand-in for "it looks centred", which the
                 * centroid alone (an averaged point) can hide. */
                ctest_equal_bool(
                    "pivot: ring ink bbox centre sits on the box centre",
                    (float)fabs(nminx - ex) <= 0.6f &&
                        (float)fabs(nmaxx - ex) <= 0.6f &&
                        (float)fabs(nminy - ex) <= 0.6f &&
                        (float)fabs(nmaxy - ex) <= 0.6f,
                    true);
                ctest_equal_bool(
                    "pivot: ring ink bbox centre is rotation-invariant",
                    (nmaxx - nminx) <= 1.0f && (nmaxy - nminy) <= 1.0f, true);
        }

        ui_widget_destroy(sp);
}

/* 9. Rotation sensitivity at a non-right angle: rotating the inset ring 45
 * degrees sweeps the gap off the top center and moves the stroke centroid
 * horizontally. Both are geometric counts, so they hold despite the inset and
 * the supersampled rotation resampling the exact arc pixels. Measured at a 40px
 * box: centroid x 20.1 -> 16.5 (shifts left as the gap opens toward the
 * top-right), top-center stroke 0 -> non-zero. */
static void check_rotate45(void)
{
        ui_widget_t *sp;
        pd_canvas_t a0, a45;
        double cx0, cx45;
        int top0, top45;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, SPIN, SPIN, 0);
        render_into(sp, &a0, SPIN, SPIN);

        ui_spinner_set_angle(sp, 45);
        ui_widget_request_refresh_style(sp);
        ui_widget_each(sp, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
        render_into(sp, &a45, SPIN, SPIN);

        /* The gap is the transparent top border. It starts centered on top and
         * a real fractional rotation carries it off that column, so the top
         * center band goes from empty to stroked. */
        top0 = count_ring(&a0, SPIN / 2 - 6, 0, SPIN / 2 + 6, SPIN / 2);
        top45 = count_ring(&a45, SPIN / 2 - 6, 0, SPIN / 2 + 6, SPIN / 2);
        ctest_equal_bool("rotate45: the top center is the gap at angle 0",
                         top0 == 0, true);
        ctest_equal_bool("rotate45: the gap should sweep off the top center",
                         top45 > top0, true);
        /* The gap opening on the upper-right removes ink there, pulling the
         * stroke centroid to the left; a fixed ring would not move. */
        cx0 = ring_centroid_x(&a0, SPIN, SPIN);
        cx45 = ring_centroid_x(&a45, SPIN, SPIN);
        ctest_equal_bool("rotate45: the stroke centroid should shift left",
                         cx0 - cx45 >= 2.0, true);

        ui_widget_destroy(sp);
        pd_canvas_destroy(&a0);
        pd_canvas_destroy(&a45);
}

/* 10. step_angle: the extracted clock math wraps to [0, 360). */
static void check_step_angle(void)
{
        ctest_equal_float("step_angle: quarter turn at 1 rps",
                          (float)ui_spinner_step_angle(0, 1.0, 0.25), 90.0f);
        ctest_equal_float("step_angle: advance adds speed*dt*360",
                          (float)ui_spinner_step_angle(90, 1.0, 0.1), 126.0f);
        ctest_equal_float("step_angle: should wrap past 360 back into range",
                          (float)ui_spinner_step_angle(350, 1.0, 0.05), 8.0f);
        ctest_equal_bool("step_angle: wrapped angle stays in [0, 360)",
                         ui_spinner_step_angle(350, 1.0, 0.05) >= 0.0 &&
                             ui_spinner_step_angle(350, 1.0, 0.05) < 360.0,
                         true);
}

/* 11. HiDPI capture (regression for ui_widget_render re-anchor): a bare default
 * spinner must still capture its ring when device scale is 1.5 (Windows 150%).
 * Before the fix, offscreen capture of the off-origin content came out blank at
 * scale != 1, so the doc-viewer showed nothing. Count ring pixels rather than
 * pin one point, since the scaled geometry shifts sample positions. */
static void check_hidpi_capture(void)
{
        pd_canvas_t out;
        ui_widget_t *sp;
        int x, y, ring_count = 0;

        ui_metrics.dpi = 144;
        sp = ui_create_spinner();
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, RING, RING, 0);
        render_into(sp, &out, 24, 24);
        for (y = 0; y < 24; ++y) {
                for (x = 0; x < 24; ++x) {
                        if (is_ring(pd_canvas_get_pixel(&out, x, y))) {
                                ring_count += 1;
                        }
                }
        }
        ctest_equal_bool("hidpi: default ring should be captured at scale 1.5",
                         ring_count >= 12, true);
        pd_canvas_destroy(&out);
        ui_widget_destroy(sp);
        ui_metrics.dpi = 96;
}

/* 12. Ring colour follows CSS `color`: the CSS engine has no `current-color`,
 * so the widget copies the spinner's inherited colour into the ring's three
 * visible borders (top stays transparent as the gap). A red `color` must put
 * red-ish stroke pixels on the ring where the default grey would not match. */
static void check_color_stroke(void)
{
        ui_widget_t *sp;
        pd_canvas_t out;
        int x, y, red_count = 0, ring_count = 0;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        ui_widget_set_style_string(sp, "color", "#ff0000");
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, SPIN, SPIN, 0);
        render_into(sp, &out, SPIN, SPIN);
        for (y = 0; y < SPIN; ++y) {
                for (x = 0; x < SPIN; ++x) {
                        pd_color_t c = pd_canvas_get_pixel(&out, x, y);
                        if (is_redish(c)) {
                                red_count += 1;
                        }
                        if (is_ring(c)) {
                                ring_count += 1;
                        }
                }
        }
        /* Measured at 40px/2px stroke/96dpi: 158 red-ish pixels, so the
         * threshold keeps a real stroke, not a stray edge. */
        ctest_equal_bool("color: ring should be tinted by `color`",
                         red_count >= 64, true);
        ctest_equal_bool("color: the default grey stroke should be gone",
                         ring_count == 0, true);
        pd_canvas_destroy(&out);
        ui_widget_destroy(sp);
}

/* Render only the sub-region [rx,ry,rw,rh] (widget-canvas coords) into a
 * rw×rh canvas. Mirrors how the renderer hands a widget a partial dirty rect:
 * the canvas quote covers the whole small buffer while paint->rect names the
 * region in the widget's own canvas coordinates. */
static void render_region(ui_widget_t *sp, pd_canvas_t *out, int rx, int ry,
                          int rw, int rh)
{
        pd_rect_t q = { 0, 0, rw, rh };
        pd_context_t paint;

        make_argb(out, rw, rh);
        pd_canvas_fill(out, BASE);
        pd_canvas_init(&paint.canvas);
        paint.rect.x = rx;
        paint.rect.y = ry;
        paint.rect.width = rw;
        paint.rect.height = rh;
        paint.with_alpha = true;
        pd_canvas_quote(&paint.canvas, out, &q);
        ui_widget_render(sp, &paint);
}

/* 12. Partial repaint (regression): rendering just the bottom half of the
 * spinner must equal the same bottom half of a full render. The previous blit
 * of the whole box surface at a negative target offset made the bottom half
 * show the top half's content under a partial dirty rect. */
static void check_partial_repaint(void)
{
        pd_canvas_t full, part;
        int x, y, diff = 0;

        ui_metrics.dpi = 96;
        prime_spinner(scene.spinner, SPIN, SPIN, 45);
        render_region(scene.spinner, &full, 0, 0, SPIN, SPIN);
        render_region(scene.spinner, &part, 0, SPIN / 2, SPIN, SPIN / 2);
        for (y = 0; y < SPIN / 2; ++y) {
                for (x = 0; x < SPIN; ++x) {
                        pd_color_t a =
                            pd_canvas_get_pixel(&full, x, SPIN / 2 + y);
                        pd_color_t b = pd_canvas_get_pixel(&part, x, y);
                        if (abs((int)a.value - (int)b.value) != 0) {
                                diff += 1;
                        }
                }
        }
        ctest_equal_bool(
            "partial repaint: bottom half must match the same region of a "
            "full render",
            diff == 0, true);
        pd_canvas_destroy(&full);
        pd_canvas_destroy(&part);
}

/* 13. Partial repaint at 144 dpi (cached-layer regression): the same
 * half-versus-full comparison as above must hold at scale 1.5, where the
 * physical box (60 px) differs from the logical one (40 px) and the cache is
 * presented from the display surface the layer keeps between paints. */
static void check_partial_repaint_hidpi(void)
{
        pd_canvas_t full, part;
        int x, y, diff = 0;
        int phys = 60; /* ceil(40 * 1.5) */

        ui_metrics.dpi = 144;
        prime_spinner(scene.spinner, SPIN, SPIN, 45);
        render_region(scene.spinner, &full, 0, 0, phys, phys);
        render_region(scene.spinner, &part, 0, phys / 2, phys, phys / 2);
        for (y = 0; y < phys / 2; ++y) {
                for (x = 0; x < phys; ++x) {
                        pd_color_t a =
                            pd_canvas_get_pixel(&full, x, phys / 2 + y);
                        pd_color_t b = pd_canvas_get_pixel(&part, x, y);
                        if (abs((int)a.value - (int)b.value) != 0) {
                                diff += 1;
                        }
                }
        }
        ctest_equal_bool(
            "partial repaint hidpi: bottom half must match the same region of "
            "a full render at 144 dpi",
            diff == 0, true);
        pd_canvas_destroy(&full);
        pd_canvas_destroy(&part);
        ui_metrics.dpi = 96;
}

/* 14. Rotate centering (invariant test): a ring drawn by the border engine and
 * rotated about the pixel center must maintain constant outer contour radius
 * (rmax) and centroid offset from center (crad) across every angle. If the
 * rotation pivot does not coincide with the ring center, these quantities would
 * oscillate with angle. Tests both 96 and 144 dpi to catch half-pixel bias at
 * non-unity scale. Uses a 64px logical box for sufficient ring pixels. */
static void check_rotate_centering(void)
{
        ui_widget_t *sp;
        pd_canvas_t out;
        int angles[] = { 0, 45, 90, 135, 180, 225, 270, 315 };
        int nangles = 8;
        int ai, x, y;
        /* 96 dpi: logical 64, physical 64; center (31.5, 31.5) */
        int W96 = 64;
        double cx96 = (W96 - 1) / 2.0;
        double rmax96[8], crad96[8];
        int count96[8];
        /* 144 dpi: logical 64, physical 96; center (47.5, 47.5) */
        int W144 = 96; /* ceil(64 * 1.5) */
        double cx144 = (W144 - 1) / 2.0;
        double rmax144[8], crad144[8];
        int count144[8];

        double rmax_lo, rmax_hi, crad_lo, crad_hi;
        double rmax_spread, crad_spread;
        int cnt_lo, cnt_hi;

        sp = ui_create_spinner();
        ui_widget_append(ui_root(), sp);

        /* ---- 96 dpi ---- */
        ui_metrics.dpi = 96;
        for (ai = 0; ai < nangles; ++ai) {
                double sx = 0.0, sy = 0.0, rmax = 0.0;
                int cnt = 0;

                prime_spinner(sp, (float)W96, (float)W96, (double)angles[ai]);
                render_region(sp, &out, 0, 0, W96, W96);
                for (y = 0; y < W96; ++y) {
                        for (x = 0; x < W96; ++x) {
                                if (is_ring(pd_canvas_get_pixel(&out, x, y))) {
                                        double dx = x - cx96;
                                        double dy = y - cx96;
                                        double r = sqrt(dx * dx + dy * dy);
                                        sx += x;
                                        sy += y;
                                        cnt += 1;
                                        if (r > rmax) {
                                                rmax = r;
                                        }
                                }
                        }
                }
                rmax96[ai] = rmax;
                crad96[ai] = cnt > 0
                                 ? sqrt((sx / cnt - cx96) * (sx / cnt - cx96) +
                                        (sy / cnt - cx96) * (sy / cnt - cx96))
                                 : -1.0;
                count96[ai] = cnt;
                pd_canvas_destroy(&out);
        }

        printf("  [rotate centering 96dpi] angle  rmax    crad   count\n");
        for (ai = 0; ai < nangles; ++ai) {
                printf("    %3d  %8.3f  %7.3f  %5d\n", angles[ai], rmax96[ai],
                       crad96[ai], count96[ai]);
        }

        /* Compute spreads for 96 dpi */
        rmax_lo = rmax_hi = rmax96[0];
        crad_lo = crad_hi = crad96[0];
        cnt_lo = cnt_hi = count96[0];
        for (ai = 1; ai < nangles; ++ai) {
                if (rmax96[ai] < rmax_lo)
                        rmax_lo = rmax96[ai];
                if (rmax96[ai] > rmax_hi)
                        rmax_hi = rmax96[ai];
                if (crad96[ai] < crad_lo)
                        crad_lo = crad96[ai];
                if (crad96[ai] > crad_hi)
                        crad_hi = crad96[ai];
                if (count96[ai] < cnt_lo)
                        cnt_lo = count96[ai];
                if (count96[ai] > cnt_hi)
                        cnt_hi = count96[ai];
        }
        rmax_spread = rmax_hi - rmax_lo;
        crad_spread = crad_hi - crad_lo;

        ctest_equal_bool("centering 96: rmax spread should be <= 2 px",
                         rmax_spread <= 2.0, true);
        ctest_equal_bool("centering 96: crad spread should be <= 2 px",
                         crad_spread <= 2.0, true);
        ctest_equal_bool("centering 96: count spread should be <= 50%",
                         (double)(cnt_hi - cnt_lo) <= (double)cnt_hi * 0.50,
                         true);

        /* ---- 144 dpi ---- */
        ui_metrics.dpi = 144;
        for (ai = 0; ai < nangles; ++ai) {
                double sx = 0.0, sy = 0.0, rmax = 0.0;
                int cnt = 0;

                prime_spinner(sp, 64.0f, 64.0f, (double)angles[ai]);
                render_region(sp, &out, 0, 0, W144, W144);
                for (y = 0; y < W144; ++y) {
                        for (x = 0; x < W144; ++x) {
                                if (is_ring(pd_canvas_get_pixel(&out, x, y))) {
                                        double dx = x - cx144;
                                        double dy = y - cx144;
                                        double r = sqrt(dx * dx + dy * dy);
                                        sx += x;
                                        sy += y;
                                        cnt += 1;
                                        if (r > rmax) {
                                                rmax = r;
                                        }
                                }
                        }
                }
                rmax144[ai] = rmax;
                crad144[ai] =
                    cnt > 0 ? sqrt((sx / cnt - cx144) * (sx / cnt - cx144) +
                                   (sy / cnt - cx144) * (sy / cnt - cx144))
                            : -1.0;
                count144[ai] = cnt;
                pd_canvas_destroy(&out);
        }

        printf("  [rotate centering 144dpi] angle  rmax    crad   count\n");
        for (ai = 0; ai < nangles; ++ai) {
                printf("    %3d  %8.3f  %7.3f  %5d\n", angles[ai], rmax144[ai],
                       crad144[ai], count144[ai]);
        }

        /* Compute spreads for 144 dpi */
        rmax_lo = rmax_hi = rmax144[0];
        crad_lo = crad_hi = crad144[0];
        cnt_lo = cnt_hi = count144[0];
        for (ai = 1; ai < nangles; ++ai) {
                if (rmax144[ai] < rmax_lo)
                        rmax_lo = rmax144[ai];
                if (rmax144[ai] > rmax_hi)
                        rmax_hi = rmax144[ai];
                if (crad144[ai] < crad_lo)
                        crad_lo = crad144[ai];
                if (crad144[ai] > crad_hi)
                        crad_hi = crad144[ai];
                if (count144[ai] < cnt_lo)
                        cnt_lo = count144[ai];
                if (count144[ai] > cnt_hi)
                        cnt_hi = count144[ai];
        }
        rmax_spread = rmax_hi - rmax_lo;
        crad_spread = crad_hi - crad_lo;

        ctest_equal_bool("centering 144: rmax spread should be <= 2 px",
                         rmax_spread <= 2.0, true);
        ctest_equal_bool("centering 144: crad spread should be <= 2 px",
                         crad_spread <= 2.0, true);
        ctest_equal_bool("centering 144: count spread should be <= 50%",
                         (double)(cnt_hi - cnt_lo) <= (double)cnt_hi * 0.50,
                         true);

        ui_metrics.dpi = 96;
        ui_widget_destroy(sp);
}

/* 15. Text size under supersampled capture (regression): a text layer is
 * rasterised at the device scale active when it was laid out, so a self-drawn
 * widget that captures its subtree at a boosted scale must rebuild the layer in
 * paint. Otherwise the cached 1x glyph bitmaps are composited into a 2x backing
 * and the text shrinks by half. Compare the text composited through the
 * spinner's capture with the same text rendered on the normal path; their ink
 * extents must agree. The comparison is between two render paths, so it holds
 * regardless of the actual font metrics. */
static void text_ink_bbox(pd_canvas_t *out, int w, int h, int *bx, int *by,
                          int *bw, int *bh)
{
        int x, y, minx = w, miny = h, maxx = -1, maxy = -1;

        for (y = 0; y < h; ++y) {
                for (x = 0; x < w; ++x) {
                        if (!is_base(pd_canvas_get_pixel(out, x, y))) {
                                if (x < minx)
                                        minx = x;
                                if (x > maxx)
                                        maxx = x;
                                if (y < miny)
                                        miny = y;
                                if (y > maxy)
                                        maxy = y;
                        }
                }
        }
        *bx = maxx >= 0 ? minx : -1;
        *by = maxy >= 0 ? miny : -1;
        *bw = maxx >= 0 ? maxx - minx + 1 : 0;
        *bh = maxy >= 0 ? maxy - miny + 1 : 0;
}

static void check_text_capture_size(void)
{
        static const char *css =
            ".t-cap-demo spinner-content { display: flex; align-items: center; "
            "justify-content: center; }"
            ".cap-text { white-space: nowrap; font-size: 18px; "
            "color: #e5484d; }"
            ".t-cap-plain { display: flex; align-items: center; "
            "justify-content: center; }";
        ui_widget_t *sp, *content, *txt, *plain, *ptxt;
        pd_canvas_t cap, ref;
        int cx, cy, cw, ch, rx, ry, rw, rh;
        int width_tolerance, height_tolerance, margin_tolerance;
        char desc[160];

        ui_metrics.dpi = 96;
        ui_load_css_string(css, "t-cap-demo");

        /* Supersampled path: text inside a spinner's captured content. */
        sp = ui_create_spinner();
        ui_widget_add_class(sp, "t-cap-demo");
        content = ui_create_widget("spinner-content");
        txt = ui_create_widget("text");
        ui_text_set_content(txt, "Hello");
        ui_widget_add_class(txt, "cap-text");
        ui_widget_append(content, txt);
        ui_spinner_set_content(sp, content);
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, 96, 32, 0);
        render_into(sp, &cap, 96, 32);

        /* Reference path: the same text laid out and rendered normally. */
        plain = ui_create_widget(NULL);
        ui_widget_add_class(plain, "t-cap-plain");
        ptxt = ui_create_widget("text");
        ui_text_set_content(ptxt, "Hello");
        ui_widget_add_class(ptxt, "cap-text");
        ui_widget_append(plain, ptxt);
        ui_widget_append(ui_root(), plain);
        ui_widget_resize(plain, 96, 32);
        ui_widget_request_refresh_style(plain);
        ui_widget_each(plain, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
        render_into(plain, &ref, 96, 32);

        text_ink_bbox(&cap, 96, 32, &cx, &cy, &cw, &ch);
        text_ink_bbox(&ref, 96, 32, &rx, &ry, &rw, &rh);

        width_tolerance = rw / 16;
        if (width_tolerance < 2) {
                width_tolerance = 2;
        }
        height_tolerance = rh / 16;
        if (height_tolerance < 2) {
                height_tolerance = 2;
        }
        margin_tolerance = rw / 16;
        if (margin_tolerance < 2) {
                margin_tolerance = 2;
        }

        ctest_equal_bool("text capture: the captured text should be visible",
                         cw > 0 && ch > 0, true);
        snprintf(desc, sizeof(desc),
                 "text capture: width should match a normal render "
                 "(captured=%d, direct=%d, tolerance=%d)",
                 cw, rw, width_tolerance);
        ctest_equal_bool(desc, cw > 0 && abs(cw - rw) <= width_tolerance, true);
        snprintf(desc, sizeof(desc),
                 "text capture: height should match a normal render "
                 "(captured=%d, direct=%d, tolerance=%d)",
                 ch, rh, height_tolerance);
        ctest_equal_bool(desc, ch > 0 && abs(ch - rh) <= height_tolerance,
                         true);
        snprintf(desc, sizeof(desc),
                 "text capture: left margin should match "
                 "(captured=%d, direct=%d, tolerance=%d)",
                 cx, rx, margin_tolerance);
        ctest_equal_bool(desc, cw > 0 && abs(cx - rx) <= margin_tolerance,
                         true);

        pd_canvas_destroy(&cap);
        pd_canvas_destroy(&ref);
        ui_widget_destroy(sp);
        ui_widget_destroy(plain);
}

/* 16. Capture-cache invalidation (regression): growing the custom content
 * subtree must drop the cached capture rather than keep rotating the stale
 * bitmap, even though nothing about the spinner itself changed. */
static void check_content_cache_invalidates(void)
{
        ui_widget_t *sp, *content, *mark;
        pd_canvas_t empty, grown;

        ui_metrics.dpi = 96;
        sp = ui_create_spinner();
        content = ui_create_widget("spinner-content");
        ui_spinner_set_content(sp, content);
        ui_widget_append(ui_root(), sp);
        prime_spinner(sp, SPIN, SPIN, 0);
        render_into(sp, &empty, SPIN, SPIN);

        /* Child-list mutation: an opaque blue block fills the content box. No
         * style refresh follows, so here only the mutation observer can drop
         * the cached capture. */
        mark = ui_create_widget(NULL);
        ui_widget_add_class(mark, "mark-alt");
        ui_widget_append(content, mark);
        ui_update();
        ui_update();
        render_into(sp, &grown, SPIN, SPIN);

        ctest_equal_bool(
            "cache: an empty content should show the base",
            is_base(pd_canvas_get_pixel(&empty, SPIN / 2, SPIN / 2)), true);
        ctest_equal_bool(
            "cache: appending a child must re-capture",
            is_color(pd_canvas_get_pixel(&grown, SPIN / 2, SPIN / 2), BLUE_R,
                     BLUE_G, BLUE_B),
            true);

        pd_canvas_destroy(&empty);
        pd_canvas_destroy(&grown);
        ui_widget_destroy(sp);
}

void test_spinner(void)
{
        lcui_init();
        spinner_scene_build(&scene);

        ctest_describe("spinner identity", check_identity);
        ctest_describe("spinner rotate", check_rotate90);
        ctest_describe("spinner setattr", check_setattr);
        ctest_describe("spinner set content", check_set_content);
        ctest_describe("spinner clip", check_clip);
        ctest_describe("spinner resize parity", check_resize_parity);
        ctest_describe("spinner dpi parity", check_dpi_parity);
        ctest_describe("spinner lifecycle", check_lifecycle);
        ctest_describe("spinner pivot invariant", check_pivot_invariant);
        ctest_describe("spinner default ring", check_default_ring);
        ctest_describe("spinner ring centered", check_ring_centered);
        ctest_describe("spinner rotate 45", check_rotate45);
        ctest_describe("spinner step angle", check_step_angle);
        ctest_describe("spinner hidpi capture", check_hidpi_capture);
        ctest_describe("spinner partial repaint", check_partial_repaint);
        ctest_describe("spinner partial repaint hidpi",
                       check_partial_repaint_hidpi);
        ctest_describe("spinner rotate centering", check_rotate_centering);
        ctest_describe("spinner color stroke", check_color_stroke);
        ctest_describe("spinner text capture", check_text_capture_size);
        ctest_describe("spinner content cache",
                       check_content_cache_invalidates);

        ui_metrics.dpi = 96;
        lcui_destroy();
}
