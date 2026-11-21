/*
 * tests/integration/test_shimmer.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <LCUI.h>
#include <ctest-custom.h>
#include "../../src/widgets/draw_backend.h"
#include "../../src/widgets/shimmer_priv.h"
#include "../scenes/shimmer_scene.h"

#define BOX 80
/* The base canvas is filled with a dark grey and the content is composited at
 * full strength (so opaque `.content-bg` blue / `.half-left` red columns read
 * much brighter than the base), while the default highlight is a white sheen.
 * A scanned opaque column is therefore lifted toward the highlight colour; a
 * transparent column, or an opaque column outside the band, stays put. This
 * makes every "lifted / not lifted" comparison a real measurement, never a
 * tautology.
 */
#define BASE pd_rgb(16, 16, 16)
#define BASE_LUM 16

static shimmer_scene_t scene;

static void make_argb(pd_canvas_t *canvas, int width, int height)
{
        pd_canvas_init(canvas);
        canvas->color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(canvas, (unsigned)width, (unsigned)height);
}

/* Weighted luminance (Rec. 601). The default sheen is white, so lifting an
 * opaque content pixel always raises its luminance toward 255. */
static int lum_of(pd_color_t c)
{
        return (int)(0.299f * c.r + 0.587f * c.g + 0.114f * c.b);
}

static int is_base(pd_color_t c)
{
        return c.r < 30 && c.g < 30 && c.b < 30;
}

static void refresh_style_each(ui_widget_t *w, void *arg)
{
        ui_widget_request_refresh_style(w);
}

/* Prepare a shimmer for a deterministic single-frame render: stop the
 * animation so set_phase is authoritative, pin the spread, resize the box,
 * then refresh the whole subtree and reflow so the offscreen content is
 * measured at the current scale before we render. Three updates absorb the
 * one-pass layout lag from the resize callback that sizes the offscreen
 * content. */
static void prime_shimmer(ui_widget_t *sh, float width, float height,
                          float phase, float spread)
{
        ui_shimmer_set_running(sh, false);
        ui_shimmer_set_spread(sh, spread);
        ui_shimmer_set_phase(sh, phase);
        ui_widget_resize(sh, width, height);
        ui_widget_request_refresh_style(sh);
        ui_widget_each(sh, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
}

static void render_into(ui_widget_t *sh, pd_canvas_t *out, int cw, int ch)
{
        pd_rect_t rect = { 0, 0, cw, ch };
        pd_context_t paint;

        make_argb(out, cw, ch);
        pd_canvas_fill(out, BASE);
        paint.with_alpha = true;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, out, &rect);
        ui_widget_render(sh, &paint);
}

/* Build a fresh shimmer on the root and install a whole-box opaque blue
 * content (`.content-bg`) or a left-half-only opaque red content
 * (`.half-left`). An owned widget per case keeps the shared scene from
 * drifting when one case swaps the captured subtree. */
static ui_widget_t *make_shimmer(const char *child_class)
{
        ui_widget_t *sh = ui_create_shimmer();
        ui_widget_t *content = ui_create_widget("shimmer-content");
        ui_widget_t *child = ui_create_widget(NULL);

        ui_widget_append(ui_root(), sh);
        ui_widget_add_class(child, child_class);
        ui_widget_append(content, child);
        ui_shimmer_set_content(sh, content);
        return sh;
}

static int col_lum(pd_canvas_t *out, int x, int row)
{
        return lum_of(pd_canvas_get_pixel(out, x, row));
}

static int peak_column(pd_canvas_t *out, int row, int width)
{
        int x, best = -1, bestv = -1;

        for (x = 0; x < width; ++x) {
                int v = col_lum(out, x, row);
                if (v > bestv) {
                        bestv = v;
                        best = x;
                }
        }
        return best;
}

static int count_bright(pd_canvas_t *out, int row, int width, int thresh)
{
        int x, n = 0;

        for (x = 0; x < width; ++x) {
                if (col_lum(out, x, row) > thresh) {
                        n += 1;
                }
        }
        return n;
}

