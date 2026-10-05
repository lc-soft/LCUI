/*
 * lib/pandagl/tests/test_font_load.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <pandagl.h>
#include <ctest.h>

#define GetSegoeUIFont(S, W) pd_font_get_id("Segoe UI", S, W)
#define GetArialFont(S, W) pd_font_get_id("Arial", S, W)

#ifdef _WIN32
static void test_segoe_ui_font_load(void)
{
        pd_font_face_t *font;
        int id;
        if (pd_font_load_file("C:/windows/fonts/segoeui.ttf") != 0) {
                return;
        }
        ctest_equal_int("should load segoeui.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeui.ttf"), 0);
        ctest_equal_int("should load segoeuib.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeuib.ttf"), 0);
        ctest_equal_int("should load segoeuii.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeuii.ttf"), 0);
        ctest_equal_int("should load segoeuil.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeuil.ttf"), 0);
        ctest_equal_int("should load segoeuisl.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeuisl.ttf"), 0);
        ctest_equal_int("should load segoeuiz.ttf",
                        pd_font_load_file("C:/windows/fonts/segoeuiz.ttf"), 0);
        ctest_equal_int("should load seguibl.ttf",
                        pd_font_load_file("C:/windows/fonts/seguibl.ttf"), 0);
        ctest_equal_int("should load seguili.ttf",
                        pd_font_load_file("C:/windows/fonts/seguili.ttf"), 0);
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: normal, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: italic, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: normal, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_LIGHT);
        ctest_equal_bool("should resolve font (style: normal, weight: light)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_LIGHT);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_EXTRA_LIGHT);
        ctest_equal_bool(
            "should resolve font (style: italic, weight: extra light)", id > 0,
            true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_EXTRA_LIGHT);
        }
}

static void test_arial_font_load(void)
{
        int id;
        pd_font_face_t *font;
        if (pd_font_load_file("C:/windows/fonts/arial.ttf") != 0) {
                return;
        }
        ctest_equal_int("should load arialbd.ttf",
                        pd_font_load_file("C:/windows/fonts/arialbd.ttf"), 0);
        ctest_equal_int("should load ariblk.ttf",
                        pd_font_load_file("C:/windows/fonts/ariblk.ttf"), 0);
        ctest_equal_int("should load arialbi.ttf",
                        pd_font_load_file("C:/windows/fonts/arialbi.ttf"), 0);
        ctest_equal_int("should load ariali.ttf",
                        pd_font_load_file("C:/windows/fonts/ariali.ttf"), 0);
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: normal, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetArialFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: italic, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: normal, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BLACK);
        ctest_equal_bool("should resolve font (style: normal, weight: black)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BLACK);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: italic, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_get(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
}

static void test_variable_font_weights(void)
{
        const char *candidates[] = { "Noto Sans SC", "Noto Sans CJK SC",
                                     "Noto Sans" };
        const char *family = NULL;
        size_t c;
        int normal_id, medium_id, bold_id;

        if (pd_font_load_file("C:/windows/fonts/NotoSansSC-VF.ttf") != 0) {
                return;
        }
        for (c = 0; c < sizeof(candidates) / sizeof(candidates[0]); ++c) {
                if (pd_font_get_id(candidates[c], PD_FONT_STYLE_NORMAL,
                                   PD_FONT_WEIGHT_NORMAL) > 0) {
                        family = candidates[c];
                        break;
                }
        }
        if (!family) {
                return;
        }
        normal_id =
            pd_font_get_id(family, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL);
        medium_id =
            pd_font_get_id(family, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_MEDIUM);
        bold_id =
            pd_font_get_id(family, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool(
            "variable font should expose distinct medium and bold faces",
            medium_id > 0 && bold_id > 0 && medium_id != normal_id &&
                bold_id != normal_id,
            true);
        if (medium_id > 0) {
                ctest_equal_int(
                    "medium instance should carry the medium weight",
                    pd_font_get(medium_id)->weight, PD_FONT_WEIGHT_MEDIUM);
        }
}

static void test_cjk_font_fallback_glyph(void)
{
        unsigned cjk = 0x4E2D; /* 中 */
        int latin_id, cjk_id;

        if (pd_font_load_file("C:/windows/fonts/segoeui.ttf") != 0) {
                return;
        }
        if (pd_font_load_file("C:/windows/fonts/msyh.ttc") != 0) {
                return;
        }
        latin_id = pd_font_get_id("Segoe UI", PD_FONT_STYLE_NORMAL,
                                  PD_FONT_WEIGHT_NORMAL);
        cjk_id = pd_font_get_id("Microsoft YaHei", PD_FONT_STYLE_NORMAL,
                                PD_FONT_WEIGHT_NORMAL);
        if (latin_id <= 0 || cjk_id <= 0) {
                return;
        }
        ctest_equal_bool("latin font should not contain the CJK glyph",
                         pd_font_get_glyph_index(cjk, latin_id) == 0, true);
        ctest_equal_bool("CJK font should contain the CJK glyph",
                         pd_font_get_glyph_index(cjk, cjk_id) > 0, true);
}

static void test_weight_fallback_safety(void)
{
        int id;

        if (pd_font_load_file("C:/windows/fonts/segoeui.ttf") != 0) {
                return;
        }
        /* Load a sparse set of weights: Regular, Light, Semilight, Bold, Black.
         * Requests for the missing medium/semibold/extra-bold tiers must fall
         * back to a neighbour instead of dereferencing a NULL slot. */
        pd_font_load_file("C:/windows/fonts/segoeuil.ttf");
        pd_font_load_file("C:/windows/fonts/segoeuisl.ttf");
        pd_font_load_file("C:/windows/fonts/segoeuib.ttf");
        pd_font_load_file("C:/windows/fonts/seguibl.ttf");
        id = pd_font_get_id("Segoe UI", PD_FONT_STYLE_NORMAL,
                            PD_FONT_WEIGHT_SEMI_BOLD);
        ctest_equal_bool("semibold request should fall back to a face", id > 0,
                         true);
        id = pd_font_get_id("Segoe UI", PD_FONT_STYLE_NORMAL,
                            PD_FONT_WEIGHT_MEDIUM);
        ctest_equal_bool("medium request should fall back to a face", id > 0,
                         true);
        id = pd_font_get_id("Segoe UI", PD_FONT_STYLE_NORMAL,
                            PD_FONT_WEIGHT_EXTRA_BOLD);
        ctest_equal_bool("extra-bold request should fall back to a face",
                         id > 0, true);
}
#endif

void test_pandagl_font_load(void)
{
        pd_font_library_init();
        ctest_equal_int("should load font from file",
                        pd_font_load_file("test_font_load.ttf"), 0);
#ifdef _WIN32
        ctest_describe("segoe ui font load", test_segoe_ui_font_load);
        ctest_describe("arial font load", test_arial_font_load);
        ctest_describe("variable font weights", test_variable_font_weights);
        ctest_describe("cjk font fallback glyph", test_cjk_font_fallback_glyph);
        ctest_describe("weight fallback safety", test_weight_fallback_safety);
#endif
        pd_font_library_destroy();
}
