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
#include <LCUI/util.h>
#include <LCUI/font.h>
#include <LCUI/css.h>
#include <LCUI/ui.h>
#include <LCUI/gui/css_fontstyle.h>

/* clang-format off */

#define DEFAULT_FONT_SIZE	14
#define DEFAULT_COLOR		0xff333333
#define MIN_FONT_SIZE		12
#define LINE_HEIGHT_SCALE	1.42857143

/* clang-format on */

#define GetFontStyleKey(CTX) css_font_style.keys[(CTX)->prop_key]
#define SetFontStyleProperty(CTX, S) \
	css_style_parser_set_property(CTX, GetFontStyleKey(CTX), S);

enum FontStyleType { FS_NORMAL, FS_ITALIC, FS_OBLIQUE };

typedef void (*StyleHandler)(LCUI_CSSFontStyle, css_unit_value_t*);

static struct LCUI_CSSFontStyleModule {
	int keys[TOTAL_FONT_STYLE_KEY];
	StyleHandler handlers[TOTAL_FONT_STYLE_KEY];
} css_font_style;

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

static int OnParseContent(css_style_parser_t *ctx, const char *str)
{
	size_t len;
	css_unit_value_t s;

	len = strlen(str);
	if (len < 1 || (str[0] == '"' && str[len - 1] != '"')) {
		return -1;
	}
	s.is_valid = TRUE;
	s.type = CSS_UNIT_STRING;
	s.val_string = malloc(sizeof(char) * (len + 1));
	if (!s.val_string) {
		return -1;
	}
	strcpy(s.val_string, str);
	SetFontStyleProperty(ctx, &s);
	return 0;
}

static int css_parse_color_value(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	if (css_parse_color(&s, str)) {
		SetFontStyleProperty(ctx, &s);
		return 0;
	}
	return -1;
}

static int OnParseFontSize(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	if (css_parse_number(&s, str)) {
		SetFontStyleProperty(ctx, &s);
		return 0;
	}
	return -1;
}

static int OnParseFontFamily(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	s.is_valid = TRUE;
	s.type = CSS_UNIT_STRING;
	s.val_string = strdup2(str);
	SetFontStyleProperty(ctx, &s);
	return 0;
}

static int OnParseFontStyle(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	if (css_parse_font_style(str, &s.val_int)) {
		s.is_valid = TRUE;
		s.type = CSS_UNIT_INT;
		SetFontStyleProperty(ctx, &s);
		return 0;
	}
	return -1;
}

static int OnParseFontWeight(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	if (css_parse_font_weight(str, &s.val_int)) {
		s.is_valid = TRUE;
		s.type = CSS_UNIT_INT;
		SetFontStyleProperty(ctx, &s);
		return 0;
	}
	return -1;
}

static int OnParseTextAlign(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	s.is_valid = TRUE;
	s.type = CSS_UNIT_STYLE;
	s.val_style = css_get_keyword_key(str);

	if (s.val_style < 0) {
		return -1;
	}
	SetFontStyleProperty(ctx, &s);
	return 0;
}

static int OnParseLineHeight(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	if (!css_parse_number(&s, str)) {
		return -1;
	}
	SetFontStyleProperty(ctx, &s);
	return 0;
}

static int css_parse_keyword_value(css_style_parser_t *ctx, const char *str)
{
	css_unit_value_t s;

	s.is_valid = TRUE;
	s.type = CSS_UNIT_STYLE;
	s.val_style = css_get_keyword_key(str);
	if (s.val_style < 0) {
		return -1;
	}
	SetFontStyleProperty(ctx, &s);
	return 0;
}

static void OnComputeFontSize(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid) {
		fs->font_size =
		    ui_compute_actual(y_max(MIN_FONT_SIZE, s->value), s->type);
		return;
	}
	fs->font_size = ui_compute_actual(DEFAULT_FONT_SIZE, CSS_UNIT_PX);
}

static void OnComputeColor(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid) {
		fs->color = s->color;
	} else {
		fs->color.value = DEFAULT_COLOR;
	}
}

static void OnComputeFontFamily(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (fs->font_ids) {
		free(fs->font_ids);
		fs->font_ids = NULL;
	}
	if (fs->font_family) {
		free(fs->font_family);
		fs->font_family = NULL;
	}
	if (!s->is_valid) {
		return;
	}
	fs->font_family = strdup2(s->string);
	LCUIFont_GetIdByNames(&fs->font_ids, fs->font_style, fs->font_weight,
			      fs->font_family);
}

static void OnComputeFontStyle(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid) {
		fs->font_style = s->val_int;
	} else {
		fs->font_style = FONT_STYLE_NORMAL;
	}
}

static void OnComputeFontWeight(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid) {
		fs->font_weight = s->val_int;
	} else {
		fs->font_weight = FONT_WEIGHT_NORMAL;
	}
}

static void OnComputeTextAlign(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid) {
		fs->text_align = s->val_style;
	} else {
		fs->text_align = CSS_KEYWORD_LEFT;
	}
}

static void OnComputeLineHeight(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	int h;
	if (s->is_valid) {
		if (s->type == CSS_UNIT_INT) {
			h = y_iround(fs->font_size * s->val_int);
		} else if (s->type == CSS_UNIT_SCALE) {
			h = y_iround(fs->font_size * s->val_scale);
		} else {
			h = ui_compute_actual(s->value, s->type);
		}
	} else {
		h = y_iround(fs->font_size * LINE_HEIGHT_SCALE);
	}
	fs->line_height = h;
}

