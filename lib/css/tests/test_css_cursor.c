/*
 * lib/css/tests/test_css_cursor.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <css.h>
#include <ctest.h>

static int set_property(css_style_decl_t *decl, const char *name,
                        const char *text)
{
        css_propdef_t *propdef = css_get_propdef_by_name(name);
        css_style_value_t value = { 0 };
        int ret = css_parse_value(propdef->valdef, text, &value);

        if (ret > 0) {
                css_style_decl_set(decl, propdef->key, &value);
        }
        css_style_value_destroy(&value);
        return ret;
}

static const char *cursor_value_names[] = {
        "auto",        "default",     "none",      "context-menu",
        "help",        "pointer",     "progress",  "wait",
        "cell",        "crosshair",   "text",      "vertical-text",
        "alias",       "copy",        "move",      "no-drop",
        "not-allowed", "grab",        "grabbing",  "all-scroll",
        "col-resize",  "row-resize",  "n-resize",  "e-resize",
        "s-resize",    "w-resize",    "ew-resize", "ns-resize",
        "ne-resize",   "nw-resize",   "se-resize", "sw-resize",
        "nesw-resize", "nwse-resize", "zoom-in",   "zoom-out",
};

static void should_register_cursor_keywords(void)
{
        ctest_equal_int("should register pointer keyword",
                        css_get_keyword_key("pointer"), CSS_KEYWORD_POINTER);
        ctest_equal_int("should register not-allowed keyword",
                        css_get_keyword_key("not-allowed"),
                        CSS_KEYWORD_NOT_ALLOWED);
        ctest_equal_int("should register zoom-out keyword",
                        css_get_keyword_key("zoom-out"), CSS_KEYWORD_ZOOM_OUT);
}

static void should_use_auto_as_initial_value(void)
{
        css_computed_style_t computed;
        css_style_decl_t *decl = css_style_decl_create();

        css_cascade_style(decl, &computed);
        ctest_equal_int("should default cursor to auto",
                        computed.type_bits.cursor, CSS_CURSOR_AUTO);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

static void should_cascade_every_cursor_value(void)
{
        unsigned i;
        size_t n = sizeof(cursor_value_names) / sizeof(cursor_value_names[0]);
        char name[128];

        for (i = 0; i < n; ++i) {
                css_computed_style_t computed;
                css_style_decl_t *decl = css_style_decl_create();

                snprintf(name, sizeof(name), "should parse cursor value: %s",
                         cursor_value_names[i]);
                ctest_equal_bool(
                    name,
                    set_property(decl, "cursor", cursor_value_names[i]) > 0,
                    true);
                css_cascade_style(decl, &computed);
                snprintf(name, sizeof(name), "should cascade cursor value: %s",
                         cursor_value_names[i]);
                ctest_equal_int(name, computed.type_bits.cursor, (int)i);
                css_computed_style_destroy(&computed);
                css_style_decl_destroy(decl);
        }
}

static void should_ignore_invalid_cursor_value(void)
{
        css_computed_style_t computed;
        css_style_decl_t *decl = css_style_decl_create();

        ctest_equal_bool("should reject unknown keyword",
                         set_property(decl, "cursor", "foobar") <= 0, true);
        ctest_equal_bool("should reject image cursor url",
                         set_property(decl, "cursor", "url(a.cur)") <= 0, true);
        css_cascade_style(decl, &computed);
        ctest_equal_int("should keep initial auto on invalid value",
                        computed.type_bits.cursor, CSS_CURSOR_AUTO);
        css_computed_style_destroy(&computed);
        css_style_decl_destroy(decl);
}

void test_css_cursor(void)
{
        css_init();
        ctest_describe("cursor keywords", should_register_cursor_keywords);
        ctest_describe("cursor initial value",
                       should_use_auto_as_initial_value);
        ctest_describe("cursor value cascade",
                       should_cascade_every_cursor_value);
        ctest_describe("invalid cursor values",
                       should_ignore_invalid_cursor_value);
        css_destroy();
}
