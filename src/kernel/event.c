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
#include <LCUI/misc/rbtree.h>
#include <LCUI/misc/linkedlist.h>
#include <LCUI/kernel/event.h>

#include <string.h>

#define MALLOC_ONE(type) (type*)malloc(sizeof(type))

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
} LCUI_EventBoxRec;

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

/** 创建一个事件容器实例 */
LCUI_EventBox LCUIEventBox_Create(void)
{
	LCUI_EventBoxRec* boxdata;
	boxdata = MALLOC_ONE(LCUI_EventBoxRec);
	boxdata->event_id = 100;
	boxdata->handler_id = 100;
	RBTree_Init( &boxdata->event_slot );
	RBTree_Init( &boxdata->event_name );
	RBTree_Init( &boxdata->event_handler );
	RBTree_OnJudge( &boxdata->event_name, CompareEventName );
	RBTree_OnDestroy( &boxdata->event_slot, DestroyEventSlot );
	RBTree_SetDataNeedFree( &boxdata->event_handler, 0 );
	RBTree_SetDataNeedFree( &boxdata->event_name, 0 );
	LinkedList_Init( &boxdata->events, sizeof(LCUI_Event) );
	return boxdata;
}

/** 销毁事件容器实例 */
void LCUIEventBox_Destroy( LCUI_EventBox box )
{
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;
	RBTree_Destroy( &boxdata->event_name );
	RBTree_Destroy( &boxdata->event_handler );
	RBTree_Destroy( &boxdata->event_slot );
	LinkedList_Destroy( &boxdata->events );
}

/** 连接事件 */
int LCUIEventBox_Conncet( LCUI_EventBox box, const char *name,
				EventCallBack func, void *data )
{
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;

	node = RBTree_CustomSearch( &boxdata->event_name, (const void*)name );
	if( !node ) {
		slot = MALLOC_ONE(LCUI_EventSlot);
		slot->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
		slot->id = ++boxdata->event_id;
		strcpy( slot->name, name );
		LinkedList_Init( &slot->handlers, sizeof(LCUI_EventHandler) );
		RBTree_Insert( &boxdata->event_slot, slot->id, slot );
		node = RBTree_CustomInsert( &boxdata->event_name, 
					(const void*)name, &slot->name );
		node->key = slot->id;
	} else {
		node = RBTree_Search( &boxdata->event_slot, node->key );
		slot = (LCUI_EventSlot*)node->data;
	}

	handler = MALLOC_ONE(LCUI_EventHandler);
	handler->id = ++boxdata->handler_id;
	handler->func = func;
	handler->func_data = data;
	LinkedList_AddData( &slot->handlers, handler );
	RBTree_Insert( &boxdata->event_handler, handler->id, (void*)(slot->id) );
	return handler->id;
}

/** 解除事件连接 */
int LCUIEventBox_Disconnect( LCUI_EventBox box, int handler_id )
{
	int i, n;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;

	node = RBTree_Search( &boxdata->event_handler, handler_id );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &boxdata->event_slot, (int)(node->data) );
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
			RBTree_Erase( &boxdata->event_handler, handler_id );
			return 0;
		}
		LinkedList_ToNext( &slot->handlers );
	}
	return -3;
}

/** 直接将事件发送至事件处理器进行处理 */
int LCUIEventBox_Send( LCUI_EventBox box, const char *name, void *data )
{
	int i, n;
	LCUI_Event event;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;
	
	node = RBTree_CustomSearch( &boxdata->event_name, (const void*)name );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &boxdata->event_slot, node->key );
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	event.data = data;
	event.name = slot->name;
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
int LCUIEventBox_Post( LCUI_EventBox box, const char *name, void *data )
{
	LCUI_Event event;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;
	
	node = RBTree_CustomSearch( &boxdata->event_name, (const void*)name );
	if( !node ) {
		return -1;
	}
	node = RBTree_Search( &boxdata->event_slot, node->key );
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	event.data = data;
	event.name =slot->name;
	event.id = slot->id;
	LinkedList_AddDataCopy( &boxdata->events, &event );
	return 0;
}

/** 分派所有已触发的事件至事件处理器 */
void LCUIEventBox_Dispatch( LCUI_EventBox box )
{
	int i, n;
	LCUI_Event *event;
	LCUI_EventBoxRec *boxdata = (LCUI_EventBoxRec*)box;

	n = LinkedList_GetTotal( &boxdata->events );
	LinkedList_Goto( &boxdata->events, 0 );
	for( i=0; i<n; ++i ) {
		event = (LCUI_Event*)LinkedList_Get( &boxdata->events );
		LCUIEventBox_Send( box, event->name, event->data );
		LinkedList_Delete( &boxdata->events );
	}
}
