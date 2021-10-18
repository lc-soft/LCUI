/*
 * linux_x11clipboard.c -- clipboard support for linux x11
 *
 * References:
 * - https://github.com/exebook/x11clipboard/blob/master/x11paste.c
 * - https://github.com/edrosten/x_clipboard/blob/master/paste.cc
 * -
 * https://github.com/libsdl-org/SDL/blob/515b7e93b5af3dc552a7974ed7245a0e0456ae2a/src/video/x11/SDL_x11events.c
 * -
 * https://github.com/libsdl-org/SDL/blob/main/src/video/x11/SDL_x11clipboard.c
 * -
 * https://github.com/GNOME/gtk/blob/b539c92312d449f41710e6930aaf086454d667d2/gdk/x11/gdkclipboard-x11.c
 * -
 * https://github.com/godotengine/godot/blob/a7011fa29488f5356949667eb8e2b296cbbd9923/platform/linuxbsd/display_server_x11.cpp
 * - https://www.jwz.org/doc/x-cut-and-paste.html
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
#include <LCUI/platform.h>
#include <LCUI/clipboard.h>
#include LCUI_EVENTS_H
#include <X11/Xatom.h>

typedef struct LCUI_ClipboardCallbackRec_ {
	void *widget;
	LCUI_ClipboardAction action;
	LCUI_BOOL running;
} LCUI_ClipboardCallbackRec, *LCUI_ClipboardCallback;

// @WhoAteDaCake
// TODO: do we need a mutex here? As the action is unlikely to happen
// often, safety seems to be a bigger priority than speed
static struct LCUI_LinuxClipboardDriver {
	char *text;
	LCUI_ClipboardCallback callback;
	//
	Atom xclipboard;
	Atom xprimary;
	Atom xutf8_string;
	Atom xa_string;
	Atom xa_targets;
	Atom xa_text;
} clipboard;

void ExecuteCallback(void)
{
	LCUI_ClipboardCallback callback = clipboard.callback;
	if (!callback->running) {
		_DEBUG_MSG("Tried to ExecuteCallback before copying started\n");
		return;
	}
	callback->action(callback->widget, clipboard.text);
	// Reset properties, so if something goes wrongly, we crash
	// instead of having undefined behaviour
	callback->widget = NULL;
	callback->action = NULL;
	callback->running = FALSE;
}

void RequestClipboardContent(void)
{
	LCUI_X11AppDriver x11 = LCUI_GetAppData();
	Display *display = x11->display;
	Window window = x11->win_main;
	// Atom CLIPBOARD = XInternAtom(display, "CLIPBOARD", FALSE);
	Window clipboard_owner =
	    XGetSelectionOwner(display, clipboard.xclipboard);
	// No need to continue, we should have stored text already
	// when copy was done
	// This branch will only get executed once we implement copy event
	if (clipboard_owner == window) {
		_DEBUG_MSG("Clipboard owned by self\n");
		ExecuteCallback();
		return;
	}
	if (clipboard_owner == None) {
		_DEBUG_MSG("Clipboard owner not found\n");
		return;
	}
	// @WhoAteDaCake
	// TODO: needs error handling if we can't access the clipboard?
	// TODO: some other implementations will try XA_STRING if no text was
	// retrieved from UTF8_STRING, however, our implementation is not
	// synchronous, so not sure how it would work, it needs further
	// investigation
	Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", FALSE);
	XConvertSelection(display, clipboard.xclipboard, clipboard.xutf8_string,
			  XSEL_DATA, window, CurrentTime);
	_DEBUG_MSG("Clipboard content requested, expect SelectionNotify\n");
}

/**
 * This function assumes that text pointer is only owned by clipboard
 */
void LCUI_LinuxX11SetClipboardText(wchar_t *text, size_t len)
{
	// X11 doesn't support wchar_t, so we need to send it regular char
	char* raw_text = malloc((len + 1) * sizeof(char));
	int raw_len = wcstombs(raw_text, text, len);
	free(text);
	if (raw_len == -1) {
		_DEBUG_MSG("Failed converting wchar_t* to char*\n");
		// Something failed here, should probably add debug message
		return;	
	}

	LCUI_X11AppDriver x11 = LCUI_GetAppData();
	Display *display = x11->display;
	Window window = x11->win_main;
	_DEBUG_MSG("Copying text to clipboard\n");
	if (clipboard.text) {
		free(clipboard.text);
	}
	clipboard.text = raw_text;
	Window clipboard_owner =
	    XGetSelectionOwner(display, clipboard.xclipboard);
	if (clipboard_owner == window) {
		_DEBUG_MSG("Clipboard already owned\n");
		return;
	}
	XSetSelectionOwner(display, XA_PRIMARY, window, CurrentTime);
	XSetSelectionOwner(display, clipboard.xclipboard, window, CurrentTime);
	_DEBUG_MSG("Taken ownership of the clipboard\n");
}

void LCUI_UseLinuxX11Clipboard(void *widget, void *action)
{
	LCUI_ClipboardCallback callback = clipboard.callback;
	if (callback->running) {
		_DEBUG_MSG("Tried to paste, while a paste was in progress\n");
		return;
	}
	callback->widget = widget;
	callback->action = action;
	callback->running = TRUE;
	RequestClipboardContent();
}