static double bright_centroid(pd_canvas_t *out, int row, int width, int thresh)
{
        int x;
        double sw = 0.0, sx = 0.0;

        for (x = 0; x < width; ++x) {
                int v = col_lum(out, x, row);
                if (v > thresh) {
                        double w = (double)(v - thresh);
                        sw += w;
                        sx += w * x;
                }
        }
        return sw > 0.0 ? sx / sw : -1.0;
}

/* 1. Sheen is clipped to the content alpha: over the half-left content the
 * band centered on the red/transparent boundary must brighten the opaque
 * (left) columns it covers while the transparent (right) columns it covers
 * stay at the bare base (the sheen is multiplied by the captured content
 * alpha, so nothing lands on transparent pixels). phase 0.5 / spread 30 puts
 * the band center at x=40 on an 80 px box, so both the opaque column 34 and
 * the transparent column 46 sit inside the band's footprint. */
static void check_mask_alpha(void)
{
        ui_widget_t *sh;
        pd_canvas_t out;
        int opaque_in_band, transparent_in_band, opaque_off_band;

        ui_metrics.dpi = 96;
        sh = make_shimmer("half-left");
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_into(sh, &out, BOX, BOX);
        opaque_in_band = col_lum(&out, 34, BOX / 2);
        transparent_in_band = col_lum(&out, 46, BOX / 2);
        opaque_off_band = col_lum(&out, 10, BOX / 2);

        ctest_equal_bool("mask: band over an opaque column should brighten it",
                         opaque_in_band > opaque_off_band + 60, true);
        ctest_equal_bool(
            "mask: band over a transparent column should not lift it",
            transparent_in_band - BASE_LUM < (opaque_in_band - BASE_LUM) / 3,
            true);
        ctest_equal_bool("mask: the transparent column should stay base-like",
                         is_base(pd_canvas_get_pixel(&out, 46, BOX / 2)), true);

        pd_canvas_destroy(&out);
        ui_widget_destroy(sh);
}

/* 2. A band is NOT a solid slab: over solid content the peak column must
 * clear the base far more than columns toward the band's left and right
 * edges. */
static void check_gradient(void)
{
        ui_widget_t *sh;
        pd_canvas_t out;
        int p, row = BOX / 2, center, left_edge, right_edge;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");
        prime_shimmer(sh, BOX, BOX, 0.5f, 40.0f);
        render_into(sh, &out, BOX, BOX);

        p = peak_column(&out, row, BOX);
        center = col_lum(&out, p, row);
        left_edge = col_lum(&out, p - 17, row);
        right_edge = col_lum(&out, p + 17, row);

        ctest_equal_bool("gradient: the band center should be strongly lifted",
                         center - BASE_LUM > 120, true);
        ctest_equal_bool("gradient: the left edge should be dimmer than center",
                         center - left_edge > 40, true);
        ctest_equal_bool(
            "gradient: the right edge should be dimmer than center",
            center - right_edge > 40, true);

        pd_canvas_destroy(&out);
        ui_widget_destroy(sh);
}

/* 3. Phase: as the phase advances the sheen footprint's centroid travels
 * left to right, and a phase past 1 wraps to the same spot (fmod). The new
 * center model keeps the band fully off-screen at phase 0 (center = -spread),
 * so the test uses in-flight phases rather than 0. */
static void check_phase(void)
{
        ui_widget_t *sh;
        pd_canvas_t c25, c75, c130, c30;
        double q25, q75, q130, q30;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");

        prime_shimmer(sh, BOX, BOX, 0.25f, 16.0f);
        render_into(sh, &c25, BOX, BOX);
        prime_shimmer(sh, BOX, BOX, 0.75f, 16.0f);
        render_into(sh, &c75, BOX, BOX);
        prime_shimmer(sh, BOX, BOX, 0.3f, 16.0f);
        render_into(sh, &c30, BOX, BOX);
        prime_shimmer(sh, BOX, BOX, 1.3f, 16.0f);
        render_into(sh, &c130, BOX, BOX);

        /* Threshold just above the opaque base so only sheened columns count.
         */
        q25 = bright_centroid(&c25, BOX / 2, BOX, BASE_LUM + 40);
        q75 = bright_centroid(&c75, BOX / 2, BOX, BASE_LUM + 40);
        q30 = bright_centroid(&c30, BOX / 2, BOX, BASE_LUM + 40);
        q130 = bright_centroid(&c130, BOX / 2, BOX, BASE_LUM + 40);

        ctest_equal_bool("phase: the band sits left of center at phase 0.25",
                         q25 > 0.0 && q25 < 20.0, true);
        ctest_equal_bool("phase: the band sits right of center at phase 0.75",
                         q75 > 60.0, true);
        ctest_equal_bool("phase: the band travels right as phase grows",
                         q75 > q25 + 20.0, true);
        ctest_equal_bool("phase: phase 1.3 wraps back to phase 0.3's spot",
                         fabs(q130 - q30) < 3.0, true);

        pd_canvas_destroy(&c25);
        pd_canvas_destroy(&c75);
        pd_canvas_destroy(&c30);
        pd_canvas_destroy(&c130);
        ui_widget_destroy(sh);
}

