/*
 * uwp_input.cpp -- UWP input support
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

#include "pch.h"
#include "uwp_input.h"
#include <LCUI/input.h>
#include <LCUI/ime.h>

using namespace LCUICore;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::System;

static touch_point_t *AddTouchPoint(list_t *points,
				     PointerPoint^ point, int state)
{
	list_node_t *node;
	touch_point_t *tp = NULL;
	for (list_each(node, points)) {
		tp = (LCUI_TouchPoint)node->data;
		if (tp->id == point->PointerId) {
			break;
		}
		tp = NULL;
	}
	if (!tp) {
		tp = malloc(sizeof(touch_point_t));
		tp->id = point->PointerId;
		/* 将第一个触点作为主触点 */
		tp->is_primary = points->length == 0;
		list_append(points, tp);
	}
	tp->state = state;
	tp->x = y_iround(point->Position.X);
	tp->y = y_iround(point->Position.Y);
	return tp;
}

static void ClearInvalidTouchPoints(list_t *points)
{
	touch_point_t *tp;
	list_node_t *node, *next;

	node = points->head.next;
	while (node) {
		next = node->next;
		tp = (LCUI_TouchPoint)node->data;
		if (tp->state == APP_EVENT_TOUCHUP) {
			list_delete_node(points, node);
		node = next;
	}
}

static int CreateTouchEvent(app_event_t *e, list_t *points)
{
	int i = 0;
	list_node_t *node;
	touch_point_t *list;

	list = malloc(sizeof(touch_point_t, points->length);
	for (list_each(node, points)) {
		list[i++] = *(LCUI_TouchPoint)node->data;
	}
	return app_touch_event_init(e, list, (int)points->length);
}

InputDriver::InputDriver()
{
	m_mouse.actived = false;
	m_mouse.leftButtonPressed = false;
	m_mouse.rightButtonPressed = false;
	list_create(&m_touch.points);
}

void InputDriver::OnPointerPressed(CoreWindow^ sender,
				   PointerEventArgs^ args)
{
	pd_pos_t pos;
	touch_point_t *tp;

	app_event_t ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;
	switch (point->PointerDevice->PointerDeviceType) {
	case PointerDeviceType::Mouse:
		if (pointProps->IsLeftButtonPressed) {
			ev.mouse.button = MOUSE_BUTTON_LEFT;
			m_mouse.leftButtonPressed = true;
		} else if (pointProps->IsRightButtonPressed) {
			ev.mouse.button = KEY_RIGHTBUTTON;
			m_mouse.rightButtonPressed = true;
		}
		break;
	case PointerDeviceType::Touch:
		ev.type = APP_EVENT_TOUCH;
		tp = AddTouchPoint(&m_touch.points, point, APP_EVENT_TOUCHDOWN);
		CreateTouchEvent(&ev, &m_touch.points);
		LCUI_TriggerEvent(&ev, NULL);
		app_event_destroy(&ev);
		ClearInvalidTouchPoints(&m_touch.points);
		/* 如果该触点是主触点，则顺便触发鼠标事件 */
		if (tp->is_primary) {
			ev.mouse.button = MOUSE_BUTTON_LEFT;
			m_mouse.leftButtonPressed = true;
			break;
		}
	default:return;
	}
	if (!m_mouse.actived) {
		m_mouse.actived = true;
		m_mouse.position = position;
	}
	ev.type = APP_EVENT_MOUSEDOWN;
	pos.x = y_iround(position.X);
	pos.y = y_iround(position.Y);
	ev.mouse.x = pos.x;
	ev.mouse.y = pos.y;
	LCUICursor_SetPos(pos);
	LCUI_TriggerEvent(&ev, NULL);
}

void InputDriver::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	Point position;
	pd_pos_t pos;
	touch_point_t *tp;

	app_event_t ev;
	PointerPoint^ point = args->CurrentPoint;
	position = args->CurrentPoint->Position;
	switch (point->PointerDevice->PointerDeviceType) {
	case PointerDeviceType::Mouse:
		break;
	case PointerDeviceType::Touch:
		ev.type = APP_EVENT_TOUCH;
		tp = AddTouchPoint(&m_touch.points, point, APP_EVENT_TOUCHMOVE);
		CreateTouchEvent(&ev, &m_touch.points);
		ClearInvalidTouchPoints(&m_touch.points);
		LCUI_TriggerEvent(&ev, NULL);
		app_event_destroy(&ev);
		if (tp->is_primary) {
			break;
		}
	default:return;
	}
	if (!m_mouse.actived) {
		m_mouse.actived = true;
		m_mouse.position = position;
	}
	pos.x = y_iround(position.X);
	pos.y = y_iround(position.Y);
	ev.type = APP_EVENT_MOUSEMOVE;
	ev.motion.x = pos.x;
	ev.motion.y = pos.y;
	ev.motion.xrel = y_iround(position.X - m_mouse.position.Y);
	ev.motion.yrel = y_iround(position.Y - m_mouse.position.Y);
	m_mouse.position = position;
	LCUI_TriggerEvent(&ev, NULL);
	app_event_destroy(&ev);
	LCUICursor_SetPos(pos);
}

void InputDriver::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	pd_pos_t pos;
	touch_point_t *tp;

	app_event_t ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;
	switch (point->PointerDevice->PointerDeviceType) {
	case PointerDeviceType::Mouse:
		if (!pointProps->IsLeftButtonPressed &&
		    m_mouse.leftButtonPressed) {
			m_mouse.leftButtonPressed = false;
			ev.mouse.button = MOUSE_BUTTON_LEFT;
		}
		if (pointProps->IsRightButtonPressed &&
		    m_mouse.rightButtonPressed) {
			m_mouse.rightButtonPressed = false;
			ev.mouse.button = KEY_RIGHTBUTTON;
		}
		break;
	case PointerDeviceType::Touch:
		ev.type = APP_EVENT_TOUCH;
		tp = AddTouchPoint(&m_touch.points, point, APP_EVENT_TOUCHUP);
		CreateTouchEvent(&ev, &m_touch.points);
		ClearInvalidTouchPoints(&m_touch.points);
		LCUI_TriggerEvent(&ev, NULL);
		app_event_destroy(&ev);
		if (tp->is_primary) {
			m_mouse.leftButtonPressed = false;
			ev.mouse.button = MOUSE_BUTTON_LEFT;
		}
		break;
	default:return;
	}
	ev.type = APP_EVENT_MOUSEUP;
	pos.x = y_iround(position.X);
	pos.y = y_iround(position.Y);
	ev.mouse.x = pos.x;
	ev.mouse.y = pos.y;
	LCUI_TriggerEvent(&ev, NULL);
	app_event_destroy(&ev);
	LCUICursor_SetPos(pos);
}

