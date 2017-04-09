/* ***************************************************************************
* event.c -- event processing module
*
* Copyright (C) 2016-2017 by Liu Chao <lc-soft@live.cn>
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
* 版权所有 (C) 2016-2017 归属于 刘超 <lc-soft@live.cn>
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

/** 事件绑定记录 */
typedef struct LCUI_EventRecordRec_ {
	int id;				/**< 事件标识号 */
	LCUI_BOOL blocked;		/**< 是否已经锁定 */
	LinkedList handlers;		/**< 事件处理器列表 */
	LinkedList trash;		/**< 待移除的处理器列表 */
} LCUI_EventRecordRec, *LCUI_EventRecord;

/** 事件处理器 */
typedef struct LCUI_EventHandlerRec_ {
	int id;				/**< 标识号 */
	void *data;			/**< 相关数据 */
	void (*destroy_data)(void*);	/**< 用于销毁数据的回调函数 */
	LCUI_EventFunc func;		/**< 事件处理函数 */
	LinkedListNode node;		/**< 处理器列表中的节点 */
	LCUI_EventRecord record;	/**< 所属事件绑定记录 */
} LCUI_EventHandlerRec, *LCUI_EventHandler;

static void DestroyEventHandler( void *data )
{
	LCUI_EventHandler handler = data;
	LinkedList_Unlink( &handler->record->handlers, &handler->node );
	if( handler->destroy_data && handler->data ) {
		handler->destroy_data( handler->data );
	}
	handler->data = NULL;
	free( handler );
}

static void DestroyEventRecord( void *data )
{
	LinkedListNode *node;
	LCUI_EventRecord record = data;
	while( record->handlers.length > 0 ) {
		node = LinkedList_GetNode( &record->handlers, 0 );
		if( node ) {
			DestroyEventHandler( node->data );
		}
	}
	LinkedList_Clear( &record->trash, NULL );
	free( record );
}

static void EventTrigger_RemoveHandler( LCUI_EventTrigger trigger,
					LCUI_EventHandler handler )
{
	LinkedListNode *node;
	LCUI_EventRecord record = handler->record;
	if( record->blocked ) {
		for( LinkedList_Each( node, &record->trash ) ) {
			if( node->data == handler ) {
				return;
			}
		}
		LinkedList_Append( &record->trash, handler );
		return;
	}
	RBTree_Erase( &trigger->handlers, handler->id );
	DestroyEventHandler( handler );
	if( record->handlers.length < 1 ) {
		RBTree_Erase( &trigger->events, record->id );
	}
}

LCUI_EventTrigger EventTrigger( void )
{
	LCUI_EventTrigger trigger = NEW( LCUI_EventTriggerRec, 1 );
	trigger->handler_base_id = 1;
	RBTree_Init( &trigger->handlers );
	RBTree_Init( &trigger->events );
	RBTree_OnDestroy( &trigger->events, DestroyEventRecord );
	return trigger;
}

void EventTrigger_Destroy( LCUI_EventTrigger trigger )
{
	RBTree_Destroy( &trigger->events );
	RBTree_Destroy( &trigger->handlers );
	free( trigger );
}

int EventTrigger_Bind( LCUI_EventTrigger trigger, int event_id,
		       LCUI_EventFunc func, void *data,
		       void (*destroy_data)(void*) )
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	record = RBTree_GetData( &trigger->events, event_id );
	if( !record ) {
		record = NEW( LCUI_EventRecordRec, 1 );
		record->id = event_id;
		record->blocked = FALSE;
		LinkedList_Init( &record->trash );
		LinkedList_Init( &record->handlers );
		RBTree_Insert( &trigger->events, event_id, record );
	}
	handler = NEW( LCUI_EventHandlerRec, 1 );
	handler->id = trigger->handler_base_id++;
	handler->destroy_data = destroy_data;
	handler->data = data;
	handler->func = func;
	handler->record = record;
	handler->node.data = handler;
	LinkedList_AppendNode( &record->handlers, &handler->node );
	RBTree_Insert( &trigger->handlers, handler->id, handler );
	return handler->id;
}

int EventTrigger_Unbind( LCUI_EventTrigger trigger, int event_id, 
			 LCUI_EventFunc func )
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	LinkedListNode *node = NULL;
	record = RBTree_GetData( &trigger->events, event_id );
	if( !record ) {
		return -1;
	}
	for( LinkedList_Each( node, &record->handlers ) ) {
		handler = node->data;
		if( handler->func != func ) {
			continue;
		}
		EventTrigger_RemoveHandler( trigger, handler );
		return 0;
	}
	return -1;
}

int EventTrigger_Unbind2( LCUI_EventTrigger trigger, int handler_id )
{
	LCUI_EventHandler handler;
	handler = RBTree_GetData( &trigger->handlers, handler_id );
	if( !handler ) {
		return -1;
	}
	EventTrigger_RemoveHandler( trigger, handler );
	return 0;
}

int EventTrigger_Unbind3( LCUI_EventTrigger trigger, int event_id,
			  int( *compare_func )(void*, void*), void *key )
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	LinkedListNode *node = NULL;
	record = RBTree_GetData( &trigger->events, event_id );
	if( !record ) {
		return -1;
	}
	for( LinkedList_Each( node, &record->handlers ) ) {
		handler = node->data;
		if( !compare_func( key, handler->data ) ) {
			continue;
		}
		EventTrigger_RemoveHandler( trigger, handler );
		return 0;
	}
	return -1;
}

int EventTrigger_Trigger( LCUI_EventTrigger trigger, int event_id, void *arg )
{
	int count = 0;
	LCUI_EventRec e;
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	LinkedListNode *node;
	record = RBTree_GetData( &trigger->events, event_id );
	if( !record ) {
		return count;
	}
	e.type = event_id;
	record->blocked = TRUE;
	for( LinkedList_Each( node, &record->handlers ) ) {
		handler = node->data;
		e.data = handler->data;
		handler->func( &e, arg );
		++count;
	}
	record->blocked = FALSE;
	while( record->trash.length > 0 ) {
		node = record->trash.head.next;
		handler = node->data;
		RBTree_Erase( &trigger->handlers, handler->id );
		LinkedList_Unlink( &record->handlers, &handler->node );
		LinkedList_DeleteNode( &record->trash, node );
		if( handler->destroy_data && handler->data ) {
			handler->destroy_data( handler->data );
		}
		handler->data = NULL;
		free( handler );
	}
	if( record->handlers.length < 1 ) {
		RBTree_Erase( &trigger->events, record->id );
	}
	return count;
}
