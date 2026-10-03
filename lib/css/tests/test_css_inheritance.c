/*
 * lib/css/tests/test_css_inheritance.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <css.h>
#include "test.h"
#include "ctest.h"

extern int css_cascade_style_with_parent(const css_style_decl_t *style,
                                         const css_computed_style_t *parent,
                                         css_computed_style_t *computed);

typedef struct shorthand_test_case {
        const char *selector;
        const css_prop_key_t *keys;
        size_t key_count;
} shorthand_test_case_t;

static const css_prop_key_t background_keys[] = {
        css_prop_background_image,      css_prop_background_color,
        css_prop_background_position_x, css_prop_background_position_y,
        css_prop_background_size,       css_prop_background_repeat
};

static const css_prop_key_t background_position_keys[] = {
        css_prop_background_position_x, css_prop_background_position_y
};

static const css_prop_key_t padding_keys[] = { css_prop_padding_top,
                                               css_prop_padding_right,
                                               css_prop_padding_bottom,
                                               css_prop_padding_left };

static const css_prop_key_t margin_keys[] = { css_prop_margin_top,
                                              css_prop_margin_right,
                                              css_prop_margin_bottom,
                                              css_prop_margin_left };

static const css_prop_key_t border_color_keys[] = {
        css_prop_border_top_color, css_prop_border_right_color,
        css_prop_border_bottom_color, css_prop_border_left_color
};

static const css_prop_key_t border_width_keys[] = {
        css_prop_border_top_width, css_prop_border_right_width,
        css_prop_border_bottom_width, css_prop_border_left_width
};

static const css_prop_key_t border_style_keys[] = {
        css_prop_border_top_style, css_prop_border_right_style,
        css_prop_border_bottom_style, css_prop_border_left_style
};

static const css_prop_key_t border_radius_keys[] = {
        css_prop_border_top_left_radius, css_prop_border_top_right_radius,
        css_prop_border_bottom_right_radius, css_prop_border_bottom_left_radius
};

static const css_prop_key_t border_top_keys[] = { css_prop_border_top_width,
                                                  css_prop_border_top_style,
                                                  css_prop_border_top_color };

static const css_prop_key_t border_right_keys[] = {
        css_prop_border_right_width, css_prop_border_right_style,
        css_prop_border_right_color
};

static const css_prop_key_t border_bottom_keys[] = {
        css_prop_border_bottom_width, css_prop_border_bottom_style,
        css_prop_border_bottom_color
};

static const css_prop_key_t border_left_keys[] = { css_prop_border_left_width,
                                                   css_prop_border_left_style,
                                                   css_prop_border_left_color };

static const css_prop_key_t border_keys[] = {
        css_prop_border_top_width,    css_prop_border_top_style,
        css_prop_border_top_color,    css_prop_border_right_width,
        css_prop_border_right_style,  css_prop_border_right_color,
        css_prop_border_bottom_width, css_prop_border_bottom_style,
        css_prop_border_bottom_color, css_prop_border_left_width,
        css_prop_border_left_style,   css_prop_border_left_color
};

static const css_prop_key_t flex_keys[] = { css_prop_flex_basis,
                                            css_prop_flex_grow,
                                            css_prop_flex_shrink };

static const css_prop_key_t gap_keys[] = { css_prop_row_gap,
                                           css_prop_column_gap };

static const shorthand_test_case_t shorthand_cases[] = {
        { ".inherit-background", background_keys,
          sizeof(background_keys) / sizeof(background_keys[0]) },
        { ".inherit-background-position", background_position_keys,
          sizeof(background_position_keys) /
              sizeof(background_position_keys[0]) },
        { ".inherit-padding", padding_keys,
          sizeof(padding_keys) / sizeof(padding_keys[0]) },
        { ".inherit-margin", margin_keys,
          sizeof(margin_keys) / sizeof(margin_keys[0]) },
        { ".inherit-border-color", border_color_keys,
          sizeof(border_color_keys) / sizeof(border_color_keys[0]) },
        { ".inherit-border-width", border_width_keys,
          sizeof(border_width_keys) / sizeof(border_width_keys[0]) },
        { ".inherit-border-style", border_style_keys,
          sizeof(border_style_keys) / sizeof(border_style_keys[0]) },
        { ".inherit-border-radius", border_radius_keys,
          sizeof(border_radius_keys) / sizeof(border_radius_keys[0]) },
        { ".inherit-border-top", border_top_keys,
          sizeof(border_top_keys) / sizeof(border_top_keys[0]) },
        { ".inherit-border-right", border_right_keys,
          sizeof(border_right_keys) / sizeof(border_right_keys[0]) },
        { ".inherit-border-bottom", border_bottom_keys,
          sizeof(border_bottom_keys) / sizeof(border_bottom_keys[0]) },
        { ".inherit-border-left", border_left_keys,
          sizeof(border_left_keys) / sizeof(border_left_keys[0]) },
        { ".inherit-border", border_keys,
          sizeof(border_keys) / sizeof(border_keys[0]) },
        { ".inherit-flex", flex_keys,
          sizeof(flex_keys) / sizeof(flex_keys[0]) },
        { ".inherit-gap", gap_keys, sizeof(gap_keys) / sizeof(gap_keys[0]) }
};

static const css_prop_key_t inherited_properties[] = {
        css_prop_color,      css_prop_font_family,   css_prop_font_size,
        css_prop_font_style, css_prop_font_weight,   css_prop_line_height,
        css_prop_text_align, css_prop_visibility,    css_prop_white_space,
        css_prop_word_break, css_prop_pointer_events
};

static bool style_has_inherit(css_style_decl_t *style, int key)
{
        css_prop_t *prop = css_style_decl_find(style, key);

        return prop && prop->value.type == CSS_ARRAY_VALUE &&
               prop->value.array_value[0].type == CSS_KEYWORD_VALUE &&
               prop->value.array_value[0].keyword_value == CSS_KEYWORD_INHERIT;
}

static css_style_decl_t *select_rule(const char *selector_name)
{
        css_selector_t *selector = css_selector_create(selector_name);
        css_style_decl_t *style = css_select_style(selector);

        css_selector_destroy(selector);
        return style;
}

static void test_parse_global_inherit(void)
{
        css_style_value_t *value;

        value = css_style_value_parse("color", " InHeRiT ");
        ctest_equal_bool("should parse inherit case-insensitively",
                         value != NULL, 1);
        ctest_equal_bool(
            "should retain inherit as the keyword value",
            value && value->type == CSS_ARRAY_VALUE &&
                value->array_value[0].type == CSS_KEYWORD_VALUE &&
                value->array_value[0].keyword_value == CSS_KEYWORD_INHERIT,
            1);
        if (value) {
                css_style_value_destroy(value);
                free(value);
        }

        value = css_style_value_parse("color", "inherit red");
        ctest_equal_bool("should reject values mixed with inherit",
                         value == NULL, 1);
        if (value) {
                css_style_value_destroy(value);
                free(value);
        }
}

static void test_expand_shorthand_inherit(void)
{
        size_t i, j;
        size_t rule_index;
        css_parser_t *parser;
        css_selector_t *selector;
        css_style_decl_t *style;
        const shorthand_test_case_t *test_case;
        static const char *rules[] = {
                ".inherit-background { background: inherit; }",
                ".inherit-background-position { background-position: inherit; "
                "}",
                ".inherit-padding { padding: inherit; }",
                ".inherit-margin { margin: inherit; }",
                ".inherit-border-color { border-color: inherit; }",
                ".inherit-border-width { border-width: inherit; }",
                ".inherit-border-style { border-style: inherit; }",
                ".inherit-border-radius { border-radius: inherit; }",
                ".inherit-border-top { border-top: inherit; }",
                ".inherit-border-right { border-right: inherit; }",
                ".inherit-border-bottom { border-bottom: inherit; }",
                ".inherit-border-left { border-left: inherit; }",
                ".inherit-border { border: inherit; }",
                ".inherit-flex { flex: inherit; }",
                ".inherit-gap { gap: inherit; }"
        };

        parser = css_parser_create(__FILE__);
        for (rule_index = 0; rule_index < sizeof(rules) / sizeof(rules[0]);
             ++rule_index) {
                css_parser_parse(parser, rules[rule_index]);
        }
        css_parser_destroy(parser);

        for (i = 0; i < sizeof(shorthand_cases) / sizeof(shorthand_cases[0]);
             ++i) {
                test_case = &shorthand_cases[i];
                selector = css_selector_create(test_case->selector);
                style = css_select_style(selector);
                css_selector_destroy(selector);
                for (j = 0; j < test_case->key_count; ++j) {
                        ctest_equal_bool(
                            "should expand shorthand inherit to each longhand",
                            style_has_inherit(style, test_case->keys[j]), 1);
                }
                css_style_decl_destroy(style);
        }
}

static void test_default_inheritance_metadata(void)
{
        unsigned i, j;
        bool inherited;
        css_propdef_t *propdef;

        for (i = 0; i < css_get_prop_count(); ++i) {
                inherited = false;
                for (j = 0; j < sizeof(inherited_properties) /
                                    sizeof(inherited_properties[0]);
                     ++j) {
                        if (i == (unsigned)inherited_properties[j]) {
                                inherited = true;
                                break;
                        }
                }
                propdef = css_get_propdef((int)i);
                ctest_equal_bool(
                    "should default-inherit only the agreed properties",
                    propdef->inherited, inherited);
        }
}

static char *duplicate_string(const char *str)
{
        size_t len = strlen(str) + 1;
        char *copy = malloc(len);

        memcpy(copy, str, len);
        return copy;
}

static void test_computed_property_copy(void)
{
        css_computed_style_t src = { 0 };
        css_computed_style_t dest = { 0 };

        src.width = 42.0f;
        src.type_bits.width = CSS_WIDTH_SET;
        src.unit_bits.width = CSS_UNIT_PERCENT;
        css_computed_style_copy_property(&dest, &src, css_prop_width);
        ctest_equal_float("should copy a length value", dest.width, 42.0f);
        ctest_equal_int("should copy a length type", dest.type_bits.width,
                        CSS_WIDTH_SET);
        ctest_equal_int("should copy a length unit", dest.unit_bits.width,
                        CSS_UNIT_PERCENT);
        ctest_equal_bool(
            "should compare equal copied lengths",
            css_computed_style_property_equal(&src, &dest, css_prop_width), 1);
        dest.width = 31.0f;
        ctest_equal_bool(
            "should detect different copied lengths",
            css_computed_style_property_equal(&src, &dest, css_prop_width), 0);

        src.type_bits.display = CSS_DISPLAY_FLEX;
        css_computed_style_copy_property(&dest, &src, css_prop_display);
        ctest_equal_int("should copy an enum bitfield", dest.type_bits.display,
                        CSS_DISPLAY_FLEX);

        src.color = css_color(255, 12, 34, 56);
        src.type_bits.color = CSS_COLOR_COLOR;
        css_computed_style_copy_property(&dest, &src, css_prop_color);
        ctest_equal_uint("should copy a color value", dest.color, src.color);
        ctest_equal_int("should copy a color type", dest.type_bits.color,
                        CSS_COLOR_COLOR);

        src.font_family = malloc(2 * sizeof(char *));
        src.font_family[0] = duplicate_string("inconsolata");
        src.font_family[1] = NULL;
        src.type_bits.font_family = CSS_FONT_FAMILY_SERIF;
        css_computed_style_copy_property(&dest, &src, css_prop_font_family);
        ctest_equal_str("should deep-copy the font family", dest.font_family[0],
                        "inconsolata");
        ctest_equal_int("should copy the font family type",
                        dest.type_bits.font_family, CSS_FONT_FAMILY_SERIF);
        ctest_equal_bool("should not alias the source font family",
                         dest.font_family != src.font_family &&
                             dest.font_family[0] != src.font_family[0],
                         1);
        ctest_equal_bool("should compare copied font families by content",
                         css_computed_style_property_equal(
                             &src, &dest, css_prop_font_family),
                         1);

        src.content = duplicate_string("inherited content");
        src.type_bits.content = CSS_CONTENT_SET;
        css_computed_style_copy_property(&dest, &src, css_prop_content);
        ctest_equal_str("should deep-copy content", dest.content,
                        "inherited content");
        ctest_equal_int("should copy the content type", dest.type_bits.content,
                        CSS_CONTENT_SET);
        ctest_equal_bool("should not alias the source content",
                         dest.content != src.content, 1);
        ctest_equal_bool(
            "should compare copied content by value",
            css_computed_style_property_equal(&src, &dest, css_prop_content),
            1);

        src.background_image = duplicate_string("inherited-image.png");
        src.type_bits.background_image = CSS_BACKGROUND_IMAGE_IMAGE;
        css_computed_style_copy_property(&dest, &src,
                                         css_prop_background_image);
        ctest_equal_str("should deep-copy the background image",
                        dest.background_image, "inherited-image.png");
        ctest_equal_int("should copy the background image type",
                        dest.type_bits.background_image,
                        CSS_BACKGROUND_IMAGE_IMAGE);
        ctest_equal_bool("should not alias the source background image",
                         dest.background_image != src.background_image, 1);
        ctest_equal_bool("should compare copied background images by content",
                         css_computed_style_property_equal(
                             &src, &dest, css_prop_background_image),
                         1);

        css_computed_style_destroy(&src);
        css_computed_style_destroy(&dest);
}

static void add_style_text(css_style_decl_t *style, const char *property,
                           const char *css_text)
{
        const css_propdef_t *propdef = css_get_propdef_by_name(property);
        css_style_value_t *value = css_style_value_parse(property, css_text);

        assert(value);
        css_style_decl_add(style, propdef->key, value);
        css_style_value_destroy(value);
        free(value);
}

static void test_parent_and_explicit_inheritance(void)
{
        css_computed_style_t parent = { 0 };
        css_computed_style_t child = { 0 };
        css_computed_style_t default_child = { 0 };
        css_computed_style_t local_child = { 0 };
        css_style_decl_t *parent_decl = css_style_decl_create();
        css_style_decl_t *child_decl = css_style_decl_create();
        css_style_decl_t *default_child_decl = css_style_decl_create();
        css_style_decl_t *local_child_decl = css_style_decl_create();

        add_style_text(parent_decl, "color", "#123456");
        add_style_text(parent_decl, "font-family", "monospace");
        add_style_text(parent_decl, "font-size", "20px");
        add_style_text(parent_decl, "width", "50%");
        css_cascade_style_with_parent(parent_decl, NULL, &parent);

        add_style_text(child_decl, "width", "inherit");
        css_cascade_style_with_parent(child_decl, &parent, &child);
        ctest_equal_uint("should inherit the parent color", child.color,
                         css_color(255, 0x12, 0x34, 0x56));
        ctest_equal_str("should inherit the parent font family",
                        child.font_family[0], "monospace");
        ctest_equal_float("should inherit the parent font size",
                          child.font_size, 20.0f);
        ctest_equal_float("should preserve inherited percentage width",
                          child.width, 50.0f);
        ctest_equal_int("should preserve inherited width unit",
                        child.unit_bits.width, CSS_UNIT_PERCENT);

        css_cascade_style_with_parent(default_child_decl, &parent,
                                      &default_child);
        ctest_equal_int("should not inherit width by default",
                        default_child.type_bits.width, CSS_WIDTH_AUTO);

        add_style_text(local_child_decl, "color", "#abcdef");
        css_cascade_style_with_parent(local_child_decl, &parent, &local_child);
        ctest_equal_uint("should let the child's color override inheritance",
                         local_child.color, css_color(255, 0xab, 0xcd, 0xef));

        css_style_decl_destroy(parent_decl);
        css_style_decl_destroy(child_decl);
        css_style_decl_destroy(default_child_decl);
        css_style_decl_destroy(local_child_decl);
        css_computed_style_destroy(&parent);
        css_computed_style_destroy(&child);
        css_computed_style_destroy(&default_child);
        css_computed_style_destroy(&local_child);
}

static void test_root_inherit_uses_initial_values(void)
{
        css_computed_style_t root = { 0 };
        css_style_decl_t *style = css_style_decl_create();

        add_style_text(style, "display", "inherit");
        add_style_text(style, "font-family", "inherit");
        css_cascade_style_with_parent(style, NULL, &root);
        ctest_equal_int("should use the initial display at the root",
                        root.type_bits.display, CSS_DISPLAY_BLOCK);
        ctest_equal_str("should use the initial font family at the root",
                        root.font_family[0], "system-ui");

        css_style_decl_destroy(style);
        css_computed_style_destroy(&root);
}

static void test_font_size_percentage_computation(void)
{
        css_computed_style_t parent = { 0 };
        css_computed_style_t child = { 0 };
        css_computed_style_t grandchild = { 0 };
        css_computed_style_t root_percentage = { 0 };
        css_computed_style_t updated_parent = { 0 };
        css_computed_style_t updated_child = { 0 };
        css_style_decl_t *parent_decl = css_style_decl_create();
        css_style_decl_t *child_decl = css_style_decl_create();
        css_style_decl_t *grandchild_decl = css_style_decl_create();
        css_style_decl_t *root_percentage_decl = css_style_decl_create();
        css_style_decl_t *updated_parent_decl = css_style_decl_create();
        css_style_decl_t *updated_child_decl = css_style_decl_create();

        add_style_text(parent_decl, "font-size", "20px");
        css_cascade_style_with_parent(parent_decl, NULL, &parent);
        add_style_text(child_decl, "font-size", "150%");
        css_cascade_style_with_parent(child_decl, &parent, &child);
        ctest_equal_float("should compute child percentage font size",
                          child.font_size, 30.0f);

        add_style_text(grandchild_decl, "font-size", "200%");
        css_cascade_style_with_parent(grandchild_decl, &child, &grandchild);
        ctest_equal_float("should compute nested percentage font size",
                          grandchild.font_size, 60.0f);

        add_style_text(root_percentage_decl, "font-size", "150%");
        css_cascade_style_with_parent(root_percentage_decl, NULL,
                                      &root_percentage);
        ctest_equal_float("should resolve root percentage against initial size",
                          root_percentage.font_size, 14.0f * 1.5f);

        add_style_text(updated_parent_decl, "font-size", "24px");
        css_cascade_style_with_parent(updated_parent_decl, NULL,
                                      &updated_parent);
        add_style_text(updated_child_decl, "font-size", "150%");
        css_cascade_style_with_parent(updated_child_decl, &updated_parent,
                                      &updated_child);
        ctest_equal_float("should recompute percentages for changed parents",
                          updated_child.font_size, 36.0f);

        css_computed_style_destroy(&grandchild);
        css_style_decl_destroy(grandchild_decl);
        css_computed_style_destroy(&child);
        css_style_decl_destroy(child_decl);
        css_computed_style_destroy(&parent);
        css_style_decl_destroy(parent_decl);
        css_computed_style_destroy(&root_percentage);
        css_style_decl_destroy(root_percentage_decl);
        css_computed_style_destroy(&updated_parent);
        css_style_decl_destroy(updated_parent_decl);
        css_computed_style_destroy(&updated_child);
        css_style_decl_destroy(updated_child_decl);
}

static void test_computed_property_table_coverage(void)
{
        css_computed_style_t a = { 0 };
        css_computed_style_t b = { 0 };
        css_prop_key_t key;

        /* P-3 guard: every css_prop_key_t below STYLE_KEY_TOTAL must have a
         * generated copy/equality case. A property registered in the enum but
         * missing from CSS_PROP_TABLE falls into `default: assert(false)`,
         * which aborts this debug binary instead of passing silently. Two
         * zeroed styles compare equal for every key. */
        for (key = 0; key < STYLE_KEY_TOTAL; ++key) {
                ctest_equal_bool("should treat two zeroed styles as equal",
                                 css_computed_style_property_equal(&a, &b, key),
                                 1);
                css_computed_style_copy_property(&b, &a, key);
                ctest_equal_bool("should stay equal after a per-key copy",
                                 css_computed_style_property_equal(&a, &b, key),
                                 1);
        }
        css_computed_style_destroy(&b);
}

void test_css_inheritance(void)
{
        css_init();
        ctest_describe("global inherit parsing", test_parse_global_inherit);
        ctest_describe("shorthand inheritance expansion",
                       test_expand_shorthand_inherit);
        ctest_describe("default inheritance metadata",
                       test_default_inheritance_metadata);
        ctest_describe("computed property copying",
                       test_computed_property_copy);
        ctest_describe("computed property table coverage",
                       test_computed_property_table_coverage);
        ctest_describe("parent and explicit inheritance",
                       test_parent_and_explicit_inheritance);
        ctest_describe("root inheritance initial values",
                       test_root_inherit_uses_initial_values);
        ctest_describe("font size percentage computation",
                       test_font_size_percentage_computation);
        css_destroy();
}
