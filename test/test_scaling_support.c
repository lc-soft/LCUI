#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

enum {
	TYPE_DENSITY,
	TYPE_SCALED_DENSITY,
	TYPE_SCALE
};

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

static void OnButtonClick( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	int *data = e->data;
	switch( data[0] ) {
	case TYPE_DENSITY:
		LCUIMetrics_SetDensityLevel( data[1] );
		break;
	case TYPE_SCALED_DENSITY:
		LCUIMetrics_SetScaledDensityLevel( data[1] );
		break;
	case TYPE_SCALE:
		LCUIMetrics_SetScale( data[1] / 100.0f );
		break;
	}
	LCUIWidget_RefreshStyle();
	LCUIDisplay_InvalidateArea( NULL );
}

static void SetButton( const char *id, int type, int level )
{
	int *data;
	LCUI_Widget btn;
	data = malloc( sizeof( int ) * 2 );
	data[0] = type;
	data[1] = level;
	btn = LCUIWidget_GetById( id );
	Widget_BindEvent( btn, "click", OnButtonClick, data, free );
}

static void InitButtons( void )
{
	SetButton( "btn-density-small", TYPE_DENSITY, DENSITY_LEVEL_SMALL );
	SetButton( "btn-density-normal", TYPE_DENSITY, DENSITY_LEVEL_NORMAL );
	SetButton( "btn-density-large", TYPE_DENSITY, DENSITY_LEVEL_LARGE );
	SetButton( "btn-density-big", TYPE_DENSITY, DENSITY_LEVEL_BIG );
	SetButton( "btn-scaled-density-small",
		   TYPE_SCALED_DENSITY, DENSITY_LEVEL_SMALL );
	SetButton( "btn-scaled-density-normal",
		   TYPE_SCALED_DENSITY, DENSITY_LEVEL_NORMAL );
	SetButton( "btn-scaled-density-large",
		   TYPE_SCALED_DENSITY, DENSITY_LEVEL_LARGE );
	SetButton( "btn-scaled-density-big",
		   TYPE_SCALED_DENSITY, DENSITY_LEVEL_BIG );
	SetButton( "btn-scale-small", TYPE_SCALE, 75 );
	SetButton( "btn-scale-normal", TYPE_SCALE, 100 );
	SetButton( "btn-scale-large", TYPE_SCALE, 150 );
	SetButton( "btn-scale-big", TYPE_SCALE, 200 );
}

int main( int argc, char **argv )
{
	LCUI_Widget root, pack;

	LCUI_Init();
#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler( LoggerHandler );
	Logger_SetHandlerW( LoggerHandlerW );
#endif
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile( "test_scaling_support.xml" );
	if( !pack ) {
		return -1;
	}
	Widget_UpdateStyle( root, TRUE );
	Widget_Append( root, pack ); 
	Widget_Unwrap( pack );
	InitButtons();
	return LCUI_Main();
}
