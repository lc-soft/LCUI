/*
 * linux_x11events.c -- Event loop support for linux xwindow.
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
#include <stdlib.h>
#include <LCUI_Build.h>
#if defined(LCUI_BUILD_IN_LINUX) && defined(LCUI_VIDEO_DRIVER_X11)
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static LCUI_X11AppDriverRec x11;

void LCUI_SetLinuxX11MainWindow(Window win)
{
	x11.win_main = win;
	XSetWMProtocols(x11.display, win, &x11.wm_delete, 1);
	XSelectInput(x11.display, win,
		     ExposureMask | KeyPressMask | ButtonPress |
			 StructureNotifyMask | ButtonReleaseMask |
			 KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
			 PointerMotionMask | Button1MotionMask |
			 VisibilityChangeMask);
	XFlush(x11.display);
}

static LCUI_BOOL X11_WaitEvent(void)
{
	int fd;
	fd_set fdset;
	struct timeval tv;
	XFlush(x11.display);
	fd = ConnectionNumber(x11.display);
	if (XEventsQueued(x11.display, QueuedAlready)) {
		return TRUE;
	}
	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	tv.tv_sec = 0;
	tv.tv_usec = 10000;
	if (select(fd + 1, &fdset, NULL, NULL, &tv) == 1) {
		return XPending(x11.display);
	}
	return FALSE;
}

static LCUI_BOOL X11_DispatchEvent(void)
{
	XEvent xevent;
	if (!XEventsQueued(x11.display, QueuedAlready)) {
		return FALSE;
	}
	XNextEvent(x11.display, &xevent);
	EventTrigger_Trigger(x11.trigger, xevent.type, &xevent);
	if (xevent.type == ClientMessage) {
		if (xevent.xclient.data.l[0] == x11.wm_delete) {
			LCUI_Quit();
		}
	}
	return TRUE;
}

static void X11_ProcessEvents(void)
{
	int i;
	if (!X11_WaitEvent()) {
		return;
	}
	for (i = 0; X11_DispatchEvent() && i < 100; ++i)
		;
}

static int X11_BindSysEvent(int event_id, LCUI_EventFunc func, void *data,
			    void (*destroy_data)(void *))
{
	return EventTrigger_Bind(x11.trigger, event_id, func, data,
				 destroy_data);
}

static int X11_UnbindSysEvent(int event_id, LCUI_EventFunc func)
{
	return EventTrigger_Unbind(x11.trigger, event_id, func);
}

static int X11_UnbindSysEvent2(int handler_id)
{
	return EventTrigger_Unbind2(x11.trigger, handler_id);
}

static void *X11_GetData(void)
{
	return &x11;
}

void LCUI_PreInitLinuxX11App(void *data)
{
	return;
}

LCUI_AppDriver LCUI_CreateLinuxX11AppDriver(void)
{
	ASSIGN(app, LCUI_AppDriver);
	x11.display = XOpenDisplay(NULL);
	if (!x11.display) {
		free(app);
		return NULL;
	}
	x11.screen = DefaultScreen(x11.display);
	x11.win_root = RootWindow(x11.display, x11.screen);
	x11.cmap = DefaultColormap(x11.display, x11.screen);
	x11.wm_delete = XInternAtom(x11.display, "WM_DELETE_WINDOW", FALSE);
	app->ProcessEvents = X11_ProcessEvents;
	app->BindSysEvent = X11_BindSysEvent;
	app->UnbindSysEvent = X11_UnbindSysEvent;
	app->UnbindSysEvent2 = X11_UnbindSysEvent2;
	app->GetData = X11_GetData;
	app->id = LCUI_APP_LINUX_X11;
	x11.trigger = EventTrigger();
	return app;
}

void LCUI_DestroyLinuxX11AppDriver(LCUI_AppDriver app)
{
	EventTrigger_Destroy(x11.trigger);
	XCloseDisplay(x11.display);
	x11.trigger = NULL;
	free(app);
}
#endif
