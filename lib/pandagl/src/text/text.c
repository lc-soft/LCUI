/*
 * lib/pandagl/src/text/text.c
 *
 * Copyright (c) 2018-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <wctype.h>
#include <pandagl.h>
#include <math.h>

typedef enum { PD_TEXT_ACTION_INSERT, PD_TEXT_ACTION_APPEND } pd_text_action_t;

#define DEFAULT_LINE_HEIGHT 1.42857143
#define isalpha(ch) (ch >= 'a' && ch <= 'z') || (ch >= 'a' && ch <= 'z')

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

static pd_text_line_t *pd_text_insert_line(pd_text_t *text, size_t line_num)
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

static int pd_text_delete_line(pd_text_t *text, int line_num)
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

PD_INLINE pd_text_line_t *pd_text_append_line(pd_text_t *text)
{
        return pd_text_insert_line(text, text->lines_length);
}

PD_INLINE pd_text_line_t *pd_text_get_line(pd_text_t *text, int line_num)
{
        return (line_num >= text->lines_length) ? NULL : text->lines[line_num];
}

static void pd_text_update_line_size(pd_text_t *text, pd_text_line_t *line)
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

static int pd_text_line_set_length(pd_text_line_t *line, int len)
{
        pd_char_t **txtstr;

        if (len < 0) {
                len = 0;
        }
        txtstr = realloc(line->string, sizeof(pd_char_t *) * (len + 1));
        if (!txtstr) {
                return -1;
        }
        txtstr[len] = NULL;
        line->string = txtstr;
        line->length = len;
        return 0;
}

static int pd_text_line_insert(pd_text_line_t *line, int offset,
                               pd_char_t *txtchar)
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

static int pd_text_line_insert_copy(pd_text_line_t *line, int ins_pos,
                                    pd_char_t *txtchar)
{
        pd_char_t *txtchar2;
        txtchar2 = malloc(sizeof(pd_char_t));
        *txtchar2 = *txtchar;
        return pd_text_line_insert(line, ins_pos, txtchar2);
}

static void pd_char_update_bitmap(pd_char_t *ch, pd_text_style_t *style)
{
        int i = 0;
        int size = style->pixel_size;
        int *font_ids = style->font_ids;

        if (ch->style) {
                if (ch->style->has_family) {
                        font_ids = ch->style->font_ids;
                }
                if (ch->style->has_pixel_size) {
                        size = ch->style->pixel_size;
                }
        }
        while (font_ids && font_ids[i] > 0) {
                int ret = pd_font_library_get_bitmap(ch->code, font_ids[i],
                                                     size, &ch->bitmap);
                if (ret == 0) {
                        return;
                }
                ++i;
        }
        pd_font_library_get_bitmap(ch->code, -1, size, &ch->bitmap);
}

pd_text_t *pd_text_create(void)
{
        pd_text_t *text;
        text = malloc(sizeof(pd_text_t));
        text->width = 0;
        text->length = 0;
        text->offset_x = 0;
        text->offset_y = 0;
        text->insert_x = 0;
        text->insert_y = 0;
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
        text->autowrap_enabled = PD_FALSE;
        text->mulitiline_enabled = PD_FALSE;
        text->style_tag_enabled = PD_FALSE;
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

static void pd_text_clear(pd_text_t *text)
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
        text->insert_x = 0;
        text->insert_y = 0;
        list_destroy(&text->styles, on_destroy_text_style);
        text->task.redraw_all = PD_TRUE;
}

void pd_text_empty(pd_text_t *text)
{
        pd_text_clear(text);
        pd_text_insert_line(text, 0);
}

void pd_text_destroy(pd_text_t *text)
{
        pd_text_clear(text);
        pd_rects_clear(&text->dirty_rects);
        pd_text_style_destroy(&text->default_style);
        free(text);
}

/* 根据对齐方式，计算文本行的起始x轴位置 */
static int pd_text_get_line_start_x(pd_text_t *text, pd_text_line_t *line)
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

void pd_text_set_typeset_task(pd_text_t *text, int start_line)
{
        if (start_line < text->task.typeset_start_line) {
                text->task.typeset_start_line = start_line;
        }
        text->task.update_typeset = PD_TRUE;
}

