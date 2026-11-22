/*
 * tests/integration/test_textinput_edit.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include "../../src/widgets/textedit.h"

static void check_text_edit_core(void)
{
        ui_text_edit_t *edit = ui_text_edit_create();
        wchar_t buf[16];

        ui_text_edit_set_text(edit, L"hello", 5);
        ctest_equal_int("should store text length",
                        (int)ui_text_edit_get_length(edit), 5);
        ctest_equal_int("should read text back",
                        (int)ui_text_edit_get_text(edit, 0, 16, buf), 5);
        ctest_equal_wcs("should return the identical text", buf, L"hello");

        ui_text_edit_text_input(edit, L"!", 1);
        ctest_equal_int("text input should insert at the cursor",
                        (int)ui_text_edit_get_length(edit), 6);
        ctest_equal_int("should read the inserted text",
                        (int)ui_text_edit_get_text(edit, 0, 16, buf), 6);
        ctest_equal_wcs("should append the input text", buf, L"hello!");

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_BACKSPACE);
        ctest_equal_int("backspace should delete one character",
                        (int)ui_text_edit_get_length(edit), 5);

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_UNDO);
        ctest_equal_int("undo should restore the deleted character",
                        (int)ui_text_edit_get_length(edit), 6);

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_REDO);
        ctest_equal_int("redo should delete it again",
                        (int)ui_text_edit_get_length(edit), 5);

        ui_text_edit_destroy(edit);
}

static void check_layer_mapping(void)
{
        pd_text_t *layer = pd_text_create();
        ui_text_edit_t *edit = ui_text_edit_create();
        wchar_t buf[16];

        pd_text_set_multiline(layer, true);
        pd_text_set_fixed_size(layer, 200, 100);
        pd_text_set_max_size(layer, 200, 100);
        ui_text_edit_set_layer(edit, layer);
        ui_text_edit_set_text(edit, L"abc\ndef", 7);
        ui_text_edit_sync_layer(edit);
        ui_text_edit_sync_caret(edit);
        /* set_text leaves the caret at the end of the text */
        ctest_equal_int("caret should start at the last line",
                        ui_text_edit_get_caret_row(edit), 1);
        ctest_equal_int("caret should start at the line end",
                        ui_text_edit_get_caret_col(edit), 3);

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
        ui_text_edit_sync_caret(edit);
        ctest_equal_int("left should stay on the same line",
                        ui_text_edit_get_caret_row(edit), 1);
        ctest_equal_int("left should reach the line start",
                        ui_text_edit_get_caret_col(edit), 0);

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
        ui_text_edit_sync_caret(edit);
        ctest_equal_int("crossing the newline should land on the previous line",
                        ui_text_edit_get_caret_row(edit), 0);
        ctest_equal_int("crossing the newline should land at the line end",
                        ui_text_edit_get_caret_col(edit), 3);

        ui_text_edit_move_caret(edit, 1, 0);
        ui_text_edit_text_input(edit, L"X", 1);
        ui_text_edit_get_text(edit, 0, 16, buf);
        ctest_equal_wcs("move_caret should place typing at the visual line",
                        buf, L"abc\nXdef");
        ui_text_edit_append_text(edit, L"!", 1);
        ui_text_edit_get_text(edit, 0, 16, buf);
        ctest_equal_wcs("append_text should always append at the end", buf,
                        L"abc\nXdef!");

        ui_text_edit_destroy(edit);
        pd_text_destroy(layer);
}

static void check_layout_queries(void)
{
        pd_text_t *layer = pd_text_create();
        ui_text_edit_t *edit = ui_text_edit_create();
        pd_text_style_t style;
        int line_h;
        size_t start, end;

        pd_text_style_init(&style);
        pd_text_style_set_default_font(&style);
        pd_text_style_set_size(&style, 14);
        pd_text_set_style(layer, &style);
        pd_text_style_destroy(&style);
        pd_text_set_multiline(layer, true);
        pd_text_set_fixed_size(layer, 200, 100);
        pd_text_set_max_size(layer, 200, 100);
        ui_text_edit_set_layer(edit, layer);
        ui_text_edit_set_text(edit, L"abc\ndef", 7);

        ui_text_edit_click(edit, 0, 0);
        ctest_equal_int("click should land on the first row",
                        ui_text_edit_get_caret_row(edit), 0);
        ctest_equal_int("click should land at the row start",
                        ui_text_edit_get_caret_col(edit), 0);

        line_h = pd_text_get_line_height(layer, 0);
        ui_text_edit_click(edit, 0, (float)line_h + 1);
        ctest_equal_int("click should land on the second row",
                        ui_text_edit_get_caret_row(edit), 1);
        ctest_equal_int("click should land at the second row start",
                        ui_text_edit_get_caret_col(edit), 0);

        ui_text_edit_move_caret(edit, 0, 0);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_RIGHT | UI_TEXT_EDIT_KEY_SHIFT);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_RIGHT | UI_TEXT_EDIT_KEY_SHIFT);
        ctest_equal_bool("shift-right should create a selection",
                         ui_text_edit_has_selection(edit), true);
        ui_text_edit_selection(edit, &start, &end);
        ctest_equal_int("selection should start at the caret origin",
                        (int)start, 0);
        ctest_equal_int("selection should cover two characters", (int)end, 2);

        ui_text_edit_destroy(edit);
        pd_text_destroy(layer);
}

