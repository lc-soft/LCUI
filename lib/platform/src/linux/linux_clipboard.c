/*
 * linux_clipboard.c -- Clipboard support for linux.
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
#include <string.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include <LCUI/clipboard.h>
#include LCUI_EVENTS_H

static struct LCUI_LinuxClipboardDriver {
	char *text;
	size_t text_len;
} clipboard;

void LCUI_UseClipboard(LCUI_ClipboardAction action, void *arg)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_UseLinuxX11Clipboard(action, arg);
		return;
	}
#else
	size_t len = clipboard.text_len + 1;
	wchar_t *wstr = malloc(sizeof(wchar_t) * len);
	len = decode_utf8(wstr, clipboard.text, len);
	wstr[len] = 0;
	// Assign the data
	LCUI_Clipboard clipboard_data = malloc(sizeof(LCUI_ClipboardRec));
	clipboard_data->text = wstr;
	clipboard_data->len = len;
	clipboard_data->image = NULL;
	action(clipboard_data, arg);
#endif
}

void LCUI_SetClipboardText(const wchar_t *text, size_t len)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_LinuxX11SetClipboardText(text, len);
		return;
	}
#else
	// X11 doesn't support wchar_t, so we need to send it regular char
	char *raw_text = malloc((len + 1) * sizeof(char));
	size_t raw_len = wcstombs(raw_text, text, len);
	raw_text[raw_len] = '\0';
	clipboard.text = raw_text;
	clipboard.text_len = raw_len;
#endif
}

void LCUI_InitClipboardDriver(void)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_InitLinuxX11Clipboard();
		return;
	}
#endif
}

void LCUI_FreeClipboardDriver(void)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_FreeLinuxX11Clipboard();
		return;
	}
#endif
}

#endif
