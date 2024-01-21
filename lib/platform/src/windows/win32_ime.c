/*
 * lib/platform/src/windows/win32_ime.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../app.h"
#ifdef LIBPLAT_WIN_DESKTOP
#include <windows.h>
#include <ime.h>

#pragma comment(lib, "Imm32.lib")

static bool win32ime_process_key(int key, bool is_pressed)
{
	return false;
}

static void win32ime_to_text(int ch)
{
	wchar_t text[2] = { ch, 0 };
	ime_commit(text, 2);
}

static void win32ime_on_char(MSG *msg, void *arg)
{
	wchar_t text[2];

	text[0] = (wchar_t)msg->wParam;
	text[1] = 0;
	ime_commit(text, 2);
}

static void win32ime_set_caret(int x, int y)
{
	HWND hwnd = GetActiveWindow();
	HIMC himc = ImmGetContext(hwnd);
	if (himc) {
		COMPOSITIONFORM composition;
		composition.dwStyle = CFS_POINT;
		composition.ptCurrentPos.x = x;
		composition.ptCurrentPos.y = y;
		ImmSetCompositionWindow(himc, &composition);
		ImmReleaseContext(hwnd, himc);
	}
}

static bool win32ime_open(void)
{
	app_on_native_event(WM_CHAR, win32ime_on_char, NULL);
	return true;
}

static bool win32ime_close(void)
{
	app_off_native_event(WM_CHAR, win32ime_on_char);
	return true;
}

int ime_add_win32(void)
{
	ime_handler_t handler;

	handler.prockey = win32ime_process_key;
	handler.totext = win32ime_to_text;
	handler.close = win32ime_close;
	handler.open = win32ime_open;
	handler.setcaret = win32ime_set_caret;
	return ime_add("Win32 Input Method", &handler);
}

#endif
