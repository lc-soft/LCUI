/*
 * src/widgets/draw_backend_software.c: software implementation of the internal
 * drawing seam. It is the only place that touches pd_canvas_t, the device
 * scale, or PandaGL canvas operations, so that self-drawn widgets can work
 * purely with opaque surfaces and logical coordinates.
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "draw_backend.h"

/* M_PI is not portably declared (MSVC hides it behind _USE_MATH_DEFINES). */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Supersample factor for the self-drawn surfaces. Content is rasterised into
 * a backing that is this many times the device resolution, every pixel
 * operation runs at that resolution, and the result is box-downsampled back to
 * device resolution only when it is blended onto a paint target. A value of 1
 * disables supersampling entirely and reproduces the pre-supersample
 * behaviour exactly. */
#define UI_DRAW_SUPERSAMPLE 2

struct ui_draw_surface {
        pd_canvas_t canvas;
        /* Device-resolution scratch used only to downsample `canvas` (which is
         * UI_DRAW_SUPERSAMPLE times larger) before it is blended onto a target.
         * It is retained between calls so supersampling allocates nothing per
         * frame; only a size change rebuilds it. Unused when ss == 1. */
        pd_canvas_t scratch;
        int scratch_w;
        int scratch_h;
        float logical_w;
        float logical_h;
        /* phys_w/phys_h are the *device* pixel size (ceil(logical * scale));
         * the backing canvas is phys_w * ss by phys_h * ss. */
        int phys_w;
        int phys_h;
        int ss;
};

struct ui_draw_layer {
        ui_draw_surface_t *display;
        int dirty;
};

/* Convert a logical rectangle to a physical rectangle, rounding the outer
 * edges outward so the intended coverage is fully represented. */
static void logical_to_phys_rect(float x, float y, float w, float h,
                                 double scale, pd_rect_t *out)
{
        float rx0 = x * (float)scale;
        float rx1 = (x + w) * (float)scale;
        float ry0 = y * (float)scale;
        float ry1 = (y + h) * (float)scale;
        int px = (int)floorf(rx0);
        int py = (int)floorf(ry0);

        out->x = px;
        out->y = py;
        out->width = (int)ceilf(rx1) - px;
        out->height = (int)ceilf(ry1) - py;
}

// object 1: surface lifecycle

ui_draw_surface_t *ui_draw_surface_create(void)
{
        ui_draw_surface_t *s = malloc(sizeof(ui_draw_surface_t));

        if (!s) {
                return NULL;
        }
        memset(s, 0, sizeof(*s));
        pd_canvas_init(&s->canvas);
        pd_canvas_init(&s->scratch);
        return s;
}

void ui_draw_surface_destroy(ui_draw_surface_t *s)
{
        if (!s) {
                return;
        }
        pd_canvas_destroy(&s->canvas);
        pd_canvas_destroy(&s->scratch);
        free(s);
}

int ui_draw_surface_resize(ui_draw_surface_t *s, float logical_w,
                           float logical_h)
{
        double scale = ui_get_actual_scale();
        int ss = UI_DRAW_SUPERSAMPLE < 1 ? 1 : UI_DRAW_SUPERSAMPLE;
        int phys_w = (int)ceil((double)logical_w * scale);
        int phys_h = (int)ceil((double)logical_h * scale);
        int back_w = phys_w * ss;
        int back_h = phys_h * ss;

        if (!s) {
                return -1;
        }
        if (phys_w < 1 || phys_h < 1) {
                if (s->phys_w == phys_w && s->phys_h == phys_h) {
                        return 0;
                }
                pd_canvas_destroy(&s->canvas);
                s->phys_w = phys_w;
                s->phys_h = phys_h;
                s->ss = ss;
                s->logical_w = logical_w;
                s->logical_h = logical_h;
                return 1;
        }
        if (s->phys_w == phys_w && s->phys_h == phys_h && s->ss == ss &&
            pd_canvas_is_valid(&s->canvas)) {
                s->logical_w = logical_w;
                s->logical_h = logical_h;
                return 0;
        }
        pd_canvas_destroy(&s->canvas);
        s->canvas.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&s->canvas, (unsigned)back_w, (unsigned)back_h);
        s->phys_w = phys_w;
        s->phys_h = phys_h;
        s->ss = ss;
        s->logical_w = logical_w;
        s->logical_h = logical_h;
        return 1;
}

