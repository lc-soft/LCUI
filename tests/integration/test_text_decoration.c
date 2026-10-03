/*
 * tests/integration/test_text_decoration.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI.h>
#include <ctest-custom.h>

#define TEST_CANVAS_WIDTH 512
#define TEST_CANVAS_HEIGHT 320
#define TEST_BACKGROUND pd_rgb(255, 255, 255)

static ui_widget_t *text_widget;
static int after_layout_count;

static void on_text_after_layout(ui_widget_t *w, ui_event_t *e, void *emit_arg)
{
        int *count = e->data;

        ++*count;
}

static void should_initialize_ui_text_style_cache_fields(void)
{
        ui_text_style_t style;

        memset(&style, 0xff, sizeof(style));
        ui_text_style_init(&style);
        ctest_equal_int("text style should initialize font size",
                        style.font_size, 0);
        ctest_equal_int("text style should initialize line height",
                        style.line_height, 0);
        ctest_equal_int("text style should initialize text alignment",
                        style.text_align, PD_TEXT_ALIGN_LEFT);
        ctest_equal_int("text style should initialize white space",
                        style.white_space, CSS_WHITE_SPACE_NORMAL);
        ui_text_style_destroy(&style);
}

static void set_text_style(const char *property, const char *value)
{
        ctest_equal_int(
            "should accept CSS text style",
            ui_widget_set_style_string(text_widget, property, value), 0);
        ui_update();
}

static void render_text(pd_canvas_t *canvas)
{
        pd_context_t paint;
        pd_rect_t rect = { 0, 0, TEST_CANVAS_WIDTH, TEST_CANVAS_HEIGHT };

        memset(&paint, 0, sizeof(paint));
        pd_canvas_init(canvas);
        pd_canvas_create(canvas, TEST_CANVAS_WIDTH, TEST_CANVAS_HEIGHT);
        pd_canvas_fill(canvas, TEST_BACKGROUND);
        paint.with_alpha = false;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, canvas, &rect);
        ui_widget_render(text_widget, &paint);
}

static bool pixel_equal(pd_color_t a, pd_color_t b)
{
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

static int count_different_pixels(const pd_canvas_t *a, const pd_canvas_t *b)
{
        int count = 0;
        int x, y;

        for (y = 0; y < (int)a->height; ++y) {
                for (x = 0; x < (int)a->width; ++x) {
                        if (!pixel_equal(pd_canvas_get_pixel(a, (unsigned)x,
                                                             (unsigned)y),
                                         pd_canvas_get_pixel(b, (unsigned)x,
                                                             (unsigned)y))) {
                                ++count;
                        }
                }
        }
        return count;
}

static int count_different_rows(const pd_canvas_t *a, const pd_canvas_t *b)
{
        int rows = 0;
        int x, y;

        for (y = 0; y < (int)a->height; ++y) {
                for (x = 0; x < (int)a->width; ++x) {
                        if (!pixel_equal(pd_canvas_get_pixel(a, (unsigned)x,
                                                             (unsigned)y),
                                         pd_canvas_get_pixel(b, (unsigned)x,
                                                             (unsigned)y))) {
                                ++rows;
                                break;
                        }
                }
        }
        return rows;
}

static int first_different_row(const pd_canvas_t *a, const pd_canvas_t *b)
{
        int x, y;

        for (y = 0; y < (int)a->height; ++y) {
                for (x = 0; x < (int)a->width; ++x) {
                        if (!pixel_equal(pd_canvas_get_pixel(a, (unsigned)x,
                                                             (unsigned)y),
                                         pd_canvas_get_pixel(b, (unsigned)x,
                                                             (unsigned)y))) {
                                return y;
                        }
                }
        }
        return -1;
}

static void should_default_to_no_decoration(void)
{
        pd_canvas_t base, repeat;

        render_text(&base);
        render_text(&repeat);
        ctest_equal_int("default style should compute no decoration",
                        text_widget->computed_style.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        ctest_equal_int("default rendering should remain stable",
                        count_different_pixels(&base, &repeat), 0);
        pd_canvas_destroy(&repeat);
        pd_canvas_destroy(&base);
}

static void should_apply_and_remove_underline(void)
{
        pd_canvas_t base, underline, removed;

        set_text_style("text-decoration", "none");
        render_text(&base);
        set_text_style("text-decoration", "underline");
        render_text(&underline);
        ctest_equal_bool("underline should add visible pixels",
                         count_different_pixels(&base, &underline) > 0, true);

        set_text_style("text-decoration", "none");
        render_text(&removed);
        ctest_equal_int("removing underline should restore original pixels",
                        count_different_pixels(&base, &removed), 0);
        pd_canvas_destroy(&removed);
        pd_canvas_destroy(&underline);
        pd_canvas_destroy(&base);
}

static void should_mark_underline_changes_dirty(void)
{
        set_text_style("text-decoration", "none");
        text_widget->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
        set_text_style("text-decoration", "underline");
        ctest_equal_bool(
            "underline change should request widget repaint",
            text_widget->rendering.dirty_rect_type == UI_DIRTY_RECT_TYPE_FULL,
            true);

        text_widget->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
        set_text_style("text-decoration", "none");
        ctest_equal_bool(
            "removing underline should request widget repaint",
            text_widget->rendering.dirty_rect_type == UI_DIRTY_RECT_TYPE_FULL,
            true);
}

static void should_not_reflow_for_underline_style_changes(void)
{
        after_layout_count = 0;
        set_text_style("text-decoration", "underline");
        set_text_style("text-underline-offset", "3px");
        set_text_style("text-decoration-thickness", "2px");
        ctest_equal_int("underline-only changes should not reflow text",
                        after_layout_count, 0);
}

static void should_move_and_resize_underline(void)
{
        pd_canvas_t base, positive, negative, thick, zero, below_zero;
        ui_rect_t original_box;
        int positive_y, negative_y;

        set_text_style("text-decoration", "none");
        render_text(&base);
        original_box = text_widget->border_box;
        set_text_style("text-decoration", "underline");
        set_text_style("text-underline-offset", "2px");
        set_text_style("text-decoration-thickness", "2px");
        render_text(&positive);
        set_text_style("text-underline-offset", "-2px");
        render_text(&negative);
        positive_y = first_different_row(&base, &positive);
        negative_y = first_different_row(&base, &negative);
        ctest_equal_int("positive offset should move underline downward",
                        positive_y - negative_y, 4);
        ctest_equal_int("two-pixel thickness should affect two rows",
                        count_different_rows(&base, &positive), 2);

        set_text_style("text-underline-offset", "2px");
        set_text_style("text-decoration-thickness", "4px");
        render_text(&thick);
        ctest_equal_int("four-pixel thickness should affect four rows",
                        count_different_rows(&base, &thick), 4);
        set_text_style("text-decoration-thickness", "0px");
        render_text(&zero);
        ctest_equal_int("zero thickness should hide underline",
                        count_different_pixels(&base, &zero), 0);
        set_text_style("text-decoration-thickness", "-1px");
        render_text(&below_zero);
        ctest_equal_int("negative thickness should hide underline",
                        count_different_pixels(&base, &below_zero), 0);
        ctest_equal_float("decoration should not change text width",
                          text_widget->border_box.width, original_box.width);
        ctest_equal_float("decoration should not change text height",
                          text_widget->border_box.height, original_box.height);

        pd_canvas_destroy(&below_zero);
        pd_canvas_destroy(&zero);
        pd_canvas_destroy(&thick);
        pd_canvas_destroy(&negative);
        pd_canvas_destroy(&positive);
        pd_canvas_destroy(&base);
}

static void should_draw_underline_beyond_line_box(void)
{
        pd_canvas_t base, underline;

        set_text_style("text-decoration", "none");
        set_text_style("text-underline-offset", "12px");
        set_text_style("text-decoration-thickness", "2px");
        set_text_style("height", "24px");
        render_text(&base);
        set_text_style("text-decoration", "underline");
        render_text(&underline);
        ctest_equal_bool(
            "underline below the text box bottom should still be drawn",
            count_different_pixels(&base, &underline) > 0, true);
        ctest_equal_bool(
            "underline overflow should expand the canvas box for real clipping",
            text_widget->canvas_box.height > text_widget->border_box.height,
            true);
        set_text_style("text-decoration", "none");
        set_text_style("height", "96px");
        set_text_style("text-underline-offset", "auto");
        set_text_style("text-decoration-thickness", "auto");
        pd_canvas_destroy(&underline);
        pd_canvas_destroy(&base);
}

static void should_align_wrapped_underline_rows(void)
{
        pd_canvas_t base, left, center, right;

        set_text_style("content", "\"A A A A A A A\"");
        set_text_style("width", "52px");
        set_text_style("white-space", "normal");
        set_text_style("text-align", "left");
        set_text_style("text-decoration", "none");
        set_text_style("text-underline-offset", "2px");
        set_text_style("text-decoration-thickness", "1px");
        render_text(&base);
        set_text_style("text-decoration", "underline");
        render_text(&left);
        ctest_equal_bool("narrow text should wrap into multiple line rows",
                         count_different_rows(&base, &left) >= 2, true);

        set_text_style("text-align", "center");
        render_text(&center);
        set_text_style("text-align", "right");
        render_text(&right);
        ctest_equal_bool("center alignment should reposition wrapped text",
                         count_different_pixels(&left, &center) > 0, true);
        ctest_equal_bool("right alignment should reposition wrapped text",
                         count_different_pixels(&center, &right) > 0, true);
        pd_canvas_destroy(&right);
        pd_canvas_destroy(&center);
        pd_canvas_destroy(&left);
        pd_canvas_destroy(&base);
}

static void should_scale_auto_and_subpixel_thickness(void)
{
        pd_canvas_t auto_96, dp_96, small_96, pixel_96;
        pd_canvas_t auto_192, dp_192, small_192, half_192;

        ui_metrics.scale = 1.f;
        ui_metrics.density = 1.f;
        ui_metrics.scaled_density = 1.f;
        ui_metrics.dpi = 96.f;
        ui_widget_request_refresh_style(text_widget);
        ui_update();
        set_text_style("text-decoration", "underline");
        set_text_style("text-underline-offset", "auto");
        set_text_style("text-decoration-thickness", "auto");
        render_text(&auto_96);
        set_text_style("text-underline-offset", "1dp");
        set_text_style("text-decoration-thickness", "1dp");
        render_text(&dp_96);
        ctest_equal_int("auto geometry should equal one dp at 96 dpi",
                        count_different_pixels(&auto_96, &dp_96), 0);
        set_text_style("text-decoration-thickness", "0.25dp");
        render_text(&small_96);
        set_text_style("text-decoration-thickness", "1px");
        render_text(&pixel_96);
        ctest_equal_int(
            "subpixel thickness should clamp to one pixel at 96 dpi",
            count_different_pixels(&small_96, &pixel_96), 0);

        ui_metrics.dpi = 192.f;
        ui_widget_request_refresh_style(text_widget);
        ui_update();
        set_text_style("text-underline-offset", "auto");
        set_text_style("text-decoration-thickness", "auto");
        render_text(&auto_192);
        set_text_style("text-underline-offset", "1dp");
        set_text_style("text-decoration-thickness", "1dp");
        render_text(&dp_192);
        ctest_equal_int("auto geometry should equal one dp at 192 dpi",
                        count_different_pixels(&auto_192, &dp_192), 0);
        set_text_style("text-decoration-thickness", "0.25dp");
        render_text(&small_192);
        set_text_style("text-decoration-thickness", "0.5px");
        render_text(&half_192);
        ctest_equal_int(
            "positive subpixel dp should clamp to one pixel at 192 dpi",
            count_different_pixels(&small_192, &half_192), 0);

        pd_canvas_destroy(&half_192);
        pd_canvas_destroy(&small_192);
        pd_canvas_destroy(&dp_192);
        pd_canvas_destroy(&auto_192);
        pd_canvas_destroy(&pixel_96);
        pd_canvas_destroy(&small_96);
        pd_canvas_destroy(&dp_96);
        pd_canvas_destroy(&auto_96);
}

static void should_scale_explicit_pixel_and_point_lengths(void)
{
        pd_canvas_t px_96, dp_96, pt_96, px_offset_96, dp_offset_96;
        pd_canvas_t px_192, dp_192, pt_192, dp_150_192, px_offset_192;
        pd_canvas_t dp_offset_192;

        ui_metrics.scale = 1.f;
        ui_metrics.density = 1.f;
        ui_metrics.scaled_density = 1.f;
        ui_metrics.dpi = 96.f;
        ui_widget_request_refresh_style(text_widget);
        ui_update();
        set_text_style("text-decoration", "underline");
        set_text_style("text-underline-offset", "0px");
        set_text_style("text-decoration-thickness", "1px");
        render_text(&px_96);
        set_text_style("text-decoration-thickness", "1dp");
        render_text(&dp_96);
        ctest_equal_int("px and dp thickness should match at 96 dpi",
                        count_different_pixels(&px_96, &dp_96), 0);
        set_text_style("text-decoration-thickness", "1pt");
        render_text(&pt_96);
        ctest_equal_int("pt and dp thickness should match at 96 dpi",
                        count_different_pixels(&dp_96, &pt_96), 0);

        set_text_style("text-decoration-thickness", "1px");
        set_text_style("text-underline-offset", "1px");
        render_text(&px_offset_96);
        set_text_style("text-underline-offset", "1dp");
        render_text(&dp_offset_96);
        ctest_equal_int("px and dp offsets should match at 96 dpi",
                        count_different_pixels(&px_offset_96, &dp_offset_96),
                        0);

        ui_metrics.dpi = 192.f;
        ui_widget_request_refresh_style(text_widget);
        ui_update();
        set_text_style("text-underline-offset", "0px");
        set_text_style("text-decoration-thickness", "1px");
        render_text(&px_192);
        set_text_style("text-decoration-thickness", "1dp");
        render_text(&dp_192);
        ctest_equal_int("px and dp thickness should match at 192 dpi",
                        count_different_pixels(&px_192, &dp_192), 0);
        set_text_style("text-decoration-thickness", "1pt");
        render_text(&pt_192);
        set_text_style("text-decoration-thickness", "1.5dp");
        render_text(&dp_150_192);
        ctest_equal_int("1pt should match 1.5dp at 192 dpi",
                        count_different_pixels(&dp_150_192, &pt_192), 0);

        set_text_style("text-decoration-thickness", "1px");
        set_text_style("text-underline-offset", "1px");
        render_text(&px_offset_192);
        set_text_style("text-underline-offset", "1dp");
        render_text(&dp_offset_192);
        ctest_equal_int("px and dp offsets should match at 192 dpi",
                        count_different_pixels(&px_offset_192, &dp_offset_192),
                        0);

        pd_canvas_destroy(&dp_offset_192);
        pd_canvas_destroy(&px_offset_192);
        pd_canvas_destroy(&dp_150_192);
        pd_canvas_destroy(&pt_192);
        pd_canvas_destroy(&dp_192);
        pd_canvas_destroy(&px_192);
        pd_canvas_destroy(&dp_offset_96);
        pd_canvas_destroy(&px_offset_96);
        pd_canvas_destroy(&pt_96);
        pd_canvas_destroy(&dp_96);
        pd_canvas_destroy(&px_96);
}

static void should_reserve_canvas_space_for_shadow_and_underline(void)
{
        float base_h, shadow_h, both_h;

        set_text_style("text-decoration", "none");
        set_text_style("box-shadow", "none");
        ui_update();
        base_h = text_widget->canvas_box.height;

        set_text_style("box-shadow", "0 0 6px #000000");
        ui_update();
        shadow_h = text_widget->canvas_box.height;

        set_text_style("text-decoration", "underline");
        ui_update();
        both_h = text_widget->canvas_box.height;

        ctest_equal_bool("box-shadow should expand the canvas box",
                         shadow_h > base_h, true);
        ctest_equal_bool("underline overflow should add on top of box-shadow",
                         both_h > shadow_h, true);

        set_text_style("text-decoration", "none");
        set_text_style("box-shadow", "none");
}

static void build_text_widget(void)
{
        text_widget = ui_create_widget("text");
        ui_widget_set_style_string(text_widget, "width", "160px");
        ui_widget_set_style_string(text_widget, "height", "96px");
        ui_widget_set_style_string(text_widget, "font-size", "16px");
        ui_widget_set_style_string(text_widget, "line-height", "24px");
        ui_widget_set_style_string(text_widget, "font-family", "monospace");
        ui_widget_set_style_string(text_widget, "color", "#000");
        ui_widget_set_style_string(text_widget, "white-space", "nowrap");
        ui_widget_set_style_string(text_widget, "content", "\"A   A   A   A\"");
        ui_widget_append(ui_root(), text_widget);
        ui_widget_on(text_widget, "afterlayout", on_text_after_layout,
                     &after_layout_count);
        ui_widget_resize(ui_root(), 512, 320);
        ui_update();
}

void test_text_decoration(void)
{
        lcui_init();
        ui_metrics.scale = 1.f;
        ui_metrics.density = 1.f;
        ui_metrics.scaled_density = 1.f;
        ui_metrics.dpi = 96.f;
        build_text_widget();
        ctest_describe("text style cache initialization",
                       should_initialize_ui_text_style_cache_fields);
        ctest_describe("text decoration defaults",
                       should_default_to_no_decoration);
        ctest_describe("text underline styles",
                       should_apply_and_remove_underline);
        ctest_describe("text underline offset and thickness",
                       should_move_and_resize_underline);
        ctest_describe("text underline beyond line box",
                       should_draw_underline_beyond_line_box);
        ctest_describe("text underline wrapping and alignment",
                       should_align_wrapped_underline_rows);
        ctest_describe("text underline dirty redraw",
                       should_mark_underline_changes_dirty);
        ctest_describe("text underline no reflow",
                       should_not_reflow_for_underline_style_changes);
        ctest_describe("text underline scaling",
                       should_scale_auto_and_subpixel_thickness);
        ctest_describe("explicit underline length scaling",
                       should_scale_explicit_pixel_and_point_lengths);
        ctest_describe("canvas box space for shadow and underline",
                       should_reserve_canvas_space_for_shadow_and_underline);
        ui_metrics.dpi = 96.f;
        ui_metrics.scale = 1.f;
        ui_metrics.density = 1.f;
        ui_metrics.scaled_density = 1.f;
        lcui_destroy();
}
