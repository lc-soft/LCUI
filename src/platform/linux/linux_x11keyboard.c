/*
 * linux_x11keyboard.c -- keyboard support for linux xwindow.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <LCUI_Build.h>
#if defined(LCUI_BUILD_IN_LINUX) && defined(LCUI_VIDEO_DRIVER_X11)
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
