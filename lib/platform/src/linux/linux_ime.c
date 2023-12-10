/*
 * lib/platform/src/linux/linux_ime.c: -- The input method engine support for linux.
 *
 * Copyright (c) 2018-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../app.h"

#ifdef LIBPLAT_LINUX
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LIBPLAT_HAS_LIBX11

static bool x11_ime_process_key(int key, bool pressed)
{
	return false;
}

static void x11_ime_to_text(int ch)
{
	wchar_t text[2] = { ch, 0 };
	ime_commit(text, 2);
}

static void on_key_press(app_event_t *e, void *arg)
{
	wchar_t text[2] = { e->key.code, 0 };
	ime_commit(text, 2);
}

static bool x11_ime_open(void)
{
	app_on_event(APP_EVENT_KEYPRESS, on_key_press, NULL);
	return true;
}

static bool x11_ime_close(void)
{
	app_off_event(APP_EVENT_KEYPRESS, on_key_press);
	return true;
}
#endif

int ime_add_linux(void)
{
#ifdef LIBPLAT_HAS_LIBX11
	ime_handler_t handler;
	if (app_get_id() == APP_ID_LINUX_X11) {
		handler.prockey = x11_ime_process_key;
		handler.totext = x11_ime_to_text;
		handler.close = x11_ime_close;
		handler.open = x11_ime_open;
		handler.setcaret = NULL;
		return ime_add("LCUI X11 Input Method", &handler);
	}
#endif
	return -1;
}

#endif
