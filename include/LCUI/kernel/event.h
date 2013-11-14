/* ***************************************************************************
 * event.h -- event processing module
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * ****************************************************************************/
 
/* ****************************************************************************
 * event.h -- 事件处理模块
 *
 * 版权所有 (C) 2012-2013 归属于
 * 刘超
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
 * ****************************************************************************/
#ifndef __LCUI_KERNEL_EVENT_H__
#define __LCUI_KERNEL_EVENT_H__

LCUI_BEGIN_HEADER

typedef struct {
	unsigned char type;
	int key_code;
} LCUI_KeyboardEvent;

typedef struct {
	unsigned char type;
	int code;
	void *data1;
	void *data2;
} LCUI_UserEvent;

typedef struct{
	unsigned char type;
	unsigned char state;
	unsigned int x, y;
	unsigned int xrel, yrel;
} LCUI_MouseMotionEvent;

typedef struct {
	unsigned char type;
	unsigned char button;
	unsigned char state;
	unsigned int x, y;
} LCUI_MouseButtonEvent;

typedef enum {
	LCUI_KEYDOWN,
	LCUI_KEYUP,
	LCUI_MOUSEMOTION,
	LCUI_MOUSEBUTTONDOWN,
	LCUI_MOUSEBUTTONUP,
	LCUI_QUIT,
	LCUI_USEREVENT
} LCUI_EVENT_TYPE;

typedef union {
	unsigned char type;
	LCUI_KeyboardEvent key;
	LCUI_MouseMotionEvent motion;
	LCUI_MouseButtonEvent button;
	LCUI_UserEvent user;
} LCUI_Event;

typedef struct {
	int id;			/**< 事件ID */
	LCUI_Queue func_data;	/**< 记录被关联的回调函数数据 */
} LCUI_EventSlot;

/** 从事件队列中取出一个事件 */
LCUI_API LCUI_BOOL LCUI_PollEvent( LCUI_Event *event );

/** 初始化事件模块 */
LCUI_API void LCUIModule_Event_Init( void );

/** 停用事件模块 */
LCUI_API void LCUIModule_Event_End( void );

/** 添加事件至事件队列中 */
LCUI_API LCUI_BOOL LCUI_PushEvent( LCUI_Event *event );

/** 初始化事件槽记录 */
LCUI_API void EventSlots_Init( LCUI_Queue *slots );

/** 根据事件的ID，获取与该事件关联的事件槽 */
LCUI_API LCUI_EventSlot* EventSlots_Find( LCUI_Queue *slots, int event_id );

/** 添加事件槽与事件的关联记录 */
LCUI_API int EventSlots_Add( LCUI_Queue *slots, int event_id, LCUI_Func *func );

/** 从事件槽中移除指定记录 */
LCUI_API int EventSlots_Delete( LCUI_Queue *slots, int event_id, int func_id );

/** 与指定系统事件建立连接，以进行响应 */
LCUI_API int LCUISysEvent_Connect(	int event_type,
					void (*func)(LCUI_Event*,void*), 
					void *arg );

/** 移除与系统事件的连接 */
LCUI_API int LCUISysEvent_Disconnect( int event_type, int connect_id );

/* 将回调函数与用户自定义的事件进行连接 */
LCUI_API int LCUIUserEvent_Connect( int event_id, void (*func)(void*, void*) );

/** 移除与用户事件的连接 */
LCUI_API int LCUIUserEvent_Disconnect( int event_type, int connect_id );

LCUI_END_HEADER

#endif
