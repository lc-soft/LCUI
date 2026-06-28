/*
 * lib/pandagl/tests/test_text_baseline.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <pandagl.h>
#include <ctest.h>

/*
 * These cases cover pd_text baseline alignment when characters come from
 * different fallback fonts. The current implementation has a bug:
 * line->height uses a uniform value (pixel_size * 1.42857) while the
 * per-character baseline formula mixes face-global bbox_height and
 * ascender, so different-font characters get misaligned baselines
 * within the same line (CJK sits lower than ASCII).
 *
 * Baseline detection: for each character, scan ink rows inside its
 * horizontal range and take the bottom-most ink row as the glyph
 * bottom (baseline_y - 1), so baseline_y = bottom_y + 1. Since both
 * A and the CJK sample here have no descenders, this is stable.
 */

#define TEST_BG pd_rgb(245, 245, 245)

typedef struct baseline_report {
        int baseline_y;
        int top_y;
        int bottom_y;
} baseline_report_t;

#ifdef _WIN32
static int g_consolas_id = -1;
static int g_msyh_id = -1;

static int is_ink(const pd_canvas_t *canvas, unsigned x, unsigned y)
{
        pd_color_t c;

        if (x >= canvas->width || y >= canvas->height) {
                return 0;
        }
        c = pd_canvas_get_pixel(canvas, x, y);
        return c.r < 128 && c.g < 128 && c.b < 128;
}

static int scan_char_bounds(const pd_canvas_t *canvas, int x_left, int x_right,
                            baseline_report_t *out)
{
        int top = -1, bottom = -1;
        int x, y;

        for (y = 0; y < (int)canvas->height; ++y) {
                for (x = x_left; x < x_right; ++x) {
                        if (is_ink(canvas, (unsigned)x, (unsigned)y)) {
                                if (top < 0) {
                                        top = y;
                                }
                                bottom = y;
                                break;
                        }
                }
        }
        out->top_y = top;
        out->bottom_y = bottom;
        out->baseline_y = bottom + 1;
        return (top >= 0 && bottom >= top) ? 0 : -1;
}

static int try_load_dual_font(void)
{
        if (pd_font_load_file("C:/Windows/Fonts/consola.ttf") != 0) {
                return -1;
        }
        if (pd_font_load_file("C:/Windows/Fonts/msyh.ttc") != 0) {
                return -1;
        }
        g_consolas_id = pd_font_get_id("Consolas", 0, 0);
        g_msyh_id = pd_font_get_id("Microsoft YaHei", 0, 0);
        if (g_consolas_id < 0 || g_msyh_id < 0) {
                return -1;
        }
        return 0;
}

static pd_text_t *make_text(const wchar_t *content, int pixel_size)
{
        pd_text_t *text;
        pd_text_style_t base;

        text = pd_text_create();
        pd_text_style_init(&base);
        base.pixel_size = pixel_size;
        base.has_pixel_size = true;
        {
                const char *names[] = { "Consolas", "Microsoft YaHei", NULL };
                pd_text_style_set_font(&base, names);
        }
        pd_text_set_style(text, &base);
        pd_text_style_destroy(&base);
        pd_text_write(text, content, NULL);
        pd_text_update(text, NULL);
        return text;
}

static void render_and_scan(pd_text_t *text, const wchar_t *content,
                            baseline_report_t *reports, size_t report_len,
                            int canvas_height)
{
        pd_canvas_t canvas;
        pd_rect_t area = { 0, 0, 800, canvas_height };
        pd_pos_t layer_pos = { 0, 0 };
        pd_text_line_t *line;
        int pen_x = text->offset_x;
        size_t idx = 0;
        int col;

        pd_canvas_init(&canvas);
        pd_canvas_create(&canvas, 800, canvas_height);
        pd_canvas_fill(&canvas, TEST_BG);
        pd_text_render_to(text, area, layer_pos, &canvas);

        line = text->lines[0];
        for (col = 0; col < line->length && idx < report_len; ++col) {
                pd_char_t *ch = line->string[col];
                int x_left = pen_x;
                int x_right = pen_x + ch->bitmap->metrics.hori_advance;

                scan_char_bounds(&canvas, x_left, x_right, &reports[idx]);
                ++idx;
                pen_x += ch->bitmap->metrics.hori_advance;
        }
        (void)content;
        pd_canvas_destroy(&canvas);
}

static void should_align_baseline_across_fonts(void)
{
        pd_text_t *text;
        baseline_report_t reps[2];
        static const wchar_t content[] = { L'A', 0x5B57, 0 };
        int diff;

        text = make_text(content, 16);
        render_and_scan(text, content, reps, 2, 80);
        ctest_equal_int("should render 2 chars (ascii + cjk)",
                        reps[0].top_y >= 0 && reps[1].top_y >= 0, 1);
        diff = reps[0].baseline_y - reps[1].baseline_y;
        ctest_printf("  [diag 16px] baseline diff = %d (A=%d, zi=%d)\n", diff,
                     reps[0].baseline_y, reps[1].baseline_y);
        ctest_equal_int("should align face baseline within 3px across fonts",
                        abs(diff) <= 3, 1);
        pd_text_destroy(text);
}

static void should_align_baseline_at_larger_size(void)
{
        pd_text_t *text;
        baseline_report_t reps[2];
        static const wchar_t content[] = { L'A', 0x5B57, 0 };
        int diff;

        text = make_text(content, 24);
        render_and_scan(text, content, reps, 2, 120);
        diff = reps[0].baseline_y - reps[1].baseline_y;
        ctest_printf("  [diag 24px] baseline diff = %d (A=%d, zi=%d)\n", diff,
                     reps[0].baseline_y, reps[1].baseline_y);
        ctest_equal_int(
            "should align face baseline within 3px across fonts at 24px",
            abs(diff) <= 3, 1);
        pd_text_destroy(text);
}

static void should_set_line_baseline_field(void)
{
        pd_text_t *text;
        static const wchar_t content[] = { L'A', 0x5B57, 0 };

        text = make_text(content, 16);
        ctest_equal_bool("line baseline_y should be set",
                         text->lines[0]->baseline_y > 0, true);
        pd_text_destroy(text);
}
#endif

void test_pandagl_text_baseline(void)
{
        pd_font_library_init();
#ifdef _WIN32
        if (try_load_dual_font() == 0) {
                ctest_describe("baseline alignment 16px",
                               should_align_baseline_across_fonts);
                ctest_describe("baseline alignment 24px",
                               should_align_baseline_at_larger_size);
                ctest_describe("baseline field set",
                               should_set_line_baseline_field);
        }
#endif
        pd_font_library_destroy();
}
