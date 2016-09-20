/* ***************************************************************************
 * css_parser.c -- css code parser module
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * css_parser.c -- css 样式代码解析模块
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

#define SPLIT_NUMBER	1
#define SPLIT_COLOR	(1<<1)
#define SPLIT_STYLE	(1<<2)

#define LEN(A) sizeof( A ) / sizeof( *A )

/** 解析器的环境参数（上下文数据） */
typedef struct CSSParserContextRec_ {
	enum {
		TARGET_NONE,		/**< 无 */
		TARGET_SELECTOR,	/**< 选择器 */
		TARGET_KEY,		/**< 属性名 */
		TARGET_VALUE,		/**< 属性值 */
		TARGET_COMMENT		/**< 注释 */
	} target, target_bak;		/**< 当前解析中的目标，以及当前备份的目标 */
	LCUI_BOOL is_line_comment;	/**< 是否为单行注释 */
	char *buffer;			/**< 缓存中的字符串 */
	size_t buffer_size;		/**< 缓存区大小 */
	int pos;			/**< 缓存中的字符串的下标位置 */
	const char *ptr;		/**< 用于遍历字符串的指针 */
	LinkedList selectors;		/**< 当前匹配到的选择器列表 */
	LCUI_StyleSheet css;		/**< 当前缓存的样式表 */
	LCUI_StyleParser parser;	/**< 当前找到的解析器 */
	char *space;			/**< 样式记录所属的空间 */
} CSSParserContextRec, *CSSParserContext;

static struct CSSParserModule {
	int count;
	DictType dicttype;	/**< 解析器表的字典类型数据 */
	Dict *parsers;		/**< 解析器表，以名称进行索引 */
} self;

