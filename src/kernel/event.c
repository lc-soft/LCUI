/* ***************************************************************************
 * event.c -- event processing module
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
 * event.c -- 事件处理模块
 *
 * 版权所有 (C) 2012-2014 归属于
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
#include <LCUI_Build.h>
#include LC_LCUI_H

//#define __USE_NEW_EVENT_CODE__
#ifdef __USE_NEW_EVENT_CODE__

#define MALLOC_ONE(type) (type*)malloc(sizeof(type))

typedef void(*EventCallBack)(LCUI_Event*, void*);

/** 事件 */
typedef struct LCUI_EventRec_ {
	int id;			/**< 事件标识号 */
	const char *name;	/**< 事件名（只读） */
	void *data;		/**< 事件附加数据 */
} LCUI_Event;

/** 事件处理器 */
typedef struct LCUI_EventHandlerRec_ {
	int id;			/**< 标识号 */
	EventCallBack func;	/**< 回调函数 */
	void *func_data;	/**< 传给回调函数的数据 */
} LCUI_EventHandler;

/** 事件槽，记录与事件链接的响应函数 */
typedef struct EventSlotRec_ {
	int id;			/**< 事件标识号 */
	char *name;		/**< 事件名 */
	LinkedList handlers;	/**< 事件处理器列表 */
} LCUI_EventSlot;

/** 事件处理器相关数据 */
typedef struct LCUI_EventBoxRec_ {
	LCUI_RBTree event_slot;		/**< 事件槽记录 */
	LCUI_RBTree event_name;		/**< 事件名称记录 */
	LCUI_RBTree event_handler;	/**< 事件处理器记录 */
	LinkedList events;		/**< 任务列表 */
	int event_id;			/**< 事件的标识号计数器 */
	int handler_id;			/**< 事件处理器的标识号计数器 */
} LCUI_EventBox;

static int CompareEventName( void *data, const void *keydata )
{
	return strcmp(*(char**)data, (const char*)keydata);
}

static void DestroyEventSlot( void *data ) {
	LCUI_EventSlot *slot = (LCUI_EventSlot*)data;
	free( slot->name );
	slot->name = NULL;
	LinkedList_Destroy( &slot->handlers );
}

/** 初始化一个事件容器实例 */
void LCUIEventBox_Init( LCUI_EventBox *box )
{
	box->event_id = 100;
	box->handler_id = 100;
	RBTree_Init( &box->event_slot );
	RBTree_Init( &box->event_name );
	RBTree_Init( &box->event_handler );
	RBTree_OnJudge( &box->event_name, CompareEventName );
	RBTree_OnDestroy( &box->event_slot, DestroyEventSlot );
	RBTree_SetDataNeedFree( &box->event_handler, FALSE );
	RBTree_SetDataNeedFree( &box->event_name, FALSE );
	LinkedList_Init( &box->events, sizeof(LCUI_Event) );
}

/** 销毁事件容器实例 */
void LCUIEventBox_Destroy( LCUI_EventBox *box )
{

}

/** 分派所有已触发的事件至事件处理器 */
void LCUIEventBox_Dispatch( LCUI_EventBox *box )
{

}

/** 连接事件 */
int LCUIEventBox_Conncet( LCUI_EventBox *box, const char *name,
				EventCallBack func, void *data )
{
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;

	node = RBTree_CustomSearch( &box->event_name, (const void*)name );
	if( !node ) {
		slot = MALLOC_ONE(LCUI_EventSlot);
		slot->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
		slot->id = ++box->event_id;
		strcpy( slot->name, name );
		LinkedList_Init( &slot->handlers, sizeof(LCUI_EventHandler) );
		RBTree_Insert( &box->event_slot, slot->id, slot );
		node = RBTree_CustomInsert( &box->event_name, 
					(const void*)name, &slot->name );
		node->key = slot->id;
	} else {
		node = RBTree_Search( &box->event_slot, node->key );
		slot = (LCUI_EventSlot*)node->data;
	}

	handler = MALLOC_ONE(LCUI_EventHandler);
	handler->id = ++box->handler_id;
	handler->func = func;
	handler->func_data = data;
	LinkedList_AddData( &slot->handlers, handler );
	RBTree_Insert( &box->event_handler, handler->id, (void*)(slot->id) );
	return handler->id;
}

/** 解除事件连接 */
int LCUIEventBox_Disconnect( LCUI_EventBox *box, int handler_id )
{
	int i, n;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;

	node = RBTree_Search( &box->event_handler, handler_id );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &box->event_slot, (int)(node->data) );
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	LinkedList_Goto( &slot->handlers, 0 );
	n = LinkedList_GetTotal( &slot->handlers );
	for( i=0; i<n; ++i ) {
		handler = (LCUI_EventHandler*)LinkedList_Get( &slot->handlers );
		if( handler->id == handler_id ) {
			LinkedList_Delete( &slot->handlers );
			RBTree_Erase( &box->event_handler, handler_id );
			return 0;
		}
		LinkedList_ToNext( &slot->handlers );
	}
	return -3;
}

