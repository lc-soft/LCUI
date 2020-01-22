/*
 * linux_keyboard.c -- Keyboard support for linux.
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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#ifdef USE_LINUX_INPUT_EVENT
#include <linux/input.h>
#else
#include <signal.h>
#include <termios.h>
#endif
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/platform.h>
#include <LCUI/input.h>
#include <LCUI/ime.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H

static struct LCUI_LinuxKeyboardDriver {
#ifdef USE_LINUX_INPUT_EVENT
	int dev_fd;
	char *dev_path;
#else
	int fd;
	struct termios tm;
#endif
	LCUI_Thread tid;
	LCUI_BOOL active;
} keyboard;

/* FIXME: this driver does not working, so we won't use it for now */
#ifdef USE_LINUX_INPUT_EVENT
static void LinuxKeybnoardThread(void *arg)
{
	size_t bytes;
	struct input_event data;

	keyboard.active = TRUE;
	Logger_Debug("[input] keyboard driver thread: %lld\n", keyboard.tid);
	while (keyboard.active) {
		bytes = read(keyboard.dev_fd, &data, sizeof(data));
		if (bytes > 0 && data.type == EV_KEY) {
			_DEBUG_MSG("Keypress value=%x, type=%x, code=%x\n",
				   data.value, data.type, data.code);
		}
	}
}

static int InitLinuxKeybord(void)
{
	keyboard.dev_path = getenv("LCUI_KEYBOARD_DEVICE");
	if (!keyboard.dev_path) {
		keyboard.dev_path = "/dev/input/event0";
	}
	if ((keyboard.dev_fd = open(keyboard.dev_path, O_RDONLY | O_NONBLOCK)) <
	    0) {
		Logger_Error("[input] open keyboard device failed");
		return -1;
	}
	LCUIThread_Create(&keyboard.tid, LinuxKeybnoardThread, NULL);
	return 0;
}

static int FreeLinuxKeyboard(void)
{
	if (!keyboard.active) {
		return -1;
	}
	keyboard.active = FALSE;
	LCUIThread_Join(keyboard.tid, NULL);
	close(keyboard.dev_fd);
	return 0;
}

#else

static void DispatchKeyboardEvent(void *arg1, void *arg2)
{
	wchar_t str[2];
	LCUI_SysEventRec ev = { 0 };

	ev.key.code = *((int *)arg1);
	switch (ev.key.code) {
	case 183:
		ev.key.code = LCUI_KEY_UP;
		break;
	case 184:
		ev.key.code = LCUI_KEY_DOWN;
		break;
	case 185:
		ev.key.code = LCUI_KEY_RIGHT;
		break;
	case 186:
		ev.key.code = LCUI_KEY_LEFT;
		break;
	case 127:
		ev.key.code = LCUI_KEY_BACKSPACE;
		break;
	case 293:
		ev.key.code = LCUI_KEY_HOME;
		break;
	case 295:
		ev.key.code = LCUI_KEY_DELETE;
		break;
	case 296:
		ev.key.code = LCUI_KEY_END;
		break;
	case 297:
		ev.key.code = LCUI_KEY_PAGEUP;
		break;
	case 298:
		ev.key.code = LCUI_KEY_PAGEDOWN;
	default:
		break;
	}
	DEBUG_MSG("%c, %d\n", ev.key.code, ev.key.code);
	ev.type = LCUI_KEYDOWN;
	LCUI_TriggerEvent(&ev, NULL);
	LCUI_DestroyEvent(&ev);
	/* FIXME: this driver is not yet possible to get the key state directly,
	 * the following is a temporary solution.
	 */
	ev.type = LCUI_KEYUP;
	LCUI_TriggerEvent(&ev, NULL);
	LCUI_DestroyEvent(&ev);
	if (ev.key.code >= ' ' && ev.key.code <= '~') {
		str[0] = ev.key.code;
		ev.type = LCUI_KEYPRESS;
		LCUI_TriggerEvent(&ev, NULL);
		LCUI_DestroyEvent(&ev);
		LCUIIME_Commit(str, 1);
	}
}

LCUI_BOOL kbhit(void)
{
	int ch, flags;
	struct termios tm;

	tcgetattr(keyboard.fd, &tm);
	flags = fcntl(keyboard.fd, F_GETFL, 0);
	fcntl(keyboard.fd, F_SETFL, flags | O_NONBLOCK);
	ch = getchar();
	tcsetattr(keyboard.fd, TCSANOW, &tm);
	fcntl(keyboard.fd, F_SETFL, flags);
	if (ch != EOF) {
		ungetc(ch, stdin);
		return TRUE;
	}
	return FALSE;
}

int getch(void)
{
	int c = EOF;
	struct termios tm;

	tm = keyboard.tm;

	/* canonical input and echo modes */
	tm.c_lflag &= ~(ICANON | ECHO);
	tm.c_cc[VMIN] = 0;
	tm.c_cc[VTIME] = 1;
	tcsetattr(keyboard.fd, TCSANOW, &tm);
	/* get char from modified stdin */
	c = getchar();
	/* reset stdin to previous state */
	tcsetattr(keyboard.fd, TCSANOW, &keyboard.tm);
	return c;
}

static void on_signal(int signal)
{
	tcsetattr(keyboard.fd, TCSANOW, &keyboard.tm);
	exit(1);
}

static void LinuxKeybnoardThread(void *arg)
{
	int key;
	LCUI_TaskRec task = { 0 };

	task.func = DispatchKeyboardEvent;
	task.destroy_arg[0] = free;
	while (keyboard.active) {
		key = getch();
		if (key == EOF) {
			continue;
		}
		while (kbhit()) {
			key += getch();
		}
		task.arg[0] = malloc(sizeof(int));
		if (!task.arg[0]) {
			perror("[input] keyboard driver");
			continue;
		}
		memcpy(task.arg[0], &key, sizeof(int));
		LCUI_PostTask(&task);
	}
}

static int InitLinuxKeybord(void)
{
	int i = 0;
	int signals[] = { SIGABRT, SIGINT, SIGKILL, SIGQUIT, SIGSTOP, SIGTERM };
	int len = sizeof(signals) / sizeof(signals[0]);

	keyboard.fd = STDIN_FILENO;
	if (tcgetattr(keyboard.fd, &keyboard.tm) < 0) {
		return -1;
	}
	for (; i < len; ++i) {
		signal(signals[i], on_signal);
	}
	keyboard.active = TRUE;
	LCUIThread_Create(&keyboard.tid, LinuxKeybnoardThread, NULL);
	Logger_Debug("[input] keyboard driver thread: %lld\n", keyboard.tid);
	return 0;
}

static int FreeLinuxKeyboard(void)
{
	if (!keyboard.active) {
		return 0;
	}
	keyboard.active = FALSE;
	LCUIThread_Join(keyboard.tid, NULL);
	if (tcsetattr(keyboard.fd, TCSANOW, &keyboard.tm) < 0) {
		return -1;
	}
	return 0;
}

#endif

void LCUI_InitLinuxKeyboard(void)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_InitLinuxX11Keyboard();
		return;
	}
#endif
	InitLinuxKeybord();
}

void LCUI_FreeLinuxKeyboard(void)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_FreeLinuxX11Keyboard();
		return;
	}
#endif
	FreeLinuxKeyboard();
}

#endif