/* 4. Spread: a wider band brightens strictly more columns and keeps the
 * peak lift (the extra width spreads the sheen, it does not thin it). */
static void check_spread(void)
{
        ui_widget_t *sh;
        pd_canvas_t narrow, wide;
        int narrow_count, wide_count, narrow_peak, wide_peak;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");

        prime_shimmer(sh, BOX, BOX, 0.5f, 16.0f);
        render_into(sh, &narrow, BOX, BOX);
        narrow_count = count_bright(&narrow, BOX / 2, BOX, BASE_LUM + 40);
        narrow_peak =
            col_lum(&narrow, peak_column(&narrow, BOX / 2, BOX), BOX / 2);

        prime_shimmer(sh, BOX, BOX, 0.5f, 44.0f);
        render_into(sh, &wide, BOX, BOX);
        wide_count = count_bright(&wide, BOX / 2, BOX, BASE_LUM + 40);
        wide_peak = col_lum(&wide, peak_column(&wide, BOX / 2, BOX), BOX / 2);

        ctest_equal_bool("spread: a wider band should lift more columns",
                         wide_count > narrow_count, true);
        ctest_equal_bool("spread: the wider peak should not be dimmer",
                         wide_peak >= narrow_peak - 10, true);

        pd_canvas_destroy(&narrow);
        pd_canvas_destroy(&wide);
        ui_widget_destroy(sh);
}

/* 5. Angle: with a tilt the band's center shears with the row, so the
 * brightest column on the top row differs from that on the bottom row; with
 * angle 0 the two agree. */
static void check_angle(void)
{
        ui_widget_t *sh;
        pd_canvas_t flat, tilted;
        int flat_top, flat_bottom, tilt_top, tilt_bottom;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");

        ui_shimmer_set_angle(sh, 0.0f);
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_into(sh, &flat, BOX, BOX);
        flat_top = peak_column(&flat, 4, BOX);
        flat_bottom = peak_column(&flat, BOX - 5, BOX);

        ui_shimmer_set_angle(sh, 45.0f);
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_into(sh, &tilted, BOX, BOX);
        tilt_top = peak_column(&tilted, 4, BOX);
        tilt_bottom = peak_column(&tilted, BOX - 5, BOX);

        ctest_equal_bool(
            "angle: at angle 0 the top and bottom peaks should align",
            abs(flat_top - flat_bottom) < 6, true);
        ctest_equal_bool(
            "angle: at angle 45 the top and bottom peaks should separate",
            abs(tilt_top - tilt_bottom) > 18, true);

        pd_canvas_destroy(&flat);
        pd_canvas_destroy(&tilted);
        ui_widget_destroy(sh);
}

/* 6. Color: the default white sheen lifts blue content toward white (all
 * channels rise); setting a pure-red overlay instead drives the red channel
 * up while leaving green low, proving the highlight follows the colour and
 * is not a fixed white. */
