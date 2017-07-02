/* ***************************************************************************
 * uwp_input.cpp -- UWP input support for LCUI
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified,
 * and distributed under the terms of the GPLv2.
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * uwp_input.cpp -- LCUI 的 UWP 版输入支持，包括鼠标、键盘、触屏等的输入处理
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是 LCUI 项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和
 * 发布。
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销
 * 性或特定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在 LICENSE 文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include "pch.h"
#include "uwp_input.h"
#include <LCUI/input.h>
#include <LCUI/cursor.h>
#include <LCUI/ime.h>

using namespace LCUICore;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Devices::Input;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Windows::System;

static LCUI_TouchPoint AddTouchPoint( LinkedList *points,
				      PointerPoint^ point, int state )
{
	LinkedListNode *node;
	LCUI_TouchPoint tp = NULL;
	for( LinkedList_Each( node, points ) ) {
		tp = (LCUI_TouchPoint)node->data;
		if( tp->id == point->PointerId ) {
			break;
		}
		tp = NULL;
	}
	if( !tp ) {
		tp = NEW( LCUI_TouchPointRec, 1 );
		tp->id = point->PointerId;
		/* 将第一个触点作为主触点 */
		tp->is_primary = points->length == 0;
		LinkedList_Append( points, tp );
	}
	tp->state = state;
	tp->x = iround( point->Position.X );
	tp->y = iround( point->Position.Y );
	return tp;
}

static void ClearInvalidTouchPoints( LinkedList *points )
{
	LCUI_TouchPoint tp;
	LinkedListNode *node, *next;
	node = points->head.next;
	while( node ) {
		next = node->next;
		tp = (LCUI_TouchPoint)node->data;
		if( tp->state == LCUI_TOUCHUP ) {
			LinkedList_DeleteNode( points, node );
		}
		node = next;
	}
}

static int CreateTouchEvent( LCUI_SysEvent e, LinkedList *points )
{
	int i = 0;
	LinkedListNode *node;
	LCUI_TouchPoint list;
	list = NEW( LCUI_TouchPointRec, points->length );
	for( LinkedList_Each( node, points ) ) {
		list[i++] = *(LCUI_TouchPoint)node->data;
	}
	return LCUI_CreateTouchEvent( e, list, points->length );
}

InputDriver::InputDriver()
{
	m_mouse.actived = false;
	m_mouse.leftButtonPressed = false;
	m_mouse.rightButtonPressed = false;
	LinkedList_Init( &m_touch.points );
}

void InputDriver::OnPointerPressed( CoreWindow^ sender,
					PointerEventArgs^ args )
{
	LCUI_Pos pos;
	LCUI_TouchPoint tp;
	LCUI_SysEventRec ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;
	switch( point->PointerDevice->PointerDeviceType ) {
	case PointerDeviceType::Mouse:
		if( pointProps->IsLeftButtonPressed ) {
			ev.button.button = LCUIKEY_LEFTBUTTON;
			m_mouse.leftButtonPressed = true;
		} else if( pointProps->IsRightButtonPressed ) {
			ev.button.button = LCUIKEY_RIGHTBUTTON;
			m_mouse.rightButtonPressed = true;
		}
		break;
	case PointerDeviceType::Touch:
		ev.type = LCUI_TOUCH;
		tp = AddTouchPoint( &m_touch.points, point, LCUI_TOUCHDOWN );
		CreateTouchEvent( &ev, &m_touch.points );
		LCUI_TriggerEvent( &ev, NULL );
		LCUI_DestroyEvent( &ev );
		ClearInvalidTouchPoints( &m_touch.points );
		/* 如果该触点是主触点，则顺便触发鼠标事件 */
		if( tp->is_primary ) {
			ev.button.button = LCUIKEY_LEFTBUTTON;
			m_mouse.leftButtonPressed = true;
			break;
		}
	default:return;
	}
	if( !m_mouse.actived ) {
		m_mouse.actived = true;
		m_mouse.position = position;
	}
	ev.type = LCUI_MOUSEDOWN;
	pos.x = iround( position.X );
	pos.y = iround( position.Y );
	ev.button.x = pos.x;
	ev.button.y = pos.y;
	LCUICursor_SetPos( pos );
	LCUI_TriggerEvent( &ev, NULL );
}

