/*
 * css_fontstyle.c -- CSS font style parse and operation set.
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


#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>

#define DEFAULT_FONT_SIZE	14
#define DEFAULT_COLOR		0xff333333
#define MIN_FONT_SIZE		12
#define LINE_HEIGHT_SCALE	1.42857143

#define GetFontStyle(CTX) &(CTX)->sheet->sheet[self.keys[(CTX)->parser->key]]
#define SetFontStyle(CTX, V, T) do {\
	SetStyle((CTX)->sheet, self.keys[(CTX)->parser->key], V, T );\
} while( 0 )

enum FontStyleType {
	FS_NORMAL,
	FS_ITALIC,
	FS_OBLIQUE
};

typedef void (*StyleHandler)( LCUI_CSSFontStyle, LCUI_Style );

static struct LCUI_CSSFontStyleModule {
	int keys[TOTAL_FONT_STYLE_KEY];
	StyleHandler handlers[TOTAL_FONT_STYLE_KEY];
} self;


static int unescape( const wchar_t *instr, wchar_t *outstr )
{
	int i = -1;
	wchar_t buff[6];
	wchar_t *pout = outstr;
	const wchar_t *pin = instr;

	for( ; *pin; ++pin ) {
		if( i >= 0 ) {
			buff[i++] = *pin;
			if( i >= 4 ) {
				buff[i] = 0;
				swscanf( buff, L"%hx", pout );
				++pout;
				i = -1;
			}
			continue;
		}
		if( *pin == L'\\' ) {
			i = 0;
			continue;
		}
		*pout++ = *pin;
	}
	if( i >= 4 ) {
		buff[i] = 0;
		swscanf( buff, L"%hx", pout );
		++pout;
	}
	*pout = 0;
	return pout - outstr;
}

static int OnParseContent( LCUI_CSSParserStyleContext ctx, const char *str )
{
	int i;
	wchar_t *content;
	size_t len = strlen( str ) + 1;
	content = malloc( len * sizeof( wchar_t ) );
	LCUI_DecodeString( content, str, (int)len, ENCODING_UTF8 );
	if( content[0] == '"' ) {
		for( i = 0; content[i + 1]; ++i ) {
			content[i] = content[i + 1];
		}
		if( content[i - 1] != '"' ) {
			free( content );
			return -1;
		}
		content[i - 1] = 0;
	}
	unescape( content, content );
	SetFontStyle( ctx, content, wstring );
	return 0;
}

static int OnParseColor( LCUI_CSSParserStyleContext ctx, const char *str )
{
	LCUI_Style s = GetFontStyle( ctx );
	if( ParseColor( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontSize( LCUI_CSSParserStyleContext ctx, const char *str )
{
	LCUI_Style s = GetFontStyle( ctx );
	if( ParseNumber( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontFamily( LCUI_CSSParserStyleContext ctx, const char *str )
{
	char *name = strdup2( str );
	LCUI_Style s = GetFontStyle( ctx );
	if( s->is_valid && s->string ) {
		free( s->string );
	}
	SetFontStyle( ctx, name, string );
	return 0;
}

static int OnParseFontStyle( LCUI_CSSParserStyleContext ctx, const char *str )
{
	int style;
	if( ParseFontStyle( str, &style ) ) {
		SetFontStyle( ctx, style, int );
		return 0;
	}
	return -1;
}

static int OnParseFontWeight( LCUI_CSSParserStyleContext ctx, const char *str )
{
	int weight;
	if( ParseFontWeight( str, &weight ) ) {
		SetFontStyle( ctx, weight, int );
		return 0;
	}
	return -1;
}

static int OnParseTextAlign( LCUI_CSSParserStyleContext ctx, const char *str )
{
	int val = LCUI_GetStyleValue( str );
	if( val < 0 ) {
		return -1;
	}
	SetFontStyle( ctx, val, style );
	return 0;
}

static int OnParseLineHeight( LCUI_CSSParserStyleContext ctx, const char *str )
{
	LCUI_StyleRec sv;
	if( !ParseNumber(&sv, str) ) {
		return -1;
	}
	ctx->sheet->sheet[self.keys[ctx->parser->key]] = sv;
	return 0;
}

static int OnParseStyleOption( LCUI_CSSParserStyleContext ctx, const char *str )
{
	LCUI_Style s = GetFontStyle( ctx );
	int v = LCUI_GetStyleValue( str );
	if( v < 0 ) {
		return -1;
	}
	s->style = v;
	s->type = SVT_STYLE;
	s->is_valid = TRUE;
	return 0;
}

static LCUI_CSSPropertyParserRec style_parsers[] = {
	{ key_color, "color", OnParseColor },
	{ key_font_family, "font-family", OnParseFontFamily },
	{ key_font_size, "font-size", OnParseFontSize },
	{ key_font_style, "font-style", OnParseFontStyle },
	{ key_font_weight, "font-weight", OnParseFontWeight },
	{ key_text_align, "text-align", OnParseTextAlign },
	{ key_line_height, "line-height", OnParseLineHeight },
	{ key_content, "content", OnParseContent },
	{ key_white_space, "white-space", OnParseStyleOption }
};

static void OnComputeFontSize( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid ) {
		fs->font_size = LCUIMetrics_ComputeActual(
			max( MIN_FONT_SIZE, s->value ), s->type
		);
		return;
	}
	fs->font_size = LCUIMetrics_ComputeActual( DEFAULT_FONT_SIZE, SVT_PX );
}

static void OnComputeColor( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid ) {
		fs->color = s->color;
	} else {
		fs->color.value = DEFAULT_COLOR;
	}
}

static void OnComputeFontFamily( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( fs->font_ids ) {
		free( fs->font_ids );
		fs->font_ids = NULL;
	}
	if( fs->font_family ) {
		free( fs->font_family );
		fs->font_family = NULL;
	}
	if( !s->is_valid ) {
		return;
	}
	fs->font_family = strdup2( s->string );
	LCUIFont_GetIdByNames( &fs->font_ids, fs->font_style,
			       fs->font_weight, fs->font_family );
}

static void OnComputeFontStyle( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid ) {
		fs->font_style = s->val_int;
	} else {
		fs->font_style = FONT_STYLE_NORMAL;
	}
}

static void OnComputeFontWeight( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid ) {
		fs->font_weight = s->val_int;
	} else {
		fs->font_weight = FONT_WEIGHT_NORMAL;
	}
}

static void OnComputeTextAlign( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid ) {
		fs->text_align = s->val_style;
	} else {
		fs->text_align = SV_LEFT;
	}
}

static void OnComputeLineHeight( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	int h;
	if( s->is_valid ) {
		if( s->type == SVT_VALUE ) {
			h = iround( fs->font_size * s->val_int );
		} else if( s->type == SVT_SCALE ) {
			h = iround( fs->font_size * s->val_scale );
		} else {
			h = LCUIMetrics_ComputeActual( s->value, s->type );
		}
	} else {
		h = iround( fs->font_size * LINE_HEIGHT_SCALE );
	}
	fs->line_height = h;
}

static void OnComputeContent( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( fs->content ) {
		free( fs->content );
		fs->content = NULL;
	}
	if( s->is_valid ) {
		fs->content = wcsdup2( s->wstring );
	}
}

static void OnComputeWhiteSpace( LCUI_CSSFontStyle fs, LCUI_Style s )
{
	if( s->is_valid && s->type == SVT_STYLE ) {
		fs->white_space = s->val_style;
	}  else {
		fs->white_space = SV_AUTO;
	}
}

void CSSFontStyle_Init( LCUI_CSSFontStyle fs )
{
	fs->content = NULL;
	fs->font_ids = NULL;
	fs->font_family = NULL;
	fs->font_style = FONT_STYLE_NORMAL;
	fs->font_weight = FONT_WEIGHT_NORMAL;
}

void CSSFontStyle_Destroy( LCUI_CSSFontStyle fs )
{
	if( fs->font_ids ) {
		free( fs->font_ids );
		fs->font_ids = NULL;
	}
	if( fs->font_family ) {
		free( fs->font_family );
		fs->font_family = NULL;
	}
	if( fs->content ) {
		free( fs->content );
		fs->content = NULL;
	}
}

int LCUI_GetFontStyleKey( int key )
{
	return self.keys[key];
}

void CSSFontStyle_Compute( LCUI_CSSFontStyle fs, LCUI_StyleSheet ss )
{
	int i;
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		if( self.keys[i] < 0 ) {
			continue;
		}
		self.handlers[i]( fs, &ss->sheet[self.keys[i]] );
	}
}

void CSSFontStyle_GetTextStyle( LCUI_CSSFontStyle fs, LCUI_TextStyle ts )
{
	size_t len;
	ts->font_ids = NULL;
	ts->has_style = TRUE;
	ts->has_weight = TRUE;
	ts->has_family = FALSE;
	ts->has_back_color = FALSE;
	ts->has_pixel_size = TRUE;
	ts->has_fore_color = TRUE;
	ts->fore_color = fs->color;
	ts->pixel_size = fs->font_size;
	ts->weight = fs->font_weight;
	ts->style = fs->font_style;
	if( fs->font_ids ) {
		for( len = 0; fs->font_ids[len]; ++len );
		ts->font_ids = malloc( sizeof( int ) * ++len );
		memcpy( ts->font_ids, fs->font_ids, len * sizeof( int ) );
		ts->has_family = TRUE;
	}
}

void LCUI_InitCSSFontStyle( void )
{
	int i;
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		LCUI_CSSPropertyParser p = &style_parsers[i];
		self.keys[p->key] = LCUI_AddCSSPropertyName( p->name );
		LCUI_AddCSSPropertyParser( p );
	}
	self.handlers[key_color] = OnComputeColor;
	self.handlers[key_font_size] = OnComputeFontSize;
	self.handlers[key_font_family] = OnComputeFontFamily;
	self.handlers[key_font_style] = OnComputeFontStyle;
	self.handlers[key_font_weight] = OnComputeFontWeight;
	self.handlers[key_line_height] = OnComputeLineHeight;
	self.handlers[key_text_align] = OnComputeTextAlign;
	self.handlers[key_white_space] = OnComputeWhiteSpace;
	self.handlers[key_content] = OnComputeContent;
}

void LCUI_FreeCSSFontStyle( void )
{

}