static void check_color(void)
{
        ui_widget_t *sh;
        pd_canvas_t white_out, red_out;
        int wp, rp;
        pd_color_t wc, rc, offc;

        ui_metrics.dpi = 96;

        sh = make_shimmer("content-bg");
        ui_shimmer_set_color(sh, pd_argb(255, 255, 255, 255));
        prime_shimmer(sh, BOX, BOX, 0.5f, 40.0f);
        render_into(sh, &white_out, BOX, BOX);

        ui_shimmer_set_color(sh, pd_argb(255, 255, 0, 0));
        prime_shimmer(sh, BOX, BOX, 0.5f, 40.0f);
        render_into(sh, &red_out, BOX, BOX);

        wp = peak_column(&white_out, BOX / 2, BOX);
        rp = peak_column(&red_out, BOX / 2, BOX);
        wc = pd_canvas_get_pixel(&white_out, wp, BOX / 2);
        rc = pd_canvas_get_pixel(&red_out, rp, BOX / 2);
        offc = pd_canvas_get_pixel(&red_out, 4, BOX / 2);

        /* The white peak raises green as much as it raises red; the red peak
         * raises red but leaves green near the untouched blue content. */
        ctest_equal_bool("color: the white peak should be bright in green",
                         wc.g > 180, true);
        ctest_equal_bool("color: the red peak should be bright in red",
                         rc.r > 180, true);
        ctest_equal_bool("color: the red peak should stay low in green",
                         rc.g < rc.r - 80, true);
        ctest_equal_bool("color: an off-band column should not be red-shifted",
                         offc.r < 40, true);

        pd_canvas_destroy(&white_out);
        pd_canvas_destroy(&red_out);
        ui_widget_destroy(sh);
}

/* 7. DPI parity: the capture buffer is rebuilt at the device scale, so a
 * point inside the enlarged 192 buffer but outside the 96 buffer carries
 * content only at 192. */
static void check_dpi_parity(void)
{
        ui_widget_t *sh;
        pd_canvas_t c96, c192;
        int out_size = BOX * 2;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");
        prime_shimmer(sh, BOX, BOX, 0.0f, 16.0f);
        render_into(sh, &c96, out_size, out_size);

        ui_metrics.dpi = 192;
        prime_shimmer(sh, BOX, BOX, 0.0f, 16.0f);
        render_into(sh, &c192, out_size, out_size);

        ctest_equal_bool("dpi: inside the box should carry content at 96",
                         col_lum(&c96, 10, 10) > BASE_LUM, true);
        ctest_equal_bool("dpi: inside the box should carry content at 192",
                         col_lum(&c192, 10, 10) > BASE_LUM, true);
        ctest_equal_bool(
            "dpi: 96 buffer stays 80 (far corner is base color)",
            is_base(pd_canvas_get_pixel(&c96, out_size - 2, out_size - 2)),
            true);
        ctest_equal_bool(
            "dpi: 192 buffer grows to 160 (far corner shows content)",
            col_lum(&c192, out_size - 2, out_size - 2) > BASE_LUM, true);

        ui_metrics.dpi = 96;
        pd_canvas_destroy(&c96);
        pd_canvas_destroy(&c192);
        ui_widget_destroy(sh);
}

/* 8. Resize parity: after growing the box the buffers rebuild to the new
 * size, so fresh content appears in the enlarged area with no leftover. */
static void check_resize_parity(void)
{
        ui_widget_t *sh;
        pd_canvas_t small, big;

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");

        prime_shimmer(sh, 16, 16, 0.5f, 6.0f);
        render_into(sh, &small, 48, 48);

        prime_shimmer(sh, 48, 48, 0.5f, 18.0f);
        render_into(sh, &big, 48, 48);

        ctest_equal_bool(
            "resize: a 16px box should leave the outer area as base color",
            is_base(pd_canvas_get_pixel(&small, 40, 40)), true);
        ctest_equal_bool("resize: growing to 48 should fill the new area",
                         col_lum(&big, 4, 4) > BASE_LUM, true);
        ctest_equal_bool("resize: content should reach the 48px far corner",
                         col_lum(&big, 44, 44) > BASE_LUM, true);

        pd_canvas_destroy(&small);
        pd_canvas_destroy(&big);
        ui_widget_destroy(sh);
}

/* 9. Lifecycle: the factory + default content path works, running can be
 * toggled, set_content swaps the captured subtree, the new setters and
 * declarative attributes (including a CSS colour string) are accepted, and
 * destroy is safe. The default content is the whole-row gray skeleton block,
 * which is opaque, so every middle row carries content. */
