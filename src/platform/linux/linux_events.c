
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

LCUI_AppDriver LCUI_CreateLinuxAppDriver( void )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		return LCUI_CreateLinuxX11AppDriver();
	}
	return NULL;
}

void LCUI_DestroyLinuxAppDriver( LCUI_AppDriver driver )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		LCUI_DestroyLinuxX11AppDriver( driver );
	}
}

#endif
