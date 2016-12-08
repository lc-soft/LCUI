/* ***************************************************************************
 * windows_mouse.c -- mouse support for windows platform.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * windows_mouse.c -- windows 平台鼠标驱动支持。
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

static void OnMouseMessage( LCUI_Event ev, void *arg )
{
	MSG *msg = arg;
	LCUI_SysEventRec sys_ev;
	static POINT mouse_pos = {0, 0};
	sys_ev.type = LCUI_NONE;
	switch( msg->message ) {
	case WM_MOUSEMOVE: {
		POINT new_pos;
		GetCursorPos( &new_pos );
		ScreenToClient( msg->hwnd, &new_pos );
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
		sys_ev.button.button = LCUIKEY_LEFTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture( msg->hwnd );
		break;
	case WM_LBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = LCUIKEY_LEFTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.button.button = LCUIKEY_RIGHTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture( msg->hwnd );
		break;
	case WM_RBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = LCUIKEY_RIGHTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_MOUSEWHEEL:
		sys_ev.type = LCUI_MOUSEWHEEL;
		sys_ev.wheel.x = mouse_pos.x;
		sys_ev.wheel.y = mouse_pos.y;
		sys_ev.wheel.delta = GET_WHEEL_DELTA_WPARAM( msg->wParam );
		break;
	case WM_TOUCH: {
		UINT i, n = LOWORD( msg->wParam );
		PTOUCHINPUT inputs = NEW( TOUCHINPUT, n );
		HTOUCHINPUT handle = (HTOUCHINPUT)msg->lParam;
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
			ScreenToClient( msg->hwnd, &pos );
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