static void check_lifecycle(void)
{
        ui_widget_t *sh, *alt;
        pd_canvas_t out;

        ui_metrics.dpi = 96;
        sh = ui_create_shimmer();
        ctest_equal_bool("lifecycle: factory should return a widget",
                         sh != NULL, true);

        ui_widget_append(ui_root(), sh);
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_into(sh, &out, BOX, BOX);
        ctest_equal_bool("lifecycle: the default content should render opaque "
                         "across the row",
                         col_lum(&out, BOX / 2, BOX / 2) > BASE_LUM, true);
        pd_canvas_destroy(&out);

        ui_shimmer_set_running(sh, true);
        ui_shimmer_set_running(sh, false);
        ui_shimmer_set_running(sh, true);

        alt = ui_create_widget("shimmer-content");
        ui_widget_add_class(alt, "content-bg");
        ui_shimmer_set_content(sh, alt);

        ui_shimmer_set_running(sh, false);
        ui_shimmer_set_duration(sh, 1000);
        ui_shimmer_set_angle(sh, 30.0f);
        ui_shimmer_set_color(sh, pd_argb(255, 0, 255, 255));
        ui_widget_set_attr(sh, "phase", "0.3");
        ui_widget_set_attr(sh, "spread", "24");
        ui_widget_set_attr(sh, "angle", "15");
        ui_widget_set_attr(sh, "duration", "1500");
        ui_widget_set_attr(sh, "color", "#ff8000");
        ui_widget_set_attr(sh, "running", "false");
        ui_widget_destroy(sh);
}

/* 10. Default full-row block (regression lock): a bare shimmer has an
 * intrinsic size and its default content fills the whole box, so every middle
 * row shows content edge to edge; the band column is brighter than an off-band
 * column, proving the content is composited at full strength (not dimmed) with
 * a sheen added on top. */