void InputDriver::OnPointerMoved( CoreWindow^ sender, PointerEventArgs^ args )
{
	Point position;
	LCUI_Pos pos;
	LCUI_TouchPoint tp;
	LCUI_SysEventRec ev;
	PointerPoint^ point = args->CurrentPoint;
	position = args->CurrentPoint->Position;
	switch( point->PointerDevice->PointerDeviceType ) {
	case PointerDeviceType::Mouse:
		break;
	case PointerDeviceType::Touch:
		ev.type = LCUI_TOUCH;
		tp = AddTouchPoint( &m_touch.points, point, LCUI_TOUCHMOVE );
		CreateTouchEvent( &ev, &m_touch.points );
		ClearInvalidTouchPoints( &m_touch.points );
		LCUI_TriggerEvent( &ev, NULL );
		LCUI_DestroyEvent( &ev );
		if( tp->is_primary ) {
			break;
		}
	default:return;
	}
	if( !m_mouse.actived ) {
		m_mouse.actived = true;
		m_mouse.position = position;
	}
	pos.x = iround( position.X );
	pos.y = iround( position.Y );
	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = pos.x;
	ev.motion.y = pos.y;
	ev.motion.xrel = iround( position.X - m_mouse.position.Y );
	ev.motion.yrel = iround( position.Y - m_mouse.position.Y );
	m_mouse.position = position;
	LCUI_TriggerEvent( &ev, NULL );
	LCUI_DestroyEvent( &ev );
	LCUICursor_SetPos( pos );
}

void InputDriver::OnPointerReleased( CoreWindow^ sender, PointerEventArgs^ args )
{
	LCUI_Pos pos;
	LCUI_TouchPoint tp;
	LCUI_SysEventRec ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;
	switch( point->PointerDevice->PointerDeviceType ) {
	case PointerDeviceType::Mouse:
		if( !pointProps->IsLeftButtonPressed &&
		    m_mouse.leftButtonPressed ) {
			m_mouse.leftButtonPressed = false;
			ev.button.button = LCUIKEY_LEFTBUTTON;
		}
		if( pointProps->IsRightButtonPressed &&
		    m_mouse.rightButtonPressed ) {
			m_mouse.rightButtonPressed = false;
			ev.button.button = LCUIKEY_RIGHTBUTTON;
		}
		break;
	case PointerDeviceType::Touch:
		ev.type = LCUI_TOUCH;
		tp = AddTouchPoint( &m_touch.points, point, LCUI_TOUCHUP );
		CreateTouchEvent( &ev, &m_touch.points );
		ClearInvalidTouchPoints( &m_touch.points );
		LCUI_TriggerEvent( &ev, NULL );
		LCUI_DestroyEvent( &ev );
		if( tp->is_primary ) {
			m_mouse.leftButtonPressed = false;
			ev.button.button = LCUIKEY_LEFTBUTTON;
		}
		break;
	default:return;
	}
	ev.type = LCUI_MOUSEUP;
	pos.x = iround( position.X );
	pos.y = iround( position.Y );
	ev.button.x = pos.x;
	ev.button.y = pos.y;
	LCUI_TriggerEvent( &ev, NULL );
	LCUI_DestroyEvent( &ev );
	LCUICursor_SetPos( pos );
}

void InputDriver::OnPointerWheelChanged( CoreWindow^ sender, PointerEventArgs^ args )
{
	LCUI_SysEventRec ev;
	PointerPoint^ point = args->CurrentPoint;
	PointerPointProperties^ pointProps = point->Properties;
	Point position = point->Position;
	
	ev.type = LCUI_MOUSEWHEEL;
	ev.wheel.x = (int)(position.X + 0.5);
	ev.wheel.y = (int)(position.Y + 0.5);
	ev.wheel.delta = pointProps->MouseWheelDelta;;
	LCUI_TriggerEvent( &ev, NULL );
	LCUI_DestroyEvent( &ev );
}

void InputDriver::OnKeyDown( CoreWindow^ sender, KeyEventArgs^ args )
{
	LCUI_SysEventRec ev;
	ev.type = LCUI_KEYDOWN;
	ev.key.code = static_cast<int>(args->VirtualKey);
	LCUI_TriggerEvent( &ev, NULL );
	LCUI_DestroyEvent( &ev );
}

void InputDriver::OnKeyUp( CoreWindow^ sender, KeyEventArgs^ args )
{
	LCUI_SysEventRec ev;
	ev.type = LCUI_KEYUP;
	ev.key.code = static_cast<int>(args->VirtualKey);
	LCUI_TriggerEvent( &ev, NULL );
	LCUI_DestroyEvent( &ev );
}

static LCUI_BOOL IME_ProcessKey( int key, int key_state )
{
	return LCUIIME_CheckCharKey( key );
}

static void IME_ToText( int ch )
{
	wchar_t text[2] = { (wchar_t)ch, 0 };
	LCUIIME_Commit( text, 2 );
}

static LCUI_BOOL IME_Open( void )
{
	return TRUE;
}

static LCUI_BOOL IME_Close( void )
{
	return TRUE;
}

void InputDriver::RegisterIME( void )
{
	LCUI_IMEHandlerRec handler;
	handler.prockey = IME_ProcessKey;
	handler.totext = IME_ToText;
	handler.close = IME_Close;
	handler.open = IME_Open;
	m_ime = LCUIIME_Register( "UWP Input Method", &handler );
}

void InputDriver::SelectIME( void )
{
	LCUIIME_Select( m_ime );
}
