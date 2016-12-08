#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

static int test_cmdsplit( void )
{
	int argc;
	char **argv;
	const char *cmd = "\"C:\\Program Files\\WindowsApps\\Test\\app.exe\" --print   \"Hello, Wolrd !\"";
	const char *results[3] = {
		"\"C:\\Program Files\\WindowsApps\\Test\\app.exe\"",
		"--print", 
		"\"Hello, Wolrd !\""
	};
	argc = cmdsplit( "  test.exe   ", NULL );
	_DEBUG_MSG( "argc: %d\n", argc );
	if( argc != 1 ) {
		return -1;
	}
	argc = cmdsplit( cmd, NULL );
	_DEBUG_MSG( "argc: %d\n", argc );
	if( argc != 3 ) {
		return -1;
	}
	argc = cmdsplit( cmd, &argv );
	while( argc-- > 0 ) {
		_DEBUG_MSG( "argv[%d]: [%s]\n", argc, argv[argc] );
		if( strcmp( argv[argc], results[argc] ) != 0 ) {
			return -1;
		}
		free( argv[argc] );
	}
	free( argv );
	return 0;
}

int test_string( void )
{
	return test_cmdsplit();
}
