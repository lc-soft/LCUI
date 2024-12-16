/*
 * lib/ui/include/ui/text_style.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_TEXT_STYLE_H
#define LIB_UI_INCLUDE_UI_TEXT_STYLE_H

#include "common.h"
#include "types.h"

#include <ui/metrics.h>

LIBUI_BEGIN_DECLS

#define UI_DEFAULT_FONT_SIZE 14
#define UI_DEFAULT_FONT_COLOR 0xff333333
#define UI_MIN_FONT_SIZE 12
#define UI_LINE_HEIGHT_SCALE 1.42857143

typedef struct ui_text_style {
	int font_size;
	int line_height;
	int *font_ids;
	char *font_family;
	wchar_t *content;
	pd_color_t color;
	uint8_t font_style;
	uint8_t font_weight;
	uint8_t text_align;
	uint8_t white_space;
	uint8_t word_break;
} ui_text_style_t;

LIBUI_PUBLIC void ui_text_style_init(ui_text_style_t *fs);

LIBUI_PUBLIC void ui_text_style_destroy(ui_text_style_t *fs);

LIBUI_PUBLIC bool ui_text_style_is_equal(const ui_text_style_t *a,
					 const ui_text_style_t *b);

LIBUI_PUBLIC void ui_compute_text_style(ui_text_style_t *fs,
					const css_computed_style_t *ss);

LIBUI_END_DECLS

#endif
