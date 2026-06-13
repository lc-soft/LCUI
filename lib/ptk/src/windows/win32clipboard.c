/*
 * lib/ptk/src/windows/win32clipboard.c: clipboard support for Windows
 *
 * Copyright (c) 2025-2026, Liu Chao <hello@lcui.dev> All rights reserved.

 * *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI,
 * distributed under the MIT License found in the
 * LICENSE.TXT file in the
 * root directory of this source tree.
 */

/*
 * References:
 * - https://docs.microsoft.com/en-us/windows/win32/dataxchg/using-the-clipboard
 * -
 * https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-openclipboard
 * -
 * https://github.com/libsdl-org/SDL/blob/main/src/video/windows/SDL_windowsclipboard.c
 */

#include <yutil.h>
#include "ptk/app.h"

#ifdef PTK_WIN_DESKTOP

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "win32clipboard.h"

static struct ptk_win32clipboard_module {
        char *text;
        size_t text_len;
        HWND hwnd;    // Window handle for clipboard operations
} ptk_win32clipboard;

int ptk_win32clipboard_request_text(ptk_clipboard_callback_t callback,
                                    void *arg)
{
        ptk_clipboard_t clipboard = { 0 };
        wchar_t *clipboard_text = NULL;
        size_t len = 0;

        // Open the clipboard
        if (!OpenClipboard(ptk_win32clipboard.hwnd)) {
                logger_warning("Failed to open clipboard\n");
                // Return empty clipboard data
                clipboard.text = calloc(1, sizeof(wchar_t));
                clipboard.len = 0;
                clipboard.image = NULL;
                callback(&clipboard, arg);
                free(clipboard.text);
                return -1;
        }

        // Get clipboard data handle
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData == NULL) {
                // No Unicode text available, try ANSI text
                hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                        // Convert ANSI to Unicode
                        char *ansi_text = (char *)GlobalLock(hData);
                        if (ansi_text != NULL) {
                                int wlen = MultiByteToWideChar(
                                    CP_ACP, 0, ansi_text, -1, NULL, 0);
                                if (wlen > 0) {
                                        clipboard_text =
                                            malloc(wlen * sizeof(wchar_t));
                                        if (clipboard_text != NULL) {
                                                MultiByteToWideChar(
                                                    CP_ACP, 0, ansi_text, -1,
                                                    clipboard_text, wlen);
                                                len =
                                                    wlen - 1;    // Exclude null
                                                                 // terminator
                                        }
                                }
                                GlobalUnlock(hData);
                        }
                }
        } else {
                // Unicode text available
                wchar_t *unicode_text = (wchar_t *)GlobalLock(hData);
                if (unicode_text != NULL) {
                        len = wcslen(unicode_text);
                        clipboard_text = malloc((len + 1) * sizeof(wchar_t));
                        if (clipboard_text != NULL) {
                                wcscpy(clipboard_text, unicode_text);
                        }
                        GlobalUnlock(hData);
                }
        }

        CloseClipboard();

        // Prepare clipboard data
        if (clipboard_text == NULL) {
                clipboard_text = calloc(1, sizeof(wchar_t));
                len = 0;
        }

        clipboard.text = clipboard_text;
        clipboard.len = len;
        clipboard.image = NULL;

        callback(&clipboard, arg);

        free(clipboard_text);
        return 0;
}

int ptk_win32clipboard_set_text(const wchar_t *text, size_t len)
{
        HGLOBAL hMem;
        wchar_t *pMem;
        size_t size;

        if (!text || len == 0) {
                return -1;
        }

        // Open the clipboard
        if (!OpenClipboard(ptk_win32clipboard.hwnd)) {
                logger_warning("Failed to open clipboard for writing\n");
                return -1;
        }

        // Empty the clipboard
        if (!EmptyClipboard()) {
                logger_warning("Failed to empty clipboard\n");
                CloseClipboard();
                return -1;
        }

        // Allocate global memory for the text
        size = (len + 1) * sizeof(wchar_t);
        hMem = GlobalAlloc(GMEM_MOVEABLE, size);
        if (hMem == NULL) {
                logger_warning(
                    "Failed to allocate memory for clipboard text\n");
                CloseClipboard();
                return -1;
        }

        // Lock the memory and copy the text
        pMem = (wchar_t *)GlobalLock(hMem);
        if (pMem == NULL) {
                logger_warning("Failed to lock clipboard memory\n");
                GlobalFree(hMem);
                CloseClipboard();
                return -1;
        }

        wcsncpy(pMem, text, len);
        pMem[len] = L'\0';
        GlobalUnlock(hMem);

        // Set the clipboard data
        if (SetClipboardData(CF_UNICODETEXT, hMem) == NULL) {
                logger_warning("Failed to set clipboard data\n");
                GlobalFree(hMem);
                CloseClipboard();
                return -1;
        }

        CloseClipboard();

        // Update internal text buffer for fallback
        size_t utf8_len = encode_utf8(NULL, text, 0);
        char *utf8_text = malloc((utf8_len + 1) * sizeof(char));
        if (utf8_text != NULL) {
                utf8_len = encode_utf8(utf8_text, text, utf8_len);
                utf8_text[utf8_len] = '\0';

                if (ptk_win32clipboard.text) {
                        free(ptk_win32clipboard.text);
                }
                ptk_win32clipboard.text = utf8_text;
                ptk_win32clipboard.text_len = utf8_len;
        }

        return 0;
}

void ptk_win32clipboard_init(void)
{
        ptk_win32clipboard.text = NULL;
        ptk_win32clipboard.text_len = 0;

        // Get the main window handle for clipboard operations
        // This should be set by the main application window
        ptk_win32clipboard.hwnd = GetActiveWindow();
        if (ptk_win32clipboard.hwnd == NULL) {
                ptk_win32clipboard.hwnd = GetDesktopWindow();
        }
}

void ptk_win32clipboard_destroy(void)
{
        if (ptk_win32clipboard.text) {
                free(ptk_win32clipboard.text);
                ptk_win32clipboard.text = NULL;
        }
        ptk_win32clipboard.text_len = 0;
        ptk_win32clipboard.hwnd = NULL;
}

// Function to set the window handle for clipboard operations
// This should be called by the main application window
void ptk_win32clipboard_set_window(HWND hwnd)
{
        ptk_win32clipboard.hwnd = hwnd;
}

#endif    // PTK_WIN_DESKTOP
