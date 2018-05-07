/*
 * windows_keyboard.c -- keyboard support for windows platform.
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
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H

static void OnKeyboardMessage( LCUI_Event ev, void *arg )
{
	MSG* msg = arg;
	static POINT mouse_pos;
	LCUI_SysEventRec sys_ev;
	switch( msg->message ) {
	case WM_KEYDOWN:
		sys_ev.type = LCUI_KEYDOWN;
		sys_ev.key.code = msg->wParam;
		break;
	case WM_KEYUP:
		sys_ev.type = LCUI_KEYUP;
		sys_ev.key.code = msg->wParam;
		break;
	default: return;
	}
	LCUI_TriggerEvent( &sys_ev, NULL );
}

void LCUI_InitWinKeyboard( void )
{
	LCUI_BindSysEvent( WM_KEYDOWN, OnKeyboardMessage, NULL, NULL );
	LCUI_BindSysEvent( WM_KEYUP, OnKeyboardMessage, NULL, NULL );
}

void LCUI_ExitWinKeyboard( void )
{
	LCUI_UnbindSysEvent( WM_KEYDOWN, OnKeyboardMessage );
	LCUI_UnbindSysEvent( WM_KEYUP, OnKeyboardMessage );
}

#endif
