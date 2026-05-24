/*
 * lib/pandagl/src/text/text_render.c
 *
 * Copyright (c) 2018-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <pandagl.h>
#include "text_internal.h"

void pd_char_update_bitmap(pd_char_t *ch, pd_text_style_t *style)
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
                int ret = pd_font_cache_get_bitmap(ch->code, font_ids[i],
                                                     size, &ch->bitmap);
                if (ret == 0) {
                        return;
                }
                ++i;
        }
        pd_font_cache_get_bitmap(ch->code, -1, size, &ch->bitmap);
}

void pd_text_update_style_cache(pd_text_t *text)
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
                pd_canvas_mix_glyph_bitmap(graph, ch_pos, ch->bitmap,
                                          ch->style->fore_color);
        } else {
                pd_canvas_mix_glyph_bitmap(graph, ch_pos, ch->bitmap,
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
                        x += ch->bitmap->metrics.hori_advance;
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
                pen.y += (line->height - ch->bitmap->metrics.bbox_height) / 2 +
                         ch->bitmap->metrics.ascender - ch->bitmap->top;
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
        text->task.update_bitmap = true;
}
