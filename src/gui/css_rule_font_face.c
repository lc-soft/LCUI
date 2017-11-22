/* ***************************************************************************
 * css_rule_font_face.c -- css @font-face rule parser module
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
 * css_rule_font_face.c -- css 的 @font-face 规则解析模块
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

enum FontFaceParserState {
	FFP_STATE_HEAD,
	FFP_STATE_KEY,
	FFP_STATE_KEY_END,
	FFP_STATE_VALUE
};

enum FontFaceKey {
	KEY_NONE,
	KEY_FONT_FAMILY,
	KEY_FONT_STYLE,
	KEY_FONT_WEIGHT,
	KEY_SRC
};

typedef struct FontFaceParserContextRec_ {
	int key;
	LCUI_CSSFontFace face;
	void( *callback )(const LCUI_CSSFontFace);
} FontFaceParserContextRec, *FontFaceParserContext;

#define GetParserContext(CTX) (CTX)->rule.parsers[CSS_RULE_FONT_FACE].data
#define SetParserContext(CTX, DATA) do {\
	(CTX)->rule.parsers[CSS_RULE_FONT_FACE].data = DATA;\
} while( 0 );

static int FontFaceParser_Begin( LCUI_CSSParserContext ctx )
{
	ctx->rule.state = FFP_STATE_HEAD;
	return 0;
}

static int GetFontFaceKey( const char *name )
{
	if( strcmp( name, "font-family" ) == 0 ) {
		return KEY_FONT_FAMILY;
	} else if( strcmp( name, "font-style" ) == 0 ) {
		return KEY_FONT_STYLE;
	} else if( strcmp( name, "font-weight" ) == 0 ) {
		return KEY_FONT_WEIGHT;
	} else if( strcmp( name, "src" ) == 0 ) {
		return KEY_SRC;
	}
	return KEY_NONE;
}

static void FontFaceParser_End( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	data = GetParserContext( ctx );
	if( data->face->font_family ) {
		free( data->face->font_family );
		data->face->font_family = NULL;
	}
	if( data->face->font_style ) {
		free( data->face->font_style );
		data->face->font_style = NULL;
	}
	if( data->face->src ) {
		free( data->face->src );
		data->face->src = NULL;
	}
	data->key = KEY_NONE;
}

static int FontFaceParser_ParseHead( LCUI_CSSParserContext ctx )
{
	switch( *ctx->cur ) {
	CASE_WHITE_SPACE:
		return 0;
	case '/':
		return CSSParser_BeginParseComment( ctx );
	case '{': break;
	default:
		return -1;
	}
	ctx->rule.state = FFP_STATE_KEY;
	CSSParser_EndBuffer( ctx );
	return 0;
}

static int FontFaceParser_ParseTail( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	data = GetParserContext( ctx );
	if( data->callback ) {
		data->callback( data->face );
	}
	FontFaceParser_End( ctx );
	CSSParser_EndParseRuleData( ctx );
	if( ctx->pos > 0 ) {
		return -1;
	}
	return 0;
}

static int FontFaceParser_ParseKeyEnd( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	switch( *ctx->cur ) {
	CASE_WHITE_SPACE:
		return 0;
	case ':':
		break;
	case '}':
		return FontFaceParser_ParseTail( ctx );
	default:
		FontFaceParser_End( ctx ); 
		return -1;
	}
	CSSParser_EndBuffer( ctx );
	data = GetParserContext( ctx );
	data->key = GetFontFaceKey( ctx->buffer );
	ctx->rule.state = FFP_STATE_VALUE;
	return 0;
}

static int FontFaceParser_ParseKey( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	switch( *ctx->cur ) {
	CASE_WHITE_SPACE:
		if( ctx->pos > 0 ) {
			ctx->rule.state = FFP_STATE_KEY_END;
			return 0;
		}
		break;
	case '}':
		return FontFaceParser_ParseTail( ctx );
	case ':':
		FontFaceParser_ParseKeyEnd( ctx );
		break;
	default:
		CSSParser_GetChar( ctx );
		break;
	}
	return 0;
}

static int FontFaceParser_ParseValue( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	switch( *ctx->cur ) {
	case '}':
	case ';':
		break;
	default:
		CSSParser_GetChar( ctx );
		return 0;
	}
	CSSParser_EndBuffer( ctx );
	data = GetParserContext( ctx );
	switch( data->key ) {
	case KEY_FONT_FAMILY:
		if( data->face->font_family ) {
			free( data->face->font_family );
		}
		data->face->font_family = malloc( strsize( ctx->buffer ) );
		if( !data->face->font_family ) {
			return -ENOMEM;
		}
		strtrim( data->face->font_family, ctx->buffer, " \"" );
		break;
	case KEY_FONT_STYLE:
		if( data->face->font_style ) {
			free( data->face->font_style );
		}
		data->face->font_style = malloc( strsize( ctx->buffer ) );
		if( !data->face->font_style ) {
			return -ENOMEM;
		}
		strtrim( data->face->font_style, ctx->buffer, " \"" );
		break;
	case KEY_SRC:
		if( data->face->src ) {
			free( data->face->src );
		}
		data->face->src = malloc( strsize( ctx->buffer ) );
		if( !data->face->src ) {
			return -ENOMEM;
		}
		strtrim( data->face->src, ctx->buffer, NULL );
		break;
	default: break;
	}
	data->key = KEY_NONE;
	if( *ctx->cur != '}' ) {
		ctx->rule.state = FFP_STATE_KEY;
		return 0;
	}
	if( data->callback ) {
		data->callback( data->face );
	}
	FontFaceParser_End( ctx );
	CSSParser_EndParseRuleData( ctx );
	return 0;
}

int FontFaceParser_Parse( LCUI_CSSParserContext ctx )
{
	switch( ctx->rule.state ) {
	case FFP_STATE_HEAD:
		return FontFaceParser_ParseHead( ctx );
	case FFP_STATE_KEY:
		return FontFaceParser_ParseKey( ctx );
	case FFP_STATE_KEY_END:
		return FontFaceParser_ParseKeyEnd( ctx );
	case FFP_STATE_VALUE:
		return FontFaceParser_ParseValue( ctx );
	default: break;
	}
	FontFaceParser_End( ctx );
	return -1;
}

void CSSRuleParser_OnFontFace( LCUI_CSSParserContext ctx,
			       void( *func )(const LCUI_CSSFontFace) )
{
	FontFaceParserContext data;
	data = GetParserContext( ctx );
	data->callback = func;
}

int CSSParser_InitFontFaceRuleParser( LCUI_CSSParserContext ctx )
{
	LCUI_CSSRuleParser parser;
	FontFaceParserContext data;
	parser = CSSParser_GetRuleParser( ctx );
	data = NEW( FontFaceParserContextRec, 1 );
	if( !data ) {
		return -ENOMEM;
	}
	data->face = NEW( LCUI_CSSFontFaceRec, 1 );
	if( !data->face ) {
		free( data );
		return -ENOMEM;
	}
	data->face->src = NULL;
	data->face->font_family = NULL;
	data->face->font_style = NULL;
	data->face->font_weight = 400;
	parser->data = data;
	parser->parse = FontFaceParser_Parse;
	parser->begin = FontFaceParser_Begin;
	strcpy( parser->name, "font-face" );
	return 0;
}

void CSSParser_FreeFontFaceRuleParser( LCUI_CSSParserContext ctx )
{
	FontFaceParserContext data;
	FontFaceParser_End( ctx );
	data = GetParserContext( ctx );
	SetParserContext( ctx, NULL );
	free( data->face );
	free( data );
}
