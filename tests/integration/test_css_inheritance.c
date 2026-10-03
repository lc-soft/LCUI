/*
 * tests/integration/test_css_inheritance.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <stdlib.h>
#include <ctest-custom.h>

static ui_widget_t *parent;
static ui_widget_t *root;
static ui_widget_t *inherited_child;
static ui_widget_t *inherited_grandchild;
static ui_widget_t *overridden_child;
static ui_widget_t *width_child;
static ui_widget_t *default_width_child;
static ui_widget_t *percentage_child;
static ui_widget_t *percentage_grandchild;
static ui_widget_t *fixed_child;
static ui_widget_t *fixed_grandchild;
static ui_widget_t *text_container;
static ui_widget_t *percentage_line_container;
static ui_widget_t *normal_line_text;
static ui_widget_t *number_line_text;
static ui_widget_t *percentage_line_text;
static ui_widget_t *wrapping_text;
static ui_widget_t *fixed_text;
static float initial_text_container_height;
static float initial_normal_line_height;
static float initial_number_line_height;
static float initial_percentage_line_height;
static float initial_wrapping_text_height;
static float initial_fixed_text_height;
static int text_container_layout_count;

static void test_text_style_comparison(void)
{
        ui_text_style_t a, b;

        ui_text_style_init(&a);
        ui_text_style_init(&b);
        ctest_equal_bool("should consider default text metrics equal",
                         ui_text_style_metrics_equal(&a, &b), true);
        ctest_equal_bool("should consider identical complete styles equal",
                         ui_text_style_is_equal(&a, &b), true);

        b.font_size = 18;
        ctest_equal_bool("should detect a changed text metric",
                         ui_text_style_metrics_equal(&a, &b), false);
        ctest_equal_bool("should detect changed metrics in full style",
                         ui_text_style_is_equal(&a, &b), false);
        b.font_size = a.font_size;

        b.word_break = PD_WORD_BREAK_BREAK_ALL;
        ctest_equal_bool("should treat word-break as a text metric",
                         ui_text_style_metrics_equal(&a, &b), false);
        ctest_equal_bool("should include word-break in full style equality",
                         ui_text_style_is_equal(&a, &b), false);
        b.word_break = a.word_break;

        a.font_ids = malloc(2 * sizeof(int));
        b.font_ids = malloc(3 * sizeof(int));
        a.font_ids[0] = 1;
        a.font_ids[1] = 0;
        b.font_ids[0] = 1;
        b.font_ids[1] = 0;
        ctest_equal_bool("should compare identical font id lists",
                         ui_text_style_metrics_equal(&a, &b), true);
        b.font_ids[0] = 2;
        ctest_equal_bool("should detect different font id lists",
                         ui_text_style_metrics_equal(&a, &b), false);
        b.font_ids[0] = 1;
        b.font_ids[1] = 2;
        b.font_ids[2] = 0;
        ctest_equal_bool("should detect different font id list lengths",
                         ui_text_style_metrics_equal(&a, &b), false);
        b.font_ids[1] = 0;
        b.font_ids[2] = 0;

        b.color.value = 0xff123456;
        ctest_equal_bool("should exclude paint color from metric equality",
                         ui_text_style_metrics_equal(&a, &b), true);
        ctest_equal_bool("should include paint color in full style equality",
                         ui_text_style_is_equal(&a, &b), false);
        b.color = a.color;

        b.text_align = PD_TEXT_ALIGN_CENTER;
        ctest_equal_bool("should exclude alignment from text metric equality",
                         ui_text_style_metrics_equal(&a, &b), true);
        ctest_equal_bool("should include alignment in full style equality",
                         ui_text_style_is_equal(&a, &b), false);
        b.text_align = a.text_align;

        b.text_decoration = CSS_TEXT_DECORATION_UNDERLINE;
        ctest_equal_bool("should exclude decoration from text metric equality",
                         ui_text_style_metrics_equal(&a, &b), true);
        ctest_equal_bool("should include decoration in full style equality",
                         ui_text_style_is_equal(&a, &b), false);
        b.text_decoration = a.text_decoration;

        b.content = L"different content";
        ctest_equal_bool("should exclude content from metric equality",
                         ui_text_style_metrics_equal(&a, &b), true);
        ctest_equal_bool("should include content in full style equality",
                         ui_text_style_is_equal(&a, &b), false);
        b.content = NULL;
        ui_text_style_destroy(&a);
        ui_text_style_destroy(&b);
}

static void on_text_container_afterlayout(ui_widget_t *w, ui_event_t *e,
                                          void *emit_arg)
{
        int *count = e->data;

        ++*count;
}

static void test_initial_inheritance(void)
{
        ctest_equal_uint(
            "should store style dependencies as 32-bit words",
            (unsigned)sizeof(inherited_child->style_dependencies),
            ((STYLE_KEY_TOTAL + 31) / 32) * (unsigned)sizeof(uint32_t));
        ctest_equal_uint("should inherit color into direct children",
                         inherited_child->specified_style.color,
                         css_color(255, 0x12, 0x34, 0x56));
        ctest_equal_uint("should inherit color through multiple generations",
                         inherited_grandchild->specified_style.color,
                         css_color(255, 0x12, 0x34, 0x56));
        ctest_equal_uint("should prefer a local color declaration",
                         overridden_child->specified_style.color,
                         css_color(255, 0xab, 0xcd, 0xef));
        ctest_equal_float("should inherit computed font size",
                          inherited_child->specified_style.font_size, 20.0f);
        ctest_equal_float("should apply explicit inherited width",
                          width_child->specified_style.width, 80.0f);
        ctest_equal_int("should preserve explicit inherited width type",
                        width_child->specified_style.type_bits.width,
                        CSS_WIDTH_SET);
        ctest_equal_int("should not inherit width by default",
                        default_width_child->specified_style.type_bits.width,
                        CSS_WIDTH_AUTO);
        ctest_equal_float("should compute percentage font size from parent",
                          percentage_child->specified_style.font_size, 30.0f);
        ctest_equal_float("should inherit computed percentage font size",
                          percentage_grandchild->specified_style.font_size,
                          30.0f);
        ctest_equal_float("should retain fixed font size branch",
                          fixed_grandchild->specified_style.font_size, 12.0f);
        initial_text_container_height = text_container->border_box.height;
        initial_normal_line_height = normal_line_text->border_box.height;
        initial_number_line_height = number_line_text->border_box.height;
        initial_percentage_line_height =
            percentage_line_text->border_box.height;
        initial_wrapping_text_height = wrapping_text->border_box.height;
        initial_fixed_text_height = fixed_text->border_box.height;
}

static void test_parent_style_change_propagates_selectively(void)
{
        ui_widget_set_style_string(parent, "color", "#654321");
        text_container_layout_count = 0;
        ui_widget_set_style_string(root, "font-size", "30px");
        ui_update();

        ctest_equal_uint("should update inherited color in one UI update",
                         inherited_child->specified_style.color,
                         css_color(255, 0x65, 0x43, 0x21));
        ctest_equal_uint("should update inherited color in descendants",
                         inherited_grandchild->specified_style.color,
                         css_color(255, 0x65, 0x43, 0x21));
        ctest_equal_uint("should keep local color overrides unchanged",
                         overridden_child->specified_style.color,
                         css_color(255, 0xab, 0xcd, 0xef));
        ctest_equal_float("should update percentage font size from parent",
                          percentage_child->specified_style.font_size, 45.0f);
        ctest_equal_float("should update percentage font size descendants",
                          percentage_grandchild->specified_style.font_size,
                          45.0f);
        ctest_equal_float("should stop propagation at fixed font size",
                          fixed_child->specified_style.font_size, 12.0f);
        ctest_equal_float("should preserve fixed font size descendants",
                          fixed_grandchild->specified_style.font_size, 12.0f);
        ctest_equal_bool(
            "should increase normal-line-height text geometry",
            normal_line_text->border_box.height > initial_normal_line_height,
            true);
        ctest_equal_bool(
            "should increase unitless-line-height text geometry",
            number_line_text->border_box.height > initial_number_line_height,
            true);
        ctest_equal_bool(
            "should resolve inherited percentage line height per element",
            percentage_line_text->border_box.height >
                initial_percentage_line_height,
            true);
        ctest_equal_bool(
            "should update wrapping text measurement at the new font size",
            wrapping_text->border_box.height != initial_wrapping_text_height,
            true);
        ctest_equal_bool("should reflow the parent in the same update",
                         text_container_layout_count > 0, true);
        ctest_equal_float("should keep fixed-font text geometry stable",
                          fixed_text->border_box.height,
                          initial_fixed_text_height);
}

static void clear_dirty_rects(void)
{
        list_t rects;

        list_create(&rects);
        ui_widget_get_dirty_rects(root, &rects);
        list_destroy(&rects, free);
}

static void test_paint_only_change_does_not_reflow(void)
{
        float container_height = text_container->border_box.height;
        float line_height = normal_line_text->border_box.height;
        list_t rects;

        clear_dirty_rects();
        ui_widget_set_style_string(root, "color", "#654abc");
        ui_update();
        list_create(&rects);
        ui_widget_get_dirty_rects(root, &rects);
        ctest_equal_bool("should request repaint for inherited text color",
                         rects.length > 0, true);
        list_destroy(&rects, free);
        ctest_equal_float("should not change text height for color alone",
                          normal_line_text->border_box.height, line_height);
        ctest_equal_float("should not reflow parent for color alone",
                          text_container->border_box.height, container_height);

        ui_widget_set_style_string(root, "font-size", "30px");
        ui_update();
        ctest_equal_float("should preserve geometry for an unchanged font size",
                          normal_line_text->border_box.height, line_height);
        ctest_equal_float("should not reflow for an unchanged font size",
                          text_container->border_box.height, container_height);
}

static void test_inherited_word_break_reflows_text(void)
{
        text_container_layout_count = 0;
        ui_widget_set_style_string(text_container, "word-break", "break-all");
        ui_update();

        ctest_equal_int("should inherit the updated word-break mode",
                        wrapping_text->specified_style.type_bits.word_break,
                        CSS_WORD_BREAK_BREAK_ALL);
        ctest_equal_bool("should reflow the parent after word-break changes",
                         text_container_layout_count > 0, true);
}

void test_css_inheritance(void)
{
        lcui_init();
        root = ui_root();
        parent = ui_create_widget(NULL);
        inherited_child = ui_create_widget(NULL);
        inherited_grandchild = ui_create_widget(NULL);
        overridden_child = ui_create_widget(NULL);
        width_child = ui_create_widget(NULL);
        default_width_child = ui_create_widget(NULL);
        percentage_child = ui_create_widget(NULL);
        percentage_grandchild = ui_create_widget(NULL);
        fixed_child = ui_create_widget(NULL);
        fixed_grandchild = ui_create_widget(NULL);
        text_container = ui_create_widget(NULL);
        percentage_line_container = ui_create_widget(NULL);
        normal_line_text = ui_create_widget("text");
        number_line_text = ui_create_widget("text");
        percentage_line_text = ui_create_widget("text");
        wrapping_text = ui_create_widget("text");
        fixed_text = ui_create_widget("text");

        ui_widget_set_style_string(parent, "color", "#123456");
        ui_widget_set_style_string(root, "font-size", "20px");
        ui_widget_set_style_string(parent, "width", "80px");
        ui_widget_set_style_string(overridden_child, "color", "#abcdef");
        ui_widget_set_style_string(width_child, "width", "inherit");
        ui_widget_set_style_string(percentage_child, "font-size", "150%");
        ui_widget_set_style_string(fixed_child, "font-size", "12px");
        ui_widget_set_style_string(text_container, "width", "120px");
        ui_widget_set_style_string(percentage_line_container, "width", "120px");
        ui_widget_set_style_string(percentage_line_container, "line-height",
                                   "150%");
        ui_widget_set_style_string(number_line_text, "line-height", "1.5");
        ui_widget_set_style_string(wrapping_text, "width", "100px");
        ui_widget_set_style_string(fixed_text, "font-size", "12px");
        ui_text_set_content(normal_line_text, "sample");
        ui_text_set_content(number_line_text, "sample");
        ui_text_set_content(percentage_line_text, "sample");
        ui_text_set_content(wrapping_text,
                            "This long sentence wraps over several lines when "
                            "the available width is narrow.");
        ui_text_set_content(fixed_text, "fixed size");

        ui_widget_append(inherited_child, inherited_grandchild);
        ui_widget_append(percentage_child, percentage_grandchild);
        ui_widget_append(fixed_child, fixed_grandchild);
        ui_widget_append(text_container, normal_line_text);
        ui_widget_append(text_container, number_line_text);
        ui_widget_append(text_container, wrapping_text);
        ui_widget_append(text_container, fixed_text);
        ui_widget_append(percentage_line_container, percentage_line_text);
        ui_widget_append(parent, inherited_child);
        ui_widget_append(parent, overridden_child);
        ui_widget_append(parent, width_child);
        ui_widget_append(parent, default_width_child);
        ui_widget_append(parent, percentage_child);
        ui_widget_append(parent, fixed_child);
        ui_widget_append(root, parent);
        ui_widget_append(root, text_container);
        ui_widget_append(root, percentage_line_container);
        ui_widget_resize(root, 400, 400);
        ui_widget_on(text_container, "afterlayout",
                     on_text_container_afterlayout,
                     &text_container_layout_count);
        ui_update();

        ctest_describe("initial inheritance", test_initial_inheritance);
        ctest_describe("text style comparison", test_text_style_comparison);
        ctest_describe("selective inheritance propagation",
                       test_parent_style_change_propagates_selectively);
        ctest_describe("paint-only style changes",
                       test_paint_only_change_does_not_reflow);
        ctest_describe("inherited word-break reflow",
                       test_inherited_word_break_reflows_text);
        lcui_destroy();
}