static int SplitValues( const char *str, LCUI_Style slist,
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
		if( strcmp( values[vj], "auto" ) == 0 ) {
			slist[vj].type = SVT_AUTO;
			slist[vj].value = SV_AUTO;
			slist[vj].is_changed = TRUE;
			slist[vj].is_valid = TRUE;
			continue;
		}
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
			val = LCUI_GetStyleValue( values[vj] );
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

static int OnParseValue( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[key];
	if( sscanf( str, "%d", &s->value ) == 1 ) {
		s->is_valid = TRUE;
		s->is_changed = TRUE;
		s->type = SVT_VALUE;
		return 0;
	}
	return -1;
}

static int OnParseNumber( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[key];
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

static int OnParseBoolean( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[key];
	if( strcasecmp(str, "true") == 0 ) {
		s->is_valid = TRUE;
		s->type = SVT_BOOL;
		s->value = TRUE;
		return 0;
	} else if( strcasecmp(str, "false") == 0 ) {
		s->is_valid = TRUE;
		s->type = SVT_BOOL;
		s->value = FALSE;
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
	LCUI_Style s = &ss->sheet[key];
	if( ParseColor( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseImage( LCUI_StyleSheet ss, int key, const char *str )
{
	char *data;
	int n;
	const char *p, *head, *tail;

	p = str;
	data = malloc( strlen(str) * sizeof(char) );
	tail = head = strstr( p, "url(" );
	while( p ) {
		tail = p;
		p = strstr( p+1, ")" );
	}
	if( tail == head ) {
		return -1;
	}
	head += 4;
	if( *head == '"' ) {
		++head;
	}
	n = tail - head;
	strncpy( data, head, n );
	data[n] = 0;
	if( n > 0 && data[n - 1] == '"' ) {
		data[n - 1] = 0;
	}
	SetStyle( ss, key, data, string );
	return 0;
}

static int OnParseStyleOption( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[key];
	int v = LCUI_GetStyleValue( str );
	if( v < 0 ) {
		return -1;
	}
	s->style = v;
	s->type = SVT_STYLE;
	s->is_valid = TRUE;
	s->is_changed = TRUE;
	return 0;
}

static int OnParseBorder( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
	if( SplitValues(str, slist, 3, mode) < 3 ) {
		return -1;
	}
	for( i = 0; i < 3; ++i ) {
		switch( slist[i].type ) {
		case SVT_COLOR:
			ss->sheet[key_border_top_color] = slist[i];
			ss->sheet[key_border_right_color] = slist[i];
			ss->sheet[key_border_bottom_color] = slist[i];
			ss->sheet[key_border_left_color] = slist[i];
			break;
		case SVT_PX:
			ss->sheet[key_border_top_width] = slist[i];
			ss->sheet[key_border_right_width] = slist[i];
			ss->sheet[key_border_bottom_width] = slist[i];
			ss->sheet[key_border_left_width] = slist[i];
			break;
		case SVT_style:
			ss->sheet[key_border_top_style] = slist[i];
			ss->sheet[key_border_right_style] = slist[i];
			ss->sheet[key_border_bottom_style] = slist[i];
			ss->sheet[key_border_left_style] = slist[i];
			break;
		default: return -1;
		}
	}
	return 0;
}

static int OnParseBorderLeft( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
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
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
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
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
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
	LCUI_StyleRec slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;
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
	LCUI_StyleRec s;
	if( !ParseColor( &s, str ) ) {
		return -1;
	}
	ss->sheet[key_border_top_color] = s;
	ss->sheet[key_border_right_color] = s;
	ss->sheet[key_border_bottom_color] = s;
	ss->sheet[key_border_left_color] = s;
	return 0;
}

static int OnParseBorderWidth( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec s;
	if( !ParseNumber( &s, str ) ) {
		return -1;
	}
	ss->sheet[key_border_top_width] = s;
	ss->sheet[key_border_right_width] = s;
	ss->sheet[key_border_bottom_width] = s;
	ss->sheet[key_border_left_width] = s;
	return 0;
}

static int OnParseBorderStyle( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec s;
	s.is_valid = TRUE;
	s.is_changed = TRUE;
	s.val_style = LCUI_GetStyleValue( str );
	if( s.val_style < 0 ) {
		return -1;
	}
	ss->sheet[key_border_top_style] = s;
	ss->sheet[key_border_right_style] = s;
	ss->sheet[key_border_bottom_style] = s;
	ss->sheet[key_border_left_style] = s;
	return 0;
}

static int OnParsePadding( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec s[4];
	switch( SplitValues( str, s, 4, SPLIT_NUMBER ) ) {
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
	LCUI_StyleRec s[4];
	switch( SplitValues( str, s, 4, SPLIT_NUMBER ) ) {
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
	LCUI_StyleRec s[5];
	switch( SplitValues( str, s, 5, SPLIT_NUMBER | SPLIT_COLOR ) ) {
	case 5:
		ss->sheet[key_box_shadow_x] = s[0];
		ss->sheet[key_box_shadow_y] = s[1];
		ss->sheet[key_box_shadow_blur] = s[2];
		ss->sheet[key_box_shadow_spread] = s[3];
		ss->sheet[key_box_shadow_color] = s[4];
		break;
	case 4:
		ss->sheet[key_box_shadow_x] = s[0];
		ss->sheet[key_box_shadow_y] = s[1];
		ss->sheet[key_box_shadow_blur] = s[2];
		ss->sheet[key_box_shadow_color] = s[3];
		break;
	default: return -1;
	}
	return 0;
}

static int OnParseBackground( LCUI_StyleSheet ss, int key, const char *str )
{
	return 0;
}

static int OnParseBackgroundPosition( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec slist[2];
	int ret = OnParseStyleOption( ss, key, str );
	if( ret == 0 ) {
		return 0;
	}
	if( SplitValues( str, slist, 2, SPLIT_NUMBER ) == 2 ) {
		ss->sheet[key_background_position_x] = slist[0];
		ss->sheet[key_background_position_y] = slist[1];
		return 0;
	}
	return -2;
}

static int OnParseBackgroundSize( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec slist[2];
	int ret = OnParseStyleOption( ss, key, str );
	if( ret == 0 ) {
		return 0;
	}
	ret = SplitValues( str, slist, 2, SPLIT_NUMBER | SPLIT_STYLE );
	if( ret != 2 ) {
		return -1;
	}
	ss->sheet[key_background_size_width] = slist[0];
	ss->sheet[key_background_size_height] = slist[1];
	return 0;
}


/** 各个样式的解析器映射表 */
static LCUI_StyleParserRec style_parser_map[] = {
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
	{ key_vertical_align, NULL, OnParseStyleOption },
	{ key_display, NULL, OnParseStyleOption },
	{ key_background_color, NULL, OnParseColor },
	{ key_background_image, NULL, OnParseImage },
	{ key_background_position, NULL, OnParseBackgroundPosition },
	{ key_background_size, NULL, OnParseBackgroundSize },
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

static CSSParserContext NewCSSParserContext( size_t buffer_size, 
					     const char *space )
{
	CSSParserContext ctx = NEW(CSSParserContextRec, 1);
	LinkedList_Init( &ctx->selectors );
	ctx->buffer = (char*)malloc( sizeof(char) * buffer_size );
	ctx->buffer_size = buffer_size;
	ctx->target_bak = TARGET_NONE;
	ctx->target = TARGET_NONE;
	ctx->space = space ? strdup( space ): NULL;
	return ctx;
}

static void DeleteCSSParserContext( CSSParserContext *ctx_ptr )
{
	CSSParserContext ctx = *ctx_ptr;
	LinkedList_Clear( &ctx->selectors, (FuncPtr)Selector_Delete );
	free( ctx->buffer );
	free( ctx );
	*ctx_ptr = NULL;
}

/** 载入CSS代码块，用于实现CSS代码的分块载入 */
static int LCUI_LoadCSSBlock( CSSParserContext ctx, const char *str )
{
	size_t size = 0;
	LCUI_Selector s;
	LinkedListNode *node;

	ctx->ptr = str;
	for( ; *ctx->ptr && size < ctx->buffer_size; ++ctx->ptr, ++size ) {
		switch( ctx->target ) {
		case TARGET_SELECTOR:
			switch( *ctx->ptr ) {
			case '/': goto proc_comment;
			case '{':
				ctx->target = TARGET_KEY;
				ctx->css = StyleSheet();
			case ',':
				ctx->buffer[ctx->pos] = 0;
				ctx->pos = 0;
				DEBUG_MSG("selector: %s\n", ctx->buffer);
				s = Selector( ctx->buffer );
				if( !s ) {
					// 解析出错 ...
					break;
				}
				LinkedList_Append( &ctx->selectors, s );
				break;
			default:
				ctx->buffer[ctx->pos++] = *ctx->ptr;
				break;
			}
			break;
		case TARGET_KEY:
			switch( *ctx->ptr ) {
			case '/': goto proc_comment;
			case ' ':
			case '\n':
			case '\r':
			case '\t':
			case ';':
				ctx->pos = 0;
				continue;
			case ':': break;
			case '}':
				ctx->target = TARGET_NONE;
				goto put_css;
			default:
				ctx->buffer[ctx->pos++] = *ctx->ptr;
				continue;
			}
			goto select_parser;
		case TARGET_VALUE:
			switch( *ctx->ptr ) {
			case '/': goto proc_comment;
			case '}':
			case ';':
				goto parse_value;
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				if( ctx->pos == 0 ) {
					continue;
				}
			default:
				ctx->buffer[ctx->pos++] = *ctx->ptr;
				continue;
			}
			break;
		case TARGET_COMMENT:
			if( ctx->is_line_comment ) {
				if( *ctx->ptr == '\n' ) {
					ctx->target = ctx->target_bak;
				}
				break;
			}
			if( *ctx->ptr == '/' && *(ctx->ptr - 1) == '*' ) {
				ctx->target = ctx->target_bak;
			}
			break;
		case TARGET_NONE:
		default:
			switch( *ctx->ptr ) {
			case '/': goto proc_comment;
			case '\n':
			case '\t':
			case '\r':
			case ' ':
			case ',':
			case '{':
			case '\\':
			case '"':
			case '}': continue;
			default: break;
			}
			ctx->pos = 0;
			ctx->buffer[ctx->pos++] = *ctx->ptr;
			ctx->target = TARGET_SELECTOR;
			break;
		}
		continue;
proc_comment:
		switch( *(ctx->ptr + 1) ) {
		case '/': ctx->is_line_comment = TRUE; break;
		case '*': ctx->is_line_comment = FALSE; break;
		default:
			ctx->buffer[ctx->pos++] = *ctx->ptr;
			continue;
		}
		if( ctx->target_bak != TARGET_COMMENT ) {
			ctx->target_bak = ctx->target;
			ctx->target = TARGET_COMMENT;
		}
		continue;
put_css:
		DEBUG_MSG("put css\n");
		/* 将记录的样式表添加至匹配到的选择器中 */
		for( LinkedList_Each( node, &ctx->selectors ) ) {
			LCUI_PutStyleSheet( node->data, ctx->css, ctx->space );
		}
		LinkedList_Clear( &ctx->selectors, (FuncPtr)Selector_Delete );
		StyleSheet_Delete( ctx->css );
		continue;
select_parser:
		ctx->target = TARGET_VALUE;
		ctx->buffer[ctx->pos] = 0;
		ctx->pos = 0;
		ctx->parser = Dict_FetchValue( self.parsers, ctx->buffer );
		DEBUG_MSG("select style: %s, parser: %p\n",
			   ctx->buffer, ctx->parser);
		continue;
parse_value:
		if( *ctx->ptr == ';' ) {
			ctx->target = TARGET_KEY;
		}
		if( !ctx->parser ) {
			continue;
		}
		ctx->buffer[ctx->pos] = 0;
		ctx->pos = 0;
		ctx->parser->parse( ctx->css, ctx->parser->key, ctx->buffer );
		DEBUG_MSG("parse style value: %s, result: %d\n", ctx->buffer);
		if( *ctx->ptr == '}' ) {
			ctx->target = TARGET_NONE;
			goto put_css;
		}
	}
	return size;
}

/** 从文件中载入CSS样式数据，并导入至样式库中 */
int LCUI_LoadCSSFile( const char *filepath )
{
	int n;
	FILE *fp;
	char buff[512];
	CSSParserContext ctx;

	fp = fopen( filepath, "r" );
	if( !fp ) {
		return -1;
	}
	ctx = NewCSSParserContext( 512, filepath );
	n = fread( buff, 1, 511, fp );
	while( n > 0 ) {
		buff[n] = 0;
		LCUI_LoadCSSBlock( ctx, buff );
		n = fread( buff, 1, 511, fp );
	}
	DeleteCSSParserContext( &ctx );
	fclose( fp );
	return 0;
}

int LCUI_LoadCSSString( const char *str, const char *space )
{
	int len = 1;
	const char *cur;
	CSSParserContext ctx;
	DEBUG_MSG("parse begin\n");
	ctx = NewCSSParserContext( 512, space );
	for( cur = str; len > 0; cur += len ) {
		len = LCUI_LoadCSSBlock( ctx, cur );
	}
	DeleteCSSParserContext( &ctx );
	DEBUG_MSG("parse end\n");
	return 0;
}

int LCUI_AddCSSParser( LCUI_StyleParser sp )
{
	LCUI_StyleParser new_sp;
	if( !sp->name || strlen( sp->name ) < 1 ) {
		return -1;
	}
	if( Dict_FetchValue( self.parsers, sp->name ) ) {
		return -2;
	}
	self.count += 1;
	new_sp = NEW( LCUI_StyleParserRec, 1 );
	new_sp->key = sp->key;
	new_sp->parse = sp->parse;
	new_sp->name = strdup( sp->name );
	Dict_Add( self.parsers, new_sp->name, new_sp );
	return 0;
}

static void DestroyStyleParser( void *privdata, void *val )
{
	LCUI_StyleParser sp = val;
	free( sp->name );
	free( sp );
}

/** 初始化 LCUI 的 CSS 代码解析功能 */
void LCUI_InitCSSParser( void )
{
	LCUI_StyleParser new_sp, sp, sp_end;

	self.count = 0;
	self.dicttype = DictType_StringKey;
	self.dicttype.valDestructor = DestroyStyleParser;
	self.parsers = Dict_Create( &self.dicttype, NULL );
	sp_end = style_parser_map + LEN( style_parser_map );
	for( sp = style_parser_map; sp < sp_end; ++sp ) {
		new_sp = malloc( sizeof( LCUI_StyleParserRec ) );
		new_sp->key = sp->key;
		new_sp->parse = sp->parse;
		if( !sp->name && sp->key >= 0 ) {
			const char *name = LCUI_GetStyleName( sp->key );
			if( !name ) {
				free( new_sp );
				continue;
			}
			new_sp->name = strdup( name );
		} else {
			new_sp->name = strdup( sp->name );
		}
		Dict_Add( self.parsers, new_sp->name, new_sp );
	}
}

void LCUI_ExitCSSParser( void )
{
	Dict_Release( self.parsers );
}