int ui_draw_surface_logical_size(const ui_draw_surface_t *s, float *w, float *h)
{
        if (!s || s->phys_w < 1 || s->phys_h < 1) {
                return -1;
        }
        if (w) {
                *w = s->logical_w;
        }
        if (h) {
                *h = s->logical_h;
        }
        return 0;
}

// object 2: pixel operations on surfaces

int ui_draw_capture(ui_widget_t *widget, ui_draw_surface_t *dst)
{
        pd_context_t paint;
        pd_rect_t rect;
        float saved_scale;
        int ss;

        if (!dst || !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        rect.x = 0;
        rect.y = 0;
        rect.width = (int)dst->canvas.width;
        rect.height = (int)dst->canvas.height;
        /* Clear before rendering: a self-drawn widget re-captures its subtree
         * into the same persistent surface every animation frame, and the
         * render composites source-over, so without this the partial-alpha
         * edges of text/AA content accumulate toward full opacity across
         * frames. */
        pd_canvas_fill(&dst->canvas, pd_argb(0, 0, 0, 0));
        pd_canvas_init(&paint.canvas);
        paint.rect = rect;
        paint.with_alpha = true;
        pd_canvas_quote(&paint.canvas, &dst->canvas, &rect);
        /* The surface backing is ss times the device resolution, so rasterise
         * the subtree at ss times the active scale: ui_widget_render derives
         * every device coordinate from ui_get_actual_scale(), and the paint
         * target is the ss-sized backing, so the whole subtree fills it at the
         * supersampled resolution. The global scale is restored immediately so
         * nothing outside the capture observes the temporary boost. */
        ss = dst->ss > 0 ? dst->ss : 1;
        saved_scale = ui_metrics.scale;
        ui_metrics.scale = saved_scale * (float)ss;
        ui_widget_render(widget, &paint);
        ui_metrics.scale = saved_scale;
        return 0;
}

/* Bilinear fetch at a floating-point source coordinate, reading the ARGB
 * backing directly. Out-of-bounds samples read as fully transparent so they can
 * be averaged into a supersampled result without a separate coverage term. */
static pd_color_t sample_bilinear(const pd_color_t *px, int stride, unsigned w,
                                  unsigned h, float sx, float sy)
{
        pd_color_t a, b, c, e, t;
        int ix, iy, ix1, iy1;
        float fx, fy;

        if (sx < 0 || sy < 0 || sx > (float)(w - 1) || sy > (float)(h - 1)) {
                return pd_argb(0, 0, 0, 0);
        }
        ix = (int)sx;
        iy = (int)sy;
        fx = sx - (float)ix;
        fy = sy - (float)iy;
        ix1 = ix + 1 < (int)w ? ix + 1 : ix;
        iy1 = iy + 1 < (int)h ? iy + 1 : iy;
        a = px[(size_t)iy * stride + ix];
        b = px[(size_t)iy * stride + ix1];
        c = px[(size_t)iy1 * stride + ix];
        e = px[(size_t)iy1 * stride + ix1];
        t.b = (uint8_t)(a.b * (1 - fx) * (1 - fy) + b.b * fx * (1 - fy) +
                        c.b * (1 - fx) * fy + e.b * fx * fy);
        t.g = (uint8_t)(a.g * (1 - fx) * (1 - fy) + b.g * fx * (1 - fy) +
                        c.g * (1 - fx) * fy + e.g * fx * fy);
        t.r = (uint8_t)(a.r * (1 - fx) * (1 - fy) + b.r * fx * (1 - fy) +
                        c.r * (1 - fx) * fy + e.r * fx * fy);
        t.a = (uint8_t)(a.a * (1 - fx) * (1 - fy) + b.a * fx * (1 - fy) +
                        c.a * (1 - fx) * fy + e.a * fx * fy);
        return t;
}

/* Supersampling grid per destination pixel when rotating. A single bilinear
 * tap leaves resampling stair-steps on curved, high-contrast edges; averaging
 * SS x SS sub-samples spread across the pixel center turns those steps into a
 * smooth ramp. It is purely per-pixel: no scratch buffer is allocated. */
#define UI_ROTATE_SUPERSAMPLE 4

int ui_draw_rotate_into(const ui_draw_surface_t *src, ui_draw_surface_t *dst,
                        double radians)
{
        const pd_canvas_t *s;
        pd_canvas_t *d;
        const pd_color_t *sargb;
        pd_color_t *dargb;
        int sstride, dstride;
        int x, y, i, j;
        int taps;
        unsigned w, h;
        float cos_r, sin_r, cx, cy, ss;

        if (!src || !dst) {
                return -1;
        }
        s = &src->canvas;
        d = &dst->canvas;
        if (!pd_canvas_is_valid(s) || !pd_canvas_is_valid(d)) {
                return -1;
        }
        if (s->width != d->width || s->height != d->height) {
                return -1;
        }
        /* Pixel access goes straight through the ARGB backing rather than the
         * per-pixel helpers: this loop is the hottest path for animation, and
         * the helpers add a call plus a modulo per sample. */
        sargb = s->argb;
        dargb = d->argb;
        sstride = (int)(s->bytes_per_row / sizeof(pd_color_t));
        dstride = (int)(d->bytes_per_row / sizeof(pd_color_t));
        /* When the source is already supersampled, the per-pixel grid is
         * redundant: the box downsample at present time supplies the
         * anti-aliasing, so a single bilinear tap per pixel is enough and
         * roughly sixteen times cheaper than the 1x-quality grid. */
        taps = src->ss > 1 ? 1 : UI_ROTATE_SUPERSAMPLE;
        ss = (float)taps;
        w = s->width;
        h = s->height;
        /* Rotate about the same centre the border engine draws the circle on:
         * CIRCLE_R = radius - 0.5 and the arc is centred at bound + radius -
         * 0.5, which for a ring centred in the surface is the pixel-index
         * midpoint (w-1)/2. The geometric box centre w/2 is a half pixel too
         * far and makes the ring visibly orbit as it rotates. */
        cx = (float)(w - 1) / 2.0f;
        cy = (float)(h - 1) / 2.0f;
        cos_r = (float)cos(radians);
        sin_r = (float)sin(radians);

        if (taps == 1) {
                for (y = 0; y < (int)h; y++) {
                        pd_color_t *row = dargb + (size_t)y * dstride;
                        float dy = (float)y - cy;

                        for (x = 0; x < (int)w; x++) {
                                float dx = (float)x - cx;
                                float sx = cos_r * dx + sin_r * dy + cx;
                                float sy = -sin_r * dx + cos_r * dy + cy;

                                row[x] = sample_bilinear(sargb, sstride, w, h,
                                                         sx, sy);
                        }
                }
                return 0;
        }

        for (y = 0; y < (int)h; y++) {
                pd_color_t *row = dargb + (size_t)y * dstride;

                for (x = 0; x < (int)w; x++) {
                        pd_color_t t;
                        float acc_r = 0.f, acc_g = 0.f, acc_b = 0.f,
                              acc_a = 0.f;
                        float inv_ss = (float)(1.0 / (ss * ss));

                        for (j = 0; j < taps; j++) {
                                for (i = 0; i < taps; i++) {
                                        /* Sub-sample at the center of the
                                         * (i, j) cell of the pixel's grid. */
                                        float px = (float)x +
                                                   ((float)i + 0.5f) / ss -
                                                   0.5f;
                                        float py = (float)y +
                                                   ((float)j + 0.5f) / ss -
                                                   0.5f;
                                        float dx = px - cx;
                                        float dy = py - cy;
                                        float sx = cos_r * dx + sin_r * dy + cx;
                                        float sy =
                                            -sin_r * dx + cos_r * dy + cy;
                                        pd_color_t c = sample_bilinear(
                                            sargb, sstride, w, h, sx, sy);

                                        acc_r += (float)c.r;
                                        acc_g += (float)c.g;
                                        acc_b += (float)c.b;
                                        acc_a += (float)c.a;
                                }
                        }
                        t.r = (uint8_t)(acc_r * inv_ss);
                        t.g = (uint8_t)(acc_g * inv_ss);
                        t.b = (uint8_t)(acc_b * inv_ss);
                        t.a = (uint8_t)(acc_a * inv_ss);
                        row[x] = t;
                }
        }
        return 0;
}

int ui_draw_clear(ui_draw_surface_t *dst)
{
        if (!dst || !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        pd_canvas_fill(&dst->canvas, pd_argb(0, 0, 0, 0));
        return 0;
}

int ui_draw_fill_rect(ui_draw_surface_t *dst, float x_logical, float y_logical,
                      float w_logical, float h_logical, unsigned char r,
                      unsigned char g, unsigned char b, unsigned char a)
{
        pd_rect_t rect;

        if (!dst || !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        /* Rects are logical; the backing is ss times the device scale, so the
         * logical-to-physical factor includes the supersample. */
        logical_to_phys_rect(
            x_logical, y_logical, w_logical, h_logical,
            ui_get_actual_scale() * (dst->ss > 0 ? dst->ss : 1), &rect);
        pd_canvas_fill_rect(&dst->canvas, pd_argb(a, r, g, b), rect);
        return 0;
}

/* Symmetric soft falloff across the sheen: 1 at the band center line, 0 at
 * either edge, with a zero slope across the whole span so the highlight fades
 * smoothly into transparency instead of showing a hard edge. It is a raised
 * cosine over the normalized distance d in [0, 1] from the center line. */
static float sheen_falloff(float d)
{
        if (d <= 0.0f) {
                return 1.0f;
        }
        if (d >= 1.0f) {
                return 0.0f;
        }
        return 0.5f * (1.0f + cosf((float)M_PI * d));
}

int ui_draw_fill_sheen(ui_draw_surface_t *dst, pd_color_t color,
                       float center_x_logical, float spread_logical,
                       float angle_deg)
{
        pd_canvas_t *c;
        float half, tan_a, midy, lx_scale, ly_scale;
        int px, py;

        if (!dst || !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        c = &dst->canvas;
        if (c->width < 1 || c->height < 1 || dst->logical_w < 1.0f ||
            dst->logical_h < 1.0f) {
                return -1;
        }
        /* Map each physical pixel back to the surface's own logical space so
         * the band is described in logical units regardless of the active
         * scale. */
        lx_scale = dst->logical_w / (float)c->width;
        ly_scale = dst->logical_h / (float)c->height;
        half = spread_logical * 0.5f;
        if (half < 0.5f) {
                half = 0.5f;
        }
        tan_a = tanf(angle_deg * (float)(M_PI / 180.0));
        midy = dst->logical_h * 0.5f;

        for (py = 0; py < (int)c->height; py++) {
                float ly = ((float)py + 0.5f) * ly_scale;
                /* The center line is sheared by the tilt, proportional to the
                 * distance of this row from the surface's logical middle. */
                float shear = tan_a * (ly - midy);
                for (px = 0; px < (int)c->width; px++) {
                        float lx = ((float)px + 0.5f) * lx_scale;
                        float u = (lx - center_x_logical) + shear;
                        float d = fabsf(u) / half;
                        float f = sheen_falloff(d);
                        pd_color_t col;

                        col = pd_argb((unsigned char)(color.a * f + 0.5f),
                                      color.r, color.g, color.b);
                        pd_canvas_set_pixel(c, (unsigned)px, (unsigned)py, col);
                }
        }
        return 0;
}

int ui_draw_copy(const ui_draw_surface_t *src, ui_draw_surface_t *dst)
{
        if (!src || !dst) {
                return -1;
        }
        if (!pd_canvas_is_valid(&src->canvas) ||
            !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        return pd_canvas_replace(&dst->canvas, &src->canvas, 0, 0) < 0 ? -1 : 0;
}

int ui_draw_mask_into(const ui_draw_surface_t *mask, ui_draw_surface_t *dst)
{
        if (!mask || !dst) {
                return -1;
        }
        if (!pd_canvas_is_valid(&mask->canvas) ||
            !pd_canvas_is_valid(&dst->canvas)) {
                return -1;
        }
        return pd_canvas_mask_composite(&dst->canvas, &mask->canvas) < 0 ? -1
                                                                         : 0;
}

int ui_draw_sheen_over(ui_draw_surface_t *dst, const ui_draw_surface_t *sheen)
{
        pd_canvas_t *d;
        const pd_canvas_t *s;
        unsigned x, y, w, h;

        if (!dst || !sheen) {
                return -1;
        }
        d = &dst->canvas;
        s = &sheen->canvas;
        if (!pd_canvas_is_valid(d) || !pd_canvas_is_valid(s)) {
                return -1;
        }
        if (d->width != s->width || d->height != s->height) {
                return -1;
        }
        w = d->width;
        h = d->height;
        for (y = 0; y < h; ++y) {
                for (x = 0; x < w; ++x) {
                        pd_color_t sp = pd_canvas_get_pixel(s, x, y);
                        pd_color_t dp;
                        float t;
                        if (sp.a == 0) {
                                continue;
                        }
                        dp = pd_canvas_get_pixel(d, x, y);
                        t = sp.a / 255.0f;
                        dp.r = (uint8_t)(dp.r * (1.0f - t) + sp.r * t + 0.5f);
                        dp.g = (uint8_t)(dp.g * (1.0f - t) + sp.g * t + 0.5f);
                        dp.b = (uint8_t)(dp.b * (1.0f - t) + sp.b * t + 0.5f);
                        /* dp.a intentionally left unchanged: the sheen must not
                         * alter the coverage of the pixels it recolours. */
                        pd_canvas_set_pixel(d, x, y, dp);
                }
        }
        return 0;
}

/* Return the surface's device-resolution downsample buffer, growing it on
 * demand. The buffer is retained on the surface so a supersampled present
 * allocates only when the required device size grows, never per frame. */
static pd_canvas_t *surface_scratch(ui_draw_surface_t *s, int w, int h)
{
        int nw, nh;

        if (w < 1 || h < 1) {
                return NULL;
        }
        if (pd_canvas_is_valid(&s->scratch) && w <= s->scratch_w &&
            h <= s->scratch_h) {
                return &s->scratch;
        }
        nw = w > s->scratch_w ? w : s->scratch_w;
        nh = h > s->scratch_h ? h : s->scratch_h;
        pd_canvas_destroy(&s->scratch);
        s->scratch.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&s->scratch, (unsigned)nw, (unsigned)nh);
        s->scratch_w = nw;
        s->scratch_h = nh;
        return &s->scratch;
}

/* Box-downsample an ss-times supersampled region of src, starting at
 * (src_x, src_y) and covering out_w * ss by out_h * ss pixels, into dst at
 * (dst_x, dst_y) at device resolution. Channels are averaged in premultiplied
 * space and then un-premultiplied so a partially covered pixel keeps its
 * colour instead of darkening toward the transparent (0,0,0,0) samples beside
 * it; sources outside src read as transparent. */
static void downsample_box(pd_canvas_t *dst, int dst_x, int dst_y,
                           const pd_canvas_t *src, int src_x, int src_y,
                           int out_w, int out_h, int ss)
{
        const pd_color_t *spx = src->argb;
        pd_color_t *dpx = dst->argb;
        int sstride = (int)(src->bytes_per_row / sizeof(pd_color_t));
        int dstride = (int)(dst->bytes_per_row / sizeof(pd_color_t));
        int sw = (int)src->width;
        int sh = (int)src->height;
        int ox, oy, sx, sy;
        int samples = ss * ss;

        for (oy = 0; oy < out_h; ++oy) {
                pd_color_t *drow = dpx + (size_t)(dst_y + oy) * dstride + dst_x;

                for (ox = 0; ox < out_w; ++ox) {
                        unsigned sum_a = 0, sum_pr = 0, sum_pg = 0, sum_pb = 0;
                        pd_color_t col;

                        for (sy = 0; sy < ss; ++sy) {
                                int py = src_y + oy * ss + sy;
                                const pd_color_t *srow;

                                if (py < 0 || py >= sh) {
                                        continue;
                                }
                                srow = spx + (size_t)py * sstride;
                                for (sx = 0; sx < ss; ++sx) {
                                        int px = src_x + ox * ss + sx;
                                        pd_color_t c;

                                        if (px < 0 || px >= sw) {
                                                continue;
                                        }
                                        c = srow[px];
                                        sum_a += c.a;
                                        sum_pr += (unsigned)c.r * c.a;
                                        sum_pg += (unsigned)c.g * c.a;
                                        sum_pb += (unsigned)c.b * c.a;
                                }
                        }
                        col.a = (uint8_t)(sum_a / samples);
                        if (sum_a > 0) {
                                col.r = (uint8_t)(sum_pr / sum_a);
                                col.g = (uint8_t)(sum_pg / sum_a);
                                col.b = (uint8_t)(sum_pb / sum_a);
                        } else {
                                col.r = col.g = col.b = 0;
                        }
                        drow[ox] = col;
                }
        }
}

int ui_draw_blend_surface(pd_context_t *target, const ui_draw_surface_t *src,
                          int x, int y, bool with_alpha, float opacity)
{
        ui_draw_surface_t *s;
        pd_canvas_t *scratch, sub;
        pd_rect_t r;
        int ret;

        if (!src || !target) {
                return -1;
        }
        /* The surface owns a mutable backing buffer; the const qualifier only
         * guards callers from naming it, so opacity is applied through the
         * underlying blend by setting the buffer's transient opacity field. */
        s = (ui_draw_surface_t *)src;
        if (s->ss > 1 && pd_canvas_is_valid(&s->canvas)) {
                /* Downsample the supersampled backing to device resolution
                 * first; blending the raw ss-sized backing 1:1 would clip the
                 * bottom/right of the target. */
                scratch = surface_scratch(s, s->phys_w, s->phys_h);
                if (!scratch) {
                        return -1;
                }
                downsample_box(scratch, 0, 0, &s->canvas, 0, 0, s->phys_w,
                               s->phys_h, s->ss);
                r.x = 0;
                r.y = 0;
                r.width = s->phys_w;
                r.height = s->phys_h;
                pd_canvas_init(&sub);
                if (pd_canvas_quote(&sub, scratch, &r) < 0) {
                        return -1;
                }
                sub.opacity = opacity;
                ret = pd_canvas_mix(&target->canvas, &sub, x, y, with_alpha);
                return ret < 0 ? ret : 0;
        }
        s->canvas.opacity = opacity;
        ret = pd_canvas_mix(&target->canvas, &s->canvas, x, y, with_alpha);
        s->canvas.opacity = 1.0;
        return ret < 0 ? ret : 0;
}

int ui_draw_blend_surface_rect(pd_context_t *target,
                               const ui_draw_surface_t *src, int src_x,
                               int src_y, int src_w, int src_h, int dst_x,
                               int dst_y, bool with_alpha, float opacity)
{
        ui_draw_surface_t *s;
        pd_canvas_t *scratch, sub;
        pd_rect_t r;
        int ret;

        if (!src || !target || src_w <= 0 || src_h <= 0) {
                return 0;
        }
        if (!pd_canvas_is_valid(&src->canvas)) {
                return -1;
        }
        s = (ui_draw_surface_t *)src;
        if (s->ss > 1) {
                /* src_x/src_y/src_w/src_h are in the surface's device pixel
                 * space; downsample the matching ss-scaled region of the
                 * backing into the device-sized scratch, then blend that. */
                scratch = surface_scratch(s, src_w, src_h);
                if (!scratch) {
                        return 0;
                }
                downsample_box(scratch, 0, 0, &s->canvas, src_x * s->ss,
                               src_y * s->ss, src_w, src_h, s->ss);
                r.x = 0;
                r.y = 0;
                r.width = src_w;
                r.height = src_h;
                pd_canvas_init(&sub);
                if (pd_canvas_quote(&sub, scratch, &r) < 0) {
                        return -1;
                }
                sub.opacity = opacity;
                ret = pd_canvas_mix(&target->canvas, &sub, dst_x, dst_y,
                                    with_alpha);
                return ret < 0 ? ret : 0;
        }
        r.x = src_x;
        r.y = src_y;
        r.width = src_w;
        r.height = src_h;
        pd_canvas_init(&sub);
        if (pd_canvas_quote(&sub, &s->canvas, &r) < 0) {
                return -1;
        }
        sub.opacity = opacity;
        ret = pd_canvas_mix(&target->canvas, &sub, dst_x, dst_y, with_alpha);
        return ret < 0 ? ret : 0;
}

int ui_draw_surface_composite(ui_draw_surface_t *dst,
                              const ui_draw_surface_t *src, bool with_alpha,
                              float opacity)
{
        pd_canvas_t *c;
        int ret;

        if (!dst || !src) {
                return -1;
        }
        if (!pd_canvas_is_valid(&dst->canvas) ||
            !pd_canvas_is_valid(&src->canvas)) {
                return -1;
        }
        /* pd_canvas_mix crops the source to the destination bounds, so a
         * larger src simply will not write past the display surface. */
        c = (pd_canvas_t *)&src->canvas;
        c->opacity = opacity;
        ret = pd_canvas_mix(&dst->canvas, c, 0, 0, with_alpha);
        c->opacity = 1.0;
        return ret < 0 ? ret : 0;
}

// object 3: cached display layer

ui_draw_layer_t *ui_draw_layer_create(void)
{
        ui_draw_layer_t *l = malloc(sizeof(ui_draw_layer_t));

        if (!l) {
                return NULL;
        }
        l->display = ui_draw_surface_create();
        l->dirty = 1;
        if (!l->display) {
                free(l);
                return NULL;
        }
        return l;
}

void ui_draw_layer_destroy(ui_draw_layer_t *l)
{
        if (!l) {
                return;
        }
        ui_draw_surface_destroy(l->display);
        free(l);
}

int ui_draw_layer_ensure(ui_draw_layer_t *l, float logical_w, float logical_h)
{
        int changed;

        if (!l) {
                return -1;
        }
        changed = ui_draw_surface_resize(l->display, logical_w, logical_h);
        if (changed < 0) {
                return -1;
        }
        if (changed > 0) {
                l->dirty = 1;
        }
        return changed > 0 ? 1 : 0;
}

void ui_draw_layer_invalidate(ui_draw_layer_t *l)
{
        if (l) {
                l->dirty = 1;
        }
}

int ui_draw_layer_needs_render(ui_draw_layer_t *l)
{
        return l && l->dirty;
}

ui_draw_surface_t *ui_draw_layer_surface(ui_draw_layer_t *l)
{
        return l ? l->display : NULL;
}

void ui_draw_layer_mark_rendered(ui_draw_layer_t *l)
{
        if (l) {
                l->dirty = 0;
        }
}

int ui_draw_layer_present(ui_draw_layer_t *l, pd_context_t *paint, int box_x,
                          int box_y, int box_w, int box_h)
{
        pd_rect_t box, overlap;
        int src_x, src_y, src_w, src_h, dst_x, dst_y;

        if (!l || !paint || box_w <= 0 || box_h <= 0) {
                return 0;
        }
        if (l->display->phys_w < 1 || l->display->phys_h < 1) {
                return 0;
        }
        box.x = box_x;
        box.y = box_y;
        box.width = box_w;
        box.height = box_h;
        if (!pd_rect_overlap(&box, &paint->rect, &overlap)) {
                return 0;
        }
        /* The box rect is the content box rounded from layout; the display
         * surface is ceil(logical * scale), so the two can differ by a pixel.
         * Clamp the source sub-rect to the surface bounds and keep the target
         * offset non-negative by moving it with the clamped edge. */
        src_x = overlap.x - box_x;
        src_y = overlap.y - box_y;
        src_w = overlap.width;
        src_h = overlap.height;
        if (src_x < 0) {
                src_w += src_x;
                src_x = 0;
        }
        if (src_y < 0) {
                src_h += src_y;
                src_y = 0;
        }
        if (src_x + src_w > l->display->phys_w) {
                src_w = l->display->phys_w - src_x;
        }
        if (src_y + src_h > l->display->phys_h) {
                src_h = l->display->phys_h - src_y;
        }
        if (src_w <= 0 || src_h <= 0) {
                return 0;
        }
        dst_x = box_x + src_x - paint->rect.x;
        dst_y = box_y + src_y - paint->rect.y;
        if (dst_x < 0 || dst_y < 0) {
                return 0;
        }
        /* Blit only the intersecting sub-rectangle: mixing the whole display
         * surface at a negative target offset smears the surface's top row
         * into a partially dirty region (see pd_canvas_mix). */
        return ui_draw_blend_surface_rect(paint, l->display, src_x, src_y,
                                          src_w, src_h, dst_x, dst_y, true,
                                          1.0f) == 0
                   ? 1
                   : 0;
}
