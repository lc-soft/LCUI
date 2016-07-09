
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

void LCUI_PreInitLinuxApp( void *data )
{
	return;
}

int LCUI_InitLinuxApp( LCUI_AppDriver app )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		return LCUI_InitLinuxX11App( app );
	}
	return -1;
}
#endif
