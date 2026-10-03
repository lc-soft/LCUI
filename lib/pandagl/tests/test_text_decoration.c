/*
 * lib/pandagl/tests/test_text_decoration.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>
#include <ctest.h>

#define TEST_BACKGROUND pd_rgb(255, 255, 255)
#define TEST_FOREGROUND pd_rgb(0, 0, 0)

static pd_text_t *create_text(const wchar_t *content, int fixed_width,
                              pd_text_align_t align)
{
        pd_text_style_t style;
        pd_text_t *text = pd_text_create();

        pd_text_style_init(&style);
        style.has_fore_color = true;
        style.fore_color = TEST_FOREGROUND;
        style.has_pixel_size = true;
        style.pixel_size = 16;
        pd_text_set_style(text, &style);
        pd_text_style_destroy(&style);
        pd_text_set_multiline(text, true);
        pd_text_set_align(text, align);
        pd_text_set_fixed_size(text, fixed_width, 64);
        pd_text_write(text, content, NULL);
        pd_text_update(text, NULL);
        return text;
}

static void render_text(pd_text_t *text, pd_canvas_t *canvas, int width,
                        int height)
{
        pd_pos_t pos = { 0, 0 };
        pd_rect_t area = { 0, 0, width, height };

        pd_canvas_init(canvas);
        pd_canvas_create(canvas, width, height);
        pd_canvas_fill(canvas, TEST_BACKGROUND);
        pd_text_render_to(text, area, pos, canvas);
}

static bool is_test_color(pd_color_t color, pd_color_t expected)
{
        return color.r == expected.r && color.g == expected.g &&
               color.b == expected.b && color.a == expected.a;
}

static int count_foreground_pixels(const pd_canvas_t *canvas, int y)
{
        int count = 0;
        int x;

        for (x = 0; x < (int)canvas->width; ++x) {
                if (is_test_color(
                        pd_canvas_get_pixel(canvas, (unsigned)x, (unsigned)y),
                        TEST_FOREGROUND)) {
                        ++count;
                }
        }
        return count;
}

static int count_foreground_pixels_in_canvas(const pd_canvas_t *canvas)
{
        int count = 0;
        int y;

        for (y = 0; y < (int)canvas->height; ++y) {
                count += count_foreground_pixels(canvas, y);
        }
        return count;
}

static int count_different_pixels(const pd_canvas_t *a, const pd_canvas_t *b)
{
        int count = 0;
        int x, y;

        for (y = 0; y < (int)a->height; ++y) {
                for (x = 0; x < (int)a->width; ++x) {
                        if (!is_test_color(pd_canvas_get_pixel(a, (unsigned)x,
                                                               (unsigned)y),
                                           pd_canvas_get_pixel(b, (unsigned)x,
                                                               (unsigned)y))) {
                                ++count;
                        }
                }
        }
        return count;
}

static void should_not_draw_underline_by_default(void)
{
        pd_canvas_t canvas;
        pd_text_t *text = create_text(L"    ", 0, PD_TEXT_ALIGN_LEFT);

        render_text(text, &canvas, 128, 64);
        ctest_equal_int("should leave the blank text canvas unchanged",
                        count_foreground_pixels_in_canvas(&canvas), 0);
        pd_canvas_destroy(&canvas);
        pd_text_destroy(text);
}

static void should_draw_line_at_baseline_offset_and_aligned_width(void)
{
        pd_canvas_t canvas;
        pd_text_line_t *line;
        pd_text_t *text = create_text(L"    ", 128, PD_TEXT_ALIGN_CENTER);
        int y, start_x;

        line = text->lines[0];
        start_x = (text->fixed_width - line->width) / 2;
        pd_text_set_underline(text, true, 2, 3);
        render_text(text, &canvas, 128, 64);
        y = line->baseline_y + 2;
        ctest_equal_int("should draw the first underline row across line width",
                        count_foreground_pixels(&canvas, y), line->width);
        ctest_equal_int(
            "should draw the second underline row across line width",
            count_foreground_pixels(&canvas, y + 1), line->width);
        ctest_equal_int("should draw the third underline row across line width",
                        count_foreground_pixels(&canvas, y + 2), line->width);
        ctest_equal_bool(
            "should start underline at aligned line start",
            is_test_color(
                pd_canvas_get_pixel(&canvas, (unsigned)start_x, (unsigned)y),
                TEST_FOREGROUND),
            true);
        ctest_equal_bool(
            "should end underline at aligned line end",
            is_test_color(pd_canvas_get_pixel(
                              &canvas, (unsigned)(start_x + line->width - 1),
                              (unsigned)y),
                          TEST_FOREGROUND),
            true);
        ctest_equal_bool(
            "underline should not extend past aligned line width",
            is_test_color(
                pd_canvas_get_pixel(&canvas, (unsigned)(start_x + line->width),
                                    (unsigned)y),
                TEST_FOREGROUND),
            false);
        pd_canvas_destroy(&canvas);
        pd_text_destroy(text);
}

static void should_skip_zero_and_negative_thickness(void)
{
        pd_canvas_t canvas;
        pd_text_t *text = create_text(L"    ", 0, PD_TEXT_ALIGN_LEFT);

        pd_text_set_underline(text, true, 1, 0);
        render_text(text, &canvas, 128, 64);
        ctest_equal_int("zero thickness should draw no pixels",
                        count_foreground_pixels_in_canvas(&canvas), 0);
        pd_canvas_destroy(&canvas);

        pd_text_set_underline(text, true, 1, -2);
        render_text(text, &canvas, 128, 64);
        ctest_equal_int("negative thickness should draw no pixels",
                        count_foreground_pixels_in_canvas(&canvas), 0);
        pd_canvas_destroy(&canvas);
        pd_text_destroy(text);
}

static void should_draw_each_multiline_row_at_its_own_position(void)
{
        pd_canvas_t canvas;
        pd_text_line_t *line0, *line1;
        pd_text_t *text = create_text(L"    \n  ", 0, PD_TEXT_ALIGN_LEFT);
        int y0, y1;

        line0 = text->lines[0];
        line1 = text->lines[1];
        pd_text_set_underline(text, true, 1, 1);
        render_text(text, &canvas, 128, 64);
        y0 = line0->baseline_y + 1;
        y1 = line0->height + line1->baseline_y + 1;
        ctest_equal_int("should use first line advance width",
                        count_foreground_pixels(&canvas, y0), line0->width);
        ctest_equal_int("should use second line advance width",
                        count_foreground_pixels(&canvas, y1), line1->width);
        ctest_equal_bool("multiline fixture should have unequal line widths",
                         line0->width != line1->width, true);
        pd_canvas_destroy(&canvas);
        pd_text_destroy(text);
}

static void should_invalidate_lines_when_underline_changes(void)
{
        list_t rects;
        pd_text_t *text = create_text(L"    \n  ", 0, PD_TEXT_ALIGN_LEFT);

        pd_rects_clear(&text->dirty_rects);
        list_create(&rects);
        pd_text_set_underline(text, true, 1, 1);
        pd_text_update(text, &rects);
        ctest_equal_bool("enabling underline should invalidate text lines",
                         rects.length > 0, true);
        pd_rects_clear(&rects);

        pd_text_set_underline(text, false, 1, 1);
        pd_text_update(text, &rects);
        ctest_equal_bool("disabling underline should invalidate old lines",
                         rects.length > 0, true);
        pd_rects_clear(&rects);
        pd_text_destroy(text);
}

static void should_repaint_underline_outside_line_box_partially(void)
{
        pd_canvas_t partial, full;
        pd_pos_t pos = { 0, 0 };
        pd_rect_t area = { 0, 0, 128, 64 };
        list_t dirty_rects;
        list_node_t *node;
        pd_text_t *text = create_text(L"    \n  ", 0, PD_TEXT_ALIGN_LEFT);

        pd_canvas_init(&partial);
        pd_canvas_create(&partial, 128, 64);
        pd_canvas_fill(&partial, TEST_BACKGROUND);
        pd_canvas_init(&full);
        pd_canvas_create(&full, 128, 64);
        pd_canvas_fill(&full, TEST_BACKGROUND);
        pd_rects_clear(&text->dirty_rects);
        list_create(&dirty_rects);
        pd_text_set_underline(text, true, 4, 2);
        pd_text_update(text, &dirty_rects);
        for (list_each(node, &dirty_rects)) {
                pd_rect_t *rect = node->data;

                pd_canvas_fill_rect(&partial, TEST_BACKGROUND, *rect);
                pd_text_render_to(text, *rect, pos, &partial);
        }
        pd_text_render_to(text, area, pos, &full);
        ctest_equal_int(
            "partial redraw should match full out-of-line-box underline",
            count_foreground_pixels_in_canvas(&partial),
            count_foreground_pixels_in_canvas(&full));
        ctest_equal_bool(
            "partial redraw should reproduce full underline pixels",
            count_different_pixels(&partial, &full), 0);
        pd_rects_clear(&dirty_rects);

        pd_canvas_fill(&full, TEST_BACKGROUND);
        pd_text_set_underline(text, true, 12, 4);
        pd_text_update(text, &dirty_rects);
        for (list_each(node, &dirty_rects)) {
                pd_rect_t *rect = node->data;

                pd_canvas_fill_rect(&partial, TEST_BACKGROUND, *rect);
                pd_text_render_to(text, *rect, pos, &partial);
        }
        pd_text_render_to(text, area, pos, &full);
        ctest_equal_int("changing offset and thickness should redraw full text",
                        count_different_pixels(&partial, &full), 0);
        pd_rects_clear(&dirty_rects);

        pd_canvas_fill(&full, TEST_BACKGROUND);
        pd_text_set_underline(text, true, -24, 2);
        pd_text_update(text, &dirty_rects);
        for (list_each(node, &dirty_rects)) {
                pd_rect_t *rect = node->data;

                pd_canvas_fill_rect(&partial, TEST_BACKGROUND, *rect);
                pd_text_render_to(text, *rect, pos, &partial);
        }
        pd_text_render_to(text, area, pos, &full);
        ctest_equal_int(
            "negative offset partial redraw should match full render",
            count_different_pixels(&partial, &full), 0);
        pd_rects_clear(&dirty_rects);

        pd_canvas_fill(&full, TEST_BACKGROUND);
        pd_text_set_underline(text, false, -24, 2);
        pd_text_update(text, &dirty_rects);
        for (list_each(node, &dirty_rects)) {
                pd_rect_t *rect = node->data;

                pd_canvas_fill_rect(&partial, TEST_BACKGROUND, *rect);
                pd_text_render_to(text, *rect, pos, &partial);
        }
        pd_text_render_to(text, area, pos, &full);
        ctest_equal_int("disabling underline should clear old pixels",
                        count_different_pixels(&partial, &full), 0);
        pd_rects_clear(&dirty_rects);
        pd_canvas_destroy(&full);
        pd_canvas_destroy(&partial);
        pd_text_destroy(text);
}

void test_pandagl_text_decoration(void)
{
        pd_font_library_init();
        ctest_describe("underline default",
                       should_not_draw_underline_by_default);
        ctest_describe("underline geometry",
                       should_draw_line_at_baseline_offset_and_aligned_width);
        ctest_describe("underline thickness",
                       should_skip_zero_and_negative_thickness);
        ctest_describe("underline multiline",
                       should_draw_each_multiline_row_at_its_own_position);
        ctest_describe("underline dirty update",
                       should_invalidate_lines_when_underline_changes);
        ctest_describe("underline out-of-line-box partial redraw",
                       should_repaint_underline_outside_line_box_partially);
        pd_font_library_destroy();
}
