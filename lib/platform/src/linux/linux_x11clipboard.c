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
#if defined(LCUI_BUILD_IN_LINUX) && defined(USE_LIBX11)
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
// #include LCUI_KEYBOARD_H
#include <X11/XKBlib.h>

// static struct LCUI_LinuxKeyboardDriver {
// #ifdef USE_LINUX_INPUT_EVENT
// 	int dev_fd;
// 	char *dev_path;
// #else
// 	int fd;
// 	struct termios tm;
// #endif
// 	LCUI_Thread tid;
// 	LCUI_BOOL active;
// } keyboard;

typedef struct LCUI_ClipboardCallback {
	void *widget;
	void *(action)(void *widget, char *text);
} LCUI_ClipboardCallback;

static struct LCUI_LinuxClipboardDriver {
	char* text;
	LCUI_ClipboardCallback callback;
} clipboard;

static void LCUI_LinuxX11UseClipboard(void *widget, void *action) {
	// TODO: maybe run a warning if it's already running
	LCUI_ClipboardCallback callback = {0};
	callback.widget = widget;
	callback.action = action;
	clipboard.callback = callback;
	
}

static void OnClipboardMessage(LCUI_Event ev, void *arg)
{
	// KeySym keysym;
	// XEvent *x_ev = arg;
	// LCUI_X11AppDriver x11;
	// LCUI_SysEventRec sys_ev;
	// int min_keycode;
	// int max_keycode;

	// switch (x_ev->type) {
	// case KeyPress:
	// 	sys_ev.type = LCUI_KEYDOWN;
	// 	break;
	// case KeyRelease:
	// 	sys_ev.type = LCUI_KEYUP;
	// 	break;
	// default:
	// 	return;
	// }
	// x11 = LCUI_GetAppData();
	// XAutoRepeatOn(x11->display);
	// keysym = XkbKeycodeToKeysym(x11->display, x_ev->xkey.keycode, 0, 1);
	// _DEBUG_MSG("keyname: %s\n", XKeysymToString(keysym));
	// _DEBUG_MSG("keycode: %d, keyscancode: %u, keysym: %lu\n", keysym,
	// 	   x_ev->xkey.keycode, keysym);
	// sys_ev.key.code = ConvertKeyCode(keysym);
	// sys_ev.key.shift_key = x_ev->xkey.state & ShiftMask ? TRUE : FALSE;
	// sys_ev.key.ctrl_key = x_ev->xkey.state & ControlMask ? TRUE : FALSE;
	// _DEBUG_MSG("shift: %d, ctrl: %d\n", sys_ev.key.shift_key,
	// 	   sys_ev.key.ctrl_key);
	// LCUI_TriggerEvent(&sys_ev, NULL);

	// XDisplayKeycodes(x11->display, &min_keycode, &max_keycode);
	// if (keysym >= min_keycode && keysym <= max_keycode &&
	//     sys_ev.type == LCUI_KEYDOWN) {
	// 	sys_ev.type = LCUI_KEYPRESS;
	// 	sys_ev.key.code = ConvertKeyCodeToChar(x11, x_ev);
	// 	_DEBUG_MSG("char: %c\n", sys_ev.key.code);
	// 	LCUI_TriggerEvent(&sys_ev, NULL);
	// }
}

void LCUI_InitLinuxX11Clipboard(void)
{
	LCUI_BindSysEvent(SelectionNotify, OnClipboardMessage);
	// LCUI_BindSysEvent(KeyPress, OnKeyboardMessage, NULL, NULL);
	// LCUI_BindSysEvent(KeyRelease, OnKeyboardMessage, NULL, NULL);
}

void LCUI_FreeLinuxX11Clipboard(void)
{
	LCUI_UnbindSysEvent(SelectionNotify, OnClipboardMessage);
	// LCUI_UnbindSysEvent(KeyRelease, OnKeyboardMessage);
}

#endif
