/*
 * lib/ptk/src/windows/win32clipboard.h
 *
 * Copyright (c) 2025-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_WIN32CLIPBOARD_H
#define PTK_WIN32CLIPBOARD_H

#include "ptk/clipboard.h"

#ifdef PTK_WIN_DESKTOP
#include <windows.h>

int ptk_win32clipboard_request_text(ptk_clipboard_callback_t callback,
                                    void *arg);
int ptk_win32clipboard_set_text(const wchar_t *text, size_t len);
void ptk_win32clipboard_init(void);
void ptk_win32clipboard_destroy(void);
void ptk_win32clipboard_set_window(HWND hwnd);

#endif

#endif
