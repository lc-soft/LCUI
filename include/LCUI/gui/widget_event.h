/*
 * widget_event.h -- LCUI widget event module.
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

#ifndef LCUI_WIDGET_EVENT_H
#define LCUI_WIDGET_EVENT_H

LCUI_BEGIN_HEADER

/** 部件事件类型枚举 */
typedef enum LCUI_WidgetEventType {
	LCUI_WEVENT_NONE,
	LCUI_WEVENT_LINK,		/**< link widget node to the parent widget children list */
	LCUI_WEVENT_UNLINK,		/**< unlink widget node from the parent widget children list */
	LCUI_WEVENT_READY,		/**< after widget initial layout was completed */
	LCUI_WEVENT_DESTROY,		/**< before destroy */
	LCUI_WEVENT_MOVE,		/**< 在移动位置时 */
	LCUI_WEVENT_RESIZE,		/**< 改变尺寸 */
	LCUI_WEVENT_SHOW,		/**< 显示 */
	LCUI_WEVENT_HIDE,		/**< 隐藏 */
	LCUI_WEVENT_FOCUS,		/**< 获得焦点 */
	LCUI_WEVENT_BLUR,		/**< 失去焦点 */
	LCUI_WEVENT_AFTERLAYOUT,	/**< 在子部件布局完成后 */
	LCUI_WEVENT_KEYDOWN,		/**< 按键按下 */
	LCUI_WEVENT_KEYUP,		/**< 按键释放 */
	LCUI_WEVENT_KEYPRESS,		/**< 按键字符输入 */
	LCUI_WEVENT_TEXTINPUT,		/**< 文本输入 */

	LCUI_WEVENT_MOUSEOVER,		/**< 鼠标在部件上 */
	LCUI_WEVENT_MOUSEMOVE,		/**< 鼠标在部件上移动 */
	LCUI_WEVENT_MOUSEOUT,		/**< 鼠标从部件上移开 */
	LCUI_WEVENT_MOUSEDOWN,		/**< 鼠标按键按下 */
	LCUI_WEVENT_MOUSEUP,		/**< 鼠标按键释放 */
	LCUI_WEVENT_MOUSEWHEEL,		/**< 鼠标滚轮滚动时 */
	LCUI_WEVENT_CLICK,		/**< 鼠标单击 */
	LCUI_WEVENT_DBLCLICK,		/**< 鼠标双击 */
	LCUI_WEVENT_TOUCH,		/**< 触控 */
	LCUI_WEVENT_TOUCHDOWN,		/**< 触点按下 */
	LCUI_WEVENT_TOUCHUP,		/**< 触点释放 */
	LCUI_WEVENT_TOUCHMOVE,		/**< 触点移动 */

	LCUI_WEVENT_TITLE,
	LCUI_WEVENT_SURFACE,
	LCUI_WEVENT_USER
} LCUI_WidgetEventType;

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
LCUI_API LCUI_BOOL Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent ev,
				     void *data, void( *destroy_data )(void*) );

/** 触发事件，直接调用事件处理器 */
LCUI_API int Widget_TriggerEvent( LCUI_Widget widget,
				  LCUI_WidgetEvent e, void *data );

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
 * @return 成功则返回事件处理器的标识号，失败则返回负数
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
 * @return 成功则返回事件处理器的标识号，失败则返回负数
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
 * @param[in] handler_id 事件处理器标识号
 */
LCUI_API int Widget_UnbindEventByHandlerId( LCUI_Widget widget, int handler_id );

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] func 与事件绑定的函数
 */
LCUI_API int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name,
				 LCUI_WidgetEventFunc func );
/**
 * 投递表面（surface）事件
 * 表面是与顶层部件绑定在一起的，只有当部件为顶层部件时，才能投递表面事件。
 * 表面事件主要用于让表面与部件同步一些数据，如：大小、位置、显示/隐藏。
 * @param event_type 事件类型
 * @param @sync_props 是否将部件的属性同步给表面
 */
LCUI_API int Widget_PostSurfaceEvent( LCUI_Widget w, int event_type,
				      LCUI_BOOL sync_props );

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
 * @returns 设置成功返回 0，如果其它部件已经捕获该触点则返回 -1
 */
LCUI_API int Widget_SetTouchCapture( LCUI_Widget w, int point_id );

/**
 * 为部件解除触点捕获
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则解除全部触点的捕获
 */
LCUI_API int Widget_ReleaseTouchCapture( LCUI_Widget w, int point_id );

/** 初始化 LCUI 部件的事件系统 */
void LCUIWidget_InitEvent(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void LCUIWidget_FreeEvent(void);

LCUI_END_HEADER

#endif
