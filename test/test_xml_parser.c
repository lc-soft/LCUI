#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include "test.h"

static int check_widget_attribute( void )
{
	int ret = 0;
	LCUI_Widget w;

	w = LCUIWidget_GetById( "test-attr-disabed-1" );
	CHECK_WITH_TEXT( "check element-1 is disabled", w->disabled );
	w = LCUIWidget_GetById( "test-attr-disabed-2" );
	CHECK_WITH_TEXT( "check element-2 is disabled", w->disabled );
	w = LCUIWidget_GetById( "test-attr-disabed-3" );
	CHECK_WITH_TEXT( "check element-3 is disabled", w->disabled );
	w = LCUIWidget_GetById( "test-attr-disabed-4" );
	CHECK_WITH_TEXT( "check element-4 is normal", !w->disabled );
	return ret;
}

int test_xml_parser( void )
{
	int ret = 0;
	LCUI_Widget root, pack;

	LCUI_Init();
	TEST_LOG( "test widget layout\n" );
	LCUIDisplay_SetSize( 960, 680 );
	root = LCUIWidget_GetRoot();
	CHECK( pack = LCUIBuilder_LoadFile( "test_xml_parser.xml" ) );
	if( !pack ) {
		LCUI_Destroy();
		return ret;
	}
	Widget_UpdateStyle( root, TRUE );
	Widget_Append( root, pack );
	Widget_Unwrap( pack );
	LCUIWidget_Update();
	ret += check_widget_attribute();
	LCUI_Destroy();
	return ret;
}
