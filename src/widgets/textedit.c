/*
 * src/widgets/textedit.c: -- text editing state used by textinput.
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <pandagl.h>
#include "textedit.h"

#define STB_TEXTEDIT_CHARTYPE wchar_t
#define STB_TEXTEDIT_NEWLINE L'\n'
#define STB_TEXTEDIT_GETWIDTH_NEWLINE (-1)

#include "third-party/stb_textedit.h"

typedef struct ui_text_edit {
        wchar_t *text;
        int length;
        int capacity;
        pd_text_t *layer;
        STB_TexteditState state;
} ui_text_edit_t;

static bool ui_text_edit_index_to_caret(ui_text_edit_t *edit, int index,
                                        int *row, int *col);
static int ui_text_edit_eol_length(pd_text_eol_t eol);
static void ui_text_edit_on_changed(ui_text_edit_t *edit);

static void ui_text_edit_rebuild_layer(ui_text_edit_t *edit)
{
        list_t rects;

        if (!edit->layer) {
                return;
        }
        pd_text_clear(edit->layer);
        if (edit->length > 0) {
                pd_text_append(edit->layer, edit->text, NULL);
        }
        list_create(&rects);
        pd_text_update(edit->layer, &rects);
        pd_rects_clear(&rects);
}

void ui_text_edit_sync_caret(ui_text_edit_t *edit)
{
        int row = 0, col = 0;

        if (!edit->layer) {
                return;
        }
        ui_text_edit_index_to_caret(edit, edit->state.cursor, &row, &col);
        pd_text_set_caret(edit->layer, row, col);
}

void ui_text_edit_sync_layer(ui_text_edit_t *edit)
{
        if (!edit) {
                return;
        }
        ui_text_edit_rebuild_layer(edit);
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_set_layer(ui_text_edit_t *edit, pd_text_t *layer)
{
        if (!edit) {
                return;
        }
        edit->layer = layer;
        ui_text_edit_sync_layer(edit);
}

void ui_text_edit_set_page_rows(ui_text_edit_t *edit, int rows)
{
        if (!edit) {
                return;
        }
        edit->state.row_count_per_page = rows > 0 ? rows : 1;
}

int ui_text_edit_get_caret_row(ui_text_edit_t *edit)
{
        return (edit && edit->layer) ? edit->layer->caret.y : 0;
}

int ui_text_edit_get_caret_col(ui_text_edit_t *edit)
{
        return (edit && edit->layer) ? edit->layer->caret.x : 0;
}

static void ui_text_edit_on_changed(ui_text_edit_t *edit)
{
        if (!edit->layer) {
                return;
        }
        ui_text_edit_rebuild_layer(edit);
        ui_text_edit_sync_caret(edit);
}

static bool ui_text_edit_index_to_caret(ui_text_edit_t *edit, int index,
                                        int *row, int *col)
{
        int i, start, len, lines;

        *row = 0;
        *col = 0;
        if (!edit->layer) {
                return false;
        }
        lines = pd_text_get_lines_length(edit->layer);
        if (lines <= 0) {
                return false;
        }
        for (i = 0, start = 0; i < lines; ++i) {
                len = pd_text_get_line_length(edit->layer, i);
                if (index < start) {
                        continue;
                }
                if (index < start + len) {
                        *row = i;
                        *col = index - start;
                        return true;
                }
                if (index == start + len) {
                        if (i + 1 < lines &&
                            pd_text_get_line_eol(edit->layer, i) ==
                                PD_TEXT_EOL_NONE) {
                                /* Soft wrap: the index belongs to the next
                                 * visual line for display; stb layout uses
                                 * layout_row_at for the same boundary. */
                                start += len;
                                continue;
                        }
                        *row = i;
                        *col = len;
                        return true;
                }
                start += len + ui_text_edit_eol_length(
                                   pd_text_get_line_eol(edit->layer, i));
        }
        *row = lines - 1;
        *col = pd_text_get_line_length(edit->layer, lines - 1);
        return true;
}

