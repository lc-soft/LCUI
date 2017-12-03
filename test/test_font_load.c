#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>
#include "test.h"

int test_font_load( void )
{
	int ret = 0;

	LCUI_InitFontLibrary();
	/* 测试是否能够从字体文件中载入字体 */
	CHECK( LCUIFont_LoadFile( "test_font_load.ttf" ) == 0 );
	LCUI_FreeFontLibrary();

	LCUI_InitFontLibrary();
	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	/* 测试是否能够根据 CSS 文件中定义的 @font-face 规则来载入字体 */
	CHECK( LCUI_LoadCSSFile( "test_font_load.css" ) == 0 );
	CHECK( LCUIFont_GetId( "icomoon", 0, 0 ) > 0 );
	LCUI_FreeCSSParser();
	LCUI_FreeCSSLibrary();
	LCUI_FreeFontLibrary();
	return ret;
}
