/* ***************************************************************************
 * linux_x11keyboard.h -- keyboard support for linux xwindow.
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
 * linux_x11keyboard.h -- linux 平台的键盘支持，基于 xwindow。
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

#include <stdio.h>
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H
#include <X11/XKBlib.h>

static void OnKeyboardMessage( LCUI_Event ev, void *arg )
{
	int key;
	KeySym keysym;
	XEvent *x_ev = arg;
	LCUI_X11AppDriver x11;
	LCUI_SysEventRec sys_ev;
	switch( x_ev->type ) {
	case KeyPress:
		sys_ev.type = LCUI_KEYDOWN;
		break;
	case KeyRelease:
		sys_ev.type = LCUI_KEYUP;
		break;
	default: return;
	}
	x11 = LCUI_GetAppData();
	XAutoRepeatOn( x11->display );
	//keysym = XKeycodeToKeysym( x11->display, x_ev->xkey.keycode, 0 );
	//XLookupString( &x_ev->xkey, buf, sizeof buf, &keysym, NULL );
	//keysym = XLookupKeysym( &x_ev->xkey, 0 );
	keysym = XkbKeycodeToKeysym( x11->display, x_ev->xkey.keycode, 0, 0 );
	switch( keysym ) {
	case XK_Tab: key = LCUIKEY_TAB; break;
	case XK_Escape: key = LCUIKEY_ESCAPE; break;
	case XK_Return: key = LCUIKEY_ENTER; break;
	case XK_Delete: key = LCUIKEY_DELETE; break;
	case XK_BackSpace: key = LCUIKEY_BACKSPACE; break;
	case XK_Home: key = LCUIKEY_HOME; break;
	case XK_Left: key = LCUIKEY_LEFT; break;
	case XK_Up: key = LCUIKEY_UP; break;
	case XK_Right: key = LCUIKEY_RIGHT; break;
	case XK_Down: key = LCUIKEY_DOWN; break;
	case XK_Page_Up: key = LCUIKEY_PAGEUP; break;
	case XK_Page_Down: key = LCUIKEY_PAGEDOWN; break;
	case XK_End: key = LCUIKEY_END; break;
	case XK_Control_R:
	case XK_Control_L: key = LCUIKEY_CONTROL; break;
	case XK_Shift_R:
	case XK_Shift_L: key = LCUIKEY_SHIFT; break;
	case XK_Alt_L:
	case XK_Alt_R: key = LCUIKEY_ALT; break;
	case XK_Caps_Lock: key = LCUIKEY_CAPITAL; break;
	case XK_comma: key = LCUIKEY_COMMA; break;
	case XK_period: key = LCUIKEY_PERIOD; break;
	case XK_minus: key = LCUIKEY_MINUS; break;
	case XK_slash: key = LCUIKEY_SLASH; break;
	case XK_semicolon: key = LCUIKEY_SEMICOLON; break;
	case XK_equal: key = LCUIKEY_EQUAL; break;
	case XK_bracketleft: key = LCUIKEY_BRACKETLEFT; break;
	case XK_bracketright: key = LCUIKEY_BRACKETRIGHT; break;
	case XK_backslash: key = LCUIKEY_BACKSLASH; break;
	case XK_apostrophe: key = LCUIKEY_APOSTROPHE; break;
	case XK_grave: key = LCUIKEY_GRAVE; break;
	default:
		key = keysym;
		if( key >= XK_a && key <= XK_z ) {
			key -= 'a' - 'A';
		}
		break;
	}
	_DEBUG_MSG("keyname: %s\n", XKeysymToString(keysym));
	sys_ev.key.code = key;
	_DEBUG_MSG("keycode: %d, keyscancode: %u, keysym: %lu\n", key, x_ev->xkey.keycode, keysym);
	LCUI_TriggerEvent( &sys_ev, NULL );
	keysym = XkbKeycodeToKeysym( x11->display, x_ev->xkey.keycode, 0, 
				     x_ev->xkey.state & ShiftMask ? 1 : 0 );
	if( keysym >= XK_space && keysym <= XK_asciitilde ) {
		sys_ev.key.code = keysym;
		sys_ev.type = LCUI_KEYPRESS;
		_DEBUG_MSG("char: %c\n", keysym);
		LCUI_TriggerEvent( &sys_ev, NULL );
	}
}

void LCUI_InitLinuxX11Keyboard( void )
{
	LCUI_BindSysEvent( KeyPress, OnKeyboardMessage, NULL, NULL );
	LCUI_BindSysEvent( KeyRelease, OnKeyboardMessage, NULL, NULL );
}

void LCUI_ExitLinuxX11Keyboard( void )
{
	LCUI_UnbindSysEvent( KeyPress, OnKeyboardMessage );
	LCUI_UnbindSysEvent( KeyRelease, OnKeyboardMessage );
}

#endif
