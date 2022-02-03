/*
 * linux_linux_mouse.c -- Mouse support for linux.
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

#include "../internal.h"

#ifdef LCUI_PLATFORM_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <LCUI/thread.h>

static struct linux_mouse_t {
	int x;
	int y;
	int button_state[2];

	int dev_fd;
	const char *dev_path;

	thread_t tid;
	LCUI_BOOL active;
} linux_mouse;

static void linux_dispatch_mouse_button_event(int button, int state)
{
	app_event_t ev = { 0 };
	if (linux_mouse.button_state[button - 1]) {
		if (!(state & button)) {
			ev.type = APP_EVENT_MOUSEUP;
			ev.mouse.x = linux_mouse.x;
			ev.mouse.y = linux_mouse.y;
			ev.mouse.button = button;
			linux_mouse.button_state[button - 1] = 0;
			app_post_event(&ev);
			app_event_destroy(&ev);
		}
	} else if (state & button) {
		ev.type = APP_EVENT_MOUSEDOWN;
		ev.mouse.x = linux_mouse.x;
		ev.mouse.y = linux_mouse.y;
		ev.mouse.button = button;
		linux_mouse.button_state[button - 1] = 1;
		app_post_event(&ev);
		app_event_destroy(&ev);
	}
}

static void linux_mouse_thread(void *arg)
{
	char buf[6];
	int state;
	fd_set readfds;
	struct timeval tv;
	app_event_t ev = { 0 };

	while (linux_mouse.active) {
		tv.tv_sec = 0;
		tv.tv_usec = 500000;
		FD_ZERO(&readfds);
		FD_SET(linux_mouse.dev_fd, &readfds);
		select(linux_mouse.dev_fd + 1, &readfds, NULL, NULL, &tv);
		if (!FD_ISSET(linux_mouse.dev_fd, &readfds)) {
			continue;
		}
		if (read(linux_mouse.dev_fd, buf, 6) <= 0) {
			continue;
		}
		state = buf[0] & 0x07;
		linux_mouse.x += buf[1];
		linux_mouse.y -= buf[2];
		linux_mouse.x = y_max(0, linux_mouse.x);
		linux_mouse.y = y_max(0, linux_mouse.y);
		linux_mouse.x = y_min(app_get_screen_width(), linux_mouse.x);
		linux_mouse.y = y_min(app_get_screen_height(), linux_mouse.y);
		ev.type = APP_EVENT_MOUSEMOVE;
		ev.mouse.x = linux_mouse.x;
		ev.mouse.y = linux_mouse.y;
		app_post_event(&ev);
		app_event_destroy(&ev);
		linux_dispatch_mouse_button_event(MOUSE_BUTTON_LEFT, state);
		linux_dispatch_mouse_button_event(MOUSE_BUTTON_RIGHT, state);
	}
}

int linux_mouse_init(void)
{
	linux_mouse.x = app_get_screen_width() / 2;
	linux_mouse.y = app_get_screen_height() / 2;
	linux_mouse.dev_path = getenv("LCUI_MOUSE_DEVICE");
	if (!linux_mouse.dev_path) {
		linux_mouse.dev_path = "/dev/input/mice";
	}
	logger_debug("[input] open mouse device: %s\n", linux_mouse.dev_path);
	if ((linux_mouse.dev_fd = open(linux_mouse.dev_path, O_RDONLY)) < 0) {
		logger_error("[input] open mouse device failed\n");
		return -1;
	}
	thread_create(&linux_mouse.tid, linux_mouse_thread, NULL);
	logger_debug("[input] mouse driver thread: %lld\n", linux_mouse.tid);
	return 0;
}

int linux_mouse_destroy(void)
{
	if (linux_mouse.active) {
		linux_mouse.active = FALSE;
		thread_join(linux_mouse.tid, NULL);
		close(linux_mouse.dev_fd);
	}
	return 0;
}

#endif
