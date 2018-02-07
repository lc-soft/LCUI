#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>
#include "test.h"

#define GetSegoeUIFont(S, W) LCUIFont_GetId( "Segoe UI", S, W )
#define GetArialFont(S, W) LCUIFont_GetId( "Arial", S, W )

int test_segoe_ui_font_load( void )
{
	int ret = 0, id;
	LCUI_Font font;
	if( LCUIFont_LoadFile( "C:/windows/fonts/segoeui.ttf" ) != 0 ) {
		return 0;
	}
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeui.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeuib.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeuii.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeuil.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeuisl.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/segoeuiz.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/seguibl.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/seguili.ttf" ) == 0 );
	CHECK_WITH_TEXT(
		"check get font (style: normal, weight: normal)",
		id = GetSegoeUIFont( FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight == FONT_WEIGHT_NORMAL );
	}
	CHECK_WITH_TEXT( 
		"check get font (style: italic, weight: normal)",
		id = GetSegoeUIFont( FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_ITALIC );
		CHECK( font->weight == FONT_WEIGHT_NORMAL );
	}
	CHECK_WITH_TEXT( 
		"check get font (style: normal, weight: bold)",
		id = GetSegoeUIFont( FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight == FONT_WEIGHT_BOLD );
	}
	CHECK_WITH_TEXT(
		"check get font (style: normal, weight: light)",
		id = GetSegoeUIFont( FONT_STYLE_NORMAL, FONT_WEIGHT_LIGHT )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight <= FONT_WEIGHT_LIGHT );
	}
	CHECK_WITH_TEXT(
		"check get font (style: italic, weight: light)",
		id = GetSegoeUIFont( FONT_STYLE_ITALIC, FONT_WEIGHT_LIGHT )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_ITALIC );
		CHECK( font->weight <= FONT_WEIGHT_LIGHT );
	}
	return ret;
}

int test_arial_font_load( void )
{
	int ret = 0, id;
	LCUI_Font font;
	if( LCUIFont_LoadFile( "C:/windows/fonts/arial.ttf" ) != 0 ) {
		return 0;
	}
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/arialbd.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/ariblk.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/arialbi.ttf" ) == 0 );
	CHECK( LCUIFont_LoadFile( "C:/windows/fonts/ariali.ttf" ) == 0 );
	CHECK_WITH_TEXT(
		"check get font (style: normal, weight: normal)",
		id = GetArialFont( FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight == FONT_WEIGHT_NORMAL );
	}
	CHECK_WITH_TEXT(
		"check get font (style: italic, weight: normal)",
		id = GetArialFont( FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_ITALIC );
		CHECK( font->weight == FONT_WEIGHT_NORMAL );
	}
	CHECK_WITH_TEXT(
		"check get font (style: normal, weight: bold)",
		id = GetArialFont( FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight == FONT_WEIGHT_BOLD );
	}
	CHECK_WITH_TEXT(
		"check get font (style: normal, weight: black)",
		id = GetArialFont( FONT_STYLE_NORMAL, FONT_WEIGHT_BLACK )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_NORMAL );
		CHECK( font->weight == FONT_WEIGHT_BLACK );
	}
	CHECK_WITH_TEXT(
		"check get font (style: italic, weight: bold)",
		id = GetSegoeUIFont( FONT_STYLE_ITALIC, FONT_WEIGHT_BOLD )
	);
	if( id > 0 ) {
		font = LCUIFont_GetById( id );
		CHECK( font->style == FONT_STYLE_ITALIC );
		CHECK( font->weight == FONT_WEIGHT_BOLD );
	}
	return ret;
}

int test_font_load( void )
{
	int ret = 0;

	LCUI_InitFontLibrary();
	/* 测试是否能够从字体文件中载入字体 */
	CHECK( LCUIFont_LoadFile( "test_font_load.ttf" ) == 0 );
#ifdef LCUI_BUILD_IN_WIN32
	ret += test_segoe_ui_font_load();
	ret += test_arial_font_load();
#endif
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
