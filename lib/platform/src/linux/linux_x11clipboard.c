/*
 * linux_x11clipboard.c -- keyboard support for linux xwindow
 *
 * References:
 * - https://github.com/exebook/x11clipboard/blob/master/x11paste.c
 * - https://github.com/libsdl-org/SDL/blob/515b7e93b5af3dc552a7974ed7245a0e0456ae2a/src/video/x11/SDL_x11events.c
 * - https://github.com/libsdl-org/SDL/blob/main/src/video/x11/SDL_x11clipboard.c
 * - https://github.com/GNOME/gtk/blob/b539c92312d449f41710e6930aaf086454d667d2/gdk/x11/gdkclipboard-x11.c
 * - https://github.com/godotengine/godot/blob/a7011fa29488f5356949667eb8e2b296cbbd9923/platform/linuxbsd/display_server_x11.cpp
 *
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
#include LCUI_CLIPBOARD_H
#include <X11/Xatom.h>

#ifdef X_HAVE_UTF8_STRING
#define TEXT_FORMAT XInternAtom(display, "UTF8_STRING", FALSE)
#else
#define TEXT_FORMAT XA_STRING
#endif

typedef struct LCUI_ClipboardCallbackRec_ {
	void *widget;
	void (*action)(void *widget, char *text);
	LCUI_BOOL running;
} LCUI_ClipboardCallbackRec, *LCUI_ClipboardCallback;

static struct LCUI_LinuxClipboardDriver {
	char* text;
	LCUI_ClipboardCallback callback;
} clipboard;


void ExecuteCallback(void)
{
	LCUI_ClipboardCallback callback = clipboard.callback;
	if (!callback->running) {
		// @WhoAteDaCake
		// TODO: some sort of error ?
		return;
	}
	callback->action(callback->widget, clipboard.text);
	// Reset properties, so if something goes wrongly, we crash
	// instead of having undefined behaviour
	callback->widget = NULL;
	callback->action = NULL;
	callback->running = FALSE;
}

void RequestClipboard(void)
{
	LCUI_X11AppDriver x11 = LCUI_GetAppData();
	Display *display = x11->display;
	// @WhoAteDaCake
	// Not sure which window should be used root or main
	Window window = x11->win_root;
	Atom CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0);
	Window clipboard_owner = XGetSelectionOwner(display, CLIPBOARD);
	// No need to continue, we should have stored text already
	// when copy was done
	if (clipboard_owner == window) {
		ExecuteCallback();
		return;
	}
	// @WhoAteDaCake
	// TODO: needs error handling if we can't access the clipboard?
	// TODO: check who the owner is, if it's us, we can call the callback
	// directly with text stored in clipboard.text
	Atom format = TEXT_FORMAT;
	Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", 0);
	XConvertSelection(display, CLIPBOARD, format, XSEL_DATA, window, CurrentTime);
}

void LCUI_LinuxX11UseClipboard(void *widget, void *action) {
	// TODO: maybe run a warning if it's already running
	LCUI_ClipboardCallback callback = clipboard.callback;
	callback->widget = widget;
	callback->action = action;
	callback->running = TRUE;
	// Request paste event
	RequestClipboard();
}

static void OnPasteReady(LCUI_Event ev, void *arg)
{
	printf("Print ready!\n");
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
	// Allocate clipboard once
	LCUI_ClipboardCallback callback;
	callback = NEW(LCUI_ClipboardCallbackRec, 1);
	callback->running = FALSE;
	clipboard.callback = callback;

	LCUI_BindSysEvent(SelectionNotify, OnPasteReady, NULL, NULL);
}

void LCUI_FreeLinuxX11Clipboard(void)
{	
	free(clipboard.callback);
	LCUI_UnbindSysEvent(SelectionNotify, OnPasteReady);
}

#endif
