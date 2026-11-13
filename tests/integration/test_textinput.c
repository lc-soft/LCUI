/*
 * tests/cases/test_textinput.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void check_password_input(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t wcs[64];

        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"secret");
        ui_widget_update(w);

        ctest_equal_bool("password input should keep the text length",
                         ui_textinput_get_text_length(w) == 6, true);
        ctest_equal_bool("password input should preserve the underlying text",
                         ui_textinput_get_text_w(w, 0, 64, wcs) == 6 &&
                             wcscmp(wcs, L"secret") == 0,
                         true);

        ui_textinput_set_password_char(w, 0);
        ui_widget_update(w);
        ctest_equal_bool(
            "disabling password masking should restore the plain text",
            ui_textinput_get_text_w(w, 0, 64, wcs) == 6 &&
                wcscmp(wcs, L"secret") == 0,
            true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_multiline_attr(void)
{
        ui_widget_t *single = ui_create_widget("textinput");
        ui_widget_t *multi = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_widget_set_attr(multi, "multiline", "true");
        ui_root_append(single);
        ui_root_append(multi);
        ui_update();

        ui_event_init(&e, "textinput");
        e.text.text = L"a\nb";
        e.text.length = 3;
        ui_widget_emit_event(single, e, NULL);
        ui_widget_emit_event(multi, e, NULL);
        ui_update();

        ctest_equal_bool("single-line input should drop newline characters",
                         ui_textinput_get_text_length(single) == 2, true);
        ctest_equal_bool("multiline input should keep newline characters",
                         ui_textinput_get_text_length(multi) == 3, true);

        ui_widget_destroy(single);
        ui_widget_destroy(multi);
        ui_clear_trash();
}

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
        check_password_input();
        check_multiline_attr();
        lcui_destroy();
}