static void check_default_full_row(void)
{
        ui_widget_t *sh, *box;
        pd_canvas_t out;
        int x, w = 40, h = 20, row = 10, present = 0;

        ui_metrics.dpi = 96;
        box = ui_create_widget(NULL);
        ui_widget_append(ui_root(), box);
        ui_widget_resize(box, 120, 60);
        sh = ui_create_shimmer();
        ui_widget_append(box, sh);
        ui_widget_request_refresh_style(sh);
        ui_widget_each(sh, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
        ctest_equal_bool(
            "default row: a bare shimmer should have intrinsic size",
            sh->content_box.width >= 8.0f && sh->content_box.height >= 8.0f,
            true);

        ui_shimmer_set_running(sh, false);
        ui_shimmer_set_spread(sh, 12.0f);
        ui_shimmer_set_phase(sh, 0.5f);
        ui_widget_resize(sh, (float)w, (float)h);
        ui_widget_request_refresh_style(sh);
        ui_widget_each(sh, refresh_style_each, NULL);
        ui_update();
        ui_update();
        ui_update();
        render_into(sh, &out, w, h);

        for (x = 2; x < w - 2; ++x) {
                if (col_lum(&out, x, row) > BASE_LUM + 8) {
                        present += 1;
                }
        }
        ctest_equal_int("default row: the whole row should carry content",
                        present, w - 4);

        {
                int peak = peak_column(&out, row, w);
                ctest_equal_bool(
                    "default row: the band peak should be brighter than "
                    "off-band",
                    col_lum(&out, peak, row) > col_lum(&out, 4, row) + 20,
                    true);
        }

        ui_widget_destroy(box);
        pd_canvas_destroy(&out);
}

/* 11. step_phase: the extracted clock math advances by elapsed/duration,
 * wraps to [0, 1), folds negatives, and leaves the phase put for a zero or
 * unset duration. */
static void check_step_phase(void)
{
        ctest_equal_float("step_phase: half of a 2000ms sweep after 1000ms",
                          ui_shimmer_step_phase(0.0f, 1000.0, 2000), 0.5f);
        ctest_equal_float("step_phase: advancing past 1 wraps into range",
                          ui_shimmer_step_phase(0.8f, 600.0, 2000), 0.1f);
        ctest_equal_float("step_phase: a negative elapsed wraps forward",
                          ui_shimmer_step_phase(0.1f, -300.0, 2000), 0.95f);
        ctest_equal_float("step_phase: a zero duration leaves the phase",
                          ui_shimmer_step_phase(0.42f, 500.0, 0), 0.42f);
}

/* Render only the sub-region [rx,ry,rw,rh] (widget-canvas coords) into a
 * rw×rh canvas. Mirrors how the renderer hands a widget a partial dirty rect:
 * the canvas quote covers the whole small buffer while paint->rect names the
 * region in the widget's own canvas coordinates. */
static void render_region(ui_widget_t *sh, pd_canvas_t *out, int rx, int ry,
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
        ui_widget_render(sh, &paint);
}

/* Compare the bottom half of a partial render against the same bottom half of
 * a full render pixel by pixel (bit-exact); returns the number of differing
 * pixels. */
static int count_bottom_half_diff(pd_canvas_t *full, pd_canvas_t *part,
                                  int phys, int half)
{
        int x, y, diff = 0;

        for (y = 0; y < half; ++y) {
                for (x = 0; x < phys; ++x) {
                        pd_color_t a = pd_canvas_get_pixel(full, x, half + y);
                        pd_color_t b = pd_canvas_get_pixel(part, x, y);
                        if (abs((int)a.value - (int)b.value) != 0) {
                                diff += 1;
                        }
                }
        }
        return diff;
}

/* 12. Partial repaint (cached layer): rendering just the bottom half of the
 * shimmer must equal the same half of a full render, at 96 dpi and at 144 dpi
 * where the physical box differs from the logical one. The layer keeps the
 * composed display between paints and presents only the dirty sub-rect, so a
 * half-region repaint never re-renders or smears. */
static void check_partial_repaint(void)
{
        ui_widget_t *sh;
        pd_canvas_t full, part;
        int phys96 = BOX;          /* ceil(80 * 1.0) */
        int phys144 = BOX * 3 / 2; /* ceil(80 * 1.5) */

        ui_metrics.dpi = 96;
        sh = make_shimmer("content-bg");
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_region(sh, &full, 0, 0, phys96, phys96);
        render_region(sh, &part, 0, phys96 / 2, phys96, phys96 / 2);
        ctest_equal_bool(
            "partial repaint: bottom half must match the same region of a "
            "full render",
            count_bottom_half_diff(&full, &part, phys96, phys96 / 2) == 0,
            true);
        pd_canvas_destroy(&full);
        pd_canvas_destroy(&part);

        ui_metrics.dpi = 144;
        prime_shimmer(sh, BOX, BOX, 0.5f, 30.0f);
        render_region(sh, &full, 0, 0, phys144, phys144);
        render_region(sh, &part, 0, phys144 / 2, phys144, phys144 / 2);
        ctest_equal_bool(
            "partial repaint hidpi: bottom half must match the same region of "
            "a full render at 144 dpi",
            count_bottom_half_diff(&full, &part, phys144, phys144 / 2) == 0,
            true);
        pd_canvas_destroy(&full);
        pd_canvas_destroy(&part);
        ui_metrics.dpi = 96;
        ui_widget_destroy(sh);
}

/* 13. The sheen must only recolour the content's pixels, never change their
 * alpha: it lightens what is already there (RGB toward the highlight) but the
 * alpha the base content established must survive untouched -- otherwise text /
 * antialiased edges get their coverage rewritten (aliasing). Render identical
 * content with the band parked off-canvas (phase 0) and parked over the middle
 * (phase 0.5) onto a TRANSPARENT target; the content carries partial alpha
 * (opacity 0.5) so a wrong source-over would raise it. Every pixel's alpha must
 * match between the two frames; some pixel's RGB must differ (non-vacuous). */
static void render_transparent(ui_widget_t *sh, pd_canvas_t *out, int cw,
                               int ch)
{
        pd_rect_t rect = { 0, 0, cw, ch };
        pd_context_t paint;

        make_argb(out, cw, ch);
        pd_canvas_fill(out, pd_argb(0, 0, 0, 0));
        pd_canvas_init(&paint.canvas);
        paint.rect = rect;
        paint.with_alpha = true;
        pd_canvas_quote(&paint.canvas, out, &rect);
        ui_widget_render(sh, &paint);
}

static void check_sheen_preserves_alpha(void)
{
        ui_widget_t *sh, *content, *child;
        pd_canvas_t a, b;
        int x, y, alpha_changed = 0, rgb_changed = 0;
        const int w = 40;

        ui_metrics.dpi = 96;
        sh = ui_create_shimmer();
        content = ui_create_widget("shimmer-content");
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "content-bg");
        ui_widget_set_style_string(child, "opacity", "0.5");
        ui_widget_append(content, child);
        ui_shimmer_set_content(sh, content);
        ui_widget_append(ui_root(), sh);
        prime_shimmer(sh, 40.0f, 40.0f, 0.0f, 12.0f);
        render_transparent(sh, &a, w, w);
        ui_shimmer_set_phase(sh, 0.5f);
        render_transparent(sh, &b, w, w);
        for (y = 0; y < w; ++y) {
                for (x = 0; x < w; ++x) {
                        pd_color_t pa = pd_canvas_get_pixel(&a, x, y);
                        pd_color_t pb = pd_canvas_get_pixel(&b, x, y);
                        if (pa.a != pb.a) {
                                alpha_changed += 1;
                        }
                        if (pa.r != pb.r || pa.g != pb.g || pa.b != pb.b) {
                                rgb_changed += 1;
                        }
                }
        }
        ctest_equal_bool(
            "sheen: band must not alter any pixel's alpha vs a no-band frame",
            alpha_changed == 0, true);
        ctest_equal_bool(
            "sheen: at least one pixel's RGB is lifted (non-vacuous)",
            rgb_changed > 0, true);
        pd_canvas_destroy(&a);
        pd_canvas_destroy(&b);
        ui_widget_destroy(sh);
}

