/*
 * lib/pandagl/src/text/text.c
 *
 * Copyright (c) 2018-2026, Liu Chao
 * <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *

 * * This file is part of LCUI, distributed under the MIT License found in the

 * * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pandagl.h>
#include "text_internal.h"

#define DEFAULT_LINE_HEIGHT 1.42857143

static void pd_text_line_init(pd_text_line_t *line)
{
        line->width = 0;
        line->height = 0;
        line->length = 0;
        line->string = NULL;
        line->eol = PD_TEXT_EOL_NONE;
}

static void pd_text_line_destroy(pd_text_line_t *line)
{
        int i;
        for (i = 0; i < line->length; ++i) {
                if (line->string[i]) {
                        free(line->string[i]);
                }
        }
        line->width = 0;
        line->height = 0;
        line->length = 0;
        if (line->string) {
                free(line->string);
        }
        line->string = NULL;
}

pd_text_line_t *pd_text_insert_line(pd_text_t *text, size_t line_num)
{
        size_t i, size;
        pd_text_line_t *line, **lines;

        if (line_num > text->length) {
                line_num = text->lines_length;
        }
        ++text->lines_length;
        size = sizeof(pd_text_line_t *) * (text->lines_length + 1);
        lines = realloc(text->lines, size);
        if (!lines) {
                --text->lines_length;
                return NULL;
        }
        lines[text->lines_length] = NULL;
        line = malloc(sizeof(pd_text_line_t));
        if (!line) {
                --text->lines_length;
                return NULL;
        }
        pd_text_line_init(line);
        for (i = text->lines_length - 1; i > line_num; --i) {
                lines[i] = lines[i - 1];
        }
        lines[line_num] = line;
        text->lines = lines;
        return line;
}

int pd_text_delete_line(pd_text_t *text, int line_num)
{
        if (line_num < 0 || line_num >= text->lines_length) {
                return -1;
        }
        pd_text_line_destroy(text->lines[line_num]);
        free(text->lines[line_num]);
        for (; line_num + 1 < text->lines_length; ++line_num) {
                text->lines[line_num] = text->lines[line_num + 1];
        }
        text->lines[line_num] = NULL;
        --text->lines_length;
        return 0;
}

pd_text_line_t *pd_text_append_line(pd_text_t *text)
{
        return pd_text_insert_line(text, text->lines_length);
}

pd_text_line_t *pd_text_get_line(pd_text_t *text, int line_num)
{
        return (line_num >= text->lines_length) ? NULL : text->lines[line_num];
}

void pd_text_update_line_size(pd_text_t *text, pd_text_line_t *line)
{
        int i;
        int text_height = text->default_style.pixel_size;
        pd_char_t *ch;

        line->width = 0;
        for (i = 0; i < line->length; ++i) {
                ch = line->string[i];
                if (!ch->bitmap) {
                        continue;
                }
                line->width += ch->bitmap->metrics.hori_advance;
                if (text_height < ch->bitmap->metrics.vert_advance) {
                        text_height = ch->bitmap->metrics.vert_advance;
                }
        }
        if (text->line_height > 0) {
                line->height = text->line_height;
        } else {
                line->height = (int)round(text_height * DEFAULT_LINE_HEIGHT);
        }
}

int pd_text_line_set_length(pd_text_line_t *line, int len)
{
        int i;
        int old_len = line->length;
        pd_char_t **txtstr;

        if (len < 0) {
                len = 0;
        }
        txtstr = realloc(line->string, sizeof(pd_char_t *) * (len + 1));
        if (!txtstr) {
                return -1;
        }
        /* 清零新增槽位，防止遗留指针被误用或重复释放 */
        for (i = old_len; i < len; ++i) {
                txtstr[i] = NULL;
        }
        txtstr[len] = NULL;
        line->string = txtstr;
        line->length = len;
        return 0;
}

/* 释放并清空指定区间 [start, end) 内的字符槽位 */
void pd_text_line_erase(pd_text_line_t *line, int start, int end)
{
        int i;

        if (start < 0) {
                start = 0;
        }
        if (end > line->length) {
                end = line->length;
        }
        for (i = start; i < end; ++i) {
                if (line->string[i]) {
                        free(line->string[i]);
                        line->string[i] = NULL;
                }
        }
}

/* 将 src 行 [src_start, src_end) 的字符所有权转移到 dst 行的 dst_start
 * 起始位置。 转移后 src 对应槽位被置 NULL，防止 delete_line 时被重复释放。
 * 调用者负责保证 dst 已有足够容量。 */
void pd_text_line_move(pd_text_line_t *dst, int dst_start, pd_text_line_t *src,
                       int src_start, int src_end)
{
        int i, j;

        if (src_end > src->length) {
                src_end = src->length;
        }
        for (i = dst_start, j = src_start; j < src_end; ++i, ++j) {
                dst->string[i] = src->string[j];
                src->string[j] = NULL;
        }
}

