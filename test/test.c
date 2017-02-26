#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

#ifdef LCUI_BUILD_IN_WIN32
#include <io.h>
#include <fcntl.h>

static void InitConsoleWindow( void )
{
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt = _open_osfhandle( (long)GetStdHandle( STD_OUTPUT_HANDLE ), _O_TEXT );
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	setvbuf( stdout, NULL, _IONBF, 0 );
	// test code
	printf( "InitConsoleWindow OK!\n" );
}

#endif

int main(void)
{
	int ret = 0;
#ifdef LCUI_BUILD_IN_WIN32
	_wchdir( L"F:\\代码库\\GitHub\\LCUI\\build\\VS2012\\LCUITest" );
	InitConsoleWindow();
#endif
	ret |= test_string();
	ret |= test_image_reader();/*
	ret |= test_css_parser();
	ret |= test_widget_render();
	ret |= test_char_render();
	ret |= test_string_render();*/
	printf("test result code: %d\n", ret);
#ifdef LCUI_BUILD_IN_WIN32
	system( "pause" );
#endif
	return ret;
}