/* 14. Capture-cache invalidation (regression): the content capture is reused
 * between animation frames, so any change to the captured subtree must drop
 * it or the sweep would run over a stale bitmap. Grow the subtree (a
 * child-list mutation the observer must catch) and swap the whole content
 * node; both must show up in the next render. */
static void check_content_cache_invalidates(void)
{
        ui_widget_t *sh, *content, *child, *alt;
        pd_canvas_t empty, appended, swapped;
        int row = BOX / 2;

        ui_metrics.dpi = 96;
        sh = ui_create_shimmer();
        content = ui_create_widget("shimmer-content");
        ui_shimmer_set_content(sh, content);
        ui_widget_append(ui_root(), sh);
        prime_shimmer(sh, BOX, BOX, 0.0f, 10.0f);
        render_into(sh, &empty, BOX, BOX);

        /* Grow the subtree. No style refresh follows, so here only the
         * mutation observer can drop the cached capture. */
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "half-left");
        ui_widget_append(content, child);
        ui_update();
        ui_update();
        render_into(sh, &appended, BOX, BOX);

        /* Replace the whole content node: a widget hook, independent of the
         * observer. */
        alt = ui_create_widget("shimmer-content");
        child = ui_create_widget(NULL);
        ui_widget_add_class(child, "content-bg");
        ui_widget_append(alt, child);
        ui_shimmer_set_content(sh, alt);
        ui_update();
        ui_update();
        render_into(sh, &swapped, BOX, BOX);

        ctest_equal_bool("cache: an empty content should show the base",
                         is_base(pd_canvas_get_pixel(&empty, 4, row)), true);
        ctest_equal_bool("cache: appending a child must re-capture (observer)",
                         pd_canvas_get_pixel(&appended, 4, row).r > 200, true);
        ctest_equal_bool("cache: swapping the content node must re-capture",
                         pd_canvas_get_pixel(&swapped, BOX - 6, row).b > 200,
                         true);

        pd_canvas_destroy(&empty);
        pd_canvas_destroy(&appended);
        pd_canvas_destroy(&swapped);
        ui_widget_destroy(sh);
}

static void render_alpha_into(ui_widget_t *w, pd_canvas_t *out, int cw, int ch)
{
        pd_rect_t rect = { 0, 0, cw, ch };
        pd_context_t paint;

        make_argb(out, cw, ch);
        pd_canvas_fill(out, pd_argb(0, 0, 0, 0));
        paint.with_alpha = true;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, out, &rect);
        ui_widget_render(w, &paint);
}

