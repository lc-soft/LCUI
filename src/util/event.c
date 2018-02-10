/* event.c -- event processing module
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
