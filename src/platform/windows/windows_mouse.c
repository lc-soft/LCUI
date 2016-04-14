
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

static void OnMouseMessage( LCUI_Event ev, void *arg )
{
	static POINT mouse_pos;
	LCUI_SysEventRec sys_ev;
	WIN_SysEvent win_ev = arg;
	sys_ev.type = LCUI_NONE;
	switch( win_ev->msg ) {
	case WM_MOUSEMOVE: {
		POINT new_pos;
		GetCursorPos( &new_pos );
		ScreenToClient( win_ev->hwnd, &new_pos );
		sys_ev.rel_x = new_pos.x - mouse_pos.x;
		sys_ev.rel_y = new_pos.y - mouse_pos.y;
		sys_ev.type = LCUI_MOUSEMOVE;
		break;
	}
	case WM_LBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.key_code = 1;
		break;
	case WM_LBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.key_code = 1;
		break;
	case WM_RBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.key_code = 2;
		break;
	case WM_RBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.key_code = 2;
		break;
	case WM_MOUSEWHEEL:
		sys_ev.type = LCUI_MOUSEWHEEL;
		sys_ev.z_delta = GET_WHEEL_DELTA_WPARAM( win_ev->wparam );
		break;
	default: break;
	}
	if( sys_ev.type != LCUI_NONE ) {
		LCUI_TriggerEvent( &sys_ev, NULL );
	}
}

void LCUI_InitWinMouse( void )
{
	LCUI_BindSysEvent( WM_MOUSEMOVE, OnMouseMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_LBUTTONDOWN, OnMouseMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_LBUTTONUP, OnMouseMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_RBUTTONDOWN, OnMouseMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_RBUTTONUP, OnMouseMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_MOUSEWHEEL, OnMouseMessage, NULL, NULL );
}

void LCUI_ExitWinMouse( void )
{
	LCUI_UnbindSysEvent( WM_MOUSEMOVE, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_LBUTTONDOWN, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_LBUTTONUP, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_RBUTTONDOWN, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_RBUTTONUP, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_MOUSEWHEEL, OnMouseMessage );
}
