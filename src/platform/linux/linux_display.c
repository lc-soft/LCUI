
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_DISPLAY_H

LCUI_DisplayDriver LCUI_CreateLinuxDisplayDriver( void )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		return LCUI_CreateLinuxX11DisplayDriver();
	}
	return NULL;
}

void LCUI_DestroyLinuxDisplayDriver( LCUI_DisplayDriver driver )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		LCUI_DestroyLinuxX11DisplayDriver( driver );
	}
}
#endif