static int ui_text_edit_reserve(ui_text_edit_t *edit, int capacity)
{
        wchar_t *text;

        if (capacity < 0 || capacity > INT_MAX - 1) {
                return -1;
        }
        if (capacity <= edit->capacity) {
                return 0;
        }
        text = realloc(edit->text, sizeof(wchar_t) * (capacity + 1));
        if (!text) {
                return -1;
        }
        edit->text = text;
        edit->capacity = capacity;
        return 0;
}

static int ui_text_edit_raw_insert(ui_text_edit_t *edit, int at,
                                   const wchar_t *text, int len)
{
        if (len <= 0 || at < 0 || at > edit->length) {
                return 0;
        }
        if (ui_text_edit_reserve(edit, edit->length + len) != 0) {
                return 0;
        }
        memmove(edit->text + at + len, edit->text + at,
                sizeof(wchar_t) * (edit->length - at));
        memcpy(edit->text + at, text, sizeof(wchar_t) * len);
        edit->length += len;
        edit->text[edit->length] = 0;
        ui_text_edit_on_changed(edit);
        return 1;
}

static int ui_text_edit_raw_delete(ui_text_edit_t *edit, int at, int len)
{
        if (len <= 0 || at < 0 || at + len > edit->length) {
                return 0;
        }
        memmove(edit->text + at, edit->text + at + len,
                sizeof(wchar_t) * (edit->length - at - len + 1));
        edit->length -= len;
        ui_text_edit_on_changed(edit);
        return 1;
}

static int ui_text_edit_eol_length(pd_text_eol_t eol)
{
        switch (eol) {
        case PD_TEXT_EOL_CR:
        case PD_TEXT_EOL_LF:
                return 1;
        case PD_TEXT_EOL_CR_LF:
                return 2;
        case PD_TEXT_EOL_NONE:
        default:
                break;
        }
        return 0;
}

static bool ui_text_edit_is_high_surrogate(wchar_t ch)
{
#if defined(_WIN32)
        return ch >= 0xD800 && ch <= 0xDBFF;
#else
        return false;
#endif
}

static bool ui_text_edit_is_low_surrogate(wchar_t ch)
{
#if defined(_WIN32)
        return ch >= 0xDC00 && ch <= 0xDFFF;
#else
        return false;
#endif
}

static int ui_text_edit_next_index(ui_text_edit_t *edit, int i)
{
        if (i < 0 || i >= edit->length) {
                return edit->length;
        }
        if (i + 1 < edit->length &&
            ui_text_edit_is_high_surrogate(edit->text[i]) &&
            ui_text_edit_is_low_surrogate(edit->text[i + 1])) {
                return i + 2;
        }
        return i + 1;
}

static int ui_text_edit_prev_index(ui_text_edit_t *edit, int i)
{
        if (i <= 0) {
                return 0;
        }
        if (i >= 2 && ui_text_edit_is_low_surrogate(edit->text[i - 1]) &&
            ui_text_edit_is_high_surrogate(edit->text[i - 2])) {
                return i - 2;
        }
        return i - 1;
}

static bool ui_text_edit_layout_row_at(ui_text_edit_t *edit, int at, int *row,
                                       int *col)
{
        int i, start, len, lines;

        *row = 0;
        *col = 0;
        if (!edit->layer) {
                return false;
        }
        lines = pd_text_get_lines_length(edit->layer);
        if (lines <= 0) {
                return false;
        }
        for (i = 0, start = 0; i < lines; ++i) {
                len = pd_text_get_line_length(edit->layer, i);
                if (at == start) {
                        *row = i;
                        *col = 0;
                        return true;
                }
                if (at > start && at < start + len) {
                        *row = i;
                        *col = at - start;
                        return true;
                }
                if (at == start + len) {
                        if (i + 1 < lines &&
                            pd_text_get_line_eol(edit->layer, i) ==
                                PD_TEXT_EOL_NONE) {
                                /* Soft wrap: the next visual line starts at
                                 * this index, so LAYOUTROW must use it. */
                                start += len;
                                continue;
                        }
                        *row = i;
                        *col = len;
                        return true;
                }
                start += len + ui_text_edit_eol_length(
                                   pd_text_get_line_eol(edit->layer, i));
        }
        *row = lines - 1;
        *col = pd_text_get_line_length(edit->layer, lines - 1);
        return true;
}