/** 直接将事件发送至事件处理器进行处理 */
int LCUIEventBox_Send( LCUI_EventBox *box, const char *name, void *data )
{
	int i, n;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	LCUI_Event event;
	
	node = RBTree_CustomSearch( &box->event_name, (const void*)name );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &box->event_slot, node->key );
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	event.data = data;
	event.name = name;
	event.id = slot->id;
	n = LinkedList_GetTotal( &slot->handlers );
	LinkedList_Goto( &slot->handlers, 0 );
	for( i=0; i<n; ++i ) {
		handler = (LCUI_EventHandler*)LinkedList_Get( &slot->handlers );
		handler->func( &event, handler->func_data );
		LinkedList_ToNext( &slot->handlers );
	}
	return 0;
}

/** 将事件投递给事件处理器，等待处理 */
int LCUIEventBox_Post( LCUI_EventBox *box, const char *name, void *data )
{
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	LCUI_Event event;
	
	node = RBTree_CustomSearch( &box->event_name, (const void*)name );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &box->event_slot, node->key );
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	event.data = data;
	event.name = name;
	event.id = slot->id;
	LinkedList_AddDataCopy( &box->events, &event );
	return 0;
}

/*
LCUI_DisconnectEvent
LCUI_ConnectEvent
LCUI_SendEvent
LCUI_PostEvent
*/
#else

static LCUI_Queue global_events;
static LCUI_Queue sys_event_slots;
static LCUI_Queue user_event_slots;
static LCUI_BOOL active = FALSE;
static LCUI_Thread eventloop_thread = -1;

/** 事件队列初始化 */
static void LCUI_EventsInit( void )
{
	Queue_Init( &global_events, sizeof(LCUI_Event), NULL );
}

/** 销毁事件队列 */
static void LCUI_DestroyEvents( void )
{
	Queue_Destroy( &global_events );
}

/** 从事件队列中取出一个事件 */
LCUI_API LCUI_BOOL LCUI_PollEvent( LCUI_Event *event )
{
	LCUI_Event *top_event;

	if( !active ) {
		return FALSE;
	}

	Queue_Lock( &global_events );
	top_event = (LCUI_Event*)Queue_Get( &global_events, 0 );
	if( !top_event ) {
		Queue_Unlock( &global_events );
		return FALSE;
	}
	*event = *top_event;
	Queue_Delete( &global_events, 0 );
	Queue_Unlock( &global_events );
	return TRUE;
}

/* 将系统事件分发到已注册的回调函数 */
static void LCUI_DispatchSystemEvent( LCUI_Event *event )
{
	int i, n;
	LCUI_EventSlot *slot;
	LCUI_Task *task;
	LCUI_Event event_buff;

	slot = EventSlots_Find( &sys_event_slots, event->type);
	if( !slot ) {
		return;
	}
	n = Queue_GetTotal( &slot->func_data );
	for(i=0; i<n; ++i) {
		task = (LCUI_Task*)Queue_Get( &slot->func_data, i );
		if( !task || !task->func ) {
			continue;
		}
		/* 备份一次，避免回调函数修改事件数据 */
		event_buff = *event;
		task->func( &event_buff, task->arg[1] );
	}
}

/** 将用户事件分发到已注册的回调函数 */
static void LCUI_DispatchUserEvent( LCUI_Event *event )
{
	int i, n;
	LCUI_EventSlot *slot;
	LCUI_Task *task;
	LCUI_UserEvent user_event;

	slot = EventSlots_Find(	&user_event_slots,
				event->user.code );
	if( !slot ) {
		return;
	}
	n = Queue_GetTotal( &slot->func_data );
	for(i=0; i<n; ++i) {
		task = (LCUI_Task*)Queue_Get( &slot->func_data, i );
		if( !task || !task->func ) {
			continue;
		}
		user_event = event->user;
		task->func( user_event.data1, user_event.data2 );
	}
}

/** 事件循环 */
static void LCUI_EventLoop( void *unused )
{
	LCUI_Event event;
	int timeout_count = 0;

	while( active ) {
		if( LCUI_PollEvent( &event ) ) {
			timeout_count = 0;
			switch( event.type ) {
			case LCUI_KEYDOWN:
			case LCUI_KEYUP:
			case LCUI_MOUSEMOTION:
			case LCUI_MOUSEBUTTONDOWN:
			case LCUI_MOUSEBUTTONUP:
				LCUI_DispatchSystemEvent( &event );
				break;
			case LCUI_USEREVENT:
				LCUI_DispatchUserEvent( &event );
				break;
			}
		} else {
			++timeout_count;
			if( timeout_count <= 15 ) {
				continue;
			}
			LCUI_MSleep( 5 );
			timeout_count = 0;
		}
	}
	_LCUIThread_Exit( NULL );
}

/** 停用事件线程 */
static void LCUI_StopEventThread( void )
{
	if( !active ) {
		return;
	}
	active = FALSE;
	_LCUIThread_Join( eventloop_thread, NULL );
}

/* 启动事件线程 */
static int LCUI_StartEventThread( void )
{
	LCUI_StopEventThread();
	active = TRUE;
	return _LCUIThread_Create( &eventloop_thread, LCUI_EventLoop, NULL );
}

