#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

static int test_cmdsplit( void )
{
	char **argv;
	int argc, ret = 0;
	const char *cmd = "\"C:\\Program Files\\WindowsApps\\Test\\app.exe\" --print   \"Hello, Wolrd !\"";
	const char *results[3] = {
		"\"C:\\Program Files\\WindowsApps\\Test\\app.exe\"",
		"--print", 
		"\"Hello, Wolrd !\""
	};
	CHECK( cmdsplit( "  test.exe   ", NULL ) == 1 );
	CHECK( cmdsplit( cmd, NULL ) == 3 );
	argc = cmdsplit( cmd, &argv );
	while( argc-- > 0 ) {
		TEST_LOG( "argv[%d]: check [%s] == [%s]\n", argc, argv[argc], results[argc] );
		CHECK( strcmp( argv[argc], results[argc] ) == 0 );
		free( argv[argc] );
	}
	free( argv );
	return ret;
}

int test_string( void )
{
	return test_cmdsplit();
}
