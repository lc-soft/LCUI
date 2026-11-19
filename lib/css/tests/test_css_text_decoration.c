/*
 * lib/css/tests/test_css_text_decoration.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <css.h>
#include <ctest.h>

static int set_property(css_style_decl_t *decl, const char *name,
                        const char *text)
{
        css_propdef_t *propdef = css_get_propdef_by_name(name);
        css_style_value_t value = { 0 };
        int ret;

        if (!propdef) {
                return 0;
        }
        ret = css_parse_value(propdef->valdef, text, &value);
        if (ret > 0) {
                css_style_decl_set(decl, propdef->key, &value);
        }
        css_style_value_destroy(&value);
        return ret;
}

static void should_register_underline_keyword(void)
{
        ctest_equal_int("should register underline keyword",
                        css_get_keyword_key("underline"),
                        CSS_KEYWORD_UNDERLINE);
}

static void should_use_none_and_auto_as_initial_values(void)
{
        css_computed_style_t computed = { 0 };
        css_style_decl_t *decl = css_style_decl_create();

        css_cascade_style(decl, &computed);
        ctest_equal_int("should default text decoration to none",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        ctest_equal_int("should default underline offset to auto",
                        computed.type_bits.text_underline_offset,
                        CSS_TEXT_UNDERLINE_OFFSET_AUTO);
        ctest_equal_int("should default decoration thickness to auto",
                        computed.type_bits.text_decoration_thickness,
                        CSS_TEXT_DECORATION_THICKNESS_AUTO);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_cascade_underline_and_none(void)
{
        css_computed_style_t computed = { 0 };
        css_style_decl_t *decl = css_style_decl_create();

        ctest_equal_bool(
            "should parse underline",
            set_property(decl, "text-decoration-line", "underline") > 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should cascade underline",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);

        decl = css_style_decl_create();
        ctest_equal_bool("should parse none",
                         set_property(decl, "text-decoration-line", "none") > 0,
                         true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should cascade none",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_expand_text_decoration_shorthand(void)
{
        css_parser_t *parser;
        css_selector_t *selector;
        css_style_decl_t *result;
        css_computed_style_t computed = { 0 };

        parser = css_parser_create(__FILE__);
        css_parser_parse(
            parser, ".td-line { text-decoration: underline }\n"
                    ".td-mixed { text-decoration: 3px underline }\n"
                    ".td-override { text-decoration: underline;\n"
                    "                      text-decoration-line: none }\n"
                    ".td-override2 { text-decoration-line: none;\n"
                    "                       text-decoration: underline }\n");
        css_parser_destroy(parser);

        selector = css_selector_create(".td-line");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);
        ctest_equal_int("should set line from shorthand",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        ctest_equal_int("should keep omitted thickness at auto",
                        computed.type_bits.text_decoration_thickness,
                        CSS_TEXT_DECORATION_THICKNESS_AUTO);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(result);
        css_selector_destroy(selector);

        selector = css_selector_create(".td-mixed");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);
        ctest_equal_int("should split length into thickness",
                        computed.unit_bits.text_decoration_thickness,
                        CSS_UNIT_PX);
        ctest_equal_float("should keep the thickness value",
                          computed.text_decoration_thickness, 3.f);
        ctest_equal_int("should split keyword into line",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(result);
        css_selector_destroy(selector);

        selector = css_selector_create(".td-override");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);
        ctest_equal_int("later longhand should override the shorthand",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(result);
        css_selector_destroy(selector);

        selector = css_selector_create(".td-override2");
        result = css_select_style(selector);
        css_cascade_style(result, &computed);
        ctest_equal_int("later shorthand should override the longhand",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(result);
        css_selector_destroy(selector);
}

static void should_parse_offset_and_thickness_values(void)
{
        css_computed_style_t computed = { 0 };
        css_style_decl_t *decl = css_style_decl_create();

        ctest_equal_bool(
            "should parse auto offset",
            set_property(decl, "text-underline-offset", "auto") > 0, true);
        ctest_equal_bool(
            "should parse auto thickness",
            set_property(decl, "text-decoration-thickness", "auto") > 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should cascade auto offset",
                        computed.type_bits.text_underline_offset,
                        CSS_TEXT_UNDERLINE_OFFSET_AUTO);
        ctest_equal_int("should cascade auto thickness",
                        computed.type_bits.text_decoration_thickness,
                        CSS_TEXT_DECORATION_THICKNESS_AUTO);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);

        decl = css_style_decl_create();
        ctest_equal_bool("should parse pixel offset",
                         set_property(decl, "text-underline-offset", "4px") > 0,
                         true);
        ctest_equal_bool(
            "should parse pixel thickness",
            set_property(decl, "text-decoration-thickness", "4px") > 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should keep pixel offset unit",
                        computed.unit_bits.text_underline_offset, CSS_UNIT_PX);
        ctest_equal_float("should keep pixel offset value",
                          computed.text_underline_offset, 4.f);
        ctest_equal_int("should keep pixel thickness unit",
                        computed.unit_bits.text_decoration_thickness,
                        CSS_UNIT_PX);
        ctest_equal_float("should keep pixel thickness value",
                          computed.text_decoration_thickness, 4.f);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);

        decl = css_style_decl_create();
        ctest_equal_bool(
            "should parse negative offset",
            set_property(decl, "text-underline-offset", "-2px") > 0, true);
        ctest_equal_bool(
            "should parse zero thickness",
            set_property(decl, "text-decoration-thickness", "0px") > 0, true);
        ctest_equal_bool(
            "should parse negative thickness",
            set_property(decl, "text-decoration-thickness", "-1px") > 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_float("should preserve negative offset for rendering",
                          computed.text_underline_offset, -2.f);
        ctest_equal_float("should preserve negative thickness for rendering",
                          computed.text_decoration_thickness, -1.f);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_convert_absolute_offset_and_thickness_units(void)
{
        css_computed_style_t computed = { 0 };
        css_metrics_t metrics = { 192.f, 2.f, 1.f, 2.f };
        css_style_decl_t *decl = css_style_decl_create();

        set_property(decl, "text-underline-offset", "4dp");
        set_property(decl, "text-decoration-thickness", "4pt");
        css_cascade_style(decl, &computed);
        css_compute_absolute_values(NULL, &computed, &metrics);

        ctest_equal_int("should convert dp offset to pixels",
                        computed.unit_bits.text_underline_offset, CSS_UNIT_PX);
        ctest_equal_float("should normalize dp offset to logical css pixels",
                          computed.text_underline_offset, 8.f);
        ctest_equal_int("should convert pt thickness to pixels",
                        computed.unit_bits.text_decoration_thickness,
                        CSS_UNIT_PX);
        ctest_equal_float("should normalize pt thickness to logical css pixels",
                          computed.text_decoration_thickness,
                          4.f * 96.f / 72.f);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_keep_pixel_lengths_in_logical_css_pixels(void)
{
        css_computed_style_t computed = { 0 };
        css_metrics_t metrics = { 192.f, 1.f, 1.f, 2.f };
        css_style_decl_t *decl = css_style_decl_create();

        set_property(decl, "text-underline-offset", "4px");
        set_property(decl, "text-decoration-thickness", "4px");
        css_cascade_style(decl, &computed);
        css_compute_absolute_values(NULL, &computed, &metrics);
        ctest_equal_float("should preserve px offset before UI scaling",
                          computed.text_underline_offset, 4.f);
        ctest_equal_float("should preserve px thickness before UI scaling",
                          computed.text_decoration_thickness, 4.f);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_reject_unsupported_decoration_lengths(void)
{
        css_propdef_t *propdef;
        css_style_value_t value = { 0 };

        propdef = css_get_propdef_by_name("text-decoration-thickness");
        ctest_equal_bool("should reject font-relative thickness",
                         css_parse_value(propdef->valdef, "1em", &value) <= 0,
                         true);
        css_style_value_destroy(&value);
        ctest_equal_bool("should reject percentage thickness",
                         css_parse_value(propdef->valdef, "10%", &value) <= 0,
                         true);
        css_style_value_destroy(&value);
        ctest_equal_bool(
            "should reject from-font thickness",
            css_parse_value(propdef->valdef, "from-font", &value) <= 0, true);
        css_style_value_destroy(&value);
}

static void should_cascade_decoration_line_longhand(void)
{
        css_computed_style_t computed = { 0 };
        css_style_decl_t *decl = css_style_decl_create();

        ctest_equal_bool(
            "should parse longhand underline",
            set_property(decl, "text-decoration-line", "underline") > 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should cascade longhand underline",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);

        decl = css_style_decl_create();
        ctest_equal_bool("should parse longhand none",
                         set_property(decl, "text-decoration-line", "none") > 0,
                         true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should cascade longhand none",
                        computed.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);

        decl = css_style_decl_create();
        ctest_equal_bool(
            "should reject overline value",
            set_property(decl, "text-decoration-line", "overline") <= 0, true);
        css_style_decl_destroy(decl);
}

void test_css_text_decoration(void)
{
        css_init();
        ctest_describe("underline keyword", should_register_underline_keyword);
        ctest_describe("text decoration initial values",
                       should_use_none_and_auto_as_initial_values);
        ctest_describe("text decoration cascade",
                       should_cascade_underline_and_none);
        ctest_describe("text decoration line longhand",
                       should_cascade_decoration_line_longhand);
        ctest_describe("text decoration shorthand expansion",
                       should_expand_text_decoration_shorthand);
        ctest_describe("underline offset and thickness values",
                       should_parse_offset_and_thickness_values);
        ctest_describe("underline absolute units",
                       should_convert_absolute_offset_and_thickness_units);
        ctest_describe("underline logical px units",
                       should_keep_pixel_lengths_in_logical_css_pixels);
        ctest_describe("unsupported underline values",
                       should_reject_unsupported_decoration_lengths);
        css_destroy();
}
