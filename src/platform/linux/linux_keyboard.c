
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H
#ifdef LCUI_BUILD_IN_LINUX

void LCUI_InitLinuxKeyboard( void )
{
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		LCUI_InitLinuxX11Keyboard();
	}
}

void LCUI_ExitLinuxKeyboard( void )
{

}

#endif