static int rightmost_alpha(pd_canvas_t *out)
{
        int x;
        int y;

        for (x = (int)out->width - 1; x >= 0; --x) {
                for (y = 0; y < (int)out->height; ++y) {
                        if (pd_canvas_get_pixel(out, (unsigned)x, (unsigned)y)
                                .a > 20) {
                                return x;
                        }
                }
        }
        return -1;
}

/* The shimmer capture temporarily boosts the device scale. Text must be
 * retypeset at that scale, not restored to the previous layout pass's fixed
 * size, otherwise the right side of a long label is clipped. */
static void check_text_capture_full_width(void)
{
        const char *content_text = "design-system.zip";
        ui_widget_t *sh = ui_create_shimmer();
        ui_widget_t *content = ui_create_widget("shimmer-content");
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *reference = ui_create_widget("text");
        pd_canvas_t captured;
        pd_canvas_t direct;
        int captured_right;
        int direct_right;
        int delta;
        int tolerance;
        char desc[192];

        ui_metrics.dpi = 144;
        ui_widget_set_style_string(text, "font-size", "14px");
        ui_widget_set_style_string(text, "line-height", "20px");
        ui_widget_set_style_string(text, "white-space", "nowrap");
        ui_widget_set_style_string(text, "color", "#ffffff");
        ui_text_set_content(text, content_text);
        ui_widget_append(content, text);
        ui_shimmer_set_content(sh, content);
        ui_widget_append(ui_root(), sh);

        ui_widget_set_style_string(reference, "width", "240px");
        ui_widget_set_style_string(reference, "height", "20px");
        ui_widget_set_style_string(reference, "font-size", "14px");
        ui_widget_set_style_string(reference, "line-height", "20px");
        ui_widget_set_style_string(reference, "white-space", "nowrap");
        ui_widget_set_style_string(reference, "color", "#ffffff");
        ui_text_set_content(reference, content_text);
        ui_widget_append(ui_root(), reference);
        ui_widget_resize(reference, 240, 20);

        prime_shimmer(sh, 240, 20, 0.0f, 0.0f);
        ui_update();
        render_alpha_into(sh, &captured, 360, 30);
        render_alpha_into(reference, &direct, 360, 30);
        captured_right = rightmost_alpha(&captured);
        direct_right = rightmost_alpha(&direct);

        ctest_equal_bool("shimmer text capture should render text",
                         captured_right > 0, true);
        ctest_equal_bool("reference text should render text", direct_right > 0,
                         true);
        delta = abs(captured_right - direct_right);
        tolerance = direct_right / 16;
        if (tolerance < 6) {
                tolerance = 6;
        }
        snprintf(desc, sizeof(desc),
                 "shimmer capture should not clip the end of a long text label "
                 "(captured=%d, direct=%d, delta=%d, tolerance=%d)",
                 captured_right, direct_right, delta, tolerance);
        ctest_equal_bool(desc, delta <= tolerance, true);

        pd_canvas_destroy(&captured);
        pd_canvas_destroy(&direct);
        ui_widget_destroy(sh);
        ui_widget_destroy(reference);
        ui_metrics.dpi = 96;
}

void test_shimmer(void)
{
        lcui_init();
        shimmer_scene_build(&scene);

        ctest_describe("shimmer mask alpha", check_mask_alpha);
        ctest_describe("shimmer gradient", check_gradient);
        ctest_describe("shimmer phase", check_phase);
        ctest_describe("shimmer spread", check_spread);
        ctest_describe("shimmer angle", check_angle);
        ctest_describe("shimmer color", check_color);
        ctest_describe("shimmer dpi parity", check_dpi_parity);
        ctest_describe("shimmer resize parity", check_resize_parity);
        ctest_describe("shimmer lifecycle", check_lifecycle);
        ctest_describe("shimmer default full row", check_default_full_row);
        ctest_describe("shimmer step phase", check_step_phase);
        ctest_describe("shimmer partial repaint", check_partial_repaint);
        ctest_describe("shimmer sheen preserves alpha",
                       check_sheen_preserves_alpha);
        ctest_describe("shimmer content cache",
                       check_content_cache_invalidates);
        ctest_describe("shimmer text capture", check_text_capture_full_width);

        ui_metrics.dpi = 96;
        lcui_destroy();
}
