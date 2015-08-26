/* ***************************************************************************
 * css_parser.c -- css parser module
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * css_parser.c -- css 样式解析模块
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget_base.h>

static const char *parseNumber( LCUI_StyleVar *s, const char *str )
{
	return NULL;
}

static const char *parseColor( LCUI_StyleVar *s, const char *str )
{
	return NULL;
}

static const char *parseImage( LCUI_StyleVar *s, const char *str )
{
	return NULL;
}

static const char *parseStyleKey( LCUI_StyleVar *s, const char *str )
{
	return NULL;
}

static const char *parseBorder( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderLeft( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderTop( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderRight( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderBottom( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderColor( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderWidth( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBorderStyle( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parsePadding( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseMargin( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBoxShadow( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

static const char *parseBackground( LCUI_StyleSheet ss, const char *str )
{
	return NULL;
}

/** 基本样式的解析器 */
typedef struct StyleParser {
	int key;
	const char *name;
	const char *(*parser)(LCUI_StyleVar*,const char*);
} StyleParser;

/** 样式组的解析器 */
typedef struct StyleGroupParser {
	const char *name;
	const char *(*parser)(LCUI_StyleSheet,const char*);
} StyleGroupParser;

/** 各个基本样式的解析器映射表 */
static StyleParser style_parser_map[] = {
	{key_width, "width", parseNumber},
	{key_height, "height", parseNumber},
	{key_background_color, "background-color", parseColor},
	{key_background_image, "background-image", parseImage},
	{key_border_top_color, "border-top-color", parseColor},
	{key_border_right_color, "border-right-color", parseColor},
	{key_border_bottom_color, "border-bottom-color", parseColor},
	{key_border_left_color, "border-left-color", parseColor},
	{key_border_top_width, "border-top-width", parseNumber},
	{key_border_right_width, "border-right-width", parseNumber},
	{key_border_bottom_width, "border-bottom-width", parseNumber},
	{key_border_left_width, "border-left-width", parseNumber},
	{key_border_top_width, "border-top-width", parseNumber},
	{key_border_right_width, "border-right-width", parseNumber},
	{key_border_bottom_width, "border-bottom-width", parseNumber},
	{key_border_left_width, "border-left-width", parseNumber},
	{key_border_top_style, "border-top-style", parseStyleKey},
	{key_border_right_style, "border-right-style", parseStyleKey},
	{key_border_bottom_style, "border-bottom-style", parseStyleKey},
	{key_border_left_style, "border-left-style", parseStyleKey},
};

/** 各个样式组的解析器映射表 */
static StyleGroupParser style_group_parser_map[] = {
	{"border", parseBorder},
	{"border-left", parseBorderLeft},
	{"border-top", parseBorderTop},
	{"border-right", parseBorderRight},
	{"border-bottom", parseBorderBottom},
	{"border-color", parseBorderColor},
	{"border-width", parseBorderWidth},
	{"border-style", parseBorderStyle},
	{"padding", parsePadding},
	{"margin", parseMargin},
	{"box-shadow", parseBoxShadow},
	{"background", parseBackground}
};

static LCUI_RBTree style_key_tree;
static LCUI_RBTree style_name_tree;

static int CompareName( void *data, const void *keydata )
{
	return strcmp(*(char**)data, (const char*)keydata);
}

/** 初始化 LCUI 的 CSS 代码解析功能 */
void LCUICssParser_Init(void)
{
	int i, n;
	StyleParser *ssp;
	StyleGroupParser *msp;

	RBTree_Init( &style_key_tree );
	RBTree_Init( &style_name_tree );
	RBTree_SetDataNeedFree( &style_key_tree, FALSE );
	RBTree_SetDataNeedFree( &style_name_tree, FALSE );
	RBTree_OnJudge( &style_name_tree, CompareName );
	n = sizeof(style_parser_map)/sizeof(StyleParser);
	for( i=0; i<n; ++i ) {
		ssp = &style_parser_map[i];
		RBTree_CustomInsert( &style_name_tree, ssp->name, ssp );
		RBTree_Insert( &style_key_tree, ssp->key, ssp );
	}
	n = sizeof(style_group_parser_map)/sizeof(StyleGroupParser);
	for( i=0; i<n; ++i ) {
		msp = &style_group_parser_map[i];
		RBTree_CustomInsert( &style_name_tree, msp->name, msp );
	}
}

/** 从字符串中解析出样式，并导入至样式库中 */
int LCUI_ParseStyle( const char *str )
{
	return 0;
}

void LCUICssParser_Destroy(void)
{

}
