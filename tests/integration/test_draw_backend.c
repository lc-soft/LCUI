/*
 * tests/integration/test_draw_backend.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include "../../src/widgets/draw_backend.h"

#define WIDGET_SIZE 32
#define BUFFER_SIZE 64
#define HALF_PI 1.57079632679489661923

static int is_red(pd_color_t c)
{
        return c.a != 0 && c.r >= 253 && c.g < 2 && c.b < 2;
}

static void make_argb(pd_canvas_t *canvas, int width, int height)
{
        pd_canvas_init(canvas);
        canvas->color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(canvas, width, height);
}

/* The surfaces are opaque to callers, so a test reads one back by compositing
 * it, at 1:1 with the transparent device canvas it was rendered on (dpi 96),
 * into a fresh ARGB canvas it can sample. */
static void read_surface_into(ui_draw_surface_t *s, pd_canvas_t *out)
{
        pd_rect_t rect = { 0, 0, BUFFER_SIZE, BUFFER_SIZE };
        pd_context_t ctx;

        make_argb(out, BUFFER_SIZE, BUFFER_SIZE);
        pd_canvas_init(&ctx.canvas);
        ctx.rect = rect;
        ctx.with_alpha = true;
        pd_canvas_quote(&ctx.canvas, out, &rect);
        ui_draw_blend_surface(&ctx, s, 0, 0, true, 1.0f);
}

static ui_draw_surface_t *make_surface(void)
{
        ui_draw_surface_t *s = ui_draw_surface_create();

        ui_draw_surface_resize(s, BUFFER_SIZE, BUFFER_SIZE);
        return s;
}

static void check_capture(void)
{
        pd_canvas_t out;
        ui_draw_surface_t *s;
        ui_widget_t *box;
        pd_color_t color;

        ui_metrics.dpi = 96;

        /* Use a CSS class rule (.bg) for the background: the UA default `*`
         * rule sets background-color: transparent at GENERAL rank, which an
         * inline style cannot override through the cascade, but a CLASS-rank
         * selector can. This guarantees the render pass paints an opaque
         * background into the capture surface. */
        box = ui_load_xml_file("test_draw_backend.xml");
        ui_widget_append(ui_root(), box);
        ui_widget_unwrap(box);
        box = ui_get_widget("box");
        ui_widget_request_refresh_style(box);
        ui_widget_update(ui_root());

        s = make_surface();
        ui_draw_capture(box, s);
        read_surface_into(s, &out);

        color = pd_canvas_get_pixel(&out, WIDGET_SIZE / 2, WIDGET_SIZE / 2);
        ctest_equal_bool("capture: should paint widget background at center",
                         is_red(color), true);

        color = pd_canvas_get_pixel(&out, BUFFER_SIZE - 1, BUFFER_SIZE - 1);
        ctest_equal_int("capture: should leave area outside widget transparent",
                        color.a, 0);

        ui_draw_surface_destroy(s);
        pd_canvas_destroy(&out);
        ui_widget_destroy(box);
}

static void check_rotate(void)
{
        pd_canvas_t out;
        ui_draw_surface_t *src, *dst;
        pd_color_t color;

        ui_metrics.dpi = 96;

        /* Build a red square occupying the top-left quadrant so the feature
         * has a location that rotation can visibly move. */
        src = make_surface();
        ui_draw_clear(src);
        ui_draw_fill_rect(src, 0, 0, WIDGET_SIZE, WIDGET_SIZE, 255, 0, 0, 255);

        dst = make_surface();
        ui_draw_rotate_into(src, dst, HALF_PI);
        read_surface_into(dst, &out);

        /* Rotated 90 degrees, the top-left square lands on the top-right. */
        color = pd_canvas_get_pixel(&out, BUFFER_SIZE - WIDGET_SIZE / 2,
                                    WIDGET_SIZE / 2);
        ctest_equal_bool("rotate: should move the feature to the top-right",
                         is_red(color), true);

        color = pd_canvas_get_pixel(&out, WIDGET_SIZE / 2, WIDGET_SIZE / 2);
        ctest_equal_int("rotate: should clear the original feature location",
                        color.a, 0);

        ui_draw_surface_destroy(src);
        ui_draw_surface_destroy(dst);
        pd_canvas_destroy(&out);
}