/** 获取指定文本行中的文本段的矩形区域 */
static int pd_text_get_line_rect(pd_text_t *text, int line_num, int start_col,
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

static void pd_text_mark_line_dirty(pd_text_t *text, int line_num, int start,
                                    int end)
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

void pd_text_set_insert_position(pd_text_t *text, int line_num, int col)
{
        if (line_num < 0) {
                line_num = 0;
        } else if (line_num >= text->lines_length) {
                if (text->lines_length < 0) {
                        line_num = 0;
                } else {
                        line_num = text->lines_length - 1;
                }
        }
        if (col < 0) {
                col = 0;
        } else if (text->lines_length > 0) {
                if (col >= text->lines[line_num]->length) {
                        col = text->lines[line_num]->length;
                }
        } else {
                col = 0;
        }
        text->insert_x = col;
        text->insert_y = line_num;
}

int pd_text_set_insert_pixel_position(pd_text_t *text, int x, int y)
{
        pd_text_line_t *line;
        int i, pixel_pos, ins_x, ins_y;

        pixel_pos = text->offset_y;
        for (i = 0; i < text->lines_length; ++i) {
                pixel_pos += text->lines[i]->height;
                if (pixel_pos >= y) {
                        ins_y = i;
                        break;
                }
        }
        if (i >= text->lines_length) {
                if (text->lines_length > 0) {
                        ins_y = text->lines_length - 1;
                } else {
                        text->insert_x = 0;
                        text->insert_y = 0;
                        return -1;
                }
        }
        line = text->lines[ins_y];
        ins_x = line->length;
        pixel_pos = text->offset_x;
        pixel_pos += pd_text_get_line_start_x(text, line);
        for (i = 0; i < line->length; ++i) {
                pd_char_t *txtchar;
                txtchar = line->string[i];
                if (!txtchar->bitmap) {
                        continue;
                }
                pixel_pos += txtchar->bitmap->metrics.hori_advance;
                /* 如果在当前字中心点的前面 */
                if (x <=
                    pixel_pos - txtchar->bitmap->metrics.hori_advance / 2) {
                        ins_x = i;
                        break;
                }
        }
        pd_text_set_insert_position(text, ins_y, ins_x);
        return 0;
}

/** 获取指定行列的文字的像素坐标 */
int pd_text_get_char_pixel_position(pd_text_t *text, int line_num, int col,
                                    pd_pos_t *pixel_pos)
{
        pd_text_line_t *line;
        int i, pixel_x = 0, pixel_y = 0;

        if (line_num < 0 || line_num >= text->lines_length) {
                return -1;
        }
        if (col < 0) {
                return -2;
        } else if (col > text->lines[line_num]->length) {
                return -3;
        }
        /* 累加前几行的高度 */
        for (i = 0; i < line_num; ++i) {
                pixel_y += text->lines[i]->height;
        }
        line = text->lines[line_num];
        pixel_x = pd_text_get_line_start_x(text, line);
        for (i = 0; i < col; ++i) {
                pd_char_t *txtchar = line->string[i];
                if (!txtchar || !txtchar->bitmap) {
                        continue;
                }
                pixel_x += txtchar->bitmap->metrics.hori_advance;
        }
        pixel_pos->x = pixel_x;
        pixel_pos->y = pixel_y;
        return 0;
}

int pd_text_get_insert_pixel_position(pd_text_t *text, pd_pos_t *pixel_pos)
{
        return pd_text_get_char_pixel_position(text, text->insert_y,
                                               text->insert_x, pixel_pos);
}

static void pd_text_break_line(pd_text_t *text, int line_num, int col,
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
        int i, j;
        pd_text_line_t *line = pd_text_get_line(text, line_num);
        pd_text_line_t *next = pd_text_get_line(text, line_num + 1);

        if (!line || !next) {
                return;
        }
        if (text->insert_y > line_num) {
                --text->insert_y;
                if (text->insert_y == line_num) {
                        text->insert_x += line->length;
                }
        }
        i = line->length;
        pd_text_line_set_length(line, line->length + next->length);
        for (j = 0; j < next->length; ++i, ++j) {
                line->string[i] = next->string[j];
                next->string[j] = NULL;
        }
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
        pd_bool_t autowrap =
            max_width > 0 && text->autowrap_enabled && text->mulitiline_enabled;

        for (col = 0; col < line->length; ++col) {
                txtchar = line->string[col];
                if (!txtchar->bitmap) {
                        continue;
                }
                /* 累加行宽度 */
                line_width += txtchar->bitmap->metrics.hori_advance;
                /* 如果是当前行的第一个字符，或者行宽度没有超过宽度限制 */
                if (!autowrap || col < 1 || line_width <= max_width) {
                        if (isalpha(txtchar->code)) {
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
static void pd_text_typeset(pd_text_t *text, int start_line)
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

static const wchar_t *pd_text_process_style_tags(pd_text_t *text,
                                                 const wchar_t *p, list_t *tags,
                                                 pd_text_style_t **style)
{
        pd_text_style_t *s;
        const wchar_t *pp;

        pp = pd_style_tags_next_close_tag(tags, p);
        if (pp) {
                s = pd_style_tags_get_text_style(tags);
                if (s) {
                        pd_text_style_merge(s, &text->default_style);
                        list_append(&text->styles, s);
                }
                *style = s;
                return pp;
        }
        pp = pd_style_tags_next_open_tag(tags, p);
        if (pp) {
                s = pd_style_tags_get_text_style(tags);
                if (s) {
                        pd_text_style_merge(s, &text->default_style);
                        list_append(&text->styles, s);
                }
                *style = s;
                return pp;
        }
        return NULL;
}

static int pd_text_process(pd_text_t *text, const wchar_t *wstr,
                           pd_text_action_t action, list_t *tags)
{
        pd_text_eol_t eol;
        pd_text_line_t *line;
        pd_char_t txtchar;
        list_t tmp_tags;
        const wchar_t *p;
        int cur_col, cur_line, start_line, ins_x, ins_y;
        pd_bool_t need_typeset, rect_has_added;
        pd_text_style_t *style = NULL;

        if (!wstr) {
                return -1;
        }
        need_typeset = PD_FALSE;
        rect_has_added = PD_FALSE;
        list_create(&tmp_tags);
        if (!tags) {
                tags = &tmp_tags;
        }
        /* 如果是将文本追加至文本末尾 */
        if (action == PD_TEXT_ACTION_APPEND) {
                if (text->lines_length > 0) {
                        cur_line = text->lines_length - 1;
                } else {
                        cur_line = 0;
                }
                line = pd_text_get_line(text, cur_line);
                if (!line) {
                        line = pd_text_append_line(text);
                }
                cur_col = line->length;
        } else { /* 否则，是将文本插入至当前插入点 */
                cur_line = text->insert_y;
                cur_col = text->insert_x;
                line = pd_text_get_line(text, cur_line);
                if (!line) {
                        line = pd_text_append_line(text);
                }
        }
        start_line = cur_line;
        ins_x = cur_col;
        ins_y = cur_line;
        for (p = wstr; *p; ++p) {
                if (text->style_tag_enabled) {
                        const wchar_t *pp;
                        pp = pd_text_process_style_tags(text, p, tags, &style);
                        if (pp) {
                                p = pp - 1;
                                continue;
                        }
                }
                if (*p == '\r' || *p == '\n') {
                        /* 判断是哪一种换行模式 */
                        if (*p == '\r') {
                                if (*(p + 1) == '\n') {
                                        eol = PD_TEXT_EOL_CR_LF;
                                } else {
                                        eol = PD_TEXT_EOL_CR;
                                }
                        } else {
                                eol = PD_TEXT_EOL_LF;
                        }
                        /* 如果没有记录过文本行的矩形区域 */
                        if (!rect_has_added) {
                                pd_text_mark_dirty(text, ins_y, -1);
                                rect_has_added = PD_TRUE;
                                start_line = ins_y;
                        }
                        /* 将当前行中的插入点为截点，进行断行 */
                        pd_text_break_line(text, ins_y, ins_x, eol);
                        text->width = y_max(text->width, line->width);
                        need_typeset = PD_TRUE;
                        ++text->length;
                        ins_x = 0;
                        ++ins_y;
                        line = pd_text_get_line(text, ins_y);
                        continue;
                }
                txtchar.style = style;
                txtchar.code = *p;
                pd_char_update_bitmap(&txtchar, &text->default_style);
                pd_text_line_insert_copy(line, ins_x, &txtchar);
                ++text->length;
                ++ins_x;
        }
        /* 更新当前行的尺寸 */
        pd_text_update_line_size(text, line);
        text->width = y_max(text->width, line->width);
        if (action == PD_TEXT_ACTION_INSERT) {
                text->insert_x = ins_x;
                text->insert_y = ins_y;
        }
        /* 若启用了自动换行模式，则标记需要重新对文本进行排版 */
        if (text->autowrap_enabled || need_typeset) {
                pd_text_set_typeset_task(text, cur_line);
        } else {
                pd_text_mark_line_dirty(text, cur_line, 0, -1);
        }
        /* 如果已经记录过文本行矩形区域 */
        if (rect_has_added) {
                pd_text_mark_dirty(text, start_line, -1);
                rect_has_added = PD_TRUE;
        }
        pd_style_tags_clear(&tmp_tags);
        return 0;
}

int pd_text_insert(pd_text_t *text, const wchar_t *wstr, list_t *tags)
{
        return pd_text_process(text, wstr, PD_TEXT_ACTION_INSERT, tags);
}

int pd_text_append(pd_text_t *text, const wchar_t *wstr, list_t *tag_stack)
{
        return pd_text_process(text, wstr, PD_TEXT_ACTION_APPEND, tag_stack);
}

int pd_text_write(pd_text_t *text, const wchar_t *wstr, list_t *tag_stack)
{
        pd_text_empty(text);
        return pd_text_append(text, wstr, tag_stack);
}

size_t pd_text_dump(pd_text_t *text, size_t start_pos, size_t max_len,
                    wchar_t *wstr_buff)
{
        size_t i, line_num, col;
        pd_text_line_t *line;

        if (max_len == 0) {
                wstr_buff[0] = 0;
                return 0;
        }
        /* 先根据一维坐标计算行列坐标 */
        for (i = 0, line_num = 0, col = 0; line_num < text->lines_length;
             ++line_num) {
                if (i >= start_pos) {
                        col = start_pos - i;
                        break;
                }
                i += text->lines[line_num]->length;
        }
        for (i = 0; line_num < text->lines_length && i < max_len; ++line_num) {
                line = text->lines[line_num];
                for (; col < line->length && i < max_len; ++col, ++i) {
                        wstr_buff[i] = line->string[col]->code;
                }
        }
        wstr_buff[i] = 0;
        return i;
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
        text->task.redraw_all = PD_TRUE;
        if (text->autowrap_enabled) {
                text->task.typeset_start_line = 0;
                text->task.update_typeset = PD_TRUE;
        }
        return 0;
}

int pd_text_set_max_size(pd_text_t *text, int width, int height)
{
        text->max_width = width;
        text->max_height = height;
        text->task.redraw_all = PD_TRUE;
        if (text->autowrap_enabled) {
                text->task.typeset_start_line = 0;
                text->task.update_typeset = PD_TRUE;
        }
        return 0;
}

/** 设置是否启用多行文本模式 */
void pd_text_set_multiline(pd_text_t *text, pd_bool_t enabled)
{
        if (text->mulitiline_enabled != enabled) {
                text->mulitiline_enabled = enabled;
                pd_text_set_typeset_task(text, 0);
        }
}

/** 删除指定行列的文字及其右边的文本 */
static int pd_text_delete_ex(pd_text_t *text, int char_y, int char_x,
                             int n_char)
{
        int end_x, end_y, i, j, len;
        pd_text_line_t *line, *end_line, *prev_line;

        if (char_x < 0) {
                char_x = 0;
        }
        if (char_y < 0) {
                char_y = 0;
        }
        if (n_char <= 0) {
                return -1;
        }
        if (char_y >= text->lines_length) {
                return -2;
        }
        line = text->lines[char_y];
        if (char_x > line->length) {
                char_x = line->length;
        }
        i = n_char;
        end_x = char_x;
        end_y = char_y;
        /* 计算结束点的位置 */
        for (; end_y < text->lines_length && n_char > 0; ++end_y) {
                line = text->lines[end_y];
                if (end_x + n_char <= line->length) {
                        end_x += n_char;
                        n_char = 0;
                        break;
                }
                n_char -= (line->length - end_x);
                if (line->eol == PD_TEXT_EOL_NONE) {
                        end_x = 0;
                } else {
                        n_char -= 1;
                        end_x = 0;
                }
        }
        if (n_char >= 0) {
                text->length -= i - n_char;
        } else {
                text->length -= n_char;
        }
        if (end_y >= text->lines_length) {
                end_y = text->lines_length - 1;
                end_line = text->lines[end_y];
                end_x = end_line->length;
        } else {
                end_line = text->lines[end_y];
        }
        if (end_x > end_line->length) {
                end_x = end_line->length;
        }
        if (end_x == char_x && end_y == char_y) {
                return 0;
        }
        /* 获取上一行文本 */
        prev_line = text->lines[char_y - 1];
        // 计算起始行与结束行拼接后的长度
        // 起始行：0 1 2 3 4 5，起点位置：2
        // 结束行：0 1 2 3 4 5，终点位置：4
        // 拼接后的长度：2 + 6 - 4 = 4
        len = char_x + end_line->length - end_x;
        if (len < 0) {
                return -3;
        }
        /* 如果是同一行 */
        if (line == end_line) {
                if (end_x > end_line->length) {
                        return -4;
                }
                pd_text_mark_line_dirty(text, char_y, char_x, -1);
                pd_text_set_typeset_task(text, char_y);
                for (i = char_x, j = end_x; j < line->length; ++i, ++j) {
                        line->string[i] = line->string[j];
                }
                /* 如果当前行为空，也不是第一行，并且上一行没有结束符 */
                if (len <= 0 && end_y > 0 &&
                    prev_line->eol != PD_TEXT_EOL_NONE) {
                        pd_text_delete_line(text, end_y);
                }
                /* 调整起始行的容量 */
                pd_text_line_set_length(line, len);
                /* 更新文本行的尺寸 */
                pd_text_update_line_size(text, line);
                return 0;
        }
        /* 如果结束点在行尾，并且该行不是最后一行 */
        if (end_x == end_line->length && end_y < text->lines_length - 1) {
                ++end_y;
                end_line = pd_text_get_line(text, end_y);
                end_x = -1;
                len = char_x + end_line->length;
        }
        pd_text_line_set_length(line, len);
        /* 标记当前行后面的所有行的矩形需区域需要刷新 */
        pd_text_mark_dirty(text, char_y + 1, -1);
        /* 移除起始行与结束行之间的文本行 */
        for (i = char_y + 1, j = i; j < end_y; ++j) {
                pd_text_mark_line_dirty(text, i, 0, -1);
                pd_text_delete_line(text, i);
        }
        i = char_x;
        j = end_x + 1;
        end_y = char_y + 1;
        /* 将结束行的内容拼接至起始行 */
        for (; i < len && j < end_line->length; ++i, ++j) {
                line->string[i] = end_line->string[j];
        }
        pd_text_update_line_size(text, line);
        pd_text_mark_line_dirty(text, end_y, 0, -1);
        /* 移除结束行 */
        pd_text_delete_line(text, end_y);
        /* 如果起始行无内容，并且上一行没有结束符（换行符），则
         * 说明需要删除起始行 */
        if (len <= 0 && char_y > 0 && prev_line->eol != PD_TEXT_EOL_NONE) {
                pd_text_mark_line_dirty(text, char_y, 0, -1);
                pd_text_delete_line(text, char_y);
        }
        pd_text_set_typeset_task(text, char_y);
        return 0;
}

/** 删除文本光标的当前坐标右边的文本 */
int pd_text_delete(pd_text_t *text, int n_char)
{
        return pd_text_delete_ex(text, text->insert_y, text->insert_x, n_char);
}

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
int pd_text_backspace(pd_text_t *text, int n_char)
{
        int n_del;
        int char_x, char_y;
        pd_text_line_t *line;

        /* 先获取当前字的位置 */
        char_x = text->insert_x;
        char_y = text->insert_y;
        /* 再计算删除 n_char 个字后的位置 */
        for (n_del = n_char; char_y >= 0; --char_y) {
                line = text->lines[char_y];
                /* 如果不是当前行，则重定位至行尾 */
                if (char_y < text->insert_y) {
                        char_x = line->length;
                        if (line->eol == PD_TEXT_EOL_NONE) {
                                --char_x;
                        }
                }
                if (char_x >= n_del) {
                        char_x = char_x - n_del;
                        n_del = 0;
                        break;
                }
                n_del = n_del - char_x - 1;
        }
        if (char_y < 0 || n_del == n_char) {
                return -1;
        }
        /* 若能够被删除的字不够 n_char 个，则调整需删除的字数 */
        if (n_del > 0) {
                n_char -= n_del;
        }
        /* 开始删除文本 */
        pd_text_delete_ex(text, char_y, char_x, n_char);
        /* 若最后一行被完全移除，则移动输入点至上一行的行尾处 */
        if (char_x == 0 && text->lines_length > 0 &&
            char_y >= text->lines_length) {
                char_y = text->lines_length - 1;
                char_x = text->lines[char_y]->length;
        }
        /* 更新文本光标的位置 */
        pd_text_set_insert_position(text, char_y, char_x);
        return 0;
}

void pd_text_set_autowrap(pd_text_t *text, pd_bool_t autowrap)
{
        if (text->autowrap_enabled != autowrap) {
                text->autowrap_enabled = autowrap;
                pd_text_set_typeset_task(text, 0);
        }
}

void pd_text_set_word_break(pd_text_t *text, pd_word_break_t word_break)
{
        if (text->word_break != word_break) {
                text->word_break = word_break;
                pd_text_set_typeset_task(text, 0);
        }
}

/** 设置是否使用样式标签 */
void pd_text_set_style_tag(pd_text_t *text, pd_bool_t enabled)
{
        text->style_tag_enabled = enabled;
}

static void pd_text_update_style_cache(pd_text_t *text)
{
        list_node_t *node;
        if (!text->default_style.has_family) {
                pd_text_style_set_default_font(&text->default_style);
        }
        /* 替换缺省字体，确保能够正确应用字体设置 */
        for (list_each(node, &text->styles)) {
                pd_text_style_merge(node->data, &text->default_style);
        }
}

/** 重新载入各个文字的字体位图 */
void pd_text_reload_bitmap(pd_text_t *text)
{
        int line_num, col;
        pd_text_update_style_cache(text);
        for (line_num = 0; line_num < text->lines_length; ++line_num) {
                pd_text_line_t *line = text->lines[line_num];
                for (col = 0; col < line->length; ++col) {
                        pd_char_t *txtchar = line->string[col];
                        pd_char_update_bitmap(txtchar, &text->default_style);
                }
                pd_text_update_line_size(text, line);
        }
}

void pd_text_update(pd_text_t *text, list_t *rects)
{
        if (text->task.update_bitmap) {
                pd_text_mark_dirty(text, 0, -1);
                pd_text_reload_bitmap(text);
                pd_text_mark_dirty(text, 0, -1);
                text->task.update_bitmap = PD_FALSE;
                text->task.redraw_all = PD_TRUE;
        }
        if (text->task.update_typeset) {
                pd_text_typeset(text, text->task.typeset_start_line);
                text->task.update_typeset = PD_FALSE;
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
                text->task.redraw_all = PD_TRUE;
        }
        if (rects) {
                list_concat(rects, &text->dirty_rects);
        }
}

static void pd_text_validate_rect(pd_text_t *text, pd_rect_t *area)
{
        int width, height;

        if (text->fixed_width > 0) {
                width = text->fixed_width;
        } else if (text->max_width > 0) {
                width = text->max_width;
        } else {
                width = text->width;
        }
        if (text->fixed_height > 0) {
                height = text->fixed_height;
        } else {
                height = pd_text_get_height(text);
        }
        pd_rect_correct(area, width, height);
}

static void pd_text_render_char(pd_text_t *text, pd_char_t *ch,
                                pd_canvas_t *graph, pd_pos_t ch_pos)
{
        /* 判断文字使用的前景颜色，再进行绘制 */
        if (ch->style && ch->style->has_fore_color) {
                pd_canvas_mix_font_bitmap(graph, ch_pos, ch->bitmap,
                                          ch->style->fore_color);
        } else {
                pd_canvas_mix_font_bitmap(graph, ch_pos, ch->bitmap,
                                          text->default_style.fore_color);
        }
}

static void pd_text_render_line(pd_text_t *text, pd_rect_t *area,
                                pd_canvas_t *graph, pd_pos_t layer_pos,
                                pd_text_line_t *line, int y)
{
        pd_char_t *ch;
        pd_pos_t pen;
        int col, x;

        x = pd_text_get_line_start_x(text, line) + text->offset_x;
        for (col = 0; col < line->length && x < area->x + area->width; ++col) {
                ch = line->string[col];
                if (!ch->bitmap) {
                        continue;
                }
                if (x + ch->bitmap->metrics.hori_advance < area->x) {
                        continue;
                }
                pen.x = layer_pos.x + x;
                pen.y = layer_pos.y + y;
                if (ch->style && ch->style->has_back_color) {
                        pd_rect_t rect;
                        rect.x = pen.x;
                        rect.y = pen.y;
                        rect.height = line->height;
                        rect.width = ch->bitmap->metrics.hori_advance;
                        pd_canvas_fill_rect(graph, ch->style->back_color, rect);
                }
                pen.x += ch->bitmap->left;
                pen.y += (line->height - ch->bitmap->metrics.bbox_height) /
                2 + ch->bitmap->metrics.ascender - ch->bitmap->top;
                pd_text_render_char(text, ch, graph, pen);
                x += ch->bitmap->metrics.hori_advance;
        }
}

int pd_text_render_to(pd_text_t *text, pd_rect_t area, pd_pos_t layer_pos,
                      pd_canvas_t *canvas)
{
        int y;
        int line_num;
        pd_text_line_t *line;

        y = text->offset_y;
        /* 确定可绘制的最大区域范围 */
        pd_text_validate_rect(text, &area);
        for (line_num = 0; line_num < text->lines_length; ++line_num) {
                line = pd_text_get_line(text, line_num);
                y += line->height;
                if (y > area.y) {
                        y -= line->height;
                        break;
                }
        }
        /* 如果没有可绘制的文本行 */
        if (line_num >= text->lines_length) {
                return -1;
        }
        for (; line_num < text->lines_length; ++line_num) {
                line = pd_text_get_line(text, line_num);
                pd_text_render_line(text, &area, canvas, layer_pos, line, y);
                y += line->height;
                /* 超出绘制区域范围就不绘制了 */
                if (y > area.y + area.height) {
                        break;
                }
        }
        return 0;
}

/** 设置全局文本样式 */
void pd_text_set_style(pd_text_t *text, pd_text_style_t *style)
{
        pd_text_style_destroy(&text->default_style);
        pd_text_style_copy(&text->default_style, style);
        text->task.update_bitmap = PD_TRUE;
}

void pd_text_set_align(pd_text_t *text, int align)
{
        text->text_align = align;
        text->task.update_typeset = PD_TRUE;
        text->task.typeset_start_line = 0;
}

void pd_text_set_line_height(pd_text_t *text, int height)
{
        text->line_height = height;
        text->task.update_typeset = PD_TRUE;
        text->task.typeset_start_line = 0;
}

pd_bool_t pd_text_set_offset(pd_text_t *text, int offset_x, int offset_y)
{
        if (text->new_offset_x != offset_x || text->new_offset_y != offset_y) {
                text->new_offset_x = offset_x;
                text->new_offset_y = offset_y;
                return PD_TRUE;
        }
        return PD_FALSE;
}
