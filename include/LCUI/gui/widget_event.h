/* ***************************************************************************
 * widget_event.h -- LCUI widget event module.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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
	WET_NONE,
	WET_ADD,
	WET_REMOVE,
	WET_DESTROY,
	WET_MOVE,
	WET_RESIZE,
	WET_SHOW,
	WET_HIDE,
	WET_FOCUS,
	WET_BLUR,
	WET_AFTERLAYOUT,
	WET_KEYDOWN,
	WET_KEYUP,
	WET_KEYPRESS,
	WET_INPUT,

	WET_MOUSEOVER,
	WET_MOUSEMOVE,
	WET_MOUSEOUT,
	WET_MOUSEDOWN,
	WET_MOUSEUP,
	WET_MOUSEWHEEL,
	WET_CLICK,

	WET_TITLE,
	WET_SURFACE,
	WET_USER
};

/** 面向部件级的事件内容结构 */
typedef struct LCUI_WidgetEventRec_ {
	int type;			/**< 事件类型标识号 */
	int which;			/**< 指示按了哪个键或按钮 */
	int x, y;			/**< 事件触发时鼠标的坐标（相对于当前部件的边框盒） */
	int z_delta;			/**< 鼠标滚轮滚动的距离差值 */
	int screen_x, screen_y;		/**< 事件触发时鼠标的屏幕坐标 */
	void *data;			/**< 附加数据 */
	LCUI_Widget target;		/**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble;	/**< 是否取消事件冒泡 */
} LCUI_WidgetEventRec, *LCUI_WidgetEvent;

typedef void(*LCUI_WidgetEventFunc)(LCUI_Widget, LCUI_WidgetEvent, void*);

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

/** 初始化 LCUI 部件的事件系统 */
void LCUIWidget_InitEvent(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void LCUIWidget_ExitEvent(void);

LCUI_END_HEADER

#endif
