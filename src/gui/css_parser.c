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
//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/parse.h>
#include <LCUI/gui/widget.h>

/** 解析器的环境参数（上下文数据） */
typedef struct LCUI_ParserContext {
	enum {
		is_none,		/**< 无 */
		is_selector,		/**< 选择器 */
		is_key,			/**< 属性名 */
		is_value,		/**< 属性值 */
		is_comment		/**< 注释 */
	} target;			/**< 当前解析中的目标 */
	char buffer[256];		/**< 缓存中的字符串 */
	int pos;			/**< 缓存中的字符串的下标位置 */
	const char *ptr;		/**< 用于遍历字符串的指针 */
	LinkedList selectors;		/**< 当前匹配到的选择器列表 */
	LCUI_StyleSheet css;		/**< 当前缓存的样式表 */
	LCUI_StyleParser *parser;	/**< 当前找到的解析器 */
} LCUI_ParserContext;

/** 样式字符串值与标识码 */
typedef struct KeyNameGroup {
	int key;
	char *name;
} KeyNameGroup;

static struct {
	LCUI_RBTree parser_tree;
	LCUI_RBTree option_tree;
	LCUI_RBTree name_tree;
	int count;
} self;

#define SPLIT_NUMBER	1
#define SPLIT_COLOR	(1<<1)
#define SPLIT_STYLE	(1<<2)

int ParseStyleOption( const char *str )
{ 
	LCUI_RBTreeNode *node;
	node = RBTree_CustomSearch( &self.option_tree, str );
	if( !node ) {
		return -1;
	}
	return ((KeyNameGroup*)(node->data))->key;
}

const char *GetStyleName( int key )
{
	return RBTree_GetData( &self.name_tree, key );
}

