/*
 * lib/ptk/src/linux/keyboard.c: -- Keyboard support for linux.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk.h"
#ifdef PTK_LINUX
#include <yutil.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <thread.h>

static struct linux_keyboard {
	int fd;
	struct termios tm;
	thread_t tid;
	bool active;
} linux_keyboard;

static void linux_keyboard_dispatch_event(int key)
{
	wchar_t str[2];
	ptk_event_t ev = { 0 };

	ev.key.code = key;
	switch (ev.key.code) {
	case 183:
		ev.key.code = KEY_UP;
		break;
	case 184:
		ev.key.code = KEY_DOWN;
		break;
	case 185:
		ev.key.code = KEY_RIGHT;
		break;
	case 186:
		ev.key.code = KEY_LEFT;
		break;
	case 127:
		ev.key.code = KEY_BACKSPACE;
		break;
	case 293:
		ev.key.code = KEY_HOME;
		break;
	case 295:
		ev.key.code = KEY_DELETE;
		break;
	case 296:
		ev.key.code = KEY_END;
		break;
	case 297:
		ev.key.code = KEY_PAGEUP;
		break;
	case 298:
		ev.key.code = KEY_PAGEDOWN;
	default:
		break;
	}
	ev.type = PTK_EVENT_KEYDOWN;
	ptk_post_event(&ev);
	ptk_event_destroy(&ev);
	/* FIXME: this driver is not yet possible to get the key state directly,
	 * the following is a temporary solution.
	 */
	ev.type = PTK_EVENT_KEYUP;
	ptk_post_event(&ev);
	ptk_event_destroy(&ev);
	if (ev.key.code >= ' ' && ev.key.code <= '~') {
		str[0] = ev.key.code;
		ev.type = PTK_EVENT_KEYPRESS;
		ptk_post_event(&ev);
		ptk_event_destroy(&ev);
		ptk_ime_commit(str, 1);
	}
}

static bool kbhit(void)
{
	int ch, flags;
	struct termios tm;

	tcgetattr(linux_keyboard.fd, &tm);
	flags = fcntl(linux_keyboard.fd, F_GETFL, 0);
	fcntl(linux_keyboard.fd, F_SETFL, flags | O_NONBLOCK);
	ch = getchar();
	tcsetattr(linux_keyboard.fd, TCSANOW, &tm);
	fcntl(linux_keyboard.fd, F_SETFL, flags);
	if (ch != EOF) {
		ungetc(ch, stdin);
		return true;
	}
	return false;
}

static int getch(void)
{
	int c = EOF;
	struct termios tm;

	tm = linux_keyboard.tm;

	/* canonical input and echo modes */
	tm.c_lflag &= ~(ICANON | ECHO);
	tm.c_cc[VMIN] = 0;
	tm.c_cc[VTIME] = 1;
	tcsetattr(linux_keyboard.fd, TCSANOW, &tm);
	/* get char from modified stdin */
	c = getchar();
	/* reset stdin to previous state */
	tcsetattr(linux_keyboard.fd, TCSANOW, &linux_keyboard.tm);
	return c;
}

static void on_signal(int signal)
{
	tcsetattr(linux_keyboard.fd, TCSANOW, &linux_keyboard.tm);
	exit(1);
}

static void linux_keyboard_thread(void *arg)
{
	int key;
	while (linux_keyboard.active) {
		key = getch();
		if (key == EOF) {
			continue;
		}
		while (kbhit()) {
			key += getch();
		}
		linux_keyboard_dispatch_event(key);
	}
}

int ptk_linux_keyboard_init(void)
{
	int i = 0;
	int signals[] = { SIGABRT, SIGINT, SIGKILL, SIGQUIT, SIGSTOP, SIGTERM };
	int len = sizeof(signals) / sizeof(signals[0]);

	linux_keyboard.fd = STDIN_FILENO;
	if (tcgetattr(linux_keyboard.fd, &linux_keyboard.tm) < 0) {
		return -1;
	}
	for (; i < len; ++i) {
		signal(signals[i], on_signal);
	}
	linux_keyboard.active = true;
	thread_create(&linux_keyboard.tid, linux_keyboard_thread, NULL);
	logger_debug("[input] keyboard driver thread: %lld\n", linux_keyboard.tid);
	return 0;
}

int ptk_linux_keyboard_destroy(void)
{
	if (!linux_keyboard.active) {
		return 0;
	}
	linux_keyboard.active = false;
	thread_join(linux_keyboard.tid, NULL);
	if (tcsetattr(linux_keyboard.fd, TCSANOW, &linux_keyboard.tm) < 0) {
		return -1;
	}
	return 0;
}

#endif
