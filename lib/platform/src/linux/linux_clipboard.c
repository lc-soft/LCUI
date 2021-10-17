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
#include LCUI_CLIPBOARD_H

void LCUI_LinuxUseClipboard(void *widget, void *action)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_LinuxX11UseClipboard(widget, action);
		return;
	}
#endif
}

void LCUI_LinuxCopyToClipboard(void *text)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_LinuxX11CopyToClipboard(text);
		return;
	}
#endif
}

void LCUI_InitLinuxClipboard(void)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_InitLinuxX11Clipboard();
		return;
	}
#endif
}

void LCUI_FreeLinuxClipboard(void)
{
#ifdef USE_LIBX11
	if (LCUI_GetAppId() == LCUI_APP_LINUX_X11) {
		LCUI_FreeLinuxX11Clipboard();
		return;
	}
#endif
}

#endif
