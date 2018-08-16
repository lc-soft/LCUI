#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

#ifdef LCUI_BUILD_IN_WIN32
static void LoggerHandler( const char *str )
{
	OutputDebugStringA( str );
}

static void LoggerHandlerW( const wchar_t *str )
{
	OutputDebugStringW( str );
}
#endif

int main( int argc, char **argv )
{
	LCUI_Widget root, pack;

	LCUI_Init();
#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler( LoggerHandler );
	Logger_SetHandlerW( LoggerHandlerW );
#endif
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile( "test_image_scaling.xml" );
	if( !pack ) {
		return -1;
	}
	Widget_UpdateStyle( root, TRUE );
	Widget_Append( root, pack ); 
	Widget_Unwrap( pack );
	return LCUI_Main();
}
