/*
 * linux_mouse.c -- Mouse support for linux.
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
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/platform.h>
#include <LCUI/display.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

enum MouseButtonId { MOUSE_BUTTON_LEFT = 1, MOUSE_BUTTON_RIGHT = 1 << 1 };

static struct LCUI_LinuxMouseDriver {
	int x;
	int y;
	int button_state[2];

	int dev_fd;
	const char *dev_path;

	LCUI_Thread tid;
	LCUI_BOOL active;
} mouse;

static void DispathMouseButtonEvent(int button, int state)
{
	LCUI_SysEventRec ev = { 0 };
	if (mouse.button_state[button - 1]) {
		if (!(state & button)) {
			ev.type = LCUI_MOUSEUP;
			ev.button.x = mouse.x;
			ev.button.y = mouse.y;
			ev.button.button = button;
			mouse.button_state[button - 1] = 0;
			LCUI_TriggerEvent(&ev, NULL);
			LCUI_DestroyEvent(&ev);
		}
	} else if (state & button) {
		ev.type = LCUI_MOUSEDOWN;
		ev.button.x = mouse.x;
		ev.button.y = mouse.y;
		ev.button.button = button;
		mouse.button_state[button - 1] = 1;
		LCUI_TriggerEvent(&ev, NULL);
		LCUI_DestroyEvent(&ev);
	}
}

static void DispatchMouseEvent(void *arg1, void *arg2)
{
	char *buf = arg1;
	int state = buf[0] & 0x07;
	LCUI_SysEventRec ev = { 0 };

	mouse.x += buf[1];
	mouse.y -= buf[2];
	mouse.x = max(0, mouse.x);
	mouse.y = max(0, mouse.y);
	mouse.x = min(LCUIDisplay_GetWidth(), mouse.x);
	mouse.y = min(LCUIDisplay_GetHeight(), mouse.y);
	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = mouse.x;
	ev.motion.y = mouse.y;
	ev.motion.xrel = buf[1];
	ev.motion.yrel = -buf[2];
	LCUI_TriggerEvent(&ev, NULL);
	LCUI_DestroyEvent(&ev);
	DispathMouseButtonEvent(MOUSE_BUTTON_LEFT, state);
	DispathMouseButtonEvent(MOUSE_BUTTON_RIGHT, state);
}

static void LinuxMouseThread(void *arg)
{
	char buf[6];
	fd_set readfds;
	struct timeval tv;
	LCUI_TaskRec task = { 0 };

	mouse.active = TRUE;
	task.func = DispatchMouseEvent;
	task.destroy_arg[0] = free;
	while (mouse.active) {
		tv.tv_sec = 0;
		tv.tv_usec = 500000;

		FD_ZERO(&readfds);
		FD_SET(mouse.dev_fd, &readfds);
		select(mouse.dev_fd + 1, &readfds, NULL, NULL, &tv);
		if (FD_ISSET(mouse.dev_fd, &readfds)) {
			if (read(mouse.dev_fd, buf, 6) <= 0) {
				continue;
			}
			task.arg[0] = malloc(sizeof(char) * 6);
			if (!task.arg[0]) {
				perror("[input] mouse driver");
				continue;
			}
			memcpy(task.arg[0], buf, sizeof(char) * 6);
			LCUI_PostTask(&task);
		}
	}
}

static int InitLinuxMouse(void)
{
	mouse.x = LCUIDisplay_GetWidth() / 2;
	mouse.y = LCUIDisplay_GetHeight() / 2;
	mouse.dev_path = getenv("LCUI_MOUSE_DEVICE");
	if (!mouse.dev_path) {
		mouse.dev_path = "/dev/input/mice";
	}
	Logger_Debug("[input] open mouse device: %s\n", mouse.dev_path);
	if ((mouse.dev_fd = open(mouse.dev_path, O_RDONLY)) < 0) {
		Logger_Error("[input] open mouse device failed\n");
		return -1;
	}
	LCUIThread_Create(&mouse.tid, LinuxMouseThread, NULL);
	Logger_Debug("[input] mouse driver thread: %lld\n", mouse.tid);
	return 0;
}

static void FreeLinuxMouse(void)
{
	if (mouse.active) {
		mouse.active = FALSE;
		LCUIThread_Join(mouse.tid, NULL);
		close(mouse.dev_fd);
	}
}

void LCUI_InitLinuxMouse(void)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_InitLinuxX11Mouse();
		return;
	}
#endif
	InitLinuxMouse();
}

void LCUI_FreeLinuxMouse(void)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_FreeLinuxX11Mouse();
		return;
	}
#endif
	FreeLinuxMouse();
}

#endif
