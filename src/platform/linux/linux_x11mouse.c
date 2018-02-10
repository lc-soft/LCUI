/*
 * linux_x11mouse.c -- mouse support for linux xwindow.
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

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#if defined(LCUI_BUILD_IN_LINUX) && defined(LCUI_VIDEO_DRIVER_X11)
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

static void OnMotionNotify( LCUI_Event e, void *arg )
{
	XEvent *ev = arg;
	LCUI_SysEventRec sys_ev;
	static LCUI_Pos mouse_pos = {0, 0};
	sys_ev.type = LCUI_MOUSEMOVE;
	sys_ev.motion.x = ev->xmotion.x;
	sys_ev.motion.y = ev->xmotion.y;
	sys_ev.motion.xrel = ev->xmotion.x - mouse_pos.x;
	sys_ev.motion.yrel = ev->xmotion.y - mouse_pos.y;
	mouse_pos.x = ev->xmotion.x;
	mouse_pos.y = ev->xmotion.y;
	LCUI_TriggerEvent( &sys_ev, NULL );
	LCUI_DestroyEvent( &sys_ev );
}

static void OnButtonPress( LCUI_Event e, void *arg )
{
	XEvent *ev = arg;
	LCUI_SysEventRec sys_ev;
	sys_ev.type = LCUI_MOUSEDOWN;
	sys_ev.button.x = ev->xbutton.x;
	sys_ev.button.y = ev->xbutton.y;
	sys_ev.button.button = ev->xbutton.button;
	LCUI_TriggerEvent( &sys_ev, NULL );
	LCUI_DestroyEvent( &sys_ev );
}

static void OnButtonRelease( LCUI_Event e, void *arg )
{
	XEvent *ev = arg;
	LCUI_SysEventRec sys_ev;
	sys_ev.type = LCUI_MOUSEUP;
	sys_ev.button.x = ev->xbutton.x;
	sys_ev.button.y = ev->xbutton.y;
	sys_ev.button.button = ev->xbutton.button;
	LCUI_TriggerEvent( &sys_ev, NULL );
	LCUI_DestroyEvent( &sys_ev );
}

void LCUI_InitLinuxX11Mouse( void )
{
	LCUI_BindSysEvent( MotionNotify, OnMotionNotify, NULL, NULL );
	LCUI_BindSysEvent( ButtonPress, OnButtonPress, NULL, NULL );
	LCUI_BindSysEvent( ButtonRelease, OnButtonRelease, NULL, NULL );
}

void LCUI_ExitLinuxX11Mouse( void )
{
	LCUI_UnbindSysEvent( MotionNotify, OnMotionNotify );
}

#endif
