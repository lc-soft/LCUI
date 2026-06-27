/*
 * lib/pandagl/tests/test_text_edit.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <wchar.h>
#include <pandagl.h>
#include <ctest.h>

/*
 * 这些用例覆盖 pd_text_delete_ex 的关键分支，包括同行删除、跨行合并、
 * 以及边界场景。由于 pd_text_read 在多行场景下存在已知问题（不在本次
 * 修复范围），这里通过断言每行长度来验证缓冲区状态。
 */

static void should_backspace_last_char_on_last_line(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"123\n123\n123\n", NULL);
        /* 光标置于末尾的空行，按退格删除前一行的换行符，
         * 此时空行被丢弃 */
        pd_text_set_caret(text, 3, 0);
        ctest_equal_int("should not crash on backspace last char",
                        pd_text_backspace(text, 1), 0);
        ctest_equal_int("should drop the trailing empty line",
                        pd_text_get_lines_length(text), 3);
        ctest_equal_int("line 0 keeps 3 chars",
                        pd_text_get_line_length(text, 0), 3);
        ctest_equal_int("line 1 keeps 3 chars",
                        pd_text_get_line_length(text, 1), 3);
        ctest_equal_int("line 2 keeps 3 chars",
                        pd_text_get_line_length(text, 2), 3);
        pd_text_destroy(text);
}

static void should_backspace_then_continue_typing(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc\ndef\n", NULL);
        pd_text_set_caret(text, 2, 0);
        pd_text_backspace(text, 1);
        pd_text_insert(text, L"X", NULL);
        ctest_equal_int("should merge into two lines after backspace + insert",
                        pd_text_get_lines_length(text), 2);
        ctest_equal_int("line 0 keeps 'abc'", pd_text_get_line_length(text, 0),
                        3);
        ctest_equal_int("line 1 becomes 'defX'",
                        pd_text_get_line_length(text, 1), 4);
        pd_text_destroy(text);
}

static void should_not_crash_on_backspace_at_start(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc", NULL);
        pd_text_set_caret(text, 0, 0);
        ctest_equal_int("should return error at buffer start",
                        pd_text_backspace(text, 1), -1);
        ctest_equal_int("should keep one line", pd_text_get_lines_length(text),
                        1);
        ctest_equal_int("line content unchanged",
                        pd_text_get_line_length(text, 0), 3);
        pd_text_destroy(text);
}

static void should_delete_char_at_caret(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t buf[16];

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abcdef", NULL);
        pd_text_set_caret(text, 0, 2);
        ctest_equal_int("should delete one char", pd_text_delete(text, 1), 0);
        pd_text_read(text, 0, 16, buf);
        ctest_equal_wcs("should produce expected text", buf, L"abdef");
        ctest_equal_int("line length is 5", pd_text_get_line_length(text, 0),
                        5);
        pd_text_destroy(text);
}

static void should_join_next_line_when_delete_at_line_end(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc\ndef\nghi", NULL);
        /* 在第二行最后一个字符处使用 delete，触发跨行拼接，
         * 之前 +1 偏移 bug 会导致 'd' 字符丢失 */
        pd_text_set_caret(text, 1, 3);
        ctest_equal_int("should not crash when delete at line end",
                        pd_text_delete(text, 1), 0);
        ctest_equal_int("should reduce one line after merge",
                        pd_text_get_lines_length(text), 2);
        ctest_equal_int("line 0 keeps 'abc'", pd_text_get_line_length(text, 0),
                        3);
        ctest_equal_int("line 1 becomes 'defghi' (6 chars)",
                        pd_text_get_line_length(text, 1), 6);
        pd_text_destroy(text);
}

static void should_preserve_chars_when_joining_lines(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc\ndef\n", NULL);
        /* 在第一行末尾处用 delete 删除换行符 */
        pd_text_set_caret(text, 0, 3);
        pd_text_delete(text, 1);
        ctest_equal_int("should reduce to two lines",
                        pd_text_get_lines_length(text), 2);
        ctest_equal_int("line 0 becomes 'abcdef' (6 chars)",
                        pd_text_get_line_length(text, 0), 6);
        ctest_equal_int("line 1 stays empty", pd_text_get_line_length(text, 1),
                        0);
        pd_text_destroy(text);
}

