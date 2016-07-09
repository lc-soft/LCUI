
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_DISPLAY_H

int LCUI_InitLinuxDisplay( LCUI_DisplayDriver driver )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		return LCUI_InitLinuxX11Display( driver );
	}
	return -1;
}

int LCUI_ExitLinuxDisplay( void )
{
	return -1;
}
#endif
