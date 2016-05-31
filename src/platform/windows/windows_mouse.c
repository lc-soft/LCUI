
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

static void OnMouseMessage( LCUI_Event ev, void *arg )
{
	LCUI_SysEventRec sys_ev;
	WIN_SysEvent win_ev = arg;
	static POINT mouse_pos = {0, 0};
	sys_ev.type = LCUI_NONE;
	switch( win_ev->msg ) {
	case WM_MOUSEMOVE: {
		POINT new_pos;
		GetCursorPos( &new_pos );
		ScreenToClient( win_ev->hwnd, &new_pos );
		sys_ev.motion.x = new_pos.x;
		sys_ev.motion.y = new_pos.y;
		sys_ev.motion.xrel = new_pos.x - mouse_pos.x;
		sys_ev.motion.yrel = new_pos.y - mouse_pos.y;
		mouse_pos.x = new_pos.x;
		mouse_pos.y = new_pos.y;
		sys_ev.type = LCUI_MOUSEMOVE;
		break;
	}
	case WM_LBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.button.button = 1;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture( win_ev->hwnd );
		break;
	case WM_LBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = 1;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.button.button = 2;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture( win_ev->hwnd );
		break;
	case WM_RBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = 2;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture( win_ev->hwnd );
		break;
	case WM_MOUSEWHEEL:
		sys_ev.type = LCUI_MOUSEWHEEL;
		sys_ev.wheel.x = mouse_pos.x;
		sys_ev.wheel.y = mouse_pos.y;
		sys_ev.wheel.delta = GET_WHEEL_DELTA_WPARAM( win_ev->wparam );
		break;
	case WM_TOUCH: {
		UINT i, n = LOWORD( win_ev->wparam );
		PTOUCHINPUT inputs = NEW( TOUCHINPUT, n );
		HTOUCHINPUT handle = (HTOUCHINPUT)win_ev->lparam;
		if( inputs == NULL ) {
			break;
		}
		sys_ev.type = LCUI_TOUCH;
		sys_ev.touch.n_points = n;
		sys_ev.touch.points = NEW( LCUI_TouchPointRec, n );
		if( sys_ev.touch.points == NULL ) {
			free( inputs );
			break;
		}
		if( !GetTouchInputInfo( handle, n, inputs,
					sizeof( TOUCHINPUT ) ) ) {
			free( inputs );
			break;
		}
		for( i = 0; i < n; ++i ) {
			POINT pos;
			pos.x = inputs[i].x / 100;
			pos.y = inputs[i].y / 100;
			ScreenToClient( win_ev->hwnd, &pos );
			sys_ev.touch.points[i].x = pos.x;
			sys_ev.touch.points[i].y = pos.y;
			sys_ev.touch.points[i].id = inputs[i].dwID;
			if( inputs[i].dwFlags & TOUCHEVENTF_PRIMARY ) {
				sys_ev.touch.points[i].is_primary = TRUE;
			} else {
				sys_ev.touch.points[i].is_primary = FALSE;
			}
			if( inputs[i].dwFlags & TOUCHEVENTF_DOWN ) {
				sys_ev.touch.points[i].state = LCUI_TOUCHDOWN;
			} else if( inputs[i].dwFlags & TOUCHEVENTF_UP ) {
				sys_ev.touch.points[i].state = LCUI_TOUCHUP;
			} else if( inputs[i].dwFlags & TOUCHEVENTF_MOVE ) {
				sys_ev.touch.points[i].state = LCUI_TOUCHMOVE;
			}
		}
		free( inputs );
		if( !CloseTouchInputHandle( handle ) ) {
			break;
		}
		break;
	}
	default: break;
	}
	if( sys_ev.type != LCUI_NONE ) {
		LCUI_TriggerEvent( &sys_ev, NULL );
		LCUI_DestroyEvent( &sys_ev );
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
	LCUI_BindSysEvent( WM_TOUCH, OnMouseMessage, NULL, NULL );
}

void LCUI_ExitWinMouse( void )
{
	LCUI_UnbindSysEvent( WM_MOUSEMOVE, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_LBUTTONDOWN, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_LBUTTONUP, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_RBUTTONDOWN, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_RBUTTONUP, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_MOUSEWHEEL, OnMouseMessage );
	LCUI_UnbindSysEvent( WM_TOUCH, OnMouseMessage );
}
#endif
