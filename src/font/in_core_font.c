/* ***************************************************************************
 * in_core_font.c -- basic in-core font-engine.
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
 * in_core_font.c -- 基础的内置字体引擎，可用于从程序内部载入字体位图
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

#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

enum in_core_font_type {
	FONT_INCONSOLATA
};

static int InCoreFont_Open( const char *filepath, LCUI_Font ***outfonts )
{
	int *code;
	LCUI_Font **fonts, *font;
	if( strcmp( filepath, "in-core.inconsolata" ) != 0 ) {
		return 0;
	}
	code = malloc( sizeof( int ) );
	*code = FONT_INCONSOLATA;
	fonts = malloc( sizeof( LCUI_Font* ) );
	font = malloc( sizeof( LCUI_Font ) );
	font->family_name = strdup2( "inconsolata" );
	font->style_name = strdup2( "Regular" );
	font->data = code;
	fonts[0] = font;
	*outfonts = fonts;
	return 1;
}

static void InCoreFont_Close( void *face )
{
	free(face);
}

static int InCoreFont_Render( LCUI_FontBitmap *bmp, wchar_t ch,
			      int pixel_size, LCUI_Font *font )
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
