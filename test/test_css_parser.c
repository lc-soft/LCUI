#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

#define assert(X) if(!(X)) {return -1;}

int TestCSSParser(void)
{
	LCUI_Widget box, btn, text;
	box = LCUIBuilder_LoadFile( "test_css_parser.xml" );
	if( !box ) {
		return -1;
	}
	text = LCUIWidget_GetById( "test-textview" );
	Widget_UpdateStyle( text, TRUE );
	Widget_Update( text );
	assert( text->style->sheet[key_width].val_px == 100 );
	assert( text->style->sheet[key_height].val_px == 60 );
	assert( text->style->sheet[key_position].val_style == SV_ABSOLUTE );
	assert( text->style->sheet[key_top].val_px == 12 );
	assert( text->style->sheet[key_left].val_px == 20 );
	btn = LCUIWidget_GetById( "test-btn" );
	Widget_AddStatus( btn, "hover" );
	Widget_UpdateStyle( btn, TRUE );
	Widget_UpdateStyle( text, TRUE );
	Widget_Update( text );
	assert( text->style->sheet[key_background_color].val_color.value == 0xffff0000 );
	assert( text->style->sheet[key_background_size].val_style == SV_CONTAIN );
	return 0;
}
