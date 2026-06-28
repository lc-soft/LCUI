/*
 * lib/pandagl/examples/font-mixed-baseline/src/main.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BG pd_rgb(245, 245, 245)
#define INK_COLOR pd_rgb(20, 20, 20)

typedef struct char_report {
        wchar_t code;
        const char *label;
        int x_left;
        int x_right;
        int font_id;
        const char *font_name;
        int expected_top_y;
        int actual_top_y;
        int actual_bottom_y;
        int delta_top;
        int formula_pen_y;
        int bitmap_top;
        int bitmap_left;
        int bitmap_rows;
        int bitmap_width;
        int bbox_w;
        int bbox_h;
        int ascender;
        int hori_advance;
        int line_height;
        int line_y;
} char_report_t;

typedef struct test_case {
        const char *name;
        const wchar_t *text;
} test_case_t;

static int g_consolas_id = -1;
static int g_msyh_id = -1;

typedef struct font_candidate {
        const char *path;
        const char *family_name;
} font_candidate_t;

static int load_font_file(const font_candidate_t *candidates, int *out_id);
static int is_ink(const pd_canvas_t *canvas, unsigned x, unsigned y);
static int scan_char_bounds(const pd_canvas_t *canvas, int x_left, int x_right,
                            int *out_top, int *out_bottom);
static void inspect_run(pd_text_t *text, pd_canvas_t *canvas, int line_y,
                        char_report_t *out, size_t out_len, size_t *out_count);
static int is_cjk(wchar_t ch);
static void run_test(const test_case_t *tc, int pixel_size);

static int load_font_file(const font_candidate_t *candidates, int *out_id)
{
        size_t i;
        int ret = -1;
        const char *loaded_family = NULL;

        for (i = 0; candidates[i].path; ++i) {
                ret = pd_font_load_file(candidates[i].path);
                if (ret >= 0) {
                        loaded_family = candidates[i].family_name;
                        break;
                }
        }
        if (!loaded_family) {
                return -1;
        }
        *out_id = pd_font_get_id(loaded_family, 0, 0);
        return *out_id >= 0 ? 0 : -1;
}

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
                            int *out_top, int *out_bottom)
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
        *out_top = top;
        *out_bottom = bottom;
        return (top >= 0 && bottom >= top) ? 0 : -1;
}

static int is_cjk(wchar_t ch)
{
        if (ch >= 0x4E00 && ch <= 0x9FFF) {
                return 1;
        }
        if (ch >= 0x3400 && ch <= 0x4DBF) {
                return 1;
        }
        if (ch >= 0xF900 && ch <= 0xFAFF) {
                return 1;
        }
        return 0;
}

static const char *font_id_to_name(int id)
{
        if (id == g_msyh_id) {
                return "YaHei";
        }
        if (id == g_consolas_id) {
                return "Consolas";
        }
        return "?";
}

static int detect_used_font(const pd_char_t *ch)
{
        if (is_cjk(ch->code)) {
                return g_msyh_id;
        }
        return g_consolas_id;
}

static void inspect_run(pd_text_t *text, pd_canvas_t *canvas, int line_y,
                        char_report_t *out, size_t out_len, size_t *out_count)
{
        pd_text_line_t *line;
        pd_char_t *ch;
        size_t idx = 0;
        int pen_x = text->offset_x;
        int col;

        line = text->lines[0];
        for (col = 0; col < line->length && idx < out_len; ++col) {
                int top, bottom;
                int expected_top, formula_y;
                int font_id;

                ch = line->string[col];
                if (!ch || !ch->bitmap) {
                        pen_x += ch ? ch->bitmap->metrics.hori_advance : 0;
                        continue;
                }
                font_id = detect_used_font(ch);
                expected_top = line_y + line->baseline_y - ch->bitmap->top;
                formula_y = line_y + line->baseline_y - ch->bitmap->top;
                scan_char_bounds(canvas, pen_x,
                                 pen_x + ch->bitmap->metrics.hori_advance, &top,
                                 &bottom);
                out[idx].code = ch->code;
                out[idx].label = is_cjk(ch->code) ? "cjk" : "ascii";
                out[idx].x_left = pen_x;
                out[idx].x_right = pen_x + ch->bitmap->metrics.hori_advance;
                out[idx].font_id = font_id;
                out[idx].font_name = font_id_to_name(font_id);
                out[idx].expected_top_y = expected_top;
                out[idx].actual_top_y = top;
                out[idx].actual_bottom_y = bottom;
                out[idx].delta_top = (top >= 0) ? top - expected_top : -9999;
                out[idx].formula_pen_y = formula_y;
                out[idx].bitmap_top = ch->bitmap->top;
                out[idx].bitmap_left = ch->bitmap->left;
                out[idx].bitmap_rows = ch->bitmap->rows;
                out[idx].bitmap_width = ch->bitmap->width;
                out[idx].bbox_w = ch->bitmap->metrics.bbox_width;
                out[idx].bbox_h = ch->bitmap->metrics.bbox_height;
                out[idx].ascender = ch->bitmap->metrics.ascender;
                out[idx].hori_advance = ch->bitmap->metrics.hori_advance;
                out[idx].line_height = line->height;
                out[idx].line_y = line_y;
                ++idx;
                pen_x += ch->bitmap->metrics.hori_advance;
        }
        *out_count = idx;
}

static void print_report(const char *title, const test_case_t *tc,
                         const char_report_t *reports, size_t count,
                         int pixel_size)
{
        size_t i;

        printf("\n=== %s ===\n", title);
        printf("    text     : ");
        {
                const wchar_t *p;
                for (p = tc->text; *p; ++p) {
                        if (*p >= 0x20 && *p < 0x7F) {
                                putchar((char)*p);
                        } else {
                                putchar('?');
                        }
                }
        }
        printf("\n    size     : %d px\n", pixel_size);
        printf("    %-3s %-8s %-5s %-4s %-4s %-4s %-6s %-6s %-7s "
               "%-7s %-5s\n",
               "ch", "font", "tag", "adv", "top", "rows", "bbox_h", "ascend",
               "line_h", "expect", "delta");
        printf("    %s\n", "--------------------------------------------"
                           "--------------------------------------");
        for (i = 0; i < count; ++i) {
                const char_report_t *r = &reports[i];
                char buf[8] = { 0 };
                if (r->code >= 0x20 && r->code < 0x7F) {
                        buf[0] = (char)r->code;
                } else {
                        buf[0] = '?';
                }
                printf("    %-3s %-8s %-5s %4d %4d %4d %6d %6d %7d %7d "
                       "%+5d\n",
                       buf, r->font_name, r->label, r->hori_advance,
                       r->bitmap_top, r->bitmap_rows, r->bbox_h, r->ascender,
                       r->line_height, r->expected_top_y, r->delta_top);
        }
}

static void run_test(const test_case_t *tc, int pixel_size)
{
        pd_canvas_t canvas;
        pd_text_t *text;
        pd_text_style_t base;
        size_t count;
        char_report_t *reports;
        size_t max_chars;
        pd_rect_t area;
        pd_pos_t layer_pos = { 0, 0 };
        int line_y;

        pd_canvas_init(&canvas);
        pd_canvas_create(&canvas, 800, 80);
        pd_canvas_fill(&canvas, DEFAULT_BG);

        text = pd_text_create();
        pd_text_style_init(&base);
        base.pixel_size = pixel_size;
        base.has_pixel_size = true;
        {
                const char *names[] = { "Consolas", "Microsoft YaHei", NULL };
                if (pd_text_style_set_font(&base, names) != 0) {
                        fprintf(stderr, "failed to set fallback fonts\n");
                        pd_text_destroy(text);
                        pd_canvas_destroy(&canvas);
                        return;
                }
        }
        pd_text_set_style(text, &base);
        pd_text_style_destroy(&base);

        pd_text_write(text, tc->text, NULL);
        pd_text_update(text, NULL);

        area.x = 0;
        area.y = 0;
        area.width = 800;
        area.height = 80;
        line_y = text->offset_y;
        pd_text_render_to(text, area, layer_pos, &canvas);

        max_chars = wcslen(tc->text);
        reports = calloc(max_chars + 1, sizeof(char_report_t));
        inspect_run(text, &canvas, line_y, reports, max_chars + 1, &count);
        print_report(tc->name, tc, reports, count, pixel_size);

        {
                char fname[64];
                snprintf(fname, sizeof(fname), "font_mixed_baseline_%s_%d.png",
                         tc->name, pixel_size);
                pd_write_png_file(fname, &canvas);
        }

        free(reports);
        pd_text_destroy(text);
        pd_canvas_destroy(&canvas);
}

int main(void)
{
        const font_candidate_t consolas_candidates[] = {
                { "C:/Windows/Fonts/consola.ttf", "Consolas" },
                { "C:/Windows/Fonts/arial.ttf", "Arial" },
                { NULL, NULL }
        };
        const font_candidate_t msyh_candidates[] = {
                { "C:/Windows/Fonts/msyh.ttc", "Microsoft YaHei" },
                { "C:/Windows/Fonts/simsun.ttc", "SimSun" },
                { NULL, NULL }
        };
        test_case_t cases[] = {
                {
                    .name = "ascii_only",
                    .text = L"AAAAAA",
                },
                {
                    .name = "cjk_only",
                    .text = L"\u554A\u554A\u554A\u554A\u554A",
                },
                {
                    .name = "mixed_ab",
                    .text = L"A\u5B57B\u6587C\u6D4BD\u8BD5",
                },
                {
                    .name = "with_descender",
                    .text = L"AgaBgqCp\u5B57",
                },
        };
        size_t i;
        int sizes[] = { 16, 24 };
        size_t j;

        pd_font_library_init();

        if (load_font_file(consolas_candidates, &g_consolas_id) != 0) {
                fprintf(stderr, "failed to load any ASCII font\n");
                pd_font_library_destroy();
                return 1;
        }
        if (load_font_file(msyh_candidates, &g_msyh_id) != 0) {
                fprintf(stderr, "failed to load any CJK font\n");
                pd_font_library_destroy();
                return 1;
        }
        printf("font ids: consolas=%d, msyh=%d\n", g_consolas_id, g_msyh_id);

        for (j = 0; j < sizeof(sizes) / sizeof(sizes[0]); ++j) {
                for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
                        run_test(&cases[i], sizes[j]);
                }
        }

        pd_font_library_destroy();
        return 0;
}
