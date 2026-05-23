/*
 * lib/pandagl/tests/test_font_load.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <pandagl.h>
#include <ctest.h>

#define GetSegoeUIFont(S, W) pd_font_library_get_font_id("Segoe UI", S, W)
#define GetArialFont(S, W) pd_font_library_get_font_id("Arial", S, W)

#ifdef _WIN32
static void test_segoe_ui_font_load(void)
{
        pd_font_t *font;
        int id;
        if (pd_font_library_load_file("C:/windows/fonts/segoeui.ttf") != 0) {
                return;
        }
        ctest_equal_int(
            "should load segoeui.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeui.ttf"), 0);
        ctest_equal_int(
            "should load segoeuib.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeuib.ttf"), 0);
        ctest_equal_int(
            "should load segoeuii.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeuii.ttf"), 0);
        ctest_equal_int(
            "should load segoeuil.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeuil.ttf"), 0);
        ctest_equal_int(
            "should load segoeuisl.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeuisl.ttf"), 0);
        ctest_equal_int(
            "should load segoeuiz.ttf",
            pd_font_library_load_file("C:/windows/fonts/segoeuiz.ttf"), 0);
        ctest_equal_int(
            "should load seguibl.ttf",
            pd_font_library_load_file("C:/windows/fonts/seguibl.ttf"), 0);
        ctest_equal_int(
            "should load seguili.ttf",
            pd_font_library_load_file("C:/windows/fonts/seguili.ttf"), 0);
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: normal, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: italic, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: normal, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_LIGHT);
        ctest_equal_bool("should resolve font (style: normal, weight: light)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
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
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_EXTRA_LIGHT);
        }
}

static void test_arial_font_load(void)
{
        int id;
        pd_font_t *font;
        if (pd_font_library_load_file("C:/windows/fonts/arial.ttf") != 0) {
                return;
        }
        ctest_equal_int(
            "should load arialbd.ttf",
            pd_font_library_load_file("C:/windows/fonts/arialbd.ttf"), 0);
        ctest_equal_int(
            "should load ariblk.ttf",
            pd_font_library_load_file("C:/windows/fonts/ariblk.ttf"), 0);
        ctest_equal_int(
            "should load arialbi.ttf",
            pd_font_library_load_file("C:/windows/fonts/arialbi.ttf"), 0);
        ctest_equal_int(
            "should load ariali.ttf",
            pd_font_library_load_file("C:/windows/fonts/ariali.ttf"), 0);
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: normal, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetArialFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_NORMAL);
        ctest_equal_bool("should resolve font (style: italic, weight: normal)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_NORMAL);
        }
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: normal, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
        id = GetArialFont(PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_BLACK);
        ctest_equal_bool("should resolve font (style: normal, weight: black)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_NORMAL);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BLACK);
        }
        id = GetSegoeUIFont(PD_FONT_STYLE_ITALIC, PD_FONT_WEIGHT_BOLD);
        ctest_equal_bool("should resolve font (style: italic, weight: bold)",
                         id > 0, true);
        if (id > 0) {
                font = pd_font_library_get_font(id);
                ctest_equal_int("should match font style", font->style,
                                PD_FONT_STYLE_ITALIC);
                ctest_equal_int("should match font weight", font->weight,
                                PD_FONT_WEIGHT_BOLD);
        }
}
#endif

void test_pandagl_font_load(void)
{
        pd_font_library_init();
        ctest_equal_int("should load font from file",
                        pd_font_library_load_file("test_font_load.ttf"), 0);
#ifdef _WIN32
        ctest_describe("segoe ui font load", test_segoe_ui_font_load);
        ctest_describe("arial font load", test_arial_font_load);
#endif
        pd_font_library_destroy();
}
