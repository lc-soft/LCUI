/*
 * windows_mouse.c -- mouse support for windows platform.
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
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H

static void OnMouseMessage(LCUI_Event ev, void *arg)
{
	MSG *msg = arg;
	LCUI_SysEventRec sys_ev;
	static POINT mouse_pos = { 0, 0 };
	sys_ev.type = LCUI_NONE;
	switch (msg->message) {
	case WM_MOUSEMOVE:
	{
		POINT new_pos;
		GetCursorPos(&new_pos);
		ScreenToClient(msg->hwnd, &new_pos);
		sys_ev.motion.x = new_pos.x;
		sys_ev.motion.y = new_pos.y;
		sys_ev.motion.xrel = new_pos.x - mouse_pos.x;
		sys_ev.motion.yrel = new_pos.y - mouse_pos.y;
		mouse_pos.x = new_pos.x;
		mouse_pos.y = new_pos.y;
		sys_ev.type = LCUI_MOUSEMOVE;
		break;
	}
	case WM_LBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.button.button = LCUI_KEY_LEFTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture(msg->hwnd);
		break;
	case WM_LBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = LCUI_KEY_LEFTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		sys_ev.type = LCUI_MOUSEDOWN;
		sys_ev.button.button = LCUI_KEY_RIGHTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		SetCapture(msg->hwnd);
		break;
	case WM_RBUTTONUP:
		sys_ev.type = LCUI_MOUSEUP;
		sys_ev.button.button = LCUI_KEY_RIGHTBUTTON;
		sys_ev.button.x = mouse_pos.x;
		sys_ev.button.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_MOUSEWHEEL:
		sys_ev.type = LCUI_MOUSEWHEEL;
		sys_ev.wheel.x = mouse_pos.x;
		sys_ev.wheel.y = mouse_pos.y;
		sys_ev.wheel.delta = GET_WHEEL_DELTA_WPARAM(msg->wParam);
		break;
#ifdef ENABLE_TOUCH_SUPPORT
	case WM_TOUCH:
	{
		UINT i, n = LOWORD(msg->wParam);
		PTOUCHINPUT inputs = NEW(TOUCHINPUT, n);
		HTOUCHINPUT handle = (HTOUCHINPUT)msg->lParam;
		if (inputs == NULL) {
			break;
		}
		sys_ev.type = LCUI_TOUCH;
		sys_ev.touch.n_points = n;
		sys_ev.touch.points = NEW(LCUI_TouchPointRec, n);
		if (sys_ev.touch.points == NULL) {
			free(inputs);
			break;
		}
		if (!GetTouchInputInfo(handle, n, inputs,
				       sizeof(TOUCHINPUT))) {
			free(inputs);
			break;
		}
		for (i = 0; i < n; ++i) {
			POINT pos;
			pos.x = inputs[i].x / 100;
			pos.y = inputs[i].y / 100;
			ScreenToClient(msg->hwnd, &pos);
			sys_ev.touch.points[i].x = pos.x;
			sys_ev.touch.points[i].y = pos.y;
			sys_ev.touch.points[i].id = inputs[i].dwID;
			if (inputs[i].dwFlags & TOUCHEVENTF_PRIMARY) {
				sys_ev.touch.points[i].is_primary = TRUE;
			} else {
				sys_ev.touch.points[i].is_primary = FALSE;
			}
			if (inputs[i].dwFlags & TOUCHEVENTF_DOWN) {
				sys_ev.touch.points[i].state = LCUI_TOUCHDOWN;
			} else if (inputs[i].dwFlags & TOUCHEVENTF_UP) {
				sys_ev.touch.points[i].state = LCUI_TOUCHUP;
			} else if (inputs[i].dwFlags & TOUCHEVENTF_MOVE) {
				sys_ev.touch.points[i].state = LCUI_TOUCHMOVE;
			}
		}
		free(inputs);
		if (!CloseTouchInputHandle(handle)) {
			break;
		}
		break;
	}
#endif
	default: break;
	}
	if (sys_ev.type != LCUI_NONE) {
		LCUI_TriggerEvent(&sys_ev, NULL);
		LCUI_DestroyEvent(&sys_ev);
	}
}

void LCUI_InitWinMouse(void)
{
	LCUI_BindSysEvent(WM_MOUSEMOVE, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_LBUTTONDOWN, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_LBUTTONUP, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_RBUTTONDOWN, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_RBUTTONUP, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_MOUSEWHEEL, OnMouseMessage, NULL, NULL);
	LCUI_BindSysEvent(WM_TOUCH, OnMouseMessage, NULL, NULL);
}

void LCUI_FreeWinMouse(void)
{
	LCUI_UnbindSysEvent(WM_MOUSEMOVE, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_LBUTTONDOWN, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_LBUTTONUP, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_RBUTTONDOWN, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_RBUTTONUP, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_MOUSEWHEEL, OnMouseMessage);
	LCUI_UnbindSysEvent(WM_TOUCH, OnMouseMessage);
}
#endif
