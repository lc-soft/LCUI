
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H

static void OnKeyboardMessage( LCUI_Event ev, void *arg )
{
	static POINT mouse_pos;
	LCUI_SysEventRec sys_ev;
	WIN_SysEvent win_ev = arg;
	switch( win_ev->msg ) {
	case WM_KEYDOWN:
		sys_ev.type = LCUI_KEYDOWN;
		sys_ev.key_code = win_ev->wparam;
		break;
	case WM_KEYUP:
		sys_ev.type = LCUI_KEYUP;
		sys_ev.key_code = win_ev->wparam;
		break;
	default: return;
	}
	LCUI_TriggerEvent( &sys_ev, NULL );
}

void LCUI_InitWinKeyboard( void )
{
	LCUI_BindSysEvent( WM_KEYDOWN, OnKeyboardMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_KEYUP, OnKeyboardMessage, NULL, NULL );
}

void LCUI_ExitWinKeyboard( void )
{
	LCUI_UnbindSysEvent( WM_KEYDOWN, OnKeyboardMessage );
	LCUI_UnbindSysEvent( WM_KEYUP, OnKeyboardMessage );
}
