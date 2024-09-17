/*
 * lib/ptk/src/windows/win32_ime.c
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
#include "ptk/app.h"
#include "ptk/ime.h"
#include "ptk/events.h"
#ifdef PTK_WIN_DESKTOP
#include <windows.h>

#pragma comment(lib, "Imm32.lib")

static bool ptk_win32ime_process_key(int key, bool is_pressed)
{
	return false;
}

static void ptk_win32ime_to_text(int ch)
{
	wchar_t text[2] = { ch, 0 };
	ptk_ime_commit(text, 2);
}

static void ptk_win32ime_on_char(MSG *msg, void *arg)
{
	wchar_t text[2];

	text[0] = (wchar_t)msg->wParam;
	text[1] = 0;
	ptk_ime_commit(text, 2);
}

static void ptk_win32ime_set_caret(int x, int y)
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

static bool ptk_win32ime_open(void)
{
	ptk_on_native_event(WM_CHAR, ptk_win32ime_on_char, NULL);
	return true;
}

static bool ptk_win32ime_close(void)
{
	ptk_off_native_event(WM_CHAR, ptk_win32ime_on_char);
	return true;
}

int ptk_ime_add_win32(void)
{
	ptk_ime_handler_t handler;

	handler.prockey = ptk_win32ime_process_key;
	handler.totext = ptk_win32ime_to_text;
	handler.close = ptk_win32ime_close;
	handler.open = ptk_win32ime_open;
	handler.setcaret = ptk_win32ime_set_caret;
	return ptk_ime_add("Win32 Input Method", &handler);
}

#endif
