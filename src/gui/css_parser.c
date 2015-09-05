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
#include <LCUI/misc/parse.h>
#include <LCUI/widget_build.h>

/** 样式字符串值与标识码 */
typedef struct StyleKeyNode {
	int key;
	const char *name;
} StyleKeyNode;

/** 样式的解析器 */
typedef struct StyleParser {
	int type;
	int key;
	const char *name;
	union {
		int (*parse)(LCUI_Style*,const char*);
		int (*parse_group)(LCUI_StyleSheet,const char*);
	};
} StyleParser;

static LCUI_RBTree style_parser_tree;
static LCUI_RBTree style_key_tree;

#define SPLIT_NUMBER	1
#define SPLIT_COLOR	(1<<1)
#define SPLIT_STYLE	(1<<2)

static int ParseStyleKey( const char *str )
{ 
	LCUI_RBTreeNode *node;
	node = RBTree_CustomSearch( &style_key_tree, str );
	if( !node ) {
		return -1;
	}
	return ((StyleKeyNode*)node)->key;
}

static void ConvertStyleValue( LCUI_Style *s, LCUI_StyleVar *sv )
{
	s->type = sv->type;
	switch( sv->type ) {
	case SVT_COLOR:
		s->value_color = sv->color;
		break;
	case SVT_PX:
		s->value_px = sv->px;
		break;
	case SVT_SCALE:
		s->value_scale = sv->scale;
	default:
		s->type = SVT_NONE;
		break;
	}
}

static int SplitValues( const char *str, LCUI_Style *slist, 
			int max_len, int mode )
{
	int vi = 0, vj = 0, has_error;
	char **values;
	const char *p;
	LCUI_StyleVar sv;

	values = (char**)calloc(max_len, sizeof(char*));
	values[0] = (char*)malloc(sizeof(char)*64);
	for( p = str; *p; ++p ) {
		if( *p != ' ' ) {
			values[vi][vj] = *p;
			continue;
		}
		if( vj > 0 ) {
			values[vi][vj] = 0;
			++vi;
			vj = 0;
			if( vi >= max_len ) {
				goto clean;
			}
			values[vi] = (char*)malloc(sizeof(char)*64);
		}
	}
	values[vi][vj] = 0;
	for( vj = 0; vj < 4; ++vj ) {
		has_error = 0;
		if( mode & SPLIT_NUMBER ) {
			if( ParseNumber( &sv, values[vj] ) ) {
				ConvertStyleValue( &slist[vj], &sv );
				continue;
			}
			has_error = 1;
		}
		if( mode & SPLIT_COLOR ) {
			if( ParseColor(&sv, values[vj]) ) {
				ConvertStyleValue( &slist[vj], &sv );
				continue;
			}
			has_error = 1;
		}
		if( mode & SPLIT_STYLE ) {
			has_error = ParseStyleKey( values[vj] );
			if( has_error > 0 )  {
				slist[vj].value_style = has_error;
				slist[vj].type = SVT_style;
				continue;
			}
			has_error = 1;
		}
		if( has_error ) {
			vi = -1;
			goto clean;
		}
	}
clean:
	for( vj = 0; vj < max_len; ++vj ) {
		values[vj] ? free(values[vj]) : 0;
	}
	free( values );
	return vi;
}

static int OnParseNumber( LCUI_Style *s, const char *str )
{
	LCUI_StyleVar sv;
	if( ParseNumber( &sv, str ) ) {
		ConvertStyleValue( s, &sv );
		return 0;
	}
	return -1;
}

static int OnParseColor( LCUI_Style *s, const char *str )
{
	LCUI_StyleVar sv;
	if( ParseColor( &sv, str ) ) {
		ConvertStyleValue( s, &sv );
		return 0;
	}
	return -1;
}

static int OnParseImage( LCUI_Style *s, const char *str )
{
	return 0;
}

static int OnParseStyleKey( LCUI_Style *s, const char *str )
{
	int v;
	v = ParseStyleKey( str );
	if( v < 0 ) {
		return -1;
	}
	s->type = SVT_style;
	s->value_style = v;
	return 0;
}

