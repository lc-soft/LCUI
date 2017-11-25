/* ***************************************************************************
 * css_fontstyle.h -- css font style parse and operation set.
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * css_fontstyle.h -- CSS 字体样式解析与操作集
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#ifndef LCUI_CSS_FONT_STYLE_H
#define LCUI_CSS_FONT_STYLE_H

#include <LCUI/font/textstyle.h>

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
	int font_weight;
	int font_style;
	int *font_ids;
	char *font_family;
	int text_align;
	int white_space;
	int line_height;
	LCUI_Color color;
	wchar_t *content;
} LCUI_CSSFontStyleRec, *LCUI_CSSFontStyle;

#define Widget_SetFontStyle(W, K, V, T) do {\
	int key = LCUI_GetFontStyleKey( K ); \
	SetStyle( (W)->custom_style, key, V, T ); \
} while( 0 )

int LCUI_GetFontStyleKey( int key );

void CSSFontStyle_Init( LCUI_CSSFontStyle fs );

void CSSFontStyle_Destroy( LCUI_CSSFontStyle fs );

void CSSFontStyle_Compute( LCUI_CSSFontStyle fs, LCUI_StyleSheet ss );

void CSSFontStyle_GetTextStyle( LCUI_CSSFontStyle fs, LCUI_TextStyle *ts );

void LCUI_InitCSSFontStyle( void );

void LCUI_FreeCSSFontStyle( void );

#endif
