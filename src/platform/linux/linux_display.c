
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>

int LCUI_InitLinuxDisplay( LCUI_DisplayDriver driver )
{
	return -1;
}

int LCUI_ExitLinuxDisplay( void )
{
	return -1;
}
#endif
