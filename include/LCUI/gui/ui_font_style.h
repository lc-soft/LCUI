/*
 * css_fontstyle.h -- CSS font style parse and operation set.
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

#ifndef LCUI_CSS_FONT_STYLE_H
#define LCUI_CSS_FONT_STYLE_H

#include <LCUI/font/textstyle.h>
#include <LCUI/css/def.h>

LCUI_BEGIN_HEADER

/* clang-format on */

enum LCUI_CSSFontStyleKey { TOTAL_FONT_STYLE_KEY };

typedef struct LCUI_CSSFontStyleRec_ {
	int font_size;
	int line_height;
	int *font_ids;
	char *font_family;
	wchar_t *content;
	pd_color_t color;
	css_font_style_t font_style;
	css_font_weight_t font_weight;
	css_keyword_value_t text_align;
	css_keyword_value_t white_space;
} LCUI_CSSFontStyleRec, *LCUI_CSSFontStyle;

/* clang-format off */

LCUI_API void CSSFontStyle_Init(LCUI_CSSFontStyle fs);

LCUI_API void CSSFontStyle_Destroy(LCUI_CSSFontStyle fs);

LCUI_API LCUI_BOOL CSSFontStyle_IsEquals(const LCUI_CSSFontStyle a,
					 const LCUI_CSSFontStyle b);

LCUI_API void CSSFontStyle_Compute(LCUI_CSSFontStyle fs, css_style_decl_t *ss);

LCUI_API void CSSFontStyle_GetTextStyle(LCUI_CSSFontStyle fs, LCUI_TextStyle ts);

LCUI_END_HEADER

#endif
