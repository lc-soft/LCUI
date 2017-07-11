#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/builder.h>
#include "test.h"

int test_css_parser( void )
{
	int ret = 0;
	LCUI_Widget root, box, btn, text;

	LCUI_Init();
	CHECK( box = LCUIBuilder_LoadFile( "test_css_parser.xml" ) );
	if( !box ) {
		LCUI_Destroy();
		return ret;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append( root, box );
	Widget_Unwrap( box );
	text = LCUIWidget_GetById( "test-textview" );
	Widget_UpdateStyle( text, TRUE );
	Widget_Update( text );
	CHECK( text->style->sheet[key_width].val_px == 100 );
	CHECK( text->style->sheet[key_height].val_px == 60 );
	CHECK( text->style->sheet[key_position].val_style == SV_ABSOLUTE );
	CHECK( text->style->sheet[key_top].val_px == 12 );
	CHECK( text->style->sheet[key_left].val_px == 20 );
	btn = LCUIWidget_GetById( "test-btn" );
	Widget_AddStatus( btn, "hover" );
	Widget_UpdateStyle( btn, TRUE );
	Widget_UpdateStyle( text, TRUE );
	Widget_Update( text );
	CHECK( text->style->sheet[key_background_color].val_color.value == 0xffff0000 );
	CHECK( text->style->sheet[key_background_size].val_style == SV_CONTAIN );
	if( ret != 0 ) {
		LCUI_PrintCSSLibrary();
		printf( "test widget style: {\n" );
		LCUI_PrintStyleSheet( text->style );
		printf( "}\n" );
	}
	LCUI_Destroy();
	return ret;
}