static void OnSelectionNotify(LCUI_Event ev, void *arg)
{
	LCUI_ClipboardCallback callback = clipboard.callback;
	if (!callback->running) {
		_DEBUG_MSG("SelectionNotify received, with no callback\n");
		return;
	}
	_DEBUG_MSG("SelectionNotify received\n");
	XEvent *x_ev = arg;
	LCUI_X11AppDriver x11 = LCUI_GetAppData();
	Display *display = x11->display;

	if (x_ev->xselection.selection == clipboard.xclipboard) {
		_DEBUG_MSG("Received Clipboard event\n");
		unsigned long N, size;
		char *data;
		Atom target;
		int format;
		XGetWindowProperty(
		    x_ev->xselection.display, x_ev->xselection.requestor,
		    x_ev->xselection.property, 0L, (~0L), 0, AnyPropertyType,
		    &target, &format, &size, &N, (unsigned char **)&data);
		if (target == clipboard.xutf8_string ||
		    target == clipboard.xa_string) {
			clipboard.text = strndup(data, size);
			XFree(data);
		}
		XDeleteProperty(x_ev->xselection.display,
				x_ev->xselection.requestor,
				x_ev->xselection.property);
		ExecuteCallback();
	}
}

static void OnSelectionClear(LCUI_Event ev, void *arg)
{
	// @WhoAteDaCake
	// TODO: validate against timestamp of event
	// if it has come too late, we should ignore event
	_DEBUG_MSG("Lost ownership of the clipboard\n");
	if (clipboard.text) {
		free(clipboard.text);
	}
	clipboard.text = NULL;
}

// This function gets called when another process requests for the
// text we copied in our clipboard
static void OnSelectionRequest(LCUI_Event ev, void *arg)
{
	_DEBUG_MSG("Received SelectionRequest\n");
	XEvent *x_ev = arg;
	// Generates a refuse request by default
	XEvent reply;
	reply.xselection.type = SelectionNotify;
	reply.xselection.requestor = x_ev->xselectionrequest.requestor;
	reply.xselection.selection = x_ev->xselectionrequest.selection;
	reply.xselection.target = x_ev->xselectionrequest.target;
	reply.xselection.property = None;
	// ^ None means refusal
	reply.xselection.time = x_ev->xselectionrequest.time;

	if (x_ev->xselectionrequest.target == clipboard.xa_targets) {
		_DEBUG_MSG("Requested supported targets\n");
		Atom target_list[4];
		target_list[0] = clipboard.xa_targets;
		target_list[1] = clipboard.xa_text;
		target_list[2] = clipboard.xutf8_string;
		target_list[3] = clipboard.xa_string;

		reply.xselection.property = x_ev->xselectionrequest.property;
		XChangeProperty(x_ev->xselection.display,
				x_ev->xselectionrequest.requestor,
				reply.xselection.property, XA_ATOM, 32,
				PropModeReplace, (unsigned char *)&target_list,
				sizeof(target_list) / sizeof(target_list[0]));
	} else if (reply.xselection.target == clipboard.xa_text ||
		   reply.xselection.target == clipboard.xutf8_string ||
		   reply.xselection.target == clipboard.xa_string) {
		if (!clipboard.text) {
			_DEBUG_MSG("SelectionRequest received with no text\n");
		} else {
			_DEBUG_MSG("Requested for clipboard text\n");
			reply.xselection.property = x_ev->xselectionrequest.property;
			XChangeProperty(
			    x_ev->xselection.display,
			    x_ev->xselectionrequest.requestor,
			    reply.xselection.property, reply.xselection.target,
			    8, PropModeReplace, (unsigned char *)clipboard.text,
			    strlen(clipboard.text));
		}
	}
	XSendEvent(x_ev->xselection.display, x_ev->xselectionrequest.requestor,
		   TRUE, 0, &reply);
}

// @WhoAteDaCake
// TODO: Implement INCR retrieval
void LCUI_InitLinuxX11Clipboard(void)
{
	LCUI_X11AppDriver x11 = LCUI_GetAppData();
	Display *display = x11->display;
	// Allocate callback once
	LCUI_ClipboardCallback callback;
	callback = NEW(LCUI_ClipboardCallbackRec, 1);
	callback->running = FALSE;
	clipboard.callback = callback;
	// Set atoms, that will be re-used
	clipboard.xclipboard = XInternAtom(display, "CLIPBOARD", FALSE);
	clipboard.xprimary = XA_PRIMARY;
	clipboard.xutf8_string = XInternAtom(display, "UTF8_STRING", FALSE);
	clipboard.xa_string = XA_STRING;
	clipboard.xa_targets = XInternAtom(display, "TARGETS", FALSE);
	clipboard.xa_text = XInternAtom(display, "TEXT", FALSE);

	LCUI_BindSysEvent(SelectionNotify, OnSelectionNotify, NULL, NULL);
	LCUI_BindSysEvent(SelectionClear, OnSelectionClear, NULL, NULL);
	LCUI_BindSysEvent(SelectionRequest, OnSelectionRequest, NULL, NULL);
}

void LCUI_FreeLinuxX11Clipboard(void)
{
	free(clipboard.callback);
	LCUI_UnbindSysEvent(SelectionNotify, OnSelectionNotify);
	LCUI_UnbindSysEvent(SelectionClear, OnSelectionClear);
	LCUI_UnbindSysEvent(SelectionRequest, OnSelectionRequest);
}

#endif