static void ui_text_edit_layout_row(ui_text_edit_t *edit, int at,
                                    StbTexteditRow *row)
{
        int line, col, len;
        pd_pos_t pos;
        pd_text_line_t *text_line;

        row->x0 = 0;
        row->x1 = 0;
        row->baseline_y_delta = 0;
        row->ymin = 0;
        row->ymax = 0;
        row->num_chars = 0;
        if (!edit->layer ||
            !ui_text_edit_layout_row_at(edit, at, &line, &col)) {
                return;
        }
        text_line = edit->layer->lines[line];
        if (!text_line) {
                return;
        }
        len = text_line->length;
        if (col > len) {
                col = len;
        }
        row->num_chars =
            len - col +
            ui_text_edit_eol_length(pd_text_get_line_eol(edit->layer, line));
        row->baseline_y_delta =
            (float)pd_text_get_line_height(edit->layer, line);
        row->ymin = 0;
        row->ymax = row->baseline_y_delta;
        if (pd_text_get_char_pixel(edit->layer, line, 0, &pos) == 0) {
                row->x0 = (float)pos.x;
                row->x1 = row->x0 + (float)text_line->width;
        }
}

static int ui_text_edit_get_width(ui_text_edit_t *edit, int row_start, int i)
{
        int line, col;
        pd_text_line_t *text_line;
        pd_char_t *ch;

        if (!edit->layer ||
            !ui_text_edit_layout_row_at(edit, row_start, &line, &col)) {
                return 0;
        }
        text_line = edit->layer->lines[line];
        if (!text_line || col + i >= text_line->length) {
                return STB_TEXTEDIT_GETWIDTH_NEWLINE;
        }
        ch = text_line->string[col + i];
        if (!ch || !ch->bitmap) {
                return 0;
        }
        return ch->bitmap->metrics.hori_advance;
}

#define STB_TEXTEDIT_STRING ui_text_edit_t
#define STB_TEXTEDIT_STRINGLEN(e) ((e)->length)
#define STB_TEXTEDIT_GETCHAR(e, i) ((e)->text[i])
#define STB_TEXTEDIT_INSERTCHARS(e, i, c, n) \
        ui_text_edit_raw_insert(e, i, c, (int)(n))
#define STB_TEXTEDIT_DELETECHARS(e, i, n) \
        ui_text_edit_raw_delete(e, i, (int)(n))
#define STB_TEXTEDIT_LAYOUTROW(r, e, i) ui_text_edit_layout_row(e, i, r)
#define STB_TEXTEDIT_GETWIDTH(e, s, i) ui_text_edit_get_width(e, s, i)
#define STB_TEXTEDIT_IS_SPACE(ch) iswspace(ch)

