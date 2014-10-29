/* ***************************************************************************
 * widget_event.h -- LCUI widget event module.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_event.h -- LCUI部件事件模块
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ***************************************************************************/

#ifndef __LCUI_WIDGET_EVENT_H__
#define __LCUI_WIDGET_EVENT_H__

LCUI_BEGIN_HEADER

enum WidgetEventType {
	WET_RESIZE,

	WET_KEYDOWN,
	WET_KEYUP,
	WET_KEYPRESS,
	WET_INPUT,
	
	WET_MOUSEOVER,
	WET_MOUSEMOVE,
	WET_MOUSEOUT,
	WET_MOUSEDOWN,
	WET_MOUSEUP,
	WET_CLICK,
	
	WET_USER
};

typedef struct LCUI_WidgetEvent {
	int type;			/**< 事件类型标识号 */
	const char *type_name;		/**< 事件类型名称 */
	int which;			/**< 指示按了哪个键或按钮 */
	int x, y;			/**< 鼠标的坐标(相对于当前部件) */
	void *data;			/**< 附加数据 */
	LCUI_Widget target;		/**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble;	/**< 是否取消事件冒泡 */
} LCUI_WidgetEvent;

/**
 * 预先注册一个事件，并指定事件名和事件ID
 * 如果需要将多个事件绑定在同一个事件处理器上，并且，不想通过进行字符串比较来
 * 区分事件类型，则可以使用该函数，但需要注意的是，指定的事件ID最好不要与系统
 * 预置的部件事件ID相同（除非你是特意的），通常，部件事件ID号在 WIDGET_USER 
 * 以后的值都可以使用，例如：WET_USER + 1，WET_USER + 200。
 */
LCUI_API int Widget_RegisterEventWithId( LCUI_Widget widget, const char *event_name, int id );

/**
 * 为部件绑定事件
 * 需要提供事件的名称、事件处理器（回调函数）、附加数据、数据销毁函数。
 * 通常，事件处理器可能会需要更多的参数，这些参数可作为附加数据，每次
 * 调用事件处理器时，都可以根据附加数据进行相应的操作。
 * 附加数据会在解除事件绑定时被释放。
 */
LCUI_API int Widget_BindEvent(	LCUI_Widget widget, const char *event_name,
			void(*func)(LCUI_Widget,LCUI_WidgetEvent*, void*), 
			void *func_data, void (*destroy_data)(void*) );

/** 
 * 解除与事件的绑定
 * 这将解除所有与该事件绑定的事件处理器，当传入事件名为NULL时，将解除所有事件
 * 绑定。
 */
LCUI_API int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name );

/** 
 * 解除指定的事件处理器的事件绑定
 * 需传入事件处理器的ID，该ID可在绑定事件时得到。
 */
LCUI_API int Widget_UnbindEventById( LCUI_Widget widget, int id );

/** 
 * 将事件投递给事件处理器，等待处理
 * 事件将会追加至事件队列中，等待下一轮的批处理时让对应的事件处理器进行处理
 */
LCUI_API int Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data );

/** 
 * 直接将事件发送至处理器 
 * 这将会直接调用与事件绑定的事件处理器（回调函数）
 */
LCUI_API int Widget_SendEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data );

/** 处理一次当前积累的部件事件 */
void LCUIWidget_Event_Step(void);

/** 初始化 LCUI 部件的事件系统 */
void LCUIWidget_Event_Init(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void LCUIWidget_Event_Destroy(void);

LCUI_END_HEADER

#endif
