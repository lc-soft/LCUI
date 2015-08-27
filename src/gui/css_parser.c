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
#include <LCUI/widget_build.h>

static int parseNumber( LCUI_Style *s, const char *str )
{
	return 0;
}

static int parseColor( LCUI_Style *s, const char *str )
{
	return 0;
}

static int parseImage( LCUI_Style *s, const char *str )
{
	return 0;
}

static int parseStyleKey( LCUI_Style *s, const char *str )
{
	return 0;
}

static int parseBorder( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderLeft( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderTop( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderRight( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderBottom( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderColor( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderWidth( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBorderStyle( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parsePadding( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseMargin( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBoxShadow( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int parseBackground( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

/** 基本样式的解析器 */
typedef struct StyleParser {
	int key;
	const char *name;
	int (*parser)(LCUI_Style*,const char*);
} StyleParser;

/** 样式组的解析器 */
typedef struct StyleGroupParser {
	const char *name;
	int (*parser)(LCUI_StyleSheet,const char*);
} StyleGroupParser;

typedef struct ParserNode {
	int type;
	void *parser;
} ParserNode;

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

static LCUI_RBTree style_name_tree;

static int CompareName( void *data, const void *keydata )
{
	ParserNode *node = (ParserNode*)data;
	if( node->type == 0 ) {
		return strcmp(((StyleParser*)node->parser)->name,
				(const char*)keydata);
	}
	return strcmp(((StyleGroupParser*)node->parser)->name,
			(const char*)keydata);
}

/** 初始化 LCUI 的 CSS 代码解析功能 */
void LCUICssParser_Init(void)
{
	int i, n;
	StyleParser *ssp;
	StyleGroupParser *msp;
	ParserNode node;

	/* 构建一个红黑树树，方便按名称查找解析器 */
	RBTree_Init( &style_name_tree );
	RBTree_SetDataNeedFree( &style_name_tree, FALSE );
	RBTree_OnJudge( &style_name_tree, CompareName );
	n = sizeof(style_parser_map)/sizeof(StyleParser);
	node.type = 0;
	for( i=0; i<n; ++i ) {
		node.parser = ssp = &style_parser_map[i];
		RBTree_CustomInsert( &style_name_tree, ssp->name, &node );
	}
	n = sizeof(style_group_parser_map)/sizeof(StyleGroupParser);
	node.type = 1;
	for( i=0; i<n; ++i ) {
		node.parser = msp = &style_group_parser_map[i];
		RBTree_CustomInsert( &style_name_tree, msp->name, &node );
	}
}

/** 从字符串中解析出样式，并导入至样式库中 */
int LCUI_ParseStyle( const char *str )
{
	LCUI_Selector s;
	StyleParser *sp;
	StyleGroupParser *sgp;
	/** 解析器的环境参数（上下文数据） */
	struct ParserContext {
		enum {
			is_none,	/**< 无 */
			is_selector,	/**< 选择器 */
			is_key,		/**< 属性名 */
			is_value,	/**< 属性值 */
			is_comment	/**< 注释 */
		} target;		/**< 当前解析中的目标 */
		char buffer[256];	/**< 缓存中的字符串 */
		int pos;		/**< 缓存中的字符串的下标位置 */
		const char *ptr;	/**< 用于遍历字符串的指针 */
		LinkedList selectors;	/**< 当前匹配到的选择器列表 */
		LCUI_StyleSheet css;	/**< 当前缓存的样式表 */
		ParserNode *parser;	/**< 当前找到的解析器 */
	} ctx = { 0 };

	LinkedList_Init( &ctx.selectors, sizeof(LCUI_Selector) );
	for( ctx.ptr = str; *ctx.ptr; ++ctx.ptr ) {
		switch( ctx.target ) {
		case is_selector:
			switch( *ctx.ptr ) {
			case '{':
				ctx.target = is_key;
				ctx.css = StyleSheet();
			case ',':
				ctx.buffer[ctx.pos] = 0;
				ctx.pos = 0;
				s = Selector(ctx.buffer);
				if( !s ) {
					// 解析出错 ...
					break;
				}
				LinkedList_Append( &ctx.selectors, s );
				break;
			default:
				ctx.buffer[ctx.pos++] = *ctx.ptr;
				break;
			}
			break;
		case is_key:
			switch( *ctx.ptr ) {
			case ' ':
			case '\n':
			case '\r':
			case '\t':
			case ';': ctx.pos = 0;
			case ':': continue;
			case '}':
				ctx.target = is_none;
				continue;
			default:
				ctx.buffer[ctx.pos++] = *ctx.ptr;
				continue;
			}
			ctx.target = is_value;
			ctx.buffer[ctx.pos] = 0;
			ctx.pos = 0;
			ctx.parser = (ParserNode*)
			RBTree_CustomSearch( &style_name_tree, ctx.buffer );
			break;
		case is_value:
			switch( *ctx.ptr ) {
			case ';':
			case '}':
				if( !ctx.parser ) {
					break;
				}
				ctx.buffer[ctx.pos] = 0;
				ctx.pos = 0;
				if( ctx.parser->type == 1 ) {
					sgp = ctx.parser->parser;
					sgp->parser( ctx.css, ctx.buffer );
					break;
				}
				sp = ctx.parser->parser;
				sp->parser( &ctx.css[sp->key], ctx.buffer );
				break;
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				if( ctx.pos == 0 ) {
					continue;
				}
			default:
				ctx.buffer[ctx.pos++] = *ctx.ptr;
				continue;
			}
			if( *ctx.ptr == ';' ) {
				ctx.target = is_key;
				break;
			}
			ctx.target = is_none;
			/* 将记录的样式表添加至匹配到的选择器中 */
			LinkedList_ForEach( s, 0, &ctx.selectors ) {
				LCUI_PutStyle( s, ctx.css );
			}
			LinkedList_Destroy( &ctx.selectors );
			DeleteStyleSheet( &ctx.css );
			break;
		case is_comment:
		case is_none:
		default:
			switch( *ctx.ptr ) {
			case '\n':
			case '\t':
			case '\r':
			case ' ':
			case ',':
			case '{':
			case '\'':
			case '"':
			case '}': continue;
			default: break;
			}
			ctx.pos = 0;
			ctx.buffer[ctx.pos] = *ctx.ptr;
			ctx.target = is_selector;
			break;
		}
	}

	return 0;
}

void LCUICssParser_Destroy(void)
{

}