/** 初始化事件模块 */
LCUI_API void LCUIModule_Event_Init( void )
{
	EventSlots_Init( &sys_event_slots );
	EventSlots_Init( &user_event_slots );
	LCUI_EventsInit();
	LCUI_StartEventThread();
}

/** 停用事件模块 */
LCUI_API void LCUIModule_Event_End( void )
{
	LCUI_StopEventThread();
	LCUI_DestroyEvents();
	Queue_Destroy( &sys_event_slots );
	Queue_Destroy( &user_event_slots );
}

/** 添加事件至事件队列中 */
LCUI_API LCUI_BOOL LCUI_PushEvent( LCUI_Event *event )
{
	if( !active ) {
		return FALSE;
	}

	Queue_Lock( &global_events );
	if( !Queue_Add( &global_events, event ) ) {
		Queue_Unlock( &global_events );
		return FALSE;
	}
	Queue_Unlock( &global_events );
	return TRUE;
}

/* 销毁事件槽数据 */
static void Destroy_EventSlot( void *arg )
{
	LCUI_EventSlot *slot;
	slot = (LCUI_EventSlot*)arg;
	if( slot ) {
		Queue_Destroy( &slot->func_data );
	}
}

/** 初始化事件槽记录 */
LCUI_API void EventSlots_Init( LCUI_Queue *slots )
{
	Queue_Init( slots, sizeof(LCUI_EventSlot), Destroy_EventSlot);
}

/** 根据事件的ID，获取与该事件关联的事件槽 */
LCUI_API LCUI_EventSlot* EventSlots_Find( LCUI_Queue *slots, int event_id )
{
	int i, total;
	LCUI_EventSlot *slot;

	total = Queue_GetTotal( slots );
	if (total <= 0) {
		return NULL;
	}
	for (i = 0; i < total; ++i) {
		slot = (LCUI_EventSlot*)Queue_Get( slots, i );
		if( slot->id == event_id ) {
			return slot;
		}
	}
	return NULL;
}

/** 添加事件槽与事件的关联记录 */
LCUI_API int EventSlots_Add( LCUI_Queue *slots, int event_id, LCUI_Func *func )
{
	LCUI_EventSlot *slot;
	LCUI_EventSlot new_slot;

	slot = EventSlots_Find( slots, event_id );
	if ( slot ) {
		func->id = ++slot->temp_connect_id;
		if( !Queue_Add( &slot->func_data, func ) ) {
			return -1;
		}
		return func->id;
	}

	new_slot.id = event_id;
	new_slot.temp_connect_id = 100;
	func->id = 100;
	Queue_Init( &new_slot.func_data, sizeof(LCUI_Func), NULL );
	if( !Queue_Add( &new_slot.func_data, func ) ) {
		return -2;
	}
	if( !Queue_Add( slots, &new_slot ) ) {
		return -3;
	}
	return new_slot.temp_connect_id;
}

/** 从事件槽中移除指定记录 */
LCUI_API int EventSlots_Delete( LCUI_Queue *slots, int event_id, int func_id )
{
	int n;
	LCUI_EventSlot *slot;
	LCUI_Func *p_func;

	slot = EventSlots_Find( slots, event_id );
	if( !slot ) {
		return -1;
	}
	n = Queue_GetTotal( &slot->func_data );
	while(n--) {
		p_func = (LCUI_Func*)Queue_Get( &slot->func_data, n );
		if( p_func->id == func_id ) {
			Queue_Delete( &slot->func_data, n );
			return 0;
		}
	}
	return 1;
}

/** 与指定系统事件建立连接，以进行响应 */
LCUI_API int LCUISysEvent_Connect(	int event_type,
					void (*func)(LCUI_Event*,void*), 
					void *arg )
{
	LCUI_Func func_data;
	func_data.func = (CallBackFunc)func;
	func_data.destroy_arg[0] = FALSE;
	func_data.destroy_arg[1] = FALSE;
	func_data.arg[0] = NULL;
	func_data.arg[1] = arg;
	return EventSlots_Add( &sys_event_slots, event_type, &func_data );
}

/** 移除与系统事件的连接 */
LCUI_API int LCUISysEvent_Disconnect( int event_type, int connect_id )
{
	return EventSlots_Delete( &sys_event_slots, event_type, connect_id );
}

/* 将回调函数与用户自定义的事件进行连接 */
LCUI_API int LCUIUserEvent_Connect( int event_id, void (*func)(void*, void*) )
{
	LCUI_Func func_data;
	func_data.func = (CallBackFunc)func;
	func_data.destroy_arg[0] = FALSE;
	func_data.destroy_arg[1] = FALSE;
	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;
	return EventSlots_Add(	&user_event_slots,
				LCUI_USEREVENT, &func_data );
}

/** 移除与用户事件的连接 */
LCUI_API int LCUIUserEvent_Disconnect( int event_type, int connect_id )
{
	return EventSlots_Delete( &user_event_slots, event_type, connect_id );
}
#endif
