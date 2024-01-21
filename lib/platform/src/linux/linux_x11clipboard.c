/*
 * lib/platform/src/linux/linux_x11clipboard.c: clipboard support for linux x11
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/*
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
 */

#include "../app.h"
#if defined(LIBPLAT_LINUX) && defined(LIBPLAT_HAS_LIBX11)
#include <thread.h>

#define CLIPBOARD_TIMEOUT 1000

typedef struct clipboard_action_t_ {
	void *arg;
	clipboard_callback_t callback;
	bool running;
} clipboard_action_t;

// @WhoAteDaCake
// TODO: do we need a mutex here? As the action is unlikely to happen
// often, safety seems to be a bigger priority than speed
static struct x11_clipboard_module_t {
	char *text;
	size_t text_len;
	clipboard_action_t *action;
	//
	Atom xclipboard;
	Atom xprimary;
	Atom xutf8_string;
	Atom xa_string;
	Atom xa_targets;
	Atom xa_text;
	// Observer thread
	thread_t observer_thread;
} x11_clipboard;

void x11_clipboard_execute_action(bool timed_out)
{
	wchar_t *wstr = NULL;
	clipboard_t clipboard_data = { 0 };
	clipboard_action_t *action = x11_clipboard.action;

	if (!timed_out) {
		_DEBUG_MSG("Didn't time out canceling observer thread\n");
		thread_cancel(x11_clipboard.observer_thread);
	}

	if (!action->running) {
		_DEBUG_MSG("Tried to x11_clipboard_execute_action before "
			   "copying started\n");
		return;
	}
	if (timed_out) {
		_DEBUG_MSG("action timed out\n");
	} else {
		size_t len = x11_clipboard.text_len + 1;
		wstr = malloc(sizeof(wchar_t) * len);
		len = decode_utf8(wstr, x11_clipboard.text, len);
		wstr[len] = 0;
		// Assign the data
		clipboard_data.text = wstr;
		clipboard_data.len = len;
		clipboard_data.image = NULL;
	}
	action->callback(&clipboard_data, action->arg);
	if (wstr != NULL) {
		free(wstr);
	}
	// Reset properties, so if something goes wrongly, we crash
	// instead of having undefined behaviour
	action->arg = NULL;
	action->callback = NULL;
	action->running = false;
}

void x11_clipboard_request_timeout(void *arg)
{
	sleep_ms(CLIPBOARD_TIMEOUT);
	if (x11_clipboard.action->running) {
		x11_clipboard_execute_action(true);
	}
}

/**
 * This function assumes that text pointer is only owned by clipboard
 */
int x11_clipboard_set_text(const wchar_t *text, size_t len)
{
	Display *display = x11_app_get_display();
	Window window = x11_app_get_main_window();
	Window clipboard_owner =
	    XGetSelectionOwner(display, x11_clipboard.xclipboard);

	// X11 doesn't support wchar_t, so we need to send it regular char
	size_t raw_len = encode_utf8(NULL, text, 0);
	char *raw_text = malloc((raw_len + 1)* sizeof(char));

	raw_len = encode_utf8(raw_text, text, raw_len);
	raw_text[raw_len] = '\0';
	if (raw_len == -1) {
		_DEBUG_MSG("Failed converting wchar_t* to char*\n");
		// Something failed here, should probably add debug message
		return -1;
	}
	// Make sure to free old text
	if (x11_clipboard.text) {
		free(x11_clipboard.text);
	}

	_DEBUG_MSG("Copying text to clipboard\n");
	if (x11_clipboard.text) {
		free(x11_clipboard.text);
	}
	x11_clipboard.text = raw_text;
	x11_clipboard.text_len = raw_len;
	if (clipboard_owner == window) {
		_DEBUG_MSG("Clipboard already owned\n");
		return -2;
	}
	XSetSelectionOwner(display, XA_PRIMARY, window, CurrentTime);
	XSetSelectionOwner(display, x11_clipboard.xclipboard, window,
			   CurrentTime);
	_DEBUG_MSG("Taken ownership of the clipboard\n");
	return 0;
}

int x11_clipboard_request_text(clipboard_callback_t callback, void *arg)
{
	Display *display = x11_app_get_display();
	Window window = x11_app_get_main_window();
	Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", false);
	Window clipboard_owner =
	    XGetSelectionOwner(display, x11_clipboard.xclipboard);
	clipboard_action_t *action = x11_clipboard.action;

	if (action->running) {
		_DEBUG_MSG("Tried to paste, while a paste was in progress\n");
		return 1;
	}
	action->arg = arg;
	action->callback = callback;
	action->running = true;

	// No need to continue, we should have stored text already
	// when copy was done
	// This branch will only get executed once we implement copy event
	if (clipboard_owner == window) {
		_DEBUG_MSG("Clipboard owned by self\n");
		x11_clipboard_execute_action(false);
		return 0;
	}
	if (clipboard_owner == None) {
		_DEBUG_MSG("Clipboard owner not found\n");
		return 1;
	}
	// @WhoAteDaCake
	// TODO: needs error handling if we can't access the clipboard?
	// TODO: some other implementations will try XA_STRING if no text was
	// retrieved from UTF8_STRING, however, our implementation is not
	// synchronous, so not sure how it would work, it needs further
	// investigation
	thread_create(&x11_clipboard.observer_thread,
			  x11_clipboard_request_timeout, NULL);
	XConvertSelection(display, x11_clipboard.xclipboard,
			  x11_clipboard.xutf8_string, XSEL_DATA, window,
			  CurrentTime);
	_DEBUG_MSG("Clipboard content requested, expect SelectionNotify\n");
	return 0;
}

