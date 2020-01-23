/*
 * css_parser.c -- css code parser module
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/font.h>

#define SPLIT_NUMBER 1
#define SPLIT_COLOR (1 << 1)
#define SPLIT_STYLE (1 << 2)

#define LEN(A) sizeof(A) / sizeof(*A)

#define SetCSSProperty CSSStyleParser_SetCSSProperty

static struct CSSParserModule {
	int count;
	DictType dicttype; /**< 解析器表的字典类型数据 */
	Dict *parsers;     /**< 解析器表，以名称进行索引 */
} self;

void CSSStyleParser_SetCSSProperty(LCUI_CSSParserStyleContext ctx, int key,
				   LCUI_Style s)
{
	if (ctx->style_handler) {
		ctx->style_handler(key, s, ctx->style_handler_arg);
	} else {
		ctx->sheet->sheet[key] = *s;
	}
}

static int SplitValues(const char *str, LCUI_Style slist, int max_len, int mode)
{
	char **values;
	const char *p;
	int val, vi = 0, vj = 0, n_quotes = 0;

	memset(slist, 0, sizeof(LCUI_StyleRec) * max_len);
	values = (char **)calloc(max_len, sizeof(char *));
	values[0] = (char *)malloc(sizeof(char) * 64);
	for (p = str; *p; ++p) {
		if (*p == '(') {
			n_quotes += 1;
			values[vi][vj++] = *p;
			continue;
		} else if (*p == ')') {
			n_quotes -= 1;
			values[vi][vj++] = *p;
			continue;
		} else if (*p != ' ' || n_quotes != 0) {
			values[vi][vj++] = *p;
			continue;
		}
		if (vj > 0) {
			values[vi][vj] = 0;
			++vi;
			vj = 0;
			if (vi >= max_len) {
				goto clean;
			}
			values[vi] = (char *)malloc(sizeof(char) * 64);
		}
	}
	values[vi][vj] = 0;
	vi++;
	for (vj = 0; vj < vi; ++vj) {
		DEBUG_MSG("[%d] %s\n", vj, values[vj]);
		if (strcmp(values[vj], "auto") == 0) {
			slist[vj].type = LCUI_STYPE_AUTO;
			slist[vj].val_style = SV_AUTO;
			slist[vj].is_valid = TRUE;
			continue;
		}
		if (mode & SPLIT_NUMBER) {
			if (ParseNumber(&slist[vj], values[vj])) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if (mode & SPLIT_COLOR) {
			if (ParseColor(&slist[vj], values[vj])) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if (mode & SPLIT_STYLE) {
			val = LCUI_GetStyleValue(values[vj]);
			if (val > 0) {
				slist[vj].style = val;
				slist[vj].type = LCUI_STYPE_style;
				slist[vj].is_valid = TRUE;
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		vi = -1;
		DEBUG_MSG("[%d]:parse error\n", vj);
		goto clean;
	}
clean:
	for (vj = 0; vj < max_len; ++vj) {
		values[vj] ? free(values[vj]) : 0;
	}
	free(values);
	return vi;
}

static int OnParseValue(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (sscanf(str, "%d", &s.val_int) == 1) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_INT;
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

static int OnParseNumber(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (ParseNumber(&s, str)) {
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_AUTO;
		s.style = SV_AUTO;
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

static int OnParseBoolean(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s = { 0 };

	if (strcmp(str, "true") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_BOOL;
		s.val_bool = TRUE;
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	if (strcmp(str, "false") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_BOOL;
		s.val_bool = FALSE;
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_AUTO;
		s.style = SV_AUTO;
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

static int OnParseColor(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (ParseColor(&s, str)) {
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

static int OnParseImage(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (ParseUrl(&s, str, ctx->dirname)) {
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

static int OnParseStyleOption(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;
	int v = LCUI_GetStyleValue(str);

	if (v < 0) {
		return -1;
	}
	s.style = v;
	s.type = LCUI_STYPE_STYLE;
	s.is_valid = TRUE;
	SetCSSProperty(ctx, ctx->parser->key, &s);
	return 0;
}

static int OnParseBorder(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (SplitValues(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		if (!slist[i].is_valid) {
			break;
		}
		switch (slist[i].type) {
		case LCUI_STYPE_COLOR:
			SetCSSProperty(ctx, key_border_top_color, &slist[i]);
			SetCSSProperty(ctx, key_border_right_color, &slist[i]);
			SetCSSProperty(ctx, key_border_bottom_color, &slist[i]);
			SetCSSProperty(ctx, key_border_left_color, &slist[i]);
			break;
		case LCUI_STYPE_PX:
		case LCUI_STYPE_INT:
			SetCSSProperty(ctx, key_border_top_width, &slist[i]);
			SetCSSProperty(ctx, key_border_right_width, &slist[i]);
			SetCSSProperty(ctx, key_border_bottom_width, &slist[i]);
			SetCSSProperty(ctx, key_border_left_width, &slist[i]);
			break;
		case LCUI_STYPE_STYLE:
			SetCSSProperty(ctx, key_border_top_style, &slist[i]);
			SetCSSProperty(ctx, key_border_right_style, &slist[i]);
			SetCSSProperty(ctx, key_border_bottom_style, &slist[i]);
			SetCSSProperty(ctx, key_border_left_style, &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int OnParseBorderRadius(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (!ParseNumber(&s, str)) {
		return -1;
	}
	SetCSSProperty(ctx, key_border_top_left_radius, &s);
	SetCSSProperty(ctx, key_border_top_right_radius, &s);
	SetCSSProperty(ctx, key_border_bottom_left_radius, &s);
	SetCSSProperty(ctx, key_border_bottom_right_radius, &s);
	return 0;
}

static int OnParseBorderLeft(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (SplitValues(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].type) {
		case LCUI_STYPE_COLOR:
			SetCSSProperty(ctx, key_border_left_color, &slist[i]);
			break;
		case LCUI_STYPE_PX:
		case LCUI_STYPE_INT:
			SetCSSProperty(ctx, key_border_left_width, &slist[i]);
			break;
		case LCUI_STYPE_style:
			SetCSSProperty(ctx, key_border_left_style, &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int OnParseBorderTop(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (SplitValues(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].type) {
		case LCUI_STYPE_COLOR:
			SetCSSProperty(ctx, key_border_top_color, &slist[i]);
			break;
		case LCUI_STYPE_PX:
		case LCUI_STYPE_INT:
			SetCSSProperty(ctx, key_border_top_width, &slist[i]);
			break;
		case LCUI_STYPE_style:
			SetCSSProperty(ctx, key_border_top_style, &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int OnParseBorderRight(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (SplitValues(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].type) {
		case LCUI_STYPE_COLOR:
			SetCSSProperty(ctx, key_border_right_color, &slist[i]);
			break;
		case LCUI_STYPE_PX:
		case LCUI_STYPE_INT:
			SetCSSProperty(ctx, key_border_right_width, &slist[i]);
			break;
		case LCUI_STYPE_style:
			SetCSSProperty(ctx, key_border_right_style, &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int OnParseBorderBottom(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (SplitValues(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].type) {
		case LCUI_STYPE_COLOR:
			SetCSSProperty(ctx, key_border_bottom_color, &slist[i]);
			break;
		case LCUI_STYPE_PX:
		case LCUI_STYPE_INT:
			SetCSSProperty(ctx, key_border_bottom_width, &slist[i]);
			break;
		case LCUI_STYPE_style:
			SetCSSProperty(ctx, key_border_bottom_style, &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int OnParseBorderColor(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	// TODO: support parsing multiple values
	// border-color: #eee transparent;
	// border-color: #f00 #0f0 transparent;
	// border-color: #0f0 #f00 #f00 #0f0;
	if (!ParseColor(&s, str)) {
		return -1;
	}
	SetCSSProperty(ctx, key_border_top_color, &s);
	SetCSSProperty(ctx, key_border_right_color, &s);
	SetCSSProperty(ctx, key_border_bottom_color, &s);
	SetCSSProperty(ctx, key_border_left_color, &s);
	return 0;
}

static int OnParseBorderWidth(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	// TODO: support parsing multiple values
	// border-width: 4px 0;
	// border-width: 4px 8px 0;
	// border-width: 4px 0 0 4px;
	if (!ParseNumber(&s, str)) {
		return -1;
	}
	SetCSSProperty(ctx, key_border_top_width, &s);
	SetCSSProperty(ctx, key_border_right_width, &s);
	SetCSSProperty(ctx, key_border_bottom_width, &s);
	SetCSSProperty(ctx, key_border_left_width, &s);
	return 0;
}

static int OnParseBorderStyle(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	s.is_valid = TRUE;
	s.val_style = LCUI_GetStyleValue(str);
	if (s.val_style < 0) {
		return -1;
	}
	SetCSSProperty(ctx, key_border_top_style, &s);
	SetCSSProperty(ctx, key_border_right_style, &s);
	SetCSSProperty(ctx, key_border_bottom_style, &s);
	SetCSSProperty(ctx, key_border_left_style, &s);
	return 0;
}

static int OnParsePadding(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s[4];

	switch (SplitValues(str, s, 4, SPLIT_NUMBER)) {
	case 1:
		SetCSSProperty(ctx, key_padding_top, &s[0]);
		SetCSSProperty(ctx, key_padding_right, &s[0]);
		SetCSSProperty(ctx, key_padding_bottom, &s[0]);
		SetCSSProperty(ctx, key_padding_left, &s[0]);
		break;
	case 2:
		SetCSSProperty(ctx, key_padding_top, &s[0]);
		SetCSSProperty(ctx, key_padding_bottom, &s[0]);
		SetCSSProperty(ctx, key_padding_left, &s[1]);
		SetCSSProperty(ctx, key_padding_right, &s[1]);
		break;
	case 3:
		SetCSSProperty(ctx, key_padding_top, &s[0]);
		SetCSSProperty(ctx, key_padding_left, &s[1]);
		SetCSSProperty(ctx, key_padding_right, &s[1]);
		SetCSSProperty(ctx, key_padding_bottom, &s[2]);
		break;
	case 4:
		SetCSSProperty(ctx, key_padding_top, &s[0]);
		SetCSSProperty(ctx, key_padding_right, &s[1]);
		SetCSSProperty(ctx, key_padding_bottom, &s[2]);
		SetCSSProperty(ctx, key_padding_left, &s[3]);
	default:
		break;
	}
	return 0;
}

static int OnParseMargin(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s[4];

	switch (SplitValues(str, s, 4, SPLIT_NUMBER)) {
	case 1:
		SetCSSProperty(ctx, key_margin_top, &s[0]);
		SetCSSProperty(ctx, key_margin_right, &s[0]);
		SetCSSProperty(ctx, key_margin_bottom, &s[0]);
		SetCSSProperty(ctx, key_margin_left, &s[0]);
		break;
	case 2:
		SetCSSProperty(ctx, key_margin_top, &s[0]);
		SetCSSProperty(ctx, key_margin_bottom, &s[0]);
		SetCSSProperty(ctx, key_margin_left, &s[1]);
		SetCSSProperty(ctx, key_margin_right, &s[1]);
		break;
	case 3:
		SetCSSProperty(ctx, key_margin_top, &s[0]);
		SetCSSProperty(ctx, key_margin_left, &s[1]);
		SetCSSProperty(ctx, key_margin_right, &s[1]);
		SetCSSProperty(ctx, key_margin_bottom, &s[2]);
		break;
	case 4:
		SetCSSProperty(ctx, key_margin_top, &s[0]);
		SetCSSProperty(ctx, key_margin_right, &s[1]);
		SetCSSProperty(ctx, key_margin_bottom, &s[2]);
		SetCSSProperty(ctx, key_margin_left, &s[3]);
	default:
		break;
	}
	return 0;
}

static int OnParseBoxShadow(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s[5];

	if (strcmp(str, "none") == 0) {
		s[0].val_int = 0;
		s[0].is_valid = TRUE;
		s[0].type = LCUI_STYPE_NONE;
		SetCSSProperty(ctx, key_box_shadow_x, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_y, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_blur, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_spread, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_color, &s[0]);
		return 0;
	}
	switch (SplitValues(str, s, 5, SPLIT_NUMBER | SPLIT_COLOR)) {
	case 5:
		SetCSSProperty(ctx, key_box_shadow_x, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_y, &s[1]);
		SetCSSProperty(ctx, key_box_shadow_blur, &s[2]);
		SetCSSProperty(ctx, key_box_shadow_spread, &s[3]);
		SetCSSProperty(ctx, key_box_shadow_color, &s[4]);
		break;
	case 4:
		SetCSSProperty(ctx, key_box_shadow_x, &s[0]);
		SetCSSProperty(ctx, key_box_shadow_y, &s[1]);
		SetCSSProperty(ctx, key_box_shadow_blur, &s[2]);
		SetCSSProperty(ctx, key_box_shadow_color, &s[3]);
		break;
	default:
		return -1;
	}
	return 0;
}

static int OnParseBackground(LCUI_CSSParserStyleContext ctx, const char *str)
{
	// TODO: support parsing multiple values
	// background: #eee;
	// background: url(image.png);
	// background: url(image.png) bottom center #eee;
	return 0;
}

static int OnParseBackgroundPosition(LCUI_CSSParserStyleContext ctx,
				     const char *str)
{
	LCUI_StyleRec slist[2];
	int ret = OnParseStyleOption(ctx, str);

	if (ret == 0) {
		return 0;
	}
	if (SplitValues(str, slist, 2, SPLIT_NUMBER) == 2) {
		SetCSSProperty(ctx, key_background_position_x, &slist[0]);
		SetCSSProperty(ctx, key_background_position_y, &slist[1]);
		return 0;
	}
	return -2;
}

static int OnParseBackgroundSize(LCUI_CSSParserStyleContext ctx,
				 const char *str)
{
	LCUI_StyleRec none;
	LCUI_StyleRec slist[2];
	int ret = OnParseStyleOption(ctx, str);

	none.is_valid = TRUE;
	none.val_none = 0;
	none.type = LCUI_STYPE_NONE;
	if (ret == 0) {
		SetCSSProperty(ctx, key_background_size_width, &none);
		SetCSSProperty(ctx, key_background_size_height, &none);
		return 0;
	}
	ret = SplitValues(str, slist, 2, SPLIT_NUMBER | SPLIT_STYLE);
	if (ret != 2) {
		return -1;
	}
	SetCSSProperty(ctx, key_background_size, &none);
	SetCSSProperty(ctx, key_background_size_width, &slist[0]);
	SetCSSProperty(ctx, key_background_size_height, &slist[1]);
	return 0;
}

static int OnParseBackgroundRepeat(LCUI_CSSParserStyleContext ctx,
				   const char *str)
{
	return 0;
}

static int OnParseVisibility(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (strcmp(str, "visible") == 0 || strcmp(str, "hidden") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_STRING;
		s.val_string = strdup2(str);
		SetCSSProperty(ctx, ctx->parser->key, &s);
		return 0;
	}
	return -1;
}

/* See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex */

static int OnParseFlex(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_NUMBER | SPLIT_STYLE;

	if (strcmp("initial", str) == 0) {
		s.type = LCUI_STYPE_INT;
		s.is_valid = TRUE;
		s.val_int = 0;
		SetCSSProperty(ctx, key_flex_grow, &s);

		s.val_int = 1;
		SetCSSProperty(ctx, key_flex_shrink, &s);

		s.type = LCUI_STYPE_AUTO;
		s.val_style = SV_AUTO;
		SetCSSProperty(ctx, key_flex_basis, &s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.type = LCUI_STYPE_INT;
		s.is_valid = TRUE;
		s.val_int = 1;
		SetCSSProperty(ctx, key_flex_grow, &s);

		s.val_int = 1;
		SetCSSProperty(ctx, key_flex_shrink, &s);

		s.type = LCUI_STYPE_AUTO;
		s.val_style = SV_AUTO;
		SetCSSProperty(ctx, key_flex_basis, &s);
		return 0;
	}
	if (strcmp("none", str) == 0) {
		s.type = LCUI_STYPE_INT;
		s.is_valid = TRUE;
		s.val_int = 0;
		SetCSSProperty(ctx, key_flex_grow, &s);

		s.val_int = 0;
		SetCSSProperty(ctx, key_flex_shrink, &s);

		s.type = LCUI_STYPE_AUTO;
		s.val_style = SV_AUTO;
		SetCSSProperty(ctx, key_flex_basis, &s);
		return 0;
	}

	i = SplitValues(str, slist, 3, mode);
	if (i == 3) {
		SetCSSProperty(ctx, key_flex_grow, &slist[0]);
		SetCSSProperty(ctx, key_flex_shrink, &slist[1]);
		SetCSSProperty(ctx, key_flex_basis, &slist[2]);
		return 0;
	}
	if (i == 2) {
		SetCSSProperty(ctx, key_flex_shrink, &slist[0]);
		SetCSSProperty(ctx, key_flex_basis, &slist[1]);
		return 0;
	}
	if (i == 1) {
		if (slist[0].type == LCUI_STYPE_INT) {
			SetCSSProperty(ctx, key_flex_grow, &slist[0]);
			return 0;
		}
		SetCSSProperty(ctx, key_flex_basis, &slist[0]);
		return 0;
	}
	return -1;
}

/* See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-flow */

static int OnParseFlexFlow(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;
	LCUI_StyleRec slist[2];

	if (strcmp(str, "wrap") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_WRAP;
		SetCSSProperty(ctx, key_flex_wrap, &s);

		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_INITIAL;
		SetCSSProperty(ctx, key_flex_direction, &s);
		return 0;
	}
	if (strcmp(str, "nowrap") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_NOWRAP;
		SetCSSProperty(ctx, key_flex_wrap, &s);

		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_INITIAL;
		SetCSSProperty(ctx, key_flex_direction, &s);
		return 0;
	}
	if (strcmp(str, "row") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_ROW;
		SetCSSProperty(ctx, key_flex_direction, &s);

		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_INITIAL;
		SetCSSProperty(ctx, key_flex_wrap, &s);
		return 0;
	}
	if (strcmp(str, "column") == 0) {
		s.is_valid = TRUE;
		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_COLUMN;
		SetCSSProperty(ctx, key_flex_direction, &s);

		s.type = LCUI_STYPE_STYLE;
		s.val_style = SV_INITIAL;
		SetCSSProperty(ctx, key_flex_wrap, &s);
		return 0;
	}
	if (SplitValues(str, slist, 2, SPLIT_STYLE) == 2) {
		SetCSSProperty(ctx, key_flex_direction, &slist[0]);
		SetCSSProperty(ctx, key_flex_wrap, &slist[1]);
		return 0;
	}
	return -1;
}

static int OnParseFlexBasis(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (OnParseStyleOption(ctx, str) == 0) {
		return 0;
	}
	if (ParseNumber(&s, str)) {
		SetCSSProperty(ctx, key_flex_basis, &s);
		return 0;
	}
	return -1;
}

static int OnParseFlexGrow(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (ParseNumber(&s, str)) {
		SetCSSProperty(ctx, key_flex_grow, &s);
		return 0;
	}
	return -1;
}

static int OnParseFlexShrink(LCUI_CSSParserStyleContext ctx, const char *str)
{
	LCUI_StyleRec s;

	if (ParseNumber(&s, str)) {
		SetCSSProperty(ctx, key_flex_grow, &s);
		return 0;
	}
	return -1;
}

/** 各个样式的解析器映射表 */
static LCUI_CSSPropertyParserRec style_parser_map[] = {
	{ key_width, NULL, OnParseNumber },
	{ key_height, NULL, OnParseNumber },
	{ key_min_width, NULL, OnParseNumber },
	{ key_min_height, NULL, OnParseNumber },
	{ key_max_width, NULL, OnParseNumber },
	{ key_max_height, NULL, OnParseNumber },
	{ key_top, NULL, OnParseNumber },
	{ key_right, NULL, OnParseNumber },
	{ key_bottom, NULL, OnParseNumber },
	{ key_left, NULL, OnParseNumber },
	{ key_z_index, NULL, OnParseValue },
	{ key_opacity, NULL, OnParseNumber },
	{ key_position, NULL, OnParseStyleOption },
	{ key_visibility, NULL, OnParseVisibility },
	{ key_vertical_align, NULL, OnParseStyleOption },
	{ key_display, NULL, OnParseStyleOption },
	{ key_background_color, NULL, OnParseColor },
	{ key_background_image, NULL, OnParseImage },
	{ key_background_position, NULL, OnParseBackgroundPosition },
	{ key_background_size, NULL, OnParseBackgroundSize },
	{ key_background_repeat, NULL, OnParseBackgroundRepeat },
	{ key_border_top_color, NULL, OnParseColor },
	{ key_border_right_color, NULL, OnParseColor },
	{ key_border_bottom_color, NULL, OnParseColor },
	{ key_border_left_color, NULL, OnParseColor },
	{ key_border_top_width, NULL, OnParseNumber },
	{ key_border_right_width, NULL, OnParseNumber },
	{ key_border_bottom_width, NULL, OnParseNumber },
	{ key_border_left_width, NULL, OnParseNumber },
	{ key_border_top_style, NULL, OnParseStyleOption },
	{ key_border_right_style, NULL, OnParseStyleOption },
	{ key_border_bottom_style, NULL, OnParseStyleOption },
	{ key_border_left_style, NULL, OnParseStyleOption },
	{ key_border_top_left_radius, NULL, OnParseNumber },
	{ key_border_top_right_radius, NULL, OnParseNumber },
	{ key_border_bottom_left_radius, NULL, OnParseNumber },
	{ key_border_bottom_right_radius, NULL, OnParseNumber },
	{ key_padding_top, NULL, OnParseNumber },
	{ key_padding_right, NULL, OnParseNumber },
	{ key_padding_bottom, NULL, OnParseNumber },
	{ key_padding_left, NULL, OnParseNumber },
	{ key_margin_top, NULL, OnParseNumber },
	{ key_margin_right, NULL, OnParseNumber },
	{ key_margin_bottom, NULL, OnParseNumber },
	{ key_margin_left, NULL, OnParseNumber },
	{ key_focusable, NULL, OnParseBoolean },
	{ key_pointer_events, NULL, OnParseStyleOption },
	{ key_box_sizing, NULL, OnParseStyleOption },

	{ key_flex_basis, NULL, OnParseFlexBasis },
	{ key_flex_grow, NULL, OnParseFlexGrow },
	{ key_flex_shrink, NULL, OnParseFlexShrink },
	{ key_flex_direction, NULL, OnParseStyleOption },
	{ key_flex_wrap, NULL, OnParseStyleOption },
	{ key_justify_content, NULL, OnParseStyleOption },
	{ key_justify_items, NULL, OnParseStyleOption },
	{ key_align_content, NULL, OnParseStyleOption },
	{ key_align_items, NULL, OnParseStyleOption },

	{ -1, "border", OnParseBorder },
	{ -1, "border-left", OnParseBorderLeft },
	{ -1, "border-top", OnParseBorderTop },
	{ -1, "border-right", OnParseBorderRight },
	{ -1, "border-bottom", OnParseBorderBottom },
	{ -1, "border-color", OnParseBorderColor },
	{ -1, "border-width", OnParseBorderWidth },
	{ -1, "border-style", OnParseBorderStyle },
	{ -1, "border-radius", OnParseBorderRadius },
	{ -1, "padding", OnParsePadding },
	{ -1, "margin", OnParseMargin },
	{ -1, "box-shadow", OnParseBoxShadow },
	{ -1, "background", OnParseBackground },
	{ -1, "flex-flow", OnParseFlexFlow },
	{ -1, "flex", OnParseFlex }
};

static int CSSParser_ParseComment(LCUI_CSSParserContext ctx)
{
	if (ctx->comment.is_line_comment) {
		if (*ctx->cur == '\n') {
			ctx->target = ctx->comment.prev_target;
		}
		return 0;
	}
	if (*ctx->cur == '/' && *(ctx->cur - 1) == '*') {
		ctx->target = ctx->comment.prev_target;
	}
	return 0;
}

int CSSParser_BeginParseComment(LCUI_CSSParserContext ctx)
{
	switch (*(ctx->cur + 1)) {
	case '/':
		ctx->comment.is_line_comment = TRUE;
		break;
	case '*':
		ctx->comment.is_line_comment = FALSE;
		break;
	default:
		CSSParser_GetChar(ctx);
		return -1;
	}
	if (ctx->comment.prev_target != CSS_TARGET_COMMENT) {
		ctx->comment.prev_target = ctx->target;
		ctx->target = CSS_TARGET_COMMENT;
	}
	return 0;
}

static void CSSParser_EndParseSheet(LCUI_CSSParserContext ctx)
{
	LinkedListNode *node;
	/* 将记录的样式表添加至匹配到的选择器中 */
	for (LinkedList_Each(node, &ctx->style.selectors)) {
		LCUI_PutStyleSheet(node->data, ctx->style.sheet, ctx->space);
	}
	LinkedList_Clear(&ctx->style.selectors, (FuncPtr)Selector_Delete);
	StyleSheet_Delete(ctx->style.sheet);
}

static int CSSParser_ParseSelector(LCUI_CSSParserContext ctx)
{
	LCUI_Selector s;

	switch (*ctx->cur) {
	case '/':
		return CSSParser_BeginParseComment(ctx);
	case '{':
		ctx->target = CSS_TARGET_KEY;
		ctx->style.sheet = StyleSheet();
	case ',':
		CSSParser_EndBuffer(ctx);
		DEBUG_MSG("selector: %s\n", ctx->buffer);
		s = Selector(ctx->buffer);
		if (!s) {
			return -1;
		}
		LinkedList_Append(&ctx->style.selectors, s);
		break;
	default:
		CSSParser_GetChar(ctx);
		break;
	}
	return 0;
}

static int CSSParser_SetRuleParser(LCUI_CSSParserContext ctx, const char *name)
{
	LCUI_CSSRule rule;
	LCUI_CSSRuleParser parser;
	for (rule = CSS_RULE_NONE; rule < CSS_RULE_TOTAL_NUM; ++rule) {
		parser = &ctx->rule.parsers[rule];
		if (strlen(parser->name) < 1) {
			continue;
		}
		if (strcmp(parser->name, name) == 0) {
			ctx->rule.rule = rule;
			parser->begin(ctx);
			return 0;
		}
	}
	return -ENOENT;
}

static int CSSParser_ParseRuleName(LCUI_CSSParserContext ctx)
{
	switch (*ctx->cur) {
	CASE_WHITE_SPACE:
		if (ctx->pos > 0) {
			break;
		}
		return -1;
	default:
		CSSParser_GetChar(ctx);
		return 0;
	}
	CSSParser_EndBuffer(ctx);
	if (CSSParser_SetRuleParser(ctx, ctx->buffer) == 0) {
		ctx->target = CSS_TARGET_RULE_DATA;
	} else {
		return -1;
	}
	return 0;
}

static int CSSParser_ParseRuleData(LCUI_CSSParserContext ctx)
{
	LCUI_CSSRuleParser parser;
	parser = &ctx->rule.parsers[ctx->rule.rule];
	if (parser->parse) {
		return parser->parse(ctx);
	}
	return -1;
}

static int CSSParser_ParseStyleName(LCUI_CSSParserContext ctx)
{
	switch (*ctx->cur) {
	CASE_WHITE_SPACE:
	case ';':
		return -1;
	case ':':
		ctx->target = CSS_TARGET_VALUE;
		CSSParser_EndBuffer(ctx);
		ctx->style.parser = LCUI_GetCSSPropertyParser(ctx->buffer);
		DEBUG_MSG("select style: %s, parser: %p\n", ctx->buffer,
			  ctx->style_parser);
		break;
	case '}':
		ctx->target = CSS_TARGET_NONE;
		CSSParser_EndParseSheet(ctx);
		break;
	default:
		CSSParser_GetChar(ctx);
		break;
	}
	return 0;
}

static int CSSParser_ParseStyleValue(LCUI_CSSParserContext ctx)
{
	switch (*ctx->cur) {
	case '/':
		return CSSParser_BeginParseComment(ctx);
	case '}':
	case ';':
		break;
	CASE_WHITE_SPACE:
		if (ctx->pos == 0) {
			return 0;
		}
	default:
		CSSParser_GetChar(ctx);
		return 0;
	}
	if (*ctx->cur == ';') {
		ctx->target = CSS_TARGET_KEY;
	}
	CSSParser_EndBuffer(ctx);
	if (ctx->style.parser) {
		ctx->style.parser->parse(&ctx->style, ctx->buffer);
	}
	DEBUG_MSG("parse style value: %s\n", ctx->buffer);
	if (*ctx->cur == '}') {
		ctx->target = CSS_TARGET_NONE;
		CSSParser_EndParseSheet(ctx);
	}
	return 0;
}

static int CSSParser_ParseTarget(LCUI_CSSParserContext ctx)
{
	switch (*ctx->cur) {
	case '/':
		return CSSParser_BeginParseComment(ctx);
	CASE_WHITE_SPACE:
	case ',':
	case '{':
	case '\\':
	case '"':
	case '}':
		return -1;
	default:
		break;
	}
	ctx->pos = 0;
	if (*ctx->cur == '@') {
		ctx->target = CSS_TARGET_RULE_NAME;
	} else {
		CSSParser_GetChar(ctx);
		ctx->target = CSS_TARGET_SELECTOR;
	}
	return 0;
}

void CSSParser_EndParseRuleData(LCUI_CSSParserContext ctx)
{
	ctx->rule.rule = CSS_RULE_NONE;
	ctx->target = CSS_TARGET_NONE;
}

static void LoadFontFile(void *arg1, void *arg2)
{
	LCUIFont_LoadFile(arg1);
	LCUIWidget_RefreshTextView();
}

static void OnParsedFontFace(LCUI_CSSFontFace face)
{
	static int worker_id = -1;
	LCUI_TaskRec task = { 0 };
	task.func = LoadFontFile;
	task.arg[0] = strdup2(face->src);
	task.destroy_arg[0] = free;
	if (worker_id > -1) {
		LCUI_PostAsyncTaskTo(&task, worker_id);
	} else {
		worker_id = LCUI_PostAsyncTask(&task);
	}
}

static char *getdirname(const char *path)
{
	char *dirname;
	size_t i, pos;
	for (i = 0, pos = SIZE_MAX; path[i]; ++i) {
		if (path[i] == '/') {
			pos = i;
		}
	}
	if (pos > i) {
		return NULL;
	}
	dirname = malloc(sizeof(char) * i);
	if (!dirname) {
		return NULL;
	}
	for (i = 0; i < pos; ++i) {
		dirname[i] = path[i];
	}
	dirname[i] = 0;
	return dirname;
}

void CSSParser_EndBuffer(LCUI_CSSParserContext ctx)
{
	int i;
	int pos;

	ctx->buffer[ctx->pos] = 0;
	for (i = 0, pos = -1; i < ctx->pos && pos != -1; ++i) {
		switch (ctx->buffer[ctx->pos]) {
		CASE_WHITE_SPACE:
			break;
		default:
			pos = i;
			break;
		}
	}
	if (pos != -1) {
		ctx->pos -= pos;
		/* trim left */
		for (i = 0; i < ctx->pos; ++i) {
			ctx->buffer[i] = ctx->buffer[pos + i];
		}
		ctx->buffer[ctx->pos] = 0;
	}
	/* trim right */
	for (pos = -1, --ctx->pos; ctx->pos >= 0 && pos != -1; --ctx->pos) {
		switch (ctx->buffer[ctx->pos]) {
		CASE_WHITE_SPACE:
			ctx->buffer[ctx->pos] = 0;
			break;
		default:
			pos = i;
			break;
		}
	}
	ctx->pos = 0;
}

LCUI_CSSParserContext CSSParser_Begin(size_t buffer_size, const char *space)
{
	ASSIGN(ctx, LCUI_CSSParserContext);
	if (space) {
		ctx->space = strdup2(space);
		ctx->style.dirname = getdirname(ctx->space);
	} else {
		ctx->space = NULL;
		ctx->style.dirname = NULL;
	}
	ctx->buffer = NEW(char, buffer_size);
	ctx->buffer_size = buffer_size;
	ctx->target = CSS_TARGET_NONE;
	ctx->style.space = ctx->space;
	ctx->style.style_handler = NULL;
	ctx->style.style_handler_arg = NULL;
	ctx->parsers[CSS_TARGET_NONE].parse = CSSParser_ParseTarget;
	ctx->parsers[CSS_TARGET_RULE_NAME].parse = CSSParser_ParseRuleName;
	ctx->parsers[CSS_TARGET_RULE_DATA].parse = CSSParser_ParseRuleData;
	ctx->parsers[CSS_TARGET_SELECTOR].parse = CSSParser_ParseSelector;
	ctx->parsers[CSS_TARGET_KEY].parse = CSSParser_ParseStyleName;
	ctx->parsers[CSS_TARGET_VALUE].parse = CSSParser_ParseStyleValue;
	ctx->parsers[CSS_TARGET_COMMENT].parse = CSSParser_ParseComment;
	ctx->comment.prev_target = CSS_TARGET_NONE;
	LinkedList_Init(&ctx->style.selectors);
	memset(&ctx->rule, 0, sizeof(ctx->rule));
	CSSParser_InitFontFaceRuleParser(ctx);
	CSSRuleParser_OnFontFace(ctx, OnParsedFontFace);
	return ctx;
}

void CSSParser_End(LCUI_CSSParserContext ctx)
{
	LinkedList_Clear(&ctx->style.selectors, (FuncPtr)Selector_Delete);
	CSSParser_FreeFontFaceRuleParser(ctx);
	if (ctx->space) {
		free(ctx->space);
	}
	if (ctx->style.dirname) {
		free(ctx->style.dirname);
	}
	free(ctx->buffer);
	free(ctx);
}

/** 载入CSS代码块，用于实现CSS代码的分块载入 */
static size_t LCUI_LoadCSSBlock(LCUI_CSSParserContext ctx, const char *str)
{
	size_t size = 0;

	ctx->cur = str;
	while (*ctx->cur && size < ctx->buffer_size) {
		ctx->parsers[ctx->target].parse(ctx);
		++ctx->cur;
		++size;
	}
	return size;
}

LCUI_CSSPropertyParser LCUI_GetCSSPropertyParser(const char *name)
{
	return Dict_FetchValue(self.parsers, name);
}

int LCUI_LoadCSSFile(const char *filepath)
{
	size_t n;
	FILE *fp;
	char buff[512];
	LCUI_CSSParserContext ctx;

	fp = fopen(filepath, "r");
	if (!fp) {
		return -1;
	}
	ctx = CSSParser_Begin(512, filepath);
	n = fread(buff, 1, 511, fp);
	while (n > 0) {
		buff[n] = 0;
		LCUI_LoadCSSBlock(ctx, buff);
		n = fread(buff, 1, 511, fp);
	}
	CSSParser_End(ctx);
	fclose(fp);
	return 0;
}

size_t LCUI_LoadCSSString(const char *str, const char *space)
{
	size_t len = 1;
	const char *cur;
	LCUI_CSSParserContext ctx;

	DEBUG_MSG("parse begin\n");
	ctx = CSSParser_Begin(512, space);
	for (cur = str; len > 0; cur += len) {
		len = LCUI_LoadCSSBlock(ctx, cur);
	}
	CSSParser_End(ctx);
	DEBUG_MSG("parse end\n");
	return 0;
}

int LCUI_AddCSSPropertyParser(LCUI_CSSPropertyParser sp)
{
	LCUI_CSSPropertyParser new_sp;
	if (!sp->name || strlen(sp->name) < 1) {
		return -1;
	}
	if (Dict_FetchValue(self.parsers, sp->name)) {
		return -2;
	}
	self.count += 1;
	new_sp = NEW(LCUI_CSSPropertyParserRec, 1);
	new_sp->key = sp->key;
	new_sp->parse = sp->parse;
	new_sp->name = strdup2(sp->name);
	Dict_Add(self.parsers, new_sp->name, new_sp);
	return 0;
}

static void DestroyStyleParser(void *privdata, void *val)
{
	LCUI_CSSPropertyParser sp = val;
	free(sp->name);
	free(sp);
}

void LCUI_InitCSSParser(void)
{
	LCUI_CSSPropertyParser new_sp, sp, sp_end;

	self.count = 0;
	self.dicttype = DictType_StringKey;
	self.dicttype.valDestructor = DestroyStyleParser;
	self.parsers = Dict_Create(&self.dicttype, NULL);
	sp_end = style_parser_map + LEN(style_parser_map);
	for (sp = style_parser_map; sp < sp_end; ++sp) {
		new_sp = malloc(sizeof(LCUI_CSSPropertyParserRec));
		new_sp->key = sp->key;
		new_sp->parse = sp->parse;
		if (!sp->name && sp->key >= 0) {
			const char *name = LCUI_GetStyleName(sp->key);
			if (!name) {
				free(new_sp);
				continue;
			}
			new_sp->name = strdup2(name);
		} else {
			new_sp->name = strdup2(sp->name);
		}
		Dict_Add(self.parsers, new_sp->name, new_sp);
	}
}

void LCUI_FreeCSSParser(void)
{
	Dict_Release(self.parsers);
}
