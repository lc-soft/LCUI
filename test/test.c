#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

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

int main( void )
{
	int ret = 0;
#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler( LoggerHandler );
	Logger_SetHandlerW( LoggerHandlerW );
#endif
	ret += test_string();
	ret += test_thread();
	ret += test_font_load();
	ret += test_image_reader();
	ret += test_css_parser();
	ret += test_xml_parser();
	ret += test_widget_layout();
	ret += test_widget_flex_layout();
	ret += test_widget_inline_block_layout();
	ret += test_widget_rect();
	PRINT_TEST_RESULT( ret );
	return ret;
}