int pd_text_line_insert(pd_text_line_t *line, int offset, pd_char_t *txtchar)
{
        size_t i, insert_index;

        if (offset < 0) {
                insert_index = line->length + 1 + offset;
                if (insert_index < 0) {
                        insert_index = 0;
                }
        } else if (offset > line->length) {
                insert_index = line->length;
        } else {
                insert_index = offset;
        }
        pd_text_line_set_length(line, line->length + 1);
        for (i = line->length - 1; i > insert_index; --i) {
                line->string[i] = line->string[i - 1];
        }
        line->string[insert_index] = txtchar;
        return 0;
}

int pd_text_line_insert_copy(pd_text_line_t *line, int ins_pos,
                             pd_char_t *txtchar)
{
        pd_char_t *txtchar2;
        txtchar2 = malloc(sizeof(pd_char_t));
        *txtchar2 = *txtchar;
        return pd_text_line_insert(line, ins_pos, txtchar2);
}

pd_text_t *pd_text_create(void)
{
        pd_text_t *text;
        text = malloc(sizeof(pd_text_t));
        text->width = 0;
        text->length = 0;
        text->offset_x = 0;
        text->offset_y = 0;
        text->caret.x = 0;
        text->caret.y = 0;
        text->max_width = 0;
        text->max_height = 0;
        text->fixed_width = 0;
        text->fixed_height = 0;
        text->new_offset_x = 0;
        text->new_offset_y = 0;
        text->line_height = -1;
        text->lines_length = 0;
        text->lines = NULL;
        text->text_align = PD_TEXT_ALIGN_LEFT;
        text->autowrap_enabled = false;
        text->multiline_enabled = false;
        text->style_tag_enabled = false;
        text->word_break = PD_WORD_BREAK_NORMAL;
        text->task.typeset_start_line = 0;
        text->task.update_typeset = 0;
        text->task.update_bitmap = 0;
        text->task.redraw_all = 0;
        list_create(&text->dirty_rects);
        list_create(&text->styles);
        pd_text_style_init(&text->default_style);
        pd_text_insert_line(text, 0);
        return text;
}

static void on_destroy_text_style(void *data)
{
        pd_text_style_destroy(data);
        free(data);
}

static void pd_text_reset_lines(pd_text_t *text)
{
        int line_num;

        pd_text_mark_dirty(text, 0, -1);
        for (line_num = 0; line_num < text->lines_length; ++line_num) {
                pd_text_line_destroy(text->lines[line_num]);
                free(text->lines[line_num]);
                text->lines[line_num] = NULL;
        }
        if (text->lines) {
                free(text->lines);
        }
        text->lines = NULL;
        text->lines_length = 0;
        text->width = 0;
        text->length = 0;
        text->caret.x = 0;
        text->caret.y = 0;
        list_destroy(&text->styles, on_destroy_text_style);
        text->task.redraw_all = true;
}

void pd_text_clear(pd_text_t *text)
{
        pd_text_reset_lines(text);
        pd_text_insert_line(text, 0);
}

void pd_text_destroy(pd_text_t *text)
{
        pd_text_reset_lines(text);
        pd_rects_clear(&text->dirty_rects);
        pd_text_style_destroy(&text->default_style);
        free(text);
}

/* 根据对齐方式，计算文本行的起始x轴位置 */
int pd_text_get_line_start_x(pd_text_t *text, pd_text_line_t *line)
{
        int width;
        if (text->fixed_width > 0) {
                width = text->fixed_width;
        } else {
                width = text->width;
        }
        switch (text->text_align) {
        case PD_TEXT_ALIGN_CENTER:
                return (width - line->width) / 2;
        case PD_TEXT_ALIGN_RIGHT:
                return width - line->width;
        case PD_TEXT_ALIGN_LEFT:
        default:
                break;
        }
        return 0;
}

int pd_text_get_lines_length(pd_text_t *text)
{
        return text->lines_length;
}

int pd_text_get_line_height(pd_text_t *text, int line_num)
{
        if (line_num >= text->lines_length) {
                return 0;
        }
        return text->lines[line_num]->height;
}

int pd_text_get_line_length(pd_text_t *text, int line_num)
{
        if (line_num >= text->lines_length) {
                return -1;
        }
        return text->lines[line_num]->length;
}

void pd_text_request_typeset(pd_text_t *text, int start_line)
{
        if (start_line < text->task.typeset_start_line) {
                text->task.typeset_start_line = start_line;
        }
        text->task.update_typeset = true;
}