#define STB_TEXTEDIT_K_SHIFT UI_TEXT_EDIT_KEY_SHIFT
#define STB_TEXTEDIT_K_LEFT UI_TEXT_EDIT_KEY_LEFT
#define STB_TEXTEDIT_K_RIGHT UI_TEXT_EDIT_KEY_RIGHT
#define STB_TEXTEDIT_K_UP UI_TEXT_EDIT_KEY_UP
#define STB_TEXTEDIT_K_DOWN UI_TEXT_EDIT_KEY_DOWN
#define STB_TEXTEDIT_K_PGUP UI_TEXT_EDIT_KEY_PGUP
#define STB_TEXTEDIT_K_PGDOWN UI_TEXT_EDIT_KEY_PGDOWN
#define STB_TEXTEDIT_K_LINESTART UI_TEXT_EDIT_KEY_LINESTART
#define STB_TEXTEDIT_K_LINEEND UI_TEXT_EDIT_KEY_LINEEND
#define STB_TEXTEDIT_K_TEXTSTART UI_TEXT_EDIT_KEY_TEXTSTART
#define STB_TEXTEDIT_K_TEXTEND UI_TEXT_EDIT_KEY_TEXTEND
#define STB_TEXTEDIT_K_DELETE UI_TEXT_EDIT_KEY_DELETE
#define STB_TEXTEDIT_K_BACKSPACE UI_TEXT_EDIT_KEY_BACKSPACE
#define STB_TEXTEDIT_K_UNDO UI_TEXT_EDIT_KEY_UNDO
#define STB_TEXTEDIT_K_REDO UI_TEXT_EDIT_KEY_REDO
#define STB_TEXTEDIT_K_WORDLEFT UI_TEXT_EDIT_KEY_WORDLEFT
#define STB_TEXTEDIT_K_WORDRIGHT UI_TEXT_EDIT_KEY_WORDRIGHT
#define STB_TEXTEDIT_GETPREVCHARINDEX(e, i) ui_text_edit_prev_index(e, i)
#define STB_TEXTEDIT_GETNEXTCHARINDEX(e, i) ui_text_edit_next_index(e, i)

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4244)
#elif defined(__GNUC__)
/* stb_textedit is vendored as-is; it contains helpers (and a dead local) that
 * our integration does not call.  Silence those diagnostics only for the
 * implementation translation unit so -Werror does not reject the header. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define STB_TEXTEDIT_IMPLEMENTATION
#include "third-party/stb_textedit.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

ui_text_edit_t *ui_text_edit_create(void)
{
        ui_text_edit_t *edit = calloc(1, sizeof(ui_text_edit_t));

        if (!edit) {
                return NULL;
        }
        ui_text_edit_reserve(edit, 32);
        stb_textedit_initialize_state(&edit->state, 0);
        return edit;
}

void ui_text_edit_destroy(ui_text_edit_t *edit)
{
        if (!edit) {
                return;
        }
        free(edit->text);
        free(edit);
}

void ui_text_edit_set_text(ui_text_edit_t *edit, const wchar_t *text,
                           size_t len)
{
        size_t i;
        int j = 0;

        if (!edit) {
                return;
        }
        if (!text) {
                len = 0;
        }
        if (len > (size_t)INT_MAX) {
                return;
        }
        if (ui_text_edit_reserve(edit, (int)len) != 0) {
                return;
        }
        for (i = 0; i < len; ++i) {
                if (text[i] == L'\r') {
                        if (i + 1 < len && text[i + 1] == L'\n') {
                                ++i;
                        }
                        edit->text[j++] = L'\n';
                } else {
                        edit->text[j++] = text[i];
                }
        }
        edit->length = j;
        edit->text[edit->length] = 0;
        stb_textedit_initialize_state(&edit->state, 0);
        /* Match the previous textinput behavior: after set_text the caret is
         * at the end, so subsequent typing appends. */
        edit->state.cursor = edit->length;
        edit->state.select_start = edit->length;
        edit->state.select_end = edit->length;
        ui_text_edit_sync_layer(edit);
}

size_t ui_text_edit_get_text(ui_text_edit_t *edit, size_t start, size_t max_len,
                             wchar_t *buf)
{
        size_t n = 0;

        if (!buf || max_len == 0) {
                if (buf) {
                        buf[0] = 0;
                }
                return 0;
        }
        if (!edit || start >= (size_t)edit->length) {
                buf[0] = 0;
                return 0;
        }
        while (n < max_len && start + n < (size_t)edit->length) {
                buf[n] = edit->text[start + n];
                ++n;
        }
        buf[n] = 0;
        return n;
}