static void check_selection_operations(void)
{
        ui_text_edit_t *edit = ui_text_edit_create();
        wchar_t buf[16];
        size_t start, end;

        ui_text_edit_set_text(edit, L"hello", 5);
        ui_text_edit_select_all(edit);
        ctest_equal_bool("select-all should select the whole text",
                         ui_text_edit_has_selection(edit), true);
        ui_text_edit_selection(edit, &start, &end);
        ctest_equal_int("select-all start", (int)start, 0);
        ctest_equal_int("select-all end", (int)end, 5);
        ctest_equal_int("copy should return the selected length",
                        (int)ui_text_edit_copy(edit, buf, 16), 5);
        ctest_equal_wcs("copy should return the selected text", buf, L"hello");

        ctest_equal_int("cut should return the selected length",
                        (int)ui_text_edit_cut(edit, buf, 16), 5);
        ctest_equal_int("cut should remove the selection",
                        (int)ui_text_edit_get_length(edit), 0);
        ctest_equal_bool("cut should clear the selection",
                         ui_text_edit_has_selection(edit), false);

        ui_text_edit_destroy(edit);
}

static void check_crlf_and_surrogates(void)
{
        pd_text_t *layer = pd_text_create();
        ui_text_edit_t *edit = ui_text_edit_create();
        pd_text_style_t style;
        wchar_t buf[32];

        pd_text_style_init(&style);
        pd_text_style_set_default_font(&style);
        pd_text_style_set_size(&style, 14);
        pd_text_set_style(layer, &style);
        pd_text_style_destroy(&style);
        pd_text_set_multiline(layer, true);
        pd_text_set_fixed_size(layer, 200, 100);
        pd_text_set_max_size(layer, 200, 100);
        ui_text_edit_set_layer(edit, layer);

        ui_text_edit_set_text(edit, L"a\r\nb", 4);
        ctest_equal_int("set_text should normalize CRLF",
                        (int)ui_text_edit_get_length(edit), 3);
        ui_text_edit_get_text(edit, 0, 32, buf);
        ctest_equal_wcs("set_text should keep LF only", buf, L"a\nb");

        ui_text_edit_set_text(edit, L"", 0);
        ui_text_edit_text_input(edit, L"a\r\nb", 4);
        ctest_equal_int("text input should normalize CRLF",
                        (int)ui_text_edit_get_length(edit), 3);
        ui_text_edit_get_text(edit, 0, 32, buf);
        ctest_equal_wcs("text input should keep LF only", buf, L"a\nb");

#if defined(_WIN32)
        {
                wchar_t emoji[4] = { 0xD83D, 0xDE00, L'a', 0 };

                ui_text_edit_set_text(edit, emoji, 3);
                ctest_equal_int("emoji text should keep three code units",
                                (int)ui_text_edit_get_length(edit), 3);
                ui_text_edit_move_caret(edit, 0, 3);
                ctest_equal_int("move_caret should reach the text end",
                                ui_text_edit_get_caret_col(edit), 3);
                ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
                ctest_equal_int("left should stop before the trailing letter",
                                ui_text_edit_get_caret_col(edit), 2);
                ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_LEFT);
                ctest_equal_int("left should skip the surrogate pair",
                                ui_text_edit_get_caret_col(edit), 0);
        }
#endif

        ui_text_edit_destroy(edit);
        pd_text_destroy(layer);
}

