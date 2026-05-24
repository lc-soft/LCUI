/*
 * lib/pandagl/src/text/text_internal.h
 *
 * Copyright (c) 2018-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 *
 * Library-private helpers shared between text.c, text_typeset.c,
 * text_render.c and text_edit.c. Not installed into the public include
 * directory.
 */

#ifndef LIB_PANDAGL_SRC_TEXT_TEXT_INTERNAL_H
#define LIB_PANDAGL_SRC_TEXT_TEXT_INTERNAL_H

#include <pandagl.h>

/* line storage */
pd_text_line_t *pd_text_get_line(pd_text_t *text, int line_num);
pd_text_line_t *pd_text_insert_line(pd_text_t *text, size_t line_num);
pd_text_line_t *pd_text_append_line(pd_text_t *text);
int pd_text_delete_line(pd_text_t *text, int line_num);

/* line-level helpers */
int pd_text_line_set_length(pd_text_line_t *line, int len);
int pd_text_line_insert(pd_text_line_t *line, int offset,
                        pd_char_t *txtchar);
int pd_text_line_insert_copy(pd_text_line_t *line, int ins_pos,
                             pd_char_t *txtchar);
void pd_text_line_erase(pd_text_line_t *line, int start, int end);
void pd_text_line_move(pd_text_line_t *dst, int dst_start,
                       pd_text_line_t *src, int src_start, int src_end);

/* line geometry */
void pd_text_update_line_size(pd_text_t *text, pd_text_line_t *line);
int pd_text_get_line_start_x(pd_text_t *text, pd_text_line_t *line);
int pd_text_get_line_rect(pd_text_t *text, int line_num, int start_col,
                          int end_col, pd_rect_t *rect);
void pd_text_mark_line_dirty(pd_text_t *text, int line_num, int start,
                             int end);

/* render helpers */
void pd_char_update_bitmap(pd_char_t *ch, pd_text_style_t *style);
void pd_text_update_style_cache(pd_text_t *text);

/* typeset entry called by core/edit */
void pd_text_typeset(pd_text_t *text, int start_line);
void pd_text_break_line(pd_text_t *text, int line_num, int col,
                        pd_text_eol_t eol);

#endif