static void check_mask_into(void)
{
        pd_canvas_t out;
        ui_draw_surface_t *dst, *mask;
        pd_color_t color;

        ui_metrics.dpi = 96;

        /* Fully opaque red surface so both halves start with alpha 255. */
        dst = make_surface();
        ui_draw_fill_rect(dst, 0, 0, BUFFER_SIZE, BUFFER_SIZE, 255, 0, 0, 255);

        /* Mask is transparent on the left half, opaque on the right half. */
        mask = make_surface();
        ui_draw_clear(mask);
        ui_draw_fill_rect(mask, BUFFER_SIZE / 2, 0, BUFFER_SIZE / 2,
                          BUFFER_SIZE, 255, 255, 255, 255);

        ui_draw_mask_into(mask, dst);
        read_surface_into(dst, &out);

        color = pd_canvas_get_pixel(&out, BUFFER_SIZE / 4, BUFFER_SIZE / 2);
        ctest_equal_int("mask: should zero alpha on the masked half", color.a,
                        0);

        color = pd_canvas_get_pixel(&out, BUFFER_SIZE * 3 / 4, BUFFER_SIZE / 2);
        ctest_equal_bool("mask: should keep the unmasked half opaque and red",
                         is_red(color), true);

        ui_draw_surface_destroy(dst);
        ui_draw_surface_destroy(mask);
        pd_canvas_destroy(&out);
}

static void check_blend_surface(void)
{
        pd_canvas_t out;
        ui_draw_surface_t *src;
        pd_rect_t rect = { 0, 0, BUFFER_SIZE, BUFFER_SIZE };
        pd_context_t ctx;
        pd_color_t color;

        ui_metrics.dpi = 96;

        /* An opaque blue target with a fully opaque red surface composited at a
         * non-zero offset: the offset region shows red, the untouched margin
         * keeps the base color, proving blend honors both position and alpha.
         */
        make_argb(&out, BUFFER_SIZE, BUFFER_SIZE);
        pd_canvas_fill(&out, pd_rgb(0, 0, 255));
        src = make_surface();
        ui_draw_fill_rect(src, 0, 0, BUFFER_SIZE / 2, BUFFER_SIZE / 2, 255, 0,
                          0, 255);

        pd_canvas_init(&ctx.canvas);
        ctx.rect = rect;
        ctx.with_alpha = true;
        pd_canvas_quote(&ctx.canvas, &out, &rect);
        ui_draw_blend_surface(&ctx, src, WIDGET_SIZE / 2, WIDGET_SIZE / 2, true,
                              1.0f);

        color = pd_canvas_get_pixel(&out, WIDGET_SIZE, WIDGET_SIZE);
        ctest_equal_bool("blend: offset region should show the surface",
                         is_red(color), true);

        color = pd_canvas_get_pixel(&out, 2, 2);
        ctest_equal_bool("blend: untouched margin should keep the base color",
                         color.b > 250 && color.r < 4, true);

        ui_draw_surface_destroy(src);
        pd_canvas_destroy(&out);
}

/* Cached display layer: ensure() gates the physical size, needs_render()
 * starts true and follows invalidate/mark_rendered, and present() blits the
 * cache's sub-rect of the target's dirty region without re-rendering it. */
