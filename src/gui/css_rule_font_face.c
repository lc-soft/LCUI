/*
 * css_rule_font_face.c -- CSS @font-face rule parser module
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

static int FontFace_ParseFontWeight( LCUI_CSSFontFace face, const char *str )
{
	int weight;
	if( ParseFontWeight( str, &weight ) ) {
		face->font_weight = weight;
		return 0;
	}
	return -1;
}

static int FontFace_ParseFontStyle( LCUI_CSSFontFace face, const char *str )
{
	int style;
	if( ParseFontStyle( str, &style ) ) {
		face->font_style = style;
		return 0;
	}
	return -1;
}

static int FontFace_ParseSrc( LCUI_CSSFontFace face, const char *str,
			      const char *dirname )
{
	LCUI_StyleRec style;
	if( face->src ) {
		free( face->src );
	}
	if( ParseUrl( &style, str, dirname ) ) {
		face->src = style.val_string;
		return 0;
	}
	free( face->src );
	face->src = NULL;
	return -1;
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
		FontFace_ParseFontStyle( data->face, ctx->buffer );
		break;
	case KEY_FONT_WEIGHT:
		FontFace_ParseFontWeight( data->face, ctx->buffer );
		break;
	case KEY_SRC:
		FontFace_ParseSrc( data->face, ctx->buffer,
				   ctx->style.dirname );
		break;
	default: break;
	}
	data->key = KEY_NONE;
	if( *ctx->cur != '}' ) {
		ctx->rule.state = FFP_STATE_KEY;
		return 0;
	}
	return FontFaceParser_ParseTail( ctx );
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
	data->face->font_style = FONT_STYLE_NORMAL;
	data->face->font_weight = FONT_WEIGHT_NORMAL;
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
