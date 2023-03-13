/*
 * css_fontstyle.c -- CSS font style parse and operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdlib.h>
#include <css.h>
#include <LCUI/ui.h>
#include <pandagl.h>

static size_t unescape(const wchar_t *instr, wchar_t *outstr)
{
	int i = -1;
	wchar_t buff[6];
	wchar_t *pout = outstr;
	const wchar_t *pin = instr;

	for (; *pin; ++pin) {
		if (i >= 0) {
			buff[i++] = *pin;
			if (i >= 4) {
				buff[i] = 0;
				swscanf(buff, L"%hx", pout);
				++pout;
				i = -1;
			}
			continue;
		}
		if (*pin == L'\\') {
			i = 0;
			continue;
		}
		*pout++ = *pin;
	}
	if (i >= 4) {
		buff[i] = 0;
		swscanf(buff, L"%hx", pout);
		++pout;
	}
	*pout = 0;
	return pout - outstr;
}

static void ui_compute_content(ui_text_style_t *fs, const char *str)
{
	size_t i;
	size_t len;
	wchar_t *content;

	if (fs->content) {
		free(fs->content);
		fs->content = NULL;
	}
	if (!str) {
		return;
	}
	len = decode_utf8(NULL, str, 0);
	content = malloc((len + 1) * sizeof(wchar_t));
	len = decode_utf8(content, str, len);
	content[len] = 0;
	if (content[0] == '"') {
		for (i = 0; content[i + 1]; ++i) {
			content[i] = content[i + 1];
		}
		if (i < 1 || content[i - 1] != '"') {
			free(content);
			return;
		}
		content[i - 1] = 0;
	}
	unescape(content, content);
	fs->content = content;
}

void ui_text_style_init(ui_text_style_t *fs)
{
	fs->color.value = 0;
	fs->content = NULL;
	fs->font_ids = NULL;
	fs->font_style = PD_FONT_STYLE_NORMAL;
	fs->font_weight = PD_FONT_WEIGHT_NORMAL;
	fs->word_break = PD_WORD_BREAK_NORMAL;
}

void ui_text_style_destroy(ui_text_style_t *fs)
{
	if (fs->font_ids) {
		free(fs->font_ids);
		fs->font_ids = NULL;
	}
	if (fs->content) {
		free(fs->content);
		fs->content = NULL;
	}
}

LCUI_BOOL ui_text_style_is_equal(const ui_text_style_t *a,
				 const ui_text_style_t *b)
{
	int i;

	if (a->color.value != b->color.value ||
	    a->line_height != b->line_height ||
	    a->text_align != b->text_align ||
	    a->white_space != b->white_space ||
	    a->font_style != b->font_style ||
	    a->font_weight != b->font_weight || a->font_size != b->font_size) {
		return FALSE;
	}
	if (a->content && b->content) {
		if (wcscmp(a->content, b->content) != 0) {
			return FALSE;
		}
	} else if (a->content != b->content) {
		return FALSE;
	}
	if (a->font_ids && b->font_ids) {
		for (i = 0; a->font_ids[i] && b->font_ids[i]; ++i) {
			if (a->font_ids[i] != b->font_ids[i]) {
				return FALSE;
			}
		}
	} else if (a->font_ids != b->font_ids) {
		return FALSE;
	}
	return TRUE;
}

void ui_compute_text_style(ui_text_style_t *fs,
			   const css_computed_style_t *style)
{
	fs->color.value = style->color;
	fs->white_space = style->type_bits.white_space;
	ui_compute_content(fs, style->content);

	switch (style->type_bits.font_size) {
	case CSS_FONT_SIZE_DIMENSION:
		fs->font_size = ui_compute(style->font_size);
		break;
	case CSS_FONT_SIZE_MEDIUM:
	default:
		fs->font_size = ui_compute(UI_DEFAULT_FONT_SIZE);
		break;
	}

	switch (style->type_bits.line_height) {
	case CSS_LINE_HEIGHT_NUMBER:
		fs->line_height =
		    ui_compute(fs->font_size * style->line_height);
		break;
	case CSS_LINE_HEIGHT_DIMENSION:
		fs->line_height = ui_compute(style->line_height);
		break;
	case CSS_LINE_HEIGHT_NORMAL:
	default:
		fs->line_height = ui_compute(fs->font_size * 1.42857143f);
		break;
	}

	switch (style->type_bits.text_align) {
	case CSS_TEXT_ALIGN_CENTER:
		fs->text_align = PD_TEXT_ALIGN_CENTER;
		break;
	case CSS_TEXT_ALIGN_RIGHT:
		fs->text_align = PD_TEXT_ALIGN_RIGHT;
		break;
	default:
		fs->text_align = PD_TEXT_ALIGN_LEFT;
		break;
	}

	switch (style->type_bits.font_style) {
	case CSS_FONT_STYLE_ITALIC:
		fs->font_style = PD_FONT_STYLE_ITALIC;
		break;
	case CSS_FONT_STYLE_OBLIQUE:
		fs->font_style = PD_FONT_STYLE_OBLIQUE;
		break;
	default:
		fs->font_style = PD_FONT_STYLE_NORMAL;
		break;
	}

	switch (style->type_bits.font_weight) {
	case CSS_FONT_WEIGHT_100:
		fs->font_weight = PD_FONT_WEIGHT_THIN;
		break;
	case CSS_FONT_WEIGHT_200:
		fs->font_weight = PD_FONT_WEIGHT_EXTRA_LIGHT;
		break;
	case CSS_FONT_WEIGHT_300:
		fs->font_weight = PD_FONT_WEIGHT_LIGHT;
		break;
	case CSS_FONT_WEIGHT_500:
		fs->font_weight = PD_FONT_WEIGHT_MEDIUM;
		break;
	case CSS_FONT_WEIGHT_600:
		fs->font_weight = PD_FONT_WEIGHT_SEMI_BOLD;
		break;
	case CSS_FONT_WEIGHT_BOLD:
	case CSS_FONT_WEIGHT_700:
		fs->font_weight = PD_FONT_WEIGHT_BOLD;
		break;
	case CSS_FONT_WEIGHT_800:
		fs->font_weight = PD_FONT_WEIGHT_EXTRA_BOLD;
		break;
	case CSS_FONT_WEIGHT_900:
		fs->font_weight = PD_FONT_WEIGHT_BLACK;
		break;
	case CSS_FONT_WEIGHT_NORMAL:
	case CSS_FONT_WEIGHT_400:
	default:
		fs->font_weight = PD_FONT_WEIGHT_NORMAL;
		break;
	}
	if (fs->font_ids) {
		free(fs->font_ids);
		fs->font_ids = NULL;
	}
	if (style->font_family) {
		pd_font_library_query(&fs->font_ids, fs->font_style, fs->font_weight,
			      (const char *const *)style->font_family);
	}
}