static void check_soft_wrap_queries(void)
{
        pd_text_t *layer = pd_text_create();
        ui_text_edit_t *edit = ui_text_edit_create();
        pd_text_style_t style;
        int line_len;

        pd_text_style_init(&style);
        pd_text_style_set_default_font(&style);
        pd_text_style_set_size(&style, 14);
        pd_text_set_style(layer, &style);
        pd_text_style_destroy(&style);
        pd_text_set_multiline(layer, true);
        pd_text_set_autowrap(layer, true);
        pd_text_set_word_break(layer, PD_WORD_BREAK_BREAK_ALL);
        pd_text_set_fixed_size(layer, 40, 100);
        pd_text_set_max_size(layer, 40, 100);
        ui_text_edit_set_layer(edit, layer);
        ui_text_edit_set_text(edit, L"aaaaaaaaaaaaaaaa", 16);

        ctest_equal_bool("narrow layer should wrap into several lines",
                         pd_text_get_lines_length(layer) >= 2, true);
        line_len = pd_text_get_line_length(layer, 0);
        ctest_equal_int("soft wrap should not consume characters",
                        pd_text_get_line_char_start(layer, 1), line_len);

        ui_text_edit_move_caret(edit, 0, line_len);
        ctest_equal_int("soft boundary should map to the next visual row",
                        ui_text_edit_get_caret_row(edit), 1);
        ctest_equal_int("soft boundary should map to the next row start",
                        ui_text_edit_get_caret_col(edit), 0);

        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_DOWN);
        ctest_equal_int("down from a soft boundary should move one row",
                        ui_text_edit_get_caret_row(edit), 2);

        ui_text_edit_move_caret(edit, 1, line_len);
        ctest_equal_int("second boundary should map to row 2",
                        ui_text_edit_get_caret_row(edit), 2);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_UP);
        ctest_equal_int("up from a boundary should move one row up",
                        ui_text_edit_get_caret_row(edit), 1);

        ui_text_edit_move_caret(edit, 0, 0);
        ui_text_edit_key(edit, UI_TEXT_EDIT_KEY_DOWN);
        ctest_equal_int("down should cross a soft wrap",
                        ui_text_edit_get_caret_row(edit), 1);

        ui_text_edit_destroy(edit);
        pd_text_destroy(layer);
}

static void check_vertical_movement(void)
{
        pd_text_t *layer = pd_text_create();
        ui_text_edit_t *edit = ui_text_edit_create();
        pd_text_style_t style;
        size_t start, end;

        pd_text_style_init(&style);
        pd_text_style_set_default_font(&style);
        pd_text_style_set_size(&style, 14);
        pd_text_set_style(layer, &style);
        pd_text_style_destroy(&style);
        pd_text_set_multiline(layer, true);
        pd_text_set_fixed_size(layer, 200, 100);
        pd_text_set_max_size(layer, 200, 100);
        ui_text_edit_set_layer(edit, layer);
        ui_text_edit_set_text(edit, L"abcdef\nxy\nabcdef", 15);

        ui_text_edit_move_caret(edit, 0, 4);
        ui_text_edit_move_vertical(edit, 1, false);
        ctest_equal_int("vertical should land on the short line",
                        ui_text_edit_get_caret_row(edit), 1);
        ctest_equal_int("vertical should clamp to the short line end",
                        ui_text_edit_get_caret_col(edit), 2);

        ui_text_edit_move_vertical(edit, 1, false);
        ctest_equal_int("vertical should reach the third line",
                        ui_text_edit_get_caret_row(edit), 2);
        ctest_equal_int("vertical should restore the preferred column",
                        ui_text_edit_get_caret_col(edit), 4);

        ui_text_edit_move_vertical(edit, -1, true);
        ctest_equal_bool("shift+vertical should create a selection",
                         ui_text_edit_has_selection(edit), true);
        ui_text_edit_selection(edit, &start, &end);
        ctest_equal_bool("selection should span the vertical range",
                         start != end, true);

#if defined(_WIN32)
        {
                wchar_t emoji[4] = { 0xD83D, 0xDE00, L'a', 0 };

                ui_text_edit_set_text(edit, emoji, 3);
                ui_text_edit_move_caret(edit, 0, 1);
                ctest_equal_int("move_caret should not split a surrogate pair",
                                ui_text_edit_get_caret_col(edit), 0);
                ui_text_edit_move_caret(edit, 0, 2);
                ctest_equal_int("move_caret should keep the pair boundary",
                                ui_text_edit_get_caret_col(edit), 2);
        }
#endif

        ui_text_edit_destroy(edit);
        pd_text_destroy(layer);
}

void test_textinput_edit(void)
{
        lcui_init();
        ctest_describe("text edit core", check_text_edit_core);
        ctest_describe("text edit layer mapping", check_layer_mapping);
        ctest_describe("text edit layout queries", check_layout_queries);
        ctest_describe("text edit selection operations",
                       check_selection_operations);
        ctest_describe("text edit crlf and surrogates",
                       check_crlf_and_surrogates);
        ctest_describe("text edit soft wrap", check_soft_wrap_queries);
        ctest_describe("text edit vertical movement", check_vertical_movement);
        lcui_destroy();
}
