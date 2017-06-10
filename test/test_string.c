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

int test_strs( void )
{
	int ret = 0;
	char **strs = NULL;
	strsadd( &strs, "first-child" );
	strsadd( &strs, "last-child" );
	CHECK( strshas( strs, "first-child" ) );
	CHECK( strshas( strs, "last-child" ) );
	CHECK( strsdel( &strs, "last-child" ) );
	CHECK( strsdel( &strs, "first-child" ) );
	CHECK( !strshas( strs, "first-child" ) );
	CHECK( strsadd( &strs, "first-child" ) );
	freestrs( strs );
	return ret;
}

int test_string( void )
{
	int ret = 0;
	ret += test_cmdsplit();
	ret += test_strs();
	return ret;
}
