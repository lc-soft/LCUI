/*
 * src/widgets/textedit.h: -- text editing state used by textinput.
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_WIDGETS_TEXTEDIT_H
#define LCUI_WIDGETS_TEXTEDIT_H

#include <stddef.h>
#include <stdbool.h>
#include <wchar.h>
#include <pandagl/text.h>

/* Keyboard input is encoded as a single integer. The shift state must be a
 * power of two. Character input is submitted through
 * ui_text_edit_text_input() instead of these key values. */
typedef enum ui_text_edit_key {
        UI_TEXT_EDIT_KEY_LEFT = 0x00000001,
        UI_TEXT_EDIT_KEY_RIGHT = 0x00000002,
        UI_TEXT_EDIT_KEY_UP = 0x00000003,
        UI_TEXT_EDIT_KEY_DOWN = 0x00000004,
        UI_TEXT_EDIT_KEY_PGUP = 0x00000005,
        UI_TEXT_EDIT_KEY_PGDOWN = 0x00000006,
        UI_TEXT_EDIT_KEY_LINESTART = 0x00000007,
        UI_TEXT_EDIT_KEY_LINEEND = 0x00000008,
        UI_TEXT_EDIT_KEY_TEXTSTART = 0x00000009,
        UI_TEXT_EDIT_KEY_TEXTEND = 0x0000000a,
        UI_TEXT_EDIT_KEY_DELETE = 0x0000000b,
        UI_TEXT_EDIT_KEY_BACKSPACE = 0x0000000c,
        UI_TEXT_EDIT_KEY_UNDO = 0x0000000d,
        UI_TEXT_EDIT_KEY_REDO = 0x0000000e,
        UI_TEXT_EDIT_KEY_WORDLEFT = 0x0000000f,
        UI_TEXT_EDIT_KEY_WORDRIGHT = 0x00000010,
        UI_TEXT_EDIT_KEY_SHIFT = 0x20000000
} ui_text_edit_key_t;

typedef struct ui_text_edit ui_text_edit_t;

ui_text_edit_t *ui_text_edit_create(void);
void ui_text_edit_destroy(ui_text_edit_t *edit);

/** Replace the whole text. CRLF and CR are normalized to LF. */
void ui_text_edit_set_text(ui_text_edit_t *edit, const wchar_t *text,
                           size_t len);

/** Copy up to max_len characters starting at start. Returns copied count. */
size_t ui_text_edit_get_text(ui_text_edit_t *edit, size_t start, size_t max_len,
                             wchar_t *buf);

size_t ui_text_edit_get_length(ui_text_edit_t *edit);

/** Insert committed text at the cursor (IME / paste / typing). */
void ui_text_edit_text_input(ui_text_edit_t *edit, const wchar_t *text,
                             size_t len);

/** Send a navigation or editing key, e.g. UI_TEXT_EDIT_KEY_BACKSPACE. */
void ui_text_edit_key(ui_text_edit_t *edit, int key);

/** Move the caret to a visual row/column. */
void ui_text_edit_move_caret(ui_text_edit_t *edit, int row, int col);

/** Move the caret to a visual row/column, optionally extending the selection.
 */
void ui_text_edit_move_caret_extend(ui_text_edit_t *edit, int row, int col,
                                    bool extend);

/** Move the caret up/down by visual rows, keeping the preferred x. */
void ui_text_edit_move_vertical(ui_text_edit_t *edit, int delta, bool extend);

/** Append text at the end regardless of the current caret. */
void ui_text_edit_append_text(ui_text_edit_t *edit, const wchar_t *text,
                              size_t len);

/** Mouse click/drag, in text-layer pixel coordinates (origin = text top-left).
 */
void ui_text_edit_click(ui_text_edit_t *edit, float x, float y);
void ui_text_edit_drag(ui_text_edit_t *edit, float x, float y);

bool ui_text_edit_has_selection(ui_text_edit_t *edit);
void ui_text_edit_selection(ui_text_edit_t *edit, size_t *start, size_t *end);
void ui_text_edit_select_all(ui_text_edit_t *edit);
void ui_text_edit_clear_selection(ui_text_edit_t *edit);
size_t ui_text_edit_copy(ui_text_edit_t *edit, wchar_t *buf, size_t max_len);
size_t ui_text_edit_cut(ui_text_edit_t *edit, wchar_t *buf, size_t max_len);

/* Layout projection: the pd_text layer is rebuilt from the canonical text
 * after every edit, and the stb caret index is mapped to its visual row/col. */
void ui_text_edit_set_layer(ui_text_edit_t *edit, pd_text_t *layer);
void ui_text_edit_set_page_rows(ui_text_edit_t *edit, int rows);
void ui_text_edit_sync_layer(ui_text_edit_t *edit);
void ui_text_edit_sync_caret(ui_text_edit_t *edit);
int ui_text_edit_get_caret_row(ui_text_edit_t *edit);
int ui_text_edit_get_caret_col(ui_text_edit_t *edit);

#endif
