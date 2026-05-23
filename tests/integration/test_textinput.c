/*
 * tests/cases/test_textinput.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

void test_textinput(void)
{
        ui_widget_t *w;
        wchar_t wcs[64];

        lcui_init();

        w = ui_create_widget("textinput");

        ui_textinput_set_text_w(w, L"hello");
        ui_widget_update(w);
        ctest_equal_bool("should match text length after set_text_w",
                         ui_textinput_get_text_length(w) == wcslen(L"hello"),
                         true);
        ctest_equal_bool(
            "should retrieve full text after set_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), true);
        ctest_equal_bool(
            "should retrieve identical wide string after set_text_w",
            wcscmp(L"hello", wcs) == 0, true);

        ui_widget_set_text(w, "hello");
        ui_widget_update(w);
        ctest_equal_bool("should match text length after set_text",
                         ui_textinput_get_text_length(w) == wcslen(L"hello"),
                         true);
        ctest_equal_bool(
            "should retrieve full text after set_text",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), true);
        ctest_equal_bool("should retrieve identical wide string after set_text",
                         wcscmp(L"hello", wcs) == 0, true);

        ui_textinput_append_text_w(w, L"world");
        ui_widget_update(w);
        ctest_equal_bool(
            "should match text length after append_text_w",
            ui_textinput_get_text_length(w) == wcslen(L"helloworld"), true);
        ctest_equal_bool(
            "should retrieve appended text via get_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"helloworld"),
            true);
        ctest_equal_bool("should retrieve identical appended wide string",
                         wcscmp(L"helloworld", wcs) == 0, true);

        ui_textinput_move_caret(w, 0, 5);
        ui_textinput_insert_text_w(w, L", ");
        ui_widget_update(w);
        ui_textinput_move_caret(w, 0, 100);
        ui_textinput_insert_text_w(w, L"!");
        ui_widget_update(w);
        ctest_equal_bool(
            "should match text length after inserting text",
            ui_textinput_get_text_length(w) == wcslen(L"hello, world!"), true);
        ctest_equal_bool(
            "should retrieve inserted text via get_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello, world!"),
            true);
        ctest_equal_bool("should retrieve identical inserted wide string",
                         wcscmp(L"hello, world!", wcs) == 0, true);
        ui_widget_destroy(w);
        lcui_destroy();
}