size_t ui_text_edit_get_length(ui_text_edit_t *edit)
{
        return edit ? (size_t)edit->length : 0;
}

void ui_text_edit_text_input(ui_text_edit_t *edit, const wchar_t *text,
                             size_t len)
{
        size_t i, j;
        wchar_t *normalized;
        bool has_cr = false;

        if (!edit || !text || len == 0) {
                return;
        }
        if (len > (size_t)INT_MAX) {
                return;
        }
        for (i = 0; i < len; ++i) {
                if (text[i] == L'\r') {
                        has_cr = true;
                        break;
                }
        }
        if (!has_cr) {
                stb_textedit_text(edit, &edit->state, (wchar_t *)text,
                                  (int)len);
                ui_text_edit_sync_caret(edit);
                return;
        }
        normalized = malloc(sizeof(wchar_t) * (len + 1));
        if (!normalized) {
                return;
        }
        for (i = 0, j = 0; i < len; ++i) {
                if (text[i] == L'\r') {
                        if (i + 1 < len && text[i + 1] == L'\n') {
                                ++i;
                        }
                        normalized[j++] = L'\n';
                } else {
                        normalized[j++] = text[i];
                }
        }
        stb_textedit_text(edit, &edit->state, normalized, (int)j);
        free(normalized);
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_key(ui_text_edit_t *edit, int key)
{
        if (!edit) {
                return;
        }
        stb_textedit_key(edit, &edit->state, key);
        ui_text_edit_sync_caret(edit);
}

static int ui_text_edit_clamp_index(ui_text_edit_t *edit, int index)
{
        if (index < 0) {
                return 0;
        }
        if (index > edit->length) {
                return edit->length;
        }
        if (index > 0 && index < edit->length &&
            ui_text_edit_is_low_surrogate(edit->text[index]) &&
            ui_text_edit_is_high_surrogate(edit->text[index - 1])) {
                --index;
        }
        return index;
}

void ui_text_edit_move_caret_extend(ui_text_edit_t *edit, int row, int col,
                                    bool extend)
{
        int index = 0;

        if (!edit) {
                return;
        }
        if (edit->layer) {
                int lines = pd_text_get_lines_length(edit->layer);
                if (row < 0) {
                        row = 0;
                } else if (lines > 0 && row >= lines) {
                        row = lines - 1;
                }
                index = pd_text_get_line_char_start(edit->layer, row);
                col = pd_text_get_line_length(edit->layer, row) < col
                          ? pd_text_get_line_length(edit->layer, row)
                          : col;
                if (col < 0) {
                        col = 0;
                }
                index += col;
        }
        index = ui_text_edit_clamp_index(edit, index);
        if (extend) {
                if (edit->state.select_start == edit->state.select_end) {
                        edit->state.select_start = edit->state.cursor;
                }
                edit->state.cursor = index;
                edit->state.select_end = index;
        } else {
                edit->state.cursor = index;
                edit->state.select_start = index;
                edit->state.select_end = index;
        }
        edit->state.has_preferred_x = 0;
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_move_caret(ui_text_edit_t *edit, int row, int col)
{
        ui_text_edit_move_caret_extend(edit, row, col, false);
}

void ui_text_edit_move_vertical(ui_text_edit_t *edit, int delta, bool extend)
{
        int row, col, lines, target, i, start, len, best_col = 0;
        int target_x, best_dx = INT_MAX;
        pd_pos_t pos;

        if (!edit || !edit->layer || delta == 0) {
                return;
        }
        ui_text_edit_index_to_caret(edit, edit->state.cursor, &row, &col);
        if (!edit->state.has_preferred_x) {
                if (pd_text_get_char_pixel(edit->layer, row, col, &pos) != 0) {
                        return;
                }
                edit->state.preferred_x = (float)pos.x;
                edit->state.has_preferred_x = 1;
        }
        target_x = (int)edit->state.preferred_x;
        lines = pd_text_get_lines_length(edit->layer);
        target = row + delta;
        if (target < 0) {
                target = 0;
        } else if (target >= lines) {
                target = lines - 1;
        }
        start = pd_text_get_line_char_start(edit->layer, target);
        len = pd_text_get_line_length(edit->layer, target);
        for (i = 0; i <= len; ++i) {
                int dx;

                if (pd_text_get_char_pixel(edit->layer, target, i, &pos) != 0) {
                        break;
                }
                dx = pos.x - target_x;
                if (dx < 0) {
                        dx = -dx;
                }
                if (dx < best_dx) {
                        best_dx = dx;
                        best_col = i;
                }
        }
        if (extend) {
                if (edit->state.select_start == edit->state.select_end) {
                        edit->state.select_start = edit->state.cursor;
                }
                edit->state.cursor =
                    ui_text_edit_clamp_index(edit, start + best_col);
                edit->state.select_end = edit->state.cursor;
        } else {
                edit->state.cursor =
                    ui_text_edit_clamp_index(edit, start + best_col);
                edit->state.select_start = edit->state.cursor;
                edit->state.select_end = edit->state.cursor;
        }
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_append_text(ui_text_edit_t *edit, const wchar_t *text,
                              size_t len)
{
        if (!edit || !text || len == 0 || len > (size_t)INT_MAX) {
                return;
        }
        edit->state.cursor = edit->length;
        edit->state.select_start = edit->length;
        edit->state.select_end = edit->length;
        ui_text_edit_text_input(edit, text, len);
}

void ui_text_edit_click(ui_text_edit_t *edit, float x, float y)
{
        if (!edit) {
                return;
        }
        stb_textedit_click(edit, &edit->state, x, y);
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_drag(ui_text_edit_t *edit, float x, float y)
{
        if (!edit) {
                return;
        }
        stb_textedit_drag(edit, &edit->state, x, y);
        ui_text_edit_sync_caret(edit);
}

bool ui_text_edit_has_selection(ui_text_edit_t *edit)
{
        return edit && edit->state.select_start != edit->state.select_end;
}

void ui_text_edit_selection(ui_text_edit_t *edit, size_t *start, size_t *end)
{
        size_t a = 0, b = 0;

        if (edit) {
                a = (size_t)edit->state.select_start;
                b = (size_t)edit->state.select_end;
        }
        if (a > b) {
                size_t tmp = a;
                a = b;
                b = tmp;
        }
        if (start) {
                *start = a;
        }
        if (end) {
                *end = b;
        }
}

void ui_text_edit_select_all(ui_text_edit_t *edit)
{
        if (!edit) {
                return;
        }
        edit->state.select_start = 0;
        edit->state.select_end = edit->length;
        edit->state.cursor = edit->length;
        edit->state.has_preferred_x = 0;
        ui_text_edit_sync_caret(edit);
}

void ui_text_edit_clear_selection(ui_text_edit_t *edit)
{
        if (!edit) {
                return;
        }
        edit->state.select_start = edit->state.cursor;
        edit->state.select_end = edit->state.cursor;
        ui_text_edit_sync_caret(edit);
}

size_t ui_text_edit_copy(ui_text_edit_t *edit, wchar_t *buf, size_t max_len)
{
        size_t start, end, n = 0;

        if (!buf || max_len == 0) {
                if (buf) {
                        buf[0] = 0;
                }
                return 0;
        }
        ui_text_edit_selection(edit, &start, &end);
        if (!edit) {
                buf[0] = 0;
                return 0;
        }
        while (n < max_len && start + n < end) {
                buf[n] = edit->text[start + n];
                ++n;
        }
        buf[n] = 0;
        return n;
}

size_t ui_text_edit_cut(ui_text_edit_t *edit, wchar_t *buf, size_t max_len)
{
        size_t n = ui_text_edit_copy(edit, buf, max_len);

        if (edit) {
                stb_textedit_cut(edit, &edit->state);
                ui_text_edit_sync_caret(edit);
        }
        return n;
}