/** 获取指定文本行中的文本段的矩形区域 */
int pd_text_get_line_rect(pd_text_t *text, int line_num, int start_col,
                          int end_col, pd_rect_t *rect)
{
        int i;
        pd_text_line_t *line;

        if (line_num >= text->lines_length) {
                return -1;
        }
        /* 先计算在有效区域内的起始行的y轴坐标 */
        rect->y = text->offset_y;
        rect->x = text->offset_x;
        for (i = 0; i < line_num; ++i) {
                rect->y += text->lines[i]->height;
        }
        line = text->lines[line_num];
        if (end_col < 0 || end_col >= line->length) {
                end_col = line->length - 1;
        }
        rect->height = line->height;
        rect->x += pd_text_get_line_start_x(text, line);
        if (start_col == 0 && end_col == line->length - 1) {
                rect->width = line->width;
        } else {
                for (i = 0; i < start_col; ++i) {
                        if (!line->string[i]->bitmap) {
                                continue;
                        }
                        rect->x +=
                            line->string[i]->bitmap->metrics.hori_advance;
                }
                rect->width = 0;
                for (i = start_col; i <= end_col && i < line->length; ++i) {
                        if (!line->string[i]->bitmap) {
                                continue;
                        }
                        rect->width +=
                            line->string[i]->bitmap->metrics.hori_advance;
                }
        }
        if (rect->width <= 0 || rect->height <= 0) {
                return 1;
        }
        return 0;
}

void pd_text_mark_line_dirty(pd_text_t *text, int line_num, int start, int end)
{
        pd_rect_t rect;
        if (pd_text_get_line_rect(text, line_num, start, end, &rect) == 0) {
                pd_rects_add(&text->dirty_rects, &rect);
        }
}

void pd_text_mark_dirty(pd_text_t *text, int start_line, int end_line)
{
        int i, y;
        pd_rect_t rect;

        if (end_line < 0 || end_line >= text->lines_length) {
                end_line = text->lines_length - 1;
        }

        y = text->offset_y;
        for (i = 0; i < text->lines_length; ++i) {
                y += text->lines[i]->height;
                if (i >= start_line && y >= 0) {
                        y -= text->lines[i]->height;
                        break;
                }
        }
        for (; i <= end_line; ++i) {
                pd_text_get_line_rect(text, i, 0, -1, &rect);
                pd_rects_add(&text->dirty_rects, &rect);
                y += text->lines[i]->height;
                if (y >= text->max_height) {
                        break;
                }
        }
}

int pd_text_get_width(pd_text_t *text)
{
        int i, line_num, w, max_w;
        pd_text_line_t *line;

        for (line_num = 0, max_w = 0; line_num < text->lines_length;
             ++line_num) {
                line = text->lines[line_num];
                for (i = 0, w = 0; i < line->length; ++i) {
                        if (!line->string[i]->bitmap ||
                            !line->string[i]->bitmap->buffer) {
                                continue;
                        }
                        w += line->string[i]->bitmap->metrics.hori_advance;
                }
                if (w > max_w) {
                        max_w = w;
                }
        }
        return max_w;
}

int pd_text_get_height(pd_text_t *text)
{
        int i, h;
        for (i = 0, h = 0; i < text->lines_length; ++i) {
                h += text->lines[i]->height;
        }
        return h;
}

int pd_text_set_fixed_size(pd_text_t *text, int width, int height)
{
        text->fixed_width = width;
        text->fixed_height = height;
        text->task.redraw_all = true;
        if (text->autowrap_enabled) {
                text->task.typeset_start_line = 0;
                text->task.update_typeset = true;
        }
        return 0;
}

int pd_text_set_max_size(pd_text_t *text, int width, int height)
{
        text->max_width = width;
        text->max_height = height;
        text->task.redraw_all = true;
        if (text->autowrap_enabled) {
                text->task.typeset_start_line = 0;
                text->task.update_typeset = true;
        }
        return 0;
}

void pd_text_update(pd_text_t *text, list_t *rects)
{
        if (text->task.update_bitmap) {
                pd_text_mark_dirty(text, 0, -1);
                pd_text_reload_bitmap(text);
                pd_text_mark_dirty(text, 0, -1);
                text->task.update_bitmap = false;
                text->task.redraw_all = true;
        }
        if (text->task.update_typeset) {
                pd_text_typeset(text, text->task.typeset_start_line);
                text->task.update_typeset = false;
                text->task.typeset_start_line = 0;
        }
        text->width = pd_text_get_width(text);
        /* 如果坐标偏移量有变化，记录各个文本行区域 */
        if (text->new_offset_x != text->offset_x ||
            text->new_offset_y != text->offset_y) {
                pd_text_mark_dirty(text, 0, -1);
                text->offset_x = text->new_offset_x;
                text->offset_y = text->new_offset_y;
                pd_text_mark_dirty(text, 0, -1);
                text->task.redraw_all = true;
        }
        if (rects) {
                list_concat(rects, &text->dirty_rects);
        }
}

bool pd_text_set_offset(pd_text_t *text, int offset_x, int offset_y)
{
        if (text->new_offset_x != offset_x || text->new_offset_y != offset_y) {
                text->new_offset_x = offset_x;
                text->new_offset_y = offset_y;
                return true;
        }
        return false;
}