void InputDriver::OnPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args)
{
	app_event_t ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;

	ev.type = APP_EVENT_WHEEL;
	ev.wheel.x = (int)(position.X + 0.5);
	ev.wheel.y = (int)(position.Y + 0.5);
	ev.wheel.delta = pointProps->MouseWheelDelta;;
	LCUI_TriggerEvent(&ev, NULL);
	app_event_destroy(&ev);
}

void InputDriver::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
{
	app_event_t ev;
	ev.type = APP_EVENT_KEYDOWN;
	ev.key.ctrl_key = FALSE;
	ev.key.shift_key = FALSE;
	ev.key.code = static_cast<int>(args->VirtualKey);
	LCUI_TriggerEvent(&ev, NULL);
	app_event_destroy(&ev);
}

void InputDriver::OnKeyUp(CoreWindow^ sender, KeyEventArgs^ args)
{
	app_event_t ev;
	ev.type = APP_EVENT_KEYUP;
	ev.key.ctrl_key = FALSE;
	ev.key.shift_key = FALSE;
	ev.key.code = static_cast<int>(args->VirtualKey);
	LCUI_TriggerEvent(&ev, NULL);
	app_event_destroy(&ev);
}

static LCUI_BOOL IME_ProcessKey(int key, int key_state)
{
	return ime_check_char_key(key);
}

static void IME_ToText(int ch)
{
	wchar_t text[2] = { (wchar_t)ch, 0 };
	ime_commit(text, 2);
}

static LCUI_BOOL IME_Open(void)
{
	return TRUE;
}

static LCUI_BOOL IME_Close(void)
{
	return TRUE;
}

void InputDriver::RegisterIME(void)
{
	ime_handler_t handler;
	handler.prockey = IME_ProcessKey;
	handler.totext = IME_ToText;
	handler.close = IME_Close;
	handler.open = IME_Open;
	m_ime = ime_add("UWP Input Method", &handler);
}

void InputDriver::SelectIME(void)
{
	ime_select(m_ime);
}