static void OnComputeContent(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	size_t i;
	size_t len;
	wchar_t *content;

	if (fs->content) {
		free(fs->content);
		fs->content = NULL;
	}
	if (!s->is_valid) {
		return;
	}

	len = decode_utf8(NULL, s->val_string, 0);
	content = malloc((len + 1) * sizeof(wchar_t));
	len = decode_utf8(content, s->val_string, len);
	content[len] = 0;
	if (content[0] == '"') {
		for (i = 0; content[i + 1]; ++i) {
			content[i] = content[i + 1];
		}
		if (content[i - 1] != '"') {
			free(content);
			return;
		}
		content[i - 1] = 0;
	}
	unescape(content, content);
	fs->content = content;
}

static void OnComputeWhiteSpace(LCUI_CSSFontStyle fs, css_unit_value_t *s)
{
	if (s->is_valid && s->type == CSS_UNIT_STYLE) {
		fs->white_space = s->val_style;
	} else {
		fs->white_space = CSS_KEYWORD_AUTO;
	}
}

void CSSFontStyle_Init(LCUI_CSSFontStyle fs)
{
	fs->color.value = 0;
	fs->content = NULL;
	fs->font_ids = NULL;
	fs->font_family = NULL;
	fs->font_style = FONT_STYLE_NORMAL;
	fs->font_weight = FONT_WEIGHT_NORMAL;
}

void CSSFontStyle_Destroy(LCUI_CSSFontStyle fs)
{
	if (fs->font_ids) {
		free(fs->font_ids);
		fs->font_ids = NULL;
	}
	if (fs->font_family) {
		free(fs->font_family);
		fs->font_family = NULL;
	}
	if (fs->content) {
		free(fs->content);
		fs->content = NULL;
	}
}

int LCUI_GetFontStyleKey(int key)
{
	return css_font_style.keys[key];
}

LCUI_BOOL CSSFontStyle_IsEquals(const LCUI_CSSFontStyle a,
				const LCUI_CSSFontStyle b)
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
	if (a->font_family && b->font_family) {
		if (strcmp(a->font_family, b->font_family) != 0) {
			return FALSE;
		}
	} else if (a->font_family != b->font_family) {
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

void CSSFontStyle_Compute(LCUI_CSSFontStyle fs, css_style_decl_t *ss)
{
	int i;
	for (i = 0; i < TOTAL_FONT_STYLE_KEY; ++i) {
		if (css_font_style.keys[i] < 0) {
			continue;
		}
		css_font_style.handlers[i](fs, &ss->sheet[css_font_style.keys[i]]);
	}
}

void CSSFontStyle_GetTextStyle(LCUI_CSSFontStyle fs, LCUI_TextStyle ts)
{
	size_t len;
	ts->font_ids = NULL;
	ts->has_style = TRUE;
	ts->has_weight = TRUE;
	ts->has_family = FALSE;
	ts->has_back_color = FALSE;
	ts->has_pixel_size = TRUE;
	ts->has_fore_color = TRUE;
	ts->fore_color = fs->color;
	ts->pixel_size = fs->font_size;
	ts->weight = fs->font_weight;
	ts->style = fs->font_style;
	if (fs->font_ids) {
		for (len = 0; fs->font_ids[len]; ++len)
			;
		ts->font_ids = malloc(sizeof(int) * ++len);
		memcpy(ts->font_ids, fs->font_ids, len * sizeof(int));
		ts->has_family = TRUE;
	}
}

void LCUI_InitCSSFontStyle(void)
{
	int i;
	css_property_parser_t prop_parsers[] = {
		{ css_key_color, "color", css_parse_color_value },
		{ css_key_font_family, "font-family", OnParseFontFamily },
		{ css_key_font_size, "font-size", OnParseFontSize },
		{ css_key_font_style, "font-style", OnParseFontStyle },
		{ css_key_font_weight, "font-weight", OnParseFontWeight },
		{ css_key_text_align, "text-align", OnParseTextAlign },
		{ css_key_line_height, "line-height", OnParseLineHeight },
		{ css_key_content, "content", OnParseContent },
		{ css_key_white_space, "white-space", css_parse_keyword_value }
	};

	for (i = 0; i < TOTAL_FONT_STYLE_KEY; ++i) {
		css_property_parser_t *p = &prop_parsers[i];
		css_font_style.keys[p->key] = css_register_property_name(p->name);
		css_register_property_parser(p->key, p->name, p->parse);
	}
	css_font_style.handlers[css_key_color] = OnComputeColor;
	css_font_style.handlers[css_key_font_size] = OnComputeFontSize;
	css_font_style.handlers[css_key_font_family] = OnComputeFontFamily;
	css_font_style.handlers[css_key_font_style] = OnComputeFontStyle;
	css_font_style.handlers[css_key_font_weight] = OnComputeFontWeight;
	css_font_style.handlers[css_key_line_height] = OnComputeLineHeight;
	css_font_style.handlers[css_key_text_align] = OnComputeTextAlign;
	css_font_style.handlers[css_key_white_space] = OnComputeWhiteSpace;
	css_font_style.handlers[css_key_content] = OnComputeContent;
}

void LCUI_FreeCSSFontStyle(void)
{
}
