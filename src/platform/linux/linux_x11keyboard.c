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

#include "config.h"
#include <stdio.h>
#include <LCUI_Build.h>
#if defined(LCUI_BUILD_IN_LINUX) && defined(LCUI_VIDEO_DRIVER_X11)
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H
#include <X11/XKBlib.h>

static int ConvertKeyCode(KeySym keysym)
{
	switch (keysym) {
	case XK_Tab:
		return LCUI_KEY_TAB;
	case XK_Escape:
		return LCUI_KEY_ESCAPE;
	case XK_Return:
		return LCUI_KEY_ENTER;
	case XK_Delete:
		return LCUI_KEY_DELETE;
	case XK_BackSpace:
		return LCUI_KEY_BACKSPACE;
	case XK_Home:
		return LCUI_KEY_HOME;
	case XK_Left:
		return LCUI_KEY_LEFT;
	case XK_Up:
		return LCUI_KEY_UP;
	case XK_Right:
		return LCUI_KEY_RIGHT;
	case XK_Down:
		return LCUI_KEY_DOWN;
	case XK_Page_Up:
		return LCUI_KEY_PAGEUP;
	case XK_Page_Down:
		return LCUI_KEY_PAGEDOWN;
	case XK_End:
		return LCUI_KEY_END;
	case XK_Control_R:
	case XK_Control_L:
		return LCUI_KEY_CONTROL;
	case XK_Shift_R:
	case XK_Shift_L:
		return LCUI_KEY_SHIFT;
	case XK_Alt_L:
	case XK_Alt_R:
		return LCUI_KEY_ALT;
	case XK_Caps_Lock:
		return LCUI_KEY_CAPITAL;
	case XK_comma:
		return LCUI_KEY_COMMA;
	case XK_period:
		return LCUI_KEY_PERIOD;
	case XK_minus:
		return LCUI_KEY_MINUS;
	case XK_slash:
		return LCUI_KEY_SLASH;
	case XK_semicolon:
		return LCUI_KEY_SEMICOLON;
	case XK_equal:
		return LCUI_KEY_EQUAL;
	case XK_bracketleft:
		return LCUI_KEY_BRACKETLEFT;
	case XK_bracketright:
		return LCUI_KEY_BRACKETRIGHT;
	case XK_backslash:
		return LCUI_KEY_BACKSLASH;
	case XK_apostrophe:
		return LCUI_KEY_APOSTROPHE;
	case XK_grave:
		return LCUI_KEY_GRAVE;
	default:
		break;
	}
	return keysym;
}

static int ConvertKeyCodeToChar(LCUI_X11AppDriver x11, XEvent *e)
{
	return XkbKeycodeToKeysym(x11->display, e->xkey.keycode, 0,
				  e->xkey.state & ShiftMask ? 1 : 0);
}

static void OnKeyboardMessage(LCUI_Event ev, void *arg)
{
	KeySym keysym;
	XEvent *x_ev = arg;
	LCUI_X11AppDriver x11;
	LCUI_SysEventRec sys_ev;
	switch (x_ev->type) {
	case KeyPress:
		sys_ev.type = LCUI_KEYDOWN;
		break;
	case KeyRelease:
		sys_ev.type = LCUI_KEYUP;
		break;
	default:
		return;
	}
	x11 = LCUI_GetAppData();
	XAutoRepeatOn(x11->display);
	keysym = XkbKeycodeToKeysym(x11->display, x_ev->xkey.keycode, 0, 1);
	_DEBUG_MSG("keyname: %s\n", XKeysymToString(keysym));
	_DEBUG_MSG("keycode: %d, keyscancode: %u, keysym: %lu\n", keysym,
		   x_ev->xkey.keycode, keysym);
	sys_ev.key.code = ConvertKeyCode(keysym);
	sys_ev.key.shift_key = x_ev->xkey.state & ShiftMask ? TRUE : FALSE;
	sys_ev.key.ctrl_key = x_ev->xkey.state & ControlMask ? TRUE : FALSE;
	_DEBUG_MSG("shift: %d, ctrl: %d\n", sys_ev.key.shift_key,
		   sys_ev.key.ctrl_key);
	LCUI_TriggerEvent(&sys_ev, NULL);
	if (keysym >= XK_space && keysym <= XK_asciitilde &&
	    sys_ev.type == LCUI_KEYDOWN) {
		sys_ev.type = LCUI_KEYPRESS;
		sys_ev.key.code = ConvertKeyCodeToChar(x11, x_ev);
		_DEBUG_MSG("char: %c\n", sys_ev.key.code);
		LCUI_TriggerEvent(&sys_ev, NULL);
	}
}

void LCUI_InitLinuxX11Keyboard(void)
{
	LCUI_BindSysEvent(KeyPress, OnKeyboardMessage, NULL, NULL);
	LCUI_BindSysEvent(KeyRelease, OnKeyboardMessage, NULL, NULL);
}

void LCUI_FreeLinuxX11Keyboard(void)
{
	LCUI_UnbindSysEvent(KeyPress, OnKeyboardMessage);
	LCUI_UnbindSysEvent(KeyRelease, OnKeyboardMessage);
}

#endif