static int SplitValues( const char *str, LCUI_Style *slist, 
			int max_len, int mode )
{
	int val, vi = 0, vj = 0;
	char **values;
	const char *p;

	values = (char**)calloc(max_len, sizeof(char*));
	values[0] = (char*)malloc(sizeof(char)*64);
	for( p = str; *p; ++p ) {
		if( *p != ' ' ) {
			values[vi][vj++] = *p;
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
	vi++;
	for( vj = 0; vj < vi; ++vj ) {
		DEBUG_MSG("[%d] %s\n", vj, values[vj]);
		if( mode & SPLIT_NUMBER ) {
			if( ParseNumber( &slist[vj], values[vj] ) ) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if( mode & SPLIT_COLOR ) {
			if( ParseColor(&slist[vj], values[vj]) ) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if( mode & SPLIT_STYLE ) {
			val = ParseStyleOption( values[vj] );
			if( val > 0 )  {
				slist[vj].style = val;
				slist[vj].type = SVT_style;
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
	for( vj = 0; vj < max_len; ++vj ) {
		values[vj] ? free(values[vj]) : 0;
	}
	free( values );
	return vi;
}

static int OnParseNumber( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style *s = &ss->sheet[key];
	if( ParseNumber( s, str ) ) {
		return 0;
	}
	if( strcmp("auto", str) == 0 ) {
		s->is_valid = TRUE;
		s->type = SVT_AUTO;
		s->style = SV_AUTO;
		return 0;
	}
	return -1;
}

static int OnParseColor( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style *s = &ss->sheet[key];
	if( ParseColor( s, str ) ) {
		return 0;
	}
	if( strcmp("transparent", str) == 0 ) {
		s->is_valid = TRUE;
		s->color = ARGB(0,255,255,255);
		s->type = SVT_COLOR;
		return 0;
	}
	return -1;
}

static int OnParseImage( LCUI_StyleSheet ss, int key, const char *str )
{
	return 0;
}

static int OnParseStyleOption( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style *s = &ss->sheet[key];
	int v = ParseStyleOption( str );
	if( v < 0 ) {
		return -1;
	}
	s->type = SVT_style;
	s->style = v;
	s->is_valid = TRUE;
	s->is_changed = TRUE;
	return 0;
}

static int OnParseBorder( LCUI_StyleSheet ss, int key, const char *str )
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
			ss->sheet[key_border_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderLeft( LCUI_StyleSheet ss, int key, const char *str )
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
			ss->sheet[key_border_left_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_left_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_left_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderTop( LCUI_StyleSheet ss, int key, const char *str )
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
			ss->sheet[key_border_top_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_top_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_top_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderRight( LCUI_StyleSheet ss, int key, const char *str )
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
			ss->sheet[key_border_right_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_right_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_right_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderBottom( LCUI_StyleSheet ss, int key, const char *str )
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
			ss->sheet[key_border_bottom_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_bottom_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_bottom_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderColor( LCUI_StyleSheet ss, int key, const char *str )
{
	return OnParseColor( ss, key_border_color, str );
}

static int OnParseBorderWidth( LCUI_StyleSheet ss, int key, const char *str )
{
	return OnParseNumber( ss, key_border_width, str );
}

static int OnParseBorderStyle( LCUI_StyleSheet ss, int key, const char *str )
{
	return OnParseStyleOption( ss, key_border_style, str );
}

static int OnParsePadding( LCUI_StyleSheet ss, int key, const char *str )
{
	int value_count;
	LCUI_Style s[4];
	
	value_count = SplitValues( str, s, 4, SPLIT_NUMBER );
	switch( value_count ) {
	case 1:
		ss->sheet[key_padding_top] = s[0];
		ss->sheet[key_padding_right] = s[0];
		ss->sheet[key_padding_bottom] = s[0];
		ss->sheet[key_padding_left] = s[0];
		break;
	case 2:
		ss->sheet[key_padding_top] = s[0];
		ss->sheet[key_padding_bottom] = s[0];
		ss->sheet[key_padding_left] = s[1];
		ss->sheet[key_padding_right] = s[1];
		break;
	case 3:
		ss->sheet[key_padding_top] = s[0];
		ss->sheet[key_padding_left] = s[1];
		ss->sheet[key_padding_right] = s[1];
		ss->sheet[key_padding_bottom] = s[2];
		break;
	case 4:
		ss->sheet[key_padding_top] = s[0];
		ss->sheet[key_padding_right] = s[1];
		ss->sheet[key_padding_bottom] = s[2];
		ss->sheet[key_padding_left] = s[3];
	default: break;
	}
	return 0;
}

static int OnParseMargin( LCUI_StyleSheet ss, int key, const char *str )
{
	int value_count;
	LCUI_Style s[4];
	
	value_count = SplitValues( str, s, 4, SPLIT_NUMBER );
	switch( value_count ) {
	case 1:
		ss->sheet[key_margin_top] = s[0];
		ss->sheet[key_margin_right] = s[0];
		ss->sheet[key_margin_bottom] = s[0];
		ss->sheet[key_margin_left] = s[0];
		break;
	case 2:
		ss->sheet[key_margin_top] = s[0];
		ss->sheet[key_margin_bottom] = s[0];
		ss->sheet[key_margin_left] = s[1];
		ss->sheet[key_margin_right] = s[1];
		break;
	case 3:
		ss->sheet[key_margin_top] = s[0];
		ss->sheet[key_margin_left] = s[1];
		ss->sheet[key_margin_right] = s[1];
		ss->sheet[key_margin_bottom] = s[2];
		break;
	case 4:
		ss->sheet[key_margin_top] = s[0];
		ss->sheet[key_margin_right] = s[1];
		ss->sheet[key_margin_bottom] = s[2];
		ss->sheet[key_margin_left] = s[3];
	default: break;
	}
	return 0;
}

static int OnParseBoxShadow( LCUI_StyleSheet ss, int key, const char *str )
{
	return 0;
}

static int OnParseBackground( LCUI_StyleSheet ss, int key, const char *str )
{
	return 0;
}

static KeyNameGroup style_name_map[] = {
	{ key_visible, "visible"},
	{ key_width, "width" },
	{ key_height, "height" },
	{ key_display, "display" },
	{ key_position, "position" },
	{ key_background_color, "background-color" },
	{ key_background_image, "background-image" },
	{ key_border_color, "border-color" },
	{ key_border_width, "border-width" },
	{ key_border_style, "border-style" },
	{ key_padding_left, "padding-left" },
	{ key_padding_right, "padding-right" },
	{ key_padding_top, "padding-top" },
	{ key_padding_bottom, "padding-bottom" },
	{ key_margin_left, "margin-left" },
	{ key_margin_right, "margin-right" },
	{ key_margin_top, "margin-top" },
	{ key_margin_bottom, "margin-bottom" },
	{ key_border_top_color, "border-top-color" },
	{ key_border_right_color, "border-right-color" },
	{ key_border_bottom_color, "border-bottom-color" },
	{ key_border_left_color, "border-left-color" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_style, "border-top-style" },
	{ key_border_right_style, "border-right-style" },
	{ key_border_bottom_style, "border-bottom-style" },
	{ key_border_left_style, "border-left-style" }
};

/** 样式字符串与标识码的映射表 */
static KeyNameGroup style_option_map[] = {
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
	{ SV_GRAPH_BOX, "graph-box" },
	{ SV_STATIC, "static" },
	{ SV_ABSOLUTE, "absolute" },
	{ SV_BLOCK, "block" },
	{ SV_INLINE_BLOCK, "inline-block" }
};

/** 各个样式的解析器映射表 */
static LCUI_StyleParser style_parser_map[] = {
	{ key_width, NULL, OnParseNumber },
	{ key_height, NULL, OnParseNumber },
	{ key_position, NULL, OnParseStyleOption },
	{ key_display, NULL, OnParseStyleOption },
	{ key_background_color, NULL, OnParseColor },
	{ key_background_image, NULL, OnParseImage },
	{ key_border_top_color, NULL, OnParseColor },
	{ key_border_right_color, NULL, OnParseColor },
	{ key_border_bottom_color, NULL, OnParseColor },
	{ key_border_left_color, NULL, OnParseColor },
	{ key_border_top_width, NULL, OnParseNumber },
	{ key_border_right_width, NULL, OnParseNumber },
	{ key_border_bottom_width, NULL, OnParseNumber },
	{ key_border_left_width, NULL, OnParseNumber },
	{ key_border_top_width, NULL, OnParseNumber },
	{ key_border_right_width, NULL, OnParseNumber },
	{ key_border_bottom_width, NULL, OnParseNumber },
	{ key_border_left_width, NULL, OnParseNumber },
	{ key_border_top_style, NULL, OnParseStyleOption },
	{ key_border_right_style, NULL, OnParseStyleOption },
	{ key_border_bottom_style, NULL, OnParseStyleOption },
	{ key_border_left_style, NULL, OnParseStyleOption },
	{ -1, "border", OnParseBorder },
	{ -1, "border-left", OnParseBorderLeft },
	{ -1, "border-top", OnParseBorderTop },
	{ -1, "border-right", OnParseBorderRight },
	{ -1, "border-bottom", OnParseBorderBottom },
	{ -1, "border-color", OnParseBorderColor },
	{ -1, "border-width", OnParseBorderWidth },
	{ -1, "border-style", OnParseBorderStyle },
	{ -1, "padding", OnParsePadding },
	{ -1, "margin", OnParseMargin },
	{ -1, "box-shadow", OnParseBoxShadow },
	{ -1, "background", OnParseBackground }
};

static int CompareParserName( void *data, const void *keydata )
{
	return strcmp(((LCUI_StyleParser*)data)->name, (const char*)keydata);
}

static int CompareName( void *data, const void *keydata )
{
	return strcmp(((KeyNameGroup*)data)->name, (const char*)keydata);
}

/** 从字符串中解析出样式，并导入至样式库中 */
int LCUI_ParseStyle( const char *str )
{
	LCUI_Selector s;
	LCUI_ParserContext ctx = { 0 };

	DEBUG_MSG("parse begin\n");
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
				DEBUG_MSG("selector: %s\n", ctx.buffer);
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
			case ';': 
				ctx.pos = 0;
				continue;
			case ':': break;
			case '}':
				ctx.target = is_none;
				goto put_css;
			default:
				ctx.buffer[ctx.pos++] = *ctx.ptr;
				continue;
			}
			goto select_parser;
		case is_value:
			switch( *ctx.ptr ) {
			case '}':
			case ';':
				goto parse_value;
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
			ctx.buffer[ctx.pos++] = *ctx.ptr;
			ctx.target = is_selector;
			break;
		}
		continue;
put_css:
		DEBUG_MSG("put css\n");
		/* 将记录的样式表添加至匹配到的选择器中 */
		LinkedList_ForEach( s, 0, &ctx.selectors ) {
			LCUI_PutStyle( s, ctx.css );
		}
		LinkedList_Destroy( &ctx.selectors );
		DeleteStyleSheet( &ctx.css );
		continue;
select_parser:	
		ctx.target = is_value;
		ctx.buffer[ctx.pos] = 0;
		ctx.pos = 0;
		ctx.parser = RBTree_CustomGetData( &self.parser_tree, 
						   ctx.buffer );
		DEBUG_MSG("select style: %s, parser: %p\n",
			   ctx.buffer, ctx.parser);
		continue;
parse_value:
		if( *ctx.ptr == ';' ) {
			ctx.target = is_key;
		}
		if( !ctx.parser ) {
			continue;
		}
		ctx.buffer[ctx.pos] = 0;
		ctx.pos = 0;
		ctx.parser->parse( ctx.css, ctx.parser->key, ctx.buffer );
		DEBUG_MSG("parse style value: %s, result: %d\n", ctx.buffer);
		if( *ctx.ptr == '}' ) {
			ctx.target = is_none;
			goto put_css;
		}
	}
	DEBUG_MSG("parse end\n");
	return 0;
}

/** 获取当前记录的样式属性的总数 */
int LCUI_GetStyleTotal(void)
{
	return self.count;
}

/** 注册新的属性和对应的属性值解析器 */
int LCUICssParser_Register( LCUI_StyleParser *sp )
{
	int key;
	LCUI_StyleParser *new_sp;
	if( !sp->name || strlen(sp->name) < 1 ) {
		return -1;
	}
	if( RBTree_CustomSearch(&self.name_tree, sp->name) ) {
		return -2;
	}
	key = self.count++;
	new_sp = calloc(1, sizeof(LCUI_StyleParser));
	new_sp->key = sp->key;
	new_sp->parse = sp->parse;
	new_sp->name = strdup( sp->name );
	RBTree_Insert( &self.name_tree, key, new_sp->name );
	RBTree_CustomInsert( &self.parser_tree, new_sp->name, new_sp );
	return key;
}

/** 初始化 LCUI 的 CSS 代码解析功能 */
void LCUICssParser_Init(void)
{
	LCUI_StyleParser *new_sp, *sp, *sp_end;
	KeyNameGroup *skn, *skn_end;

	/* 构建一个红黑树，方便按名称查找解析器 */
	RBTree_Init( &self.parser_tree );
	RBTree_Init( &self.option_tree );
	RBTree_Init( &self.name_tree );
	RBTree_SetDataNeedFree( &self.parser_tree, TRUE );
	RBTree_SetDataNeedFree( &self.option_tree, FALSE );
	RBTree_SetDataNeedFree( &self.name_tree, FALSE );
	RBTree_OnJudge( &self.parser_tree, CompareParserName );
	RBTree_OnJudge( &self.option_tree, CompareName );
	skn = style_name_map;
	skn_end = skn + sizeof(style_name_map)/sizeof(KeyNameGroup);
	for( ; skn < skn_end; ++skn ) {
		RBTree_Insert( &self.name_tree, skn->key, skn->name );
	}
	sp = style_parser_map;
	sp_end = sp + sizeof(style_parser_map)/sizeof(LCUI_StyleParser);
	for( ; sp < sp_end; ++sp ) {
		new_sp = malloc(sizeof(LCUI_StyleParser));
		new_sp->key = sp->key;
		new_sp->parse = sp->parse;
		if( !sp->name && sp->key >= 0 ) {
			new_sp->name = strdup( GetStyleName(sp->key) );
		} else {
			new_sp->name = strdup( sp->name );
		}
		RBTree_CustomInsert( &self.parser_tree, new_sp->name, new_sp );
	}
	skn = style_option_map;
	skn_end = skn + sizeof(style_option_map)/sizeof(KeyNameGroup);
	for( ; skn < skn_end; ++skn ) {
		RBTree_CustomInsert( &self.option_tree, skn->name, skn );
	}
	self.count = STYLE_KEY_TOTAL;
}

void LCUICssParser_Destroy(void)
{

}
