/* ***************************************************************************
 * widget_event.h -- LCUI widget event module.
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_WIDGET_EVENT_H
#define LCUI_WIDGET_EVENT_H

LCUI_BEGIN_HEADER

/** 部件事件类型枚举 */
enum WidgetEventType {
	WET_NONE,
	WET_ADD,
	WET_READY,		/**< 在初次更新尺寸和位置后 */
	WET_REMOVE,		/**< 在被标记需要移除时 */
	WET_DESTROY,		/**< 在开始彻底销毁前 */
	WET_MOVE,		/**< 在移动位置时 */
	WET_RESIZE,		/**< 改变尺寸 */
	WET_SHOW,		/**< 显示 */
	WET_HIDE,		/**< 隐藏 */
	WET_FOCUS,		/**< 获得焦点 */
	WET_BLUR,		/**< 失去焦点 */
	WET_AFTERLAYOUT,	/**< 在子部件布局完成后 */
	WET_KEYDOWN,		/**< 按键按下 */
	WET_KEYUP,		/**< 按键释放 */
	WET_KEYPRESS,		/**< 按键字符输入 */
	WET_TEXTINPUT,		/**< 文本输入 */

	WET_MOUSEOVER,		/**< 鼠标在部件上 */
	WET_MOUSEMOVE,		/**< 鼠标在部件上移动 */
	WET_MOUSEOUT,		/**< 鼠标从部件上移开 */
	WET_MOUSEDOWN,		/**< 鼠标按键按下 */
	WET_MOUSEUP,		/**< 鼠标按键释放 */
	WET_MOUSEWHEEL,		/**< 鼠标滚轮滚动时 */
	WET_CLICK,		/**< 鼠标单击 */
	WET_DBLCLICK,		/**< 鼠标双击 */
	WET_TOUCH,		/**< 触控 */
	WET_TOUCHDOWN,		/**< 触点按下 */
	WET_TOUCHUP,		/**< 触点释放 */
	WET_TOUCHMOVE,		/**< 触点移动 */

	WET_TITLE,
	WET_SURFACE,
	WET_USER
};

/* 部件的事件数据结构和系统事件一样 */
typedef LCUI_MouseMotionEvent LCUI_WidgetMouseMotionEvent;
typedef LCUI_MouseButtonEvent LCUI_WidgetMouseButtonEvent;
typedef LCUI_MouseWheelEvent LCUI_WidgetMouseWheelEvent;
typedef LCUI_TextInputEvent LCUI_WidgetTextInputEvent;
typedef LCUI_KeyboardEvent LCUI_WidgetKeyboardEvent;
typedef LCUI_TouchEvent LCUI_WidgetTouchEvent;

/** 面向部件级的事件内容结构 */
typedef struct LCUI_WidgetEventRec_ {
	uint32_t type;			/**< 事件类型标识号 */
	void *data;			/**< 附加数据 */
	LCUI_Widget target;		/**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble;	/**< 是否取消事件冒泡 */
	union {
		LCUI_WidgetMouseMotionEvent motion;
		LCUI_WidgetMouseButtonEvent button;
		LCUI_WidgetMouseWheelEvent wheel;
		LCUI_WidgetKeyboardEvent key;
		LCUI_WidgetTouchEvent touch;
		LCUI_WidgetTextInputEvent text;
	};
} LCUI_WidgetEventRec, *LCUI_WidgetEvent;

typedef void(*LCUI_WidgetEventFunc)(LCUI_Widget, LCUI_WidgetEvent, void*);

/** 设置阻止部件及其子级部件的事件 */
#define Widget_BlockEvent(WIDGET, FLAG) (WIDGET)->event_blocked = FLAG

/** 触发事件，让事件处理器在主循环中调用 */
LCUI_API int Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent e,
			       void *data, void( *destroy_data )(void*) );

/** 触发事件，直接调用事件处理器 */
LCUI_API int Widget_TriggerEvent( LCUI_Widget widget, LCUI_WidgetEvent e,
				  void *data );

/** 自动分配一个可用的事件标识号 */
LCUI_API int LCUIWidget_AllocEventId( void );

/** 设置与事件标识号对应的名称 */
LCUI_API int LCUIWidget_SetEventName( int event_id, const char *event_name );

/** 获取与事件标识号对应的名称 */
LCUI_API const char *LCUIWidget_GetEventName( int event_id );

/** 获取与事件名称对应的标识号 */
LCUI_API int LCUIWidget_GetEventId( const char *event_name );

/**
* 添加部件事件绑定
* @param[in] widget 目标部件
* @param[in] event_id 事件标识号
* @param[in] func 事件处理函数
* @param[in] data 事件处理函数的附加数据
* @param[in] destroy_data 数据的销毁函数
*/
LCUI_API int Widget_BindEventById( LCUI_Widget widget, int event_id,
				   LCUI_WidgetEventFunc func, void *data,
				   void( *destroy_data )(void*) );

/**
* 添加部件事件绑定
* @param[in] widget 目标部件
* @param[in] event_name 事件名称
* @param[in] func 事件处理函数
* @param[in] data 事件处理函数的附加数据
* @param[in] destroy_data 数据的销毁函数
*/
LCUI_API int Widget_BindEvent( LCUI_Widget widget, const char *event_name,
			       LCUI_WidgetEventFunc func, void *data,
			       void( *destroy_data )(void*) );

/**
* 解除部件事件绑定
* @param[in] widget 目标部件
* @param[in] event_id 事件标识号
* @param[in] func 与事件绑定的函数
*/
LCUI_API int Widget_UnbindEventById( LCUI_Widget widget, int event_id,
				     LCUI_WidgetEventFunc func );
/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] func 与事件绑定的函数
 */
LCUI_API int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name,
				 LCUI_WidgetEventFunc func );
/**
 * 投递 surface 事件
 * surface 是与顶层部件绑定在一起的，只有当部件为顶层部件时，才能投递 surface 事件。
 * surface 事件主要用于让 surface 与 widget 同步一些数据，如：大小、位置、显示/隐藏。
 */
LCUI_API int Widget_PostSurfaceEvent( LCUI_Widget w, int event_type );

/** 清除事件对象，通常在部件销毁时调用该函数，以避免部件销毁后还有事件发送给它 */
LCUI_API void LCUIWidget_ClearEventTarget( LCUI_Widget widget );

/** 将一个部件设置为焦点 */
LCUI_API int LCUIWidget_SetFocus( LCUI_Widget widget );

/** 停止部件的事件传播 */
LCUI_API int Widget_StopEventPropagation( LCUI_Widget widget );

/** 为部件设置鼠标捕获，设置后将捕获全局范围内的鼠标事件 */
LCUI_API void Widget_SetMouseCapture( LCUI_Widget w );

/** 为部件解除鼠标捕获 */
LCUI_API void Widget_ReleaseMouseCapture( LCUI_Widget w );

/**
 * 为部件设置触点捕获，设置后将捕获全局范围内的触点事件
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则捕获全部触点
 */
LCUI_API void Widget_SetTouchCapture( LCUI_Widget w, int point_id );

/**
 * 为部件解除触点捕获
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则解除全部触点的捕获
 */
LCUI_API int Widget_ReleaseTouchCapture( LCUI_Widget w, int point_id );

/** 初始化 LCUI 部件的事件系统 */
void LCUIWidget_InitEvent(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void LCUIWidget_ExitEvent(void);

LCUI_END_HEADER

#endif
