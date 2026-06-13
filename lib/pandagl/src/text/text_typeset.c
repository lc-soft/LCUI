/*
 * lib/pandagl/src/text/text_typeset.c
 *
 * Copyright (c) 2018-2026, Liu
 * Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier:
 * MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <pandagl.h>
#include "text_internal.h"

#define isalpha_(ch) ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))

void pd_text_break_line(pd_text_t *text, int line_num, int col,
                        pd_text_eol_t eol)
{
        int n;
        pd_text_line_t *line, *next;

        line = pd_text_get_line(text, line_num);
        next = pd_text_insert_line(text, line_num + 1);
        /* 将本行原有的行尾符转移至下一行 */
        next->eol = line->eol;
        line->eol = eol;
        for (n = line->length - 1; n >= col; --n) {
                pd_text_line_insert(next, 0, line->string[n]);
                line->string[n] = NULL;
        }
        line->length = col;
        pd_text_update_line_size(text, line);
        pd_text_update_line_size(text, next);
}

static void pd_text_merge_line(pd_text_t *text, int line_num)
{
        int old_len;
        pd_text_line_t *line = pd_text_get_line(text, line_num);
        pd_text_line_t *next = pd_text_get_line(text, line_num + 1);

        if (!line || !next) {
                return;
        }
        if (text->caret.y > line_num) {
                --text->caret.y;
                if (text->caret.y == line_num) {
                        text->caret.x += line->length;
                }
        }
        old_len = line->length;
        pd_text_line_set_length(line, line->length + next->length);
        pd_text_line_move(line, old_len, next, 0, next->length);
        line->eol = next->eol;
        pd_text_update_line_size(text, line);
        pd_text_delete_line(text, line_num + 1);
}

static void pd_text_typeset_line(pd_text_t *text, int line_num)
{
        int col, line_width = 0, word_col = 0;
        int max_width =
            text->fixed_width > 0 ? text->fixed_width : text->max_width;

        pd_char_t *txtchar;
        pd_text_line_t *line = text->lines[line_num];
        bool autowrap =
            max_width > 0 && text->autowrap_enabled && text->multiline_enabled;

        for (col = 0; col < line->length; ++col) {
                txtchar = line->string[col];
                if (!txtchar->bitmap) {
                        continue;
                }
                /* 累加行宽度 */
                line_width += txtchar->bitmap->metrics.hori_advance;
                /* 如果是当前行的第一个字符，或者行宽度没有超过宽度限制 */
                if (!autowrap || col < 1 || line_width <= max_width) {
                        if (isalpha_(txtchar->code)) {
                        } else {
                                word_col = col + 1;
                        }
                        continue;
                }
                if (text->word_break == PD_WORD_BREAK_NORMAL) {
                        if (word_col < 1) {
                                continue;
                        }
                        col = word_col;
                }
                pd_text_break_line(text, line_num, col, PD_TEXT_EOL_NONE);
                return;
        }
        pd_text_update_line_size(text, line);
        /* 如果本行有换行符，或者是最后一行 */
        if (line->eol != PD_TEXT_EOL_NONE ||
            line_num == text->lines_length - 1) {
                return;
        }
        line_width = line->width;
        /* 本行的文本宽度未达到限制宽度，需要将下行的文本转移至本行 */
        if (line->eol == PD_TEXT_EOL_NONE) {
                pd_text_mark_line_dirty(text, line_num, 0, -1);
                pd_text_mark_line_dirty(text, line_num + 1, 0, -1);
                pd_text_merge_line(text, line_num);
                pd_text_typeset_line(text, line_num);
        }
}

/** 从指定行开始，对文本进行排版 */
void pd_text_typeset(pd_text_t *text, int start_line)
{
        int line_num;
        /* 记录排版前各个文本行的矩形区域 */
        pd_text_mark_dirty(text, start_line, -1);
        for (line_num = start_line; line_num < text->lines_length; ++line_num) {
                pd_text_typeset_line(text, line_num);
        }
        /* 记录排版后各个文本行的矩形区域 */
        pd_text_mark_dirty(text, start_line, -1);
}

/** 设置是否启用多行文本模式 */
void pd_text_set_multiline(pd_text_t *text, bool enabled)
{
        if (text->multiline_enabled != enabled) {
                text->multiline_enabled = enabled;
                pd_text_request_typeset(text, 0);
        }
}

void pd_text_set_autowrap(pd_text_t *text, bool autowrap)
{
        if (text->autowrap_enabled != autowrap) {
                text->autowrap_enabled = autowrap;
                pd_text_request_typeset(text, 0);
        }
}

void pd_text_set_word_break(pd_text_t *text, pd_word_break_t word_break)
{
        if (text->word_break != word_break) {
                text->word_break = word_break;
                pd_text_request_typeset(text, 0);
        }
}

void pd_text_set_align(pd_text_t *text, int align)
{
        text->text_align = align;
        text->task.update_typeset = true;
        text->task.typeset_start_line = 0;
}

void pd_text_set_line_height(pd_text_t *text, int height)
{
        text->line_height = height;
        text->task.update_typeset = true;
        text->task.typeset_start_line = 0;
}
