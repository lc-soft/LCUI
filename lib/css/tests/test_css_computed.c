/*
 * lib/css/tests/test_css_computed.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include "test.h"
#include "ctest.h"
#include "../include/css.h"

static void check_length(const char *str, css_numeric_value_t actual,
                         css_numeric_value_t expected)
{
        ctest_equal_int(str, (int)actual, (int)expected);
}

static void test_button_focus_style(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".primary.button:focus");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_str("content", computed.content, "submit");
        check_length("font-size", computed.font_size, 16);
        check_length("min-width", computed.min_width, 80);
        ctest_equal_int("box-sizing", computed.type_bits.box_sizing,
                        CSS_BOX_SIZING_BORDER_BOX);

        check_length("padding-left", computed.padding_left, 8);
        check_length("padding-right", computed.padding_right, 8);
        check_length("padding-top", computed.padding_top, 4);
        check_length("padding-bottom", computed.padding_bottom, 4);

        check_length("border-top-width", computed.border_top_width, 1);
        check_length("border-right-width", computed.border_right_width, 1);
        check_length("border-bottom-width", computed.border_bottom_width, 1);
        check_length("border-left-width", computed.border_left_width, 1);

        ctest_equal_int("border-top-style", computed.type_bits.border_top_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_int("border-right-style",
                        computed.type_bits.border_right_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_int("border-bottom-style",
                        computed.type_bits.border_bottom_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_int("border-left-style",
                        computed.type_bits.border_left_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_uint("background-color", computed.background_color,
                         css_color(255, 0, 0x50, 0xc9));

        check_length("border-top-left-radius", computed.border_top_left_radius,
                     4);
        check_length("border-top-right-radius",
                     computed.border_top_right_radius, 4);
        check_length("border-bottom-left-radius",
                     computed.border_bottom_left_radius, 4);
        check_length("border-bottom-right-radius",
                     computed.border_bottom_right_radius, 4);
        check_length("box-shadow-x", computed.box_shadow_x, 0);
        check_length("box-shadow-y", computed.box_shadow_y, 0);
        check_length("box-shadow-blur", computed.box_shadow_blur, 0);
        check_length("box-shadow-spread", computed.box_shadow_spread, 2);
        ctest_equal_uint("box-shadow-color", computed.box_shadow_color,
                         css_color(127, 0, 0, 200));

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_container_style(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".container");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        check_length("margin-top", computed.margin_top, 24);
        check_length("margin-bottom", computed.margin_top, 24);
        ctest_equal_int("margin-left", computed.type_bits.margin_left,
                        CSS_MARGIN_AUTO);
        ctest_equal_int("margin-right", computed.type_bits.margin_right,
                        CSS_MARGIN_AUTO);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_w360px_style(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".w-\\[360px\\]");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        check_length("width", computed.width, 360);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_button_borderless_style(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".button.borderless");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        check_length("border-top-width", computed.border_top_width, 0);
        check_length("border-right-width", computed.border_right_width, 0);
        check_length("border-bottom-width", computed.border_bottom_width, 0);
        check_length("border-left-width", computed.border_left_width, 0);
        ctest_equal_int("border-top-style", computed.type_bits.border_top_style,
                        CSS_BORDER_STYLE_NONE);
        ctest_equal_int("border-right-style",
                        computed.type_bits.border_right_style,
                        CSS_BORDER_STYLE_NONE);
        ctest_equal_int("border-bottom-style",
                        computed.type_bits.border_bottom_style,
                        CSS_BORDER_STYLE_NONE);
        ctest_equal_int("border-left-style",
                        computed.type_bits.border_left_style,
                        CSS_BORDER_STYLE_NONE);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_flex_gap_shorthand_one_value(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".flex-gap");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_int("row-gap type", computed.type_bits.row_gap,
                        CSS_GAP_SET);
        ctest_equal_int("column-gap type", computed.type_bits.column_gap,
                        CSS_GAP_SET);
        ctest_equal_int("row-gap unit", computed.unit_bits.row_gap,
                        CSS_UNIT_PX);
        ctest_equal_int("column-gap unit", computed.unit_bits.column_gap,
                        CSS_UNIT_PX);
        check_length("row-gap", computed.row_gap, 12);
        check_length("column-gap", computed.column_gap, 12);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_flex_gap_shorthand_two_values(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".flex-gap-two-values");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_int("row-gap type", computed.type_bits.row_gap,
                        CSS_GAP_SET);
        ctest_equal_int("column-gap type", computed.type_bits.column_gap,
                        CSS_GAP_SET);
        check_length("row-gap", computed.row_gap, 8);
        check_length("column-gap", computed.column_gap, 16);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_flex_gap_axis(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".flex-gap-axis");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_int("row-gap type", computed.type_bits.row_gap,
                        CSS_GAP_SET);
        ctest_equal_int("column-gap type", computed.type_bits.column_gap,
                        CSS_GAP_SET);
        check_length("row-gap", computed.row_gap, 4);
        check_length("column-gap", computed.column_gap, 20);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_flex_gap_normal(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".flex-gap-normal");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_int("row-gap type", computed.type_bits.row_gap,
                        CSS_GAP_NORMAL);
        ctest_equal_int("column-gap type", computed.type_bits.column_gap,
                        CSS_GAP_NORMAL);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_flex_gap_percentage(void)
{
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed;

        selector = css_selector_create(".flex-gap-percentage");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);

        ctest_equal_int("row-gap type", computed.type_bits.row_gap,
                        CSS_GAP_SET);
        ctest_equal_int("column-gap type", computed.type_bits.column_gap,
                        CSS_GAP_SET);
        ctest_equal_int("row-gap unit", computed.unit_bits.row_gap,
                        CSS_UNIT_PERCENT);
        ctest_equal_int("column-gap unit", computed.unit_bits.column_gap,
                        CSS_UNIT_PERCENT);
        check_length("row-gap", computed.row_gap, 25);
        check_length("column-gap", computed.column_gap, 50);

        css_style_decl_destroy(result);
        css_selector_destroy(selector);
        css_computed_style_destroy(&computed);
}

void test_css_computed(void)
{
        size_t n;
        FILE *fp;
        char buff[512];
        css_parser_t *parser;

        fp = fopen("test_css_computed.css", "r");
        if (!fp) {
                return;
        }
        css_init();
        parser = css_parser_create(__FILE__);
        n = fread(buff, 1, 511, fp);
        while (n > 0) {
                buff[n] = 0;
                css_parser_parse(parser, buff);
                n = fread(buff, 1, 511, fp);
        }
        css_parser_destroy(parser);
        fclose(fp);

        ctest_describe(".primary.button:focus", test_button_focus_style);
        ctest_describe(".container", test_container_style);
        ctest_describe(".button.borderless", test_button_borderless_style);
        ctest_describe(".w-[360px]", test_w360px_style);
        ctest_describe(".flex-gap (one value)",
                       test_flex_gap_shorthand_one_value);
        ctest_describe(".flex-gap-two-values",
                       test_flex_gap_shorthand_two_values);
        ctest_describe(".flex-gap-axis", test_flex_gap_axis);
        ctest_describe(".flex-gap-normal", test_flex_gap_normal);
        ctest_describe(".flex-gap-percentage", test_flex_gap_percentage);

        css_destroy();
}