static int OnParseBorder( LCUI_StyleSheet ss, const char *str )
{
	LCUI_Style slist[3];
	int i, mode;

	mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss[key_border_color] = slist[i];
			break;
		case SVT_PX:
			ss[key_border_width] = slist[i];
			break;
		case SVT_style:
			ss[key_border_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderLeft( LCUI_StyleSheet ss, const char *str )
{
	LCUI_Style slist[3];
	int i, mode;

	mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss[key_border_left_color] = slist[i];
			break;
		case SVT_PX:
			ss[key_border_left_width] = slist[i];
			break;
		case SVT_style:
			ss[key_border_left_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderTop( LCUI_StyleSheet ss, const char *str )
{
	LCUI_Style slist[3];
	int i, mode;

	mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss[key_border_top_color] = slist[i];
			break;
		case SVT_PX:
			ss[key_border_top_width] = slist[i];
			break;
		case SVT_style:
			ss[key_border_top_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderRight( LCUI_StyleSheet ss, const char *str )
{
	LCUI_Style slist[3];
	int i, mode;

	mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss[key_border_right_color] = slist[i];
			break;
		case SVT_PX:
			ss[key_border_right_width] = slist[i];
			break;
		case SVT_style:
			ss[key_border_right_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderBottom( LCUI_StyleSheet ss, const char *str )
{
	LCUI_Style slist[3];
	int i, mode;

	mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss[key_border_bottom_color] = slist[i];
			break;
		case SVT_PX:
			ss[key_border_bottom_width] = slist[i];
			break;
		case SVT_style:
			ss[key_border_bottom_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderColor( LCUI_StyleSheet ss, const char *str )
{
	return OnParseColor( &ss[key_border_color], str );
}

static int OnParseBorderWidth( LCUI_StyleSheet ss, const char *str )
{
	return OnParseNumber( &ss[key_border_width], str );
}

static int OnParseBorderStyle( LCUI_StyleSheet ss, const char *str )
{
	return OnParseStyleKey( &ss[key_border_style], str );
}

static int OnParsePadding( LCUI_StyleSheet ss, const char *str )
{
	int value_count;
	LCUI_Style s[4];
	
	value_count = SplitValues( str, s, 4, SPLIT_NUMBER );
	switch( value_count ) {
	case 1:
		ss[key_padding_top] = s[0];
		ss[key_padding_right] = s[0];
		ss[key_padding_bottom] = s[0];
		ss[key_padding_left] = s[0];
		break;
	case 2:
		ss[key_padding_top] = s[0];
		ss[key_padding_bottom] = s[0];
		ss[key_padding_left] = s[1];
		ss[key_padding_right] = s[1];
		break;
	case 3:
		ss[key_padding_top] = s[0];
		ss[key_padding_left] = s[1];
		ss[key_padding_right] = s[1];
		ss[key_padding_bottom] = s[2];
		break;
	case 4:
		ss[key_padding_top] = s[0];
		ss[key_padding_left] = s[1];
		ss[key_padding_right] = s[2];
		ss[key_padding_bottom] = s[3];
	default: break;
	}
	return 0;
}

static int OnParseMargin( LCUI_StyleSheet ss, const char *str )
{
	int value_count;
	LCUI_Style s[4];
	
	value_count = SplitValues( str, s, 4, SPLIT_NUMBER );
	switch( value_count ) {
	case 1:
		ss[key_margin_top] = s[0];
		ss[key_margin_right] = s[0];
		ss[key_margin_bottom] = s[0];
		ss[key_margin_left] = s[0];
		break;
	case 2:
		ss[key_margin_top] = s[0];
		ss[key_margin_bottom] = s[0];
		ss[key_margin_left] = s[1];
		ss[key_margin_right] = s[1];
		break;
	case 3:
		ss[key_margin_top] = s[0];
		ss[key_margin_left] = s[1];
		ss[key_margin_right] = s[1];
		ss[key_margin_bottom] = s[2];
		break;
	case 4:
		ss[key_margin_top] = s[0];
		ss[key_margin_left] = s[1];
		ss[key_margin_right] = s[2];
		ss[key_margin_bottom] = s[3];
	default: break;
	}
	return 0;
}

static int OnParseBoxShadow( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

static int OnParseBackground( LCUI_StyleSheet ss, const char *str )
{
	return 0;
}

/** 样式字符串与标识码的映射表 */
static StyleKeyNode style_key_map[] = {
	{ SV_NONE, "none" },
	{ SV_AUTO, "auto" },
	{ SV_CONTAIN, "contain" },
	{ SV_COVER, "cover" },
	{ SV_LEFT, "left" },
	{ SV_CENTER, "center" },
	{ SV_RIGHT, "right" },
	{ SV_TOP, "top" },
	{ SV_TOP_LEFT, "top left" },
	{ SV_TOP_CENTER, "top center" },
	{ SV_TOP_RIGHT, "top right" },
	{ SV_CENTER_LEFT, "center left" },
	{ SV_CENTER_CENTER, "center center" },
	{ SV_CENTER_RIGHT, "center right" },
	{ SV_BOTTOM_LEFT, "bottom left" },
	{ SV_BOTTOM_CENTER, "bottom center" },
	{ SV_BOTTOM_RIGHT, "bottom right" },
	{ SV_SOLID, "solid" },
	{ SV_DOTTED, "dotted" },
	{ SV_DOUBLE, "double" },
	{ SV_DASHED, "dashed" },
	{ SV_CONTENT_BOX, "content-box" },
	{ SV_PADDING_BOX, "padding-box" },
	{ SV_BORDER_BOX, "border-box" },
	{ SV_GRAPH_BOX, "graph-box" }
};

/** 各个样式的解析器映射表 */
static StyleParser style_parser_map[] = {
	{0, key_width, "width", OnParseNumber},
	{0, key_height, "height", OnParseNumber},
	{0, key_background_color, "background-color", OnParseColor},
	{0, key_background_image, "background-image",OnParseImage},
	{0, key_border_top_color, "border-top-color",OnParseColor},
	{0, key_border_right_color, "border-right-color",OnParseColor},
	{0, key_border_bottom_color, "border-bottom-color",OnParseColor},
	{0, key_border_left_color, "border-left-color",OnParseColor},
	{0, key_border_top_width, "border-top-width",OnParseNumber},
	{0, key_border_right_width, "border-right-width",OnParseNumber},
	{0, key_border_bottom_width, "border-bottom-width",OnParseNumber},
	{0, key_border_left_width, "border-left-width",OnParseNumber},
	{0, key_border_top_width, "border-top-width",OnParseNumber},
	{0, key_border_right_width, "border-right-width",OnParseNumber},
	{0, key_border_bottom_width, "border-bottom-width",OnParseNumber},
	{0, key_border_left_width, "border-left-width",OnParseNumber},
	{0, key_border_top_style, "border-top-style",OnParseStyleKey},
	{0, key_border_right_style, "border-right-style",OnParseStyleKey},
	{0, key_border_bottom_style, "border-bottom-style",OnParseStyleKey},
	{0, key_border_left_style, "border-left-style",OnParseStyleKey},
	{1, -1, "border",OnParseBorder},
	{1, -1, "border-left",OnParseBorderLeft},
	{1, -1, "border-top",OnParseBorderTop},
	{1, -1, "border-right",OnParseBorderRight},
	{1, -1, "border-bottom",OnParseBorderBottom},
	{1, -1, "border-color",OnParseBorderColor},
	{1, -1, "border-width",OnParseBorderWidth},
	{1, -1, "border-style",OnParseBorderStyle},
	{1, -1, "padding",OnParsePadding},
	{1, -1, "margin",OnParseMargin},
	{1, -1, "box-shadow",OnParseBoxShadow},
	{1, -1, "background",OnParseBackground}
};

static int CompareParserName( void *data, const void *keydata )
{
	return strcmp(((StyleParser*)data)->name, (const char*)keydata);
}

static int CompareName( void *data, const void *keydata )
{
	return strcmp(((StyleKeyNode*)data)->name, (const char*)keydata);
}

/** 初始化 LCUI 的 CSS 代码解析功能 */
void LCUICssParser_Init(void)
{
	StyleParser *ssp, *ssp_end;
	StyleKeyNode *skn, *skn_end;

	/* 构建一个红黑树树，方便按名称查找解析器 */
	RBTree_Init( &style_parser_tree );
	RBTree_Init( &style_key_tree );
	RBTree_SetDataNeedFree( &style_parser_tree, FALSE );
	RBTree_SetDataNeedFree( &style_key_tree, FALSE );
	RBTree_OnJudge( &style_parser_tree, CompareParserName );
	RBTree_OnJudge( &style_key_tree, CompareName );
	ssp = style_parser_map;
	ssp_end = ssp + sizeof(style_parser_map)/sizeof(StyleParser);
	for( ; ssp < ssp_end; ++ssp ) {
		RBTree_CustomInsert( &style_parser_tree, ssp->name, ssp );
	}
	skn = style_key_map;
	skn_end = skn + sizeof(style_key_map)/sizeof(StyleKeyNode);
	for( ; skn < skn_end; ++skn ) {
		RBTree_CustomInsert( &style_key_tree, skn->name, skn );
	}
}

/** 从字符串中解析出样式，并导入至样式库中 */
int LCUI_ParseStyle( const char *str )
{
	LCUI_Selector s;
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
		StyleParser *parser;	/**< 当前找到的解析器 */
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
			ctx.parser = RBTree_CustomGetData( 
				&style_parser_tree, ctx.buffer
			);
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
					ctx.parser->parse_group( 
						ctx.css, ctx.buffer 
					);
					break;
				}
				ctx.parser->parse( 
					&ctx.css[ctx.parser->key], ctx.buffer
				);
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
