/*
 * windows_ime.c -- Input method engine support for windows platform.
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


#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>

#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static LCUI_BOOL IME_ProcessKey( int key, int key_state )
{
	return FALSE;
}

static void IME_ToText( int ch )
{
	wchar_t text[2] = { ch, 0 };
	LCUIIME_Commit( text, 2 );
}

static void WinIME_OnChar( LCUI_Event e, void *arg )
{
	MSG *msg = arg;
	wchar_t text[2];
	text[0] = msg->wParam;
	text[1] = 0;
	LCUIIME_Commit( text, 2 );
}

/**
* 输入法被打开时的处理
* 可以在输入法被打开时，初始化相关数据，链接至词库什么的
**/
static LCUI_BOOL IME_Open(void)
{
	LCUI_BindSysEvent( WM_CHAR, WinIME_OnChar, NULL, NULL );
	return TRUE;
}

/** 输入法被关闭时的处理 */
static LCUI_BOOL IME_Close(void)
{
	LCUI_UnbindSysEvent( WM_CHAR, WinIME_OnChar );
	return TRUE;
}

int LCUI_RegisterWin32IME( void )
{
	LCUI_IMEHandlerRec handler;
	handler.prockey = IME_ProcessKey;
	handler.totext = IME_ToText;
	handler.close = IME_Close;
	handler.open = IME_Open;
	return LCUIIME_Register( "LCUI Input Method", &handler );
}

#endif