static void check_layer(void)
{
        pd_canvas_t out, before_canvas;
        pd_rect_t rect = { 0, 0, BUFFER_SIZE, BUFFER_SIZE };
        pd_context_t ctx;
        ui_draw_layer_t *l;
        ui_draw_surface_t *display;
        pd_color_t before, after;
        int x, y, differs;

        ui_metrics.dpi = 96;

        l = ui_draw_layer_create();
        ctest_equal_bool("layer: create should return a layer", l != NULL,
                         true);
        ctest_equal_bool("layer: a fresh layer needs a render",
                         ui_draw_layer_needs_render(l), true);

        ctest_equal_int(
            "layer: first ensure builds the backing",
            ui_draw_layer_ensure(l, BUFFER_SIZE / 2, BUFFER_SIZE / 2), 1);
        ctest_equal_int(
            "layer: ensure at the same size is a no-op",
            ui_draw_layer_ensure(l, BUFFER_SIZE / 2, BUFFER_SIZE / 2), 0);

        /* Write an opaque red block into the display surface as the "full
         * render" would, then close the gate. */
        display = ui_draw_layer_surface(l);
        ui_draw_fill_rect(display, 0, 0, BUFFER_SIZE / 4, BUFFER_SIZE / 4, 255,
                          0, 0, 255);
        ui_draw_layer_mark_rendered(l);
        ctest_equal_bool("layer: mark_rendered closes the gate",
                         ui_draw_layer_needs_render(l), false);

        make_argb(&out, BUFFER_SIZE, BUFFER_SIZE);
        pd_canvas_fill(&out, pd_rgb(255, 255, 255));
        pd_canvas_init(&ctx.canvas);
        ctx.rect = rect;
        ctx.with_alpha = true;
        pd_canvas_quote(&ctx.canvas, &out, &rect);
        ctest_equal_int("layer: present blits the cache into the target",
                        ui_draw_layer_present(l, &ctx, 0, 0, BUFFER_SIZE / 2,
                                              BUFFER_SIZE / 2),
                        1);

        after = pd_canvas_get_pixel(&out, BUFFER_SIZE / 8, BUFFER_SIZE / 8);
        ctest_equal_bool("layer: the rendered block should reach the target",
                         is_red(after), true);
        after = pd_canvas_get_pixel(&out, BUFFER_SIZE - 2, BUFFER_SIZE - 2);
        ctest_equal_bool("layer: pixels outside the box stay untouched",
                         after.r > 250 && after.g > 250 && after.b > 250, true);

        /* A second present over the same target re-blits the cache: present
         * must neither re-render nor clear it, so the target is
         * pixel-identical. */
        make_argb(&before_canvas, BUFFER_SIZE, BUFFER_SIZE);
        pd_canvas_replace(&before_canvas, &out, 0, 0);
        pd_canvas_init(&ctx.canvas);
        ctx.rect = rect;
        ctx.with_alpha = true;
        pd_canvas_quote(&ctx.canvas, &out, &rect);
        ctest_equal_int("layer: a second present still blits",
                        ui_draw_layer_present(l, &ctx, 0, 0, BUFFER_SIZE / 2,
                                              BUFFER_SIZE / 2),
                        1);
        differs = 0;
        for (y = 0; y < BUFFER_SIZE; ++y) {
                for (x = 0; x < BUFFER_SIZE; ++x) {
                        before = pd_canvas_get_pixel(&before_canvas, x, y);
                        after = pd_canvas_get_pixel(&out, x, y);
                        if (before.value != after.value) {
                                differs += 1;
                        }
                }
        }
        ctest_equal_int("layer: re-presenting must not disturb the cache",
                        differs, 0);
        after = pd_canvas_get_pixel(&out, BUFFER_SIZE / 8, BUFFER_SIZE / 8);
        ctest_equal_bool("layer: the cache still carries the block",
                         is_red(after), true);

        ui_draw_layer_invalidate(l);
        ctest_equal_bool("layer: invalidate reopens the gate",
                         ui_draw_layer_needs_render(l), true);

        ui_draw_layer_destroy(l);
        pd_canvas_destroy(&out);
        pd_canvas_destroy(&before_canvas);
}

void test_draw_backend(void)
{
        lcui_init();
        ctest_describe("draw backend capture", check_capture);
        ctest_describe("draw backend rotate", check_rotate);
        ctest_describe("draw backend mask into", check_mask_into);
        ctest_describe("draw backend blend surface", check_blend_surface);
        ctest_describe("draw backend layer", check_layer);
        lcui_destroy();
}
