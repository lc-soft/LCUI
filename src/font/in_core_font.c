/*
 * in_core_font.c -- basic in-core font-engine.
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

enum in_core_font_type {
	FONT_INCONSOLATA
};

static int InCoreFont_Open( const char *filepath, LCUI_Font **outfonts )
{
	int *code;
	LCUI_Font *fonts, font;
	if( strcmp( filepath, "in-core.inconsolata" ) != 0 ) {
		return 0;
	}
	code = malloc( sizeof( int ) );
	*code = FONT_INCONSOLATA;
	font = Font( "inconsolata", "Regular" );
	fonts = malloc( sizeof( LCUI_Font ) );
	font->data = code;
	fonts[0] = font;
	*outfonts = fonts;
	return 1;
}

static void InCoreFont_Close( void *face )
{
	free( face );
}

static int InCoreFont_Render( LCUI_FontBitmap *bmp, wchar_t ch,
			      int pixel_size, LCUI_Font font )
{
	int *code = (int*)font->data;
	switch( *code ) {
	case FONT_INCONSOLATA:
	default:
		return FontInconsolata_GetBitmap( bmp, ch, pixel_size );
	}
	return -1;
}

int LCUIFont_InitInCoreFont( LCUI_FontEngine *engine )
{
	engine->render = InCoreFont_Render;
	engine->close = InCoreFont_Close;
	engine->open = InCoreFont_Open;
	strcpy( engine->name, "in-core" );
	return 0;
}

int LCUIFont_ExitInCoreFont( void )
{
	return 0;
}