static void x11_clipboard_on_notify(app_native_event_t *ev, void *arg)
{
	XEvent *x_ev = arg;
	clipboard_action_t *action = x11_clipboard.action;

	if (!action->running) {
		_DEBUG_MSG("SelectionNotify received, with no action\n");
		return;
	}
	_DEBUG_MSG("SelectionNotify received\n");

	if (x_ev->xselection.selection == x11_clipboard.xclipboard) {
		_DEBUG_MSG("Received Clipboard event\n");
		unsigned long N, size;
		char *data;
		Atom target;
		int format;
		XGetWindowProperty(
		    x_ev->xselection.display, x_ev->xselection.requestor,
		    x_ev->xselection.property, 0L, (~0L), 0, AnyPropertyType,
		    &target, &format, &size, &N, (unsigned char **)&data);
		if (target == x11_clipboard.xutf8_string ||
		    target == x11_clipboard.xa_string) {
			x11_clipboard.text = strndup(data, size);
			x11_clipboard.text_len = size;
			XFree(data);
		}
		XDeleteProperty(x_ev->xselection.display,
				x_ev->xselection.requestor,
				x_ev->xselection.property);
		x11_clipboard_execute_action(false);
	}
}

static void x11_clipboard_on_clear(app_native_event_t *ev, void *arg)
{
	// @WhoAteDaCake
	// TODO: validate against timestamp of event
	// if it has come too late, we should ignore event
	_DEBUG_MSG("Lost ownership of the clipboard\n");
	if (x11_clipboard.text) {
		free(x11_clipboard.text);
	}
	x11_clipboard.text = NULL;
}

// This function gets called when another process requests for the
// text we copied in our clipboard
static void x11_clipboard_on_request(app_native_event_t *ev, void *arg)
{
	XEvent *x_ev = ev;
	XEvent reply;

	_DEBUG_MSG("Received SelectionRequest\n");
	// Generates a refuse request by default
	reply.xselection.type = SelectionNotify;
	reply.xselection.requestor = x_ev->xselectionrequest.requestor;
	reply.xselection.selection = x_ev->xselectionrequest.selection;
	reply.xselection.target = x_ev->xselectionrequest.target;
	reply.xselection.property = None;
	// ^ None means refusal
	reply.xselection.time = x_ev->xselectionrequest.time;

	if (x_ev->xselectionrequest.target == x11_clipboard.xa_targets) {
		_DEBUG_MSG("Requested supported targets\n");
		Atom target_list[4];
		target_list[0] = x11_clipboard.xa_targets;
		target_list[1] = x11_clipboard.xa_text;
		target_list[2] = x11_clipboard.xutf8_string;
		target_list[3] = x11_clipboard.xa_string;

		reply.xselection.property = x_ev->xselectionrequest.property;
		XChangeProperty(x_ev->xselection.display,
				x_ev->xselectionrequest.requestor,
				reply.xselection.property, XA_ATOM, 32,
				PropModeReplace, (unsigned char *)&target_list,
				sizeof(target_list) / sizeof(target_list[0]));
	} else if (reply.xselection.target == x11_clipboard.xa_text ||
		   reply.xselection.target == x11_clipboard.xutf8_string ||
		   reply.xselection.target == x11_clipboard.xa_string) {
		if (!x11_clipboard.text) {
			_DEBUG_MSG("SelectionRequest received with no text\n");
		} else {
			_DEBUG_MSG("Requested for clipboard text\n");
			reply.xselection.property =
			    x_ev->xselectionrequest.property;
			XChangeProperty(x_ev->xselection.display,
					x_ev->xselectionrequest.requestor,
					reply.xselection.property,
					reply.xselection.target, 8,
					PropModeReplace,
					(unsigned char *)x11_clipboard.text,
					x11_clipboard.text_len);
		}
	}
	XSendEvent(x_ev->xselection.display, x_ev->xselectionrequest.requestor,
		   true, 0, &reply);
}

// @WhoAteDaCake
// TODO: Implement INCR retrieval
void x11_clipboard_init(void)
{
	Display *display = x11_app_get_display();

	// Allocate action once
	x11_clipboard.action = malloc(sizeof(clipboard_action_t));
	x11_clipboard.action->running = false;
	// Set atoms, that will be re-used
	x11_clipboard.xclipboard = XInternAtom(display, "CLIPBOARD", false);
	x11_clipboard.xprimary = XA_PRIMARY;
	x11_clipboard.xutf8_string = XInternAtom(display, "UTF8_STRING", false);
	x11_clipboard.xa_string = XA_STRING;
	x11_clipboard.xa_targets = XInternAtom(display, "TARGETS", false);
	x11_clipboard.xa_text = XInternAtom(display, "TEXT", false);

	app_on_native_event(SelectionNotify, x11_clipboard_on_notify, NULL);
	app_on_native_event(SelectionClear, x11_clipboard_on_clear, NULL);
	app_on_native_event(SelectionRequest, x11_clipboard_on_request, NULL);
}

void x11_clipboard_destroy(void)
{
	free(x11_clipboard.action);
	app_off_native_event(SelectionNotify, x11_clipboard_on_notify);
	app_off_native_event(SelectionClear, x11_clipboard_on_clear);
	app_off_native_event(SelectionRequest, x11_clipboard_on_request);
}

#endif
