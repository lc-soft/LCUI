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

#include <LCUI_Build.h>

#ifdef LCUI_BUILD_IN_LINUX
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

#ifdef LCUI_VIDEO_DRIVER_X11

static struct LCUI_LinuxIMEModule {
	int handler_id;
} self;

static LCUI_BOOL X11IME_ProcessKey(int key, int key_state)
{
	return FALSE;
}

static void X11IME_ToText(int ch)
{
	wchar_t text[2] = { ch, 0 };
	LCUIIME_Commit(text, 2);
}

static void OnKeyPress(LCUI_SysEvent e, void *arg)
{
	wchar_t text[2] = { e->key.code, 0 };
	_DEBUG_MSG("char: %c\n", e->key.code);
	LCUIIME_Commit(text, 2);
}

static LCUI_BOOL X11IME_Open(void)
{
	self.handler_id = LCUI_BindEvent(LCUI_KEYPRESS, OnKeyPress, NULL, NULL);
	return TRUE;
}

static LCUI_BOOL X11IME_Close(void)
{
	LCUI_UnbindEvent(self.handler_id);
	return TRUE;
}
#endif

int LCUI_RegisterLinuxIME(void)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	LCUI_IMEHandlerRec handler;
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		handler.prockey = X11IME_ProcessKey;
		handler.totext = X11IME_ToText;
		handler.close = X11IME_Close;
		handler.open = X11IME_Open;
		return LCUIIME_Register("LCUI X11 Input Method", &handler);
	}
#endif
	return -1;
}

#endif
