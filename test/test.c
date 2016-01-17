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
	_wchdir( L"../test/" );
	InitConsoleWindow();
#endif
	LCUI_Init();
	ret += TestCSSParser();
	printf("test result code: %d\n", ret);
#ifdef LCUI_BUILD_IN_WIN32
	system("pause");
#endif
	return ret;
}
