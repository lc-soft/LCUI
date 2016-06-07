#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

int main( int argc, char **argv )
{
	LCUI_Widget root, pack;
	LCUI_Init();
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile( "helloworld.xml" );
	if( !pack ) {
		return -1;
	}
	Widget_Append( root, pack ); 
	Widget_Unwrap( pack );
	return LCUI_Main();
}

/* version 2
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>

int main( int argc, char **argv )
{
	LCUI_Widget root, txt;

	LCUI_Init();
	root = LCUIWidget_GetRoot();
	txt = LCUIWidget_New( "textview" );
	TextView_SetTextAlign( txt, SV_CENTER );
	TextView_SetText( txt, "[size=18px]Hello, World![/size]" );
	SetStyle( txt->custom_style, key_margin_top, 25, px );
	SetStyle( txt->custom_style, key_margin_left, 25, px );
	SetStyle( txt->custom_style, key_padding_top, 25, px );
	SetStyle( txt->custom_style, key_border_width, 1, px );
	SetStyle( txt->custom_style, key_border_color, RGB(0,0,0), color );
	Widget_Resize( txt, 200, 100 );
	Widget_Append( root, txt ); 
	return LCUI_Main();
}
*/
