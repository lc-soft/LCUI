/*
 * linux_ime.c -- The input method engine support for linux.
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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_LIBX11

static LCUI_BOOL x11_ime_process_key(int key, LCUI_BOOL pressed)
{
	return FALSE;
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

static LCUI_BOOL x11_ime_open(void)
{
	app_on_event(APP_EVENT_KEYPRESS, on_key_press, NULL);
	return TRUE;
}

static LCUI_BOOL x11_ime_close(void)
{
	app_off_event(APP_EVENT_KEYPRESS, on_key_press);
	return TRUE;
}
#endif

int ime_add_linux(void)
{
#ifdef HAVE_LIBX11
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
