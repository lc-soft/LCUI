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

LCUI_BEGIN_HEADER

/* clang-format on */

enum LCUI_CSSFontStyleKey {
	key_color,
	key_font_size,
	key_font_style,
	key_font_weight,
	key_font_family,
	key_line_height,
	key_text_align,
	key_content,
	key_white_space,
	TOTAL_FONT_STYLE_KEY
};

typedef struct LCUI_CSSFontStyleRec_ {
	int font_size;
	int line_height;
	int *font_ids;
	char *font_family;
	wchar_t *content;
	LCUI_Color color;
	LCUI_FontStyle font_style;
	LCUI_FontWeight font_weight;
	LCUI_StyleValue text_align;
	LCUI_StyleValue white_space;
} LCUI_CSSFontStyleRec, *LCUI_CSSFontStyle;

#define Widget_SetFontStyle(W, K, V, T)            \
	do {                                       \
		int key = LCUI_GetFontStyleKey(K); \
		Widget_SetStyle(W, key, V, T);     \
	} while (0)

/* clang-format off */

LCUI_API int LCUI_GetFontStyleKey(int key);

LCUI_API void CSSFontStyle_Init(LCUI_CSSFontStyle fs);

LCUI_API void CSSFontStyle_Destroy(LCUI_CSSFontStyle fs);

LCUI_API LCUI_BOOL CSSFontStyle_IsEquals(const LCUI_CSSFontStyle a,
					 const LCUI_CSSFontStyle b);

LCUI_API void CSSFontStyle_Compute(LCUI_CSSFontStyle fs, LCUI_StyleSheet ss);

LCUI_API void CSSFontStyle_GetTextStyle(LCUI_CSSFontStyle fs, LCUI_TextStyle ts);

LCUI_API void LCUI_InitCSSFontStyle(void);

LCUI_API void LCUI_FreeCSSFontStyle(void);

LCUI_END_HEADER

#endif