static void should_handle_multi_line_backspace_sequence(void)
{
        pd_text_t *text = pd_text_create();

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"123\n123\n123\n", NULL);
        pd_text_set_caret(text, 3, 0);
        /* 连续 backspace，模拟用户按住退格。每次删一格：
         * 1) 删第 4 行（空行） -> 3 行
         * 2) 删第 3 行的 '3' -> 2.x（剩 '12'）
         * 3) 删 '2'
         * 4) 删 '1' -> 第 3 行空且无 eol，进入下次 backspace 跨行 */
        pd_text_backspace(text, 1);
        pd_text_backspace(text, 1);
        pd_text_backspace(text, 1);
        pd_text_backspace(text, 1);
        ctest_equal_int("should remain 2 full lines",
                        pd_text_get_lines_length(text), 2);
        ctest_equal_int("line 0 keeps 3 chars",
                        pd_text_get_line_length(text, 0), 3);
        ctest_equal_int("line 1 keeps 3 chars",
                        pd_text_get_line_length(text, 1), 3);
        pd_text_destroy(text);
}

static void should_dump_single_line(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t buf[32];

        pd_text_write(text, L"hello", NULL);
        ctest_equal_uint("should return char count",
                         (unsigned)pd_text_read(text, 0, 32, buf), 5u);
        ctest_equal_wcs("should produce same single-line text", buf, L"hello");
        pd_text_destroy(text);
}

static void should_dump_multiline_with_lf(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t buf[32];

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc\ndef\nghi", NULL);
        pd_text_read(text, 0, 32, buf);
        ctest_equal_wcs("should preserve LF line endings", buf,
                        L"abc\ndef\nghi");
        pd_text_destroy(text);
}

static void should_dump_multiline_with_trailing_newline(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t buf[32];

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"a\nb\n", NULL);
        pd_text_read(text, 0, 32, buf);
        ctest_equal_wcs("should preserve trailing newline", buf, L"a\nb\n");
        pd_text_destroy(text);
}

static void should_dump_respect_max_len(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t buf[8];

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"abc\ndef\nghi", NULL);
        /* 只允许写 4 个 wchar，第 5 位是终止符 */
        ctest_equal_uint("should stop at max_len",
                         (unsigned)pd_text_read(text, 0, 4, buf), 4u);
        ctest_equal_wcs("should truncate without crossing buffer", buf,
                        L"abc\n");
        pd_text_destroy(text);
}

static void should_to_wcs_return_full_text(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t *buf;

        pd_text_set_multiline(text, true);
        pd_text_write(text, L"hello\nworld\n", NULL);
        buf = pd_text_to_wcs(text);
        ctest_equal_bool("should not return NULL", buf != NULL, true);
        if (buf) {
                ctest_equal_wcs("should match original including trailing LF",
                                buf, L"hello\nworld\n");
                free(buf);
        }
        pd_text_destroy(text);
}

static void should_to_wcs_handle_empty_text(void)
{
        pd_text_t *text = pd_text_create();
        wchar_t *buf = pd_text_to_wcs(text);

        ctest_equal_bool("should not return NULL on empty text", buf != NULL,
                         true);
        if (buf) {
                ctest_equal_wcs("should produce empty wide string", buf, L"");
                free(buf);
        }
        pd_text_destroy(text);
}

void test_pandagl_text_edit(void)
{
        pd_font_library_init();
        ctest_describe("text backspace",
                       should_backspace_last_char_on_last_line);
        ctest_describe("text backspace + insert",
                       should_backspace_then_continue_typing);
        ctest_describe("text backspace at buffer start",
                       should_not_crash_on_backspace_at_start);
        ctest_describe("text delete at caret", should_delete_char_at_caret);
        ctest_describe("text delete at line end",
                       should_join_next_line_when_delete_at_line_end);
        ctest_describe("text delete preserves chars",
                       should_preserve_chars_when_joining_lines);
        ctest_describe("text backspace sequence",
                       should_handle_multi_line_backspace_sequence);
        ctest_describe("text dump single line", should_dump_single_line);
        ctest_describe("text dump multiline lf", should_dump_multiline_with_lf);
        ctest_describe("text dump trailing newline",
                       should_dump_multiline_with_trailing_newline);
        ctest_describe("text dump max len", should_dump_respect_max_len);
        ctest_describe("text to_wcs full text", should_to_wcs_return_full_text);
        ctest_describe("text to_wcs empty", should_to_wcs_handle_empty_text);
        pd_font_library_destroy();
}
