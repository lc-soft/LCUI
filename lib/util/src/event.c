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
#include <LCUI/header.h>
#include <LCUI/types.h>
#include <LCUI/util.h>

/** 事件绑定记录 */
typedef struct LCUI_EventRecordRec_ {
	int id;				/**< 事件标识号 */
	LCUI_BOOL blocked;		/**< 是否已经锁定 */
	list_t handlers;		/**< 事件处理器列表 */
	list_t trash;		/**< 待移除的处理器列表 */
} LCUI_EventRecordRec, *LCUI_EventRecord;

/** 事件处理器 */
typedef struct LCUI_EventHandlerRec_ {
	int id;				/**< 标识号 */
	void *data;			/**< 相关数据 */
	void(*destroy_data)(void*);	/**< 用于销毁数据的回调函数 */
	LCUI_EventFunc func;		/**< 事件处理函数 */
	list_node_t node;		/**< 处理器列表中的节点 */
	LCUI_EventRecord record;	/**< 所属事件绑定记录 */
} LCUI_EventHandlerRec, *LCUI_EventHandler;

static void DestroyEventHandler(void *data)
{
	LCUI_EventHandler handler = data;
	list_unlink(&handler->record->handlers, &handler->node);
	if (handler->destroy_data && handler->data) {
		handler->destroy_data(handler->data);
	}
	handler->data = NULL;
	free(handler);
}

static void DestroyEventRecord(void *data)
{
	list_node_t *node;
	LCUI_EventRecord record = data;
	while (record->handlers.length > 0) {
		node = list_get_node(&record->handlers, 0);
		if (node) {
			DestroyEventHandler(node->data);
		}
	}
	list_destroy(&record->trash, NULL);
	free(record);
}

static void EventTrigger_RemoveHandler(LCUI_EventTrigger trigger,
				       LCUI_EventHandler handler)
{
	list_node_t *node;
	LCUI_EventRecord record = handler->record;
	if (record->blocked) {
		for (list_each(node, &record->trash)) {
			if (node->data == handler) {
				return;
			}
		}
		list_append(&record->trash, handler);
		return;
	}
	rbtree_delete_by_key(&trigger->handlers, handler->id);
	DestroyEventHandler(handler);
	if (record->handlers.length < 1) {
		rbtree_delete_by_key(&trigger->events, record->id);
	}
}

LCUI_EventTrigger EventTrigger(void)
{
	LCUI_EventTrigger trigger = malloc(sizeof(LCUI_EventTriggerRec));
	trigger->handler_base_id = 1;
	rbtree_init(&trigger->handlers);
	rbtree_init(&trigger->events);
	rbtree_set_destroy_func(&trigger->events, DestroyEventRecord);
	return trigger;
}

void EventTrigger_Destroy(LCUI_EventTrigger trigger)
{
	rbtree_destroy(&trigger->events);
	rbtree_destroy(&trigger->handlers);
	free(trigger);
}

int EventTrigger_Bind(LCUI_EventTrigger trigger, int event_id,
		      LCUI_EventFunc func, void *data,
		      void(*destroy_data)(void*))
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	record = rbtree_get_data_by_key(&trigger->events, event_id);
	if (!record) {
		record = malloc(sizeof(LCUI_EventRecordRec));
		record->id = event_id;
		record->blocked = FALSE;
		list_create(&record->trash);
		list_create(&record->handlers);
		rbtree_insert_by_key(&trigger->events, event_id, record);
	}
	handler = malloc(sizeof(LCUI_EventHandlerRec));
	handler->id = trigger->handler_base_id++;
	handler->destroy_data = destroy_data;
	handler->data = data;
	handler->func = func;
	handler->record = record;
	handler->node.data = handler;
	list_append_node(&record->handlers, &handler->node);
	rbtree_insert_by_key(&trigger->handlers, handler->id, handler);
	return handler->id;
}

int EventTrigger_Unbind(LCUI_EventTrigger trigger, int event_id,
			LCUI_EventFunc func)
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	list_node_t *node = NULL;
	record = rbtree_get_data_by_key(&trigger->events, event_id);
	if (!record) {
		return -1;
	}
	for (list_each(node, &record->handlers)) {
		handler = node->data;
		if (handler->func != func) {
			continue;
		}
		EventTrigger_RemoveHandler(trigger, handler);
		return 0;
	}
	return -1;
}

int EventTrigger_Unbind2(LCUI_EventTrigger trigger, int handler_id)
{
	LCUI_EventHandler handler;
	handler = rbtree_get_data_by_key(&trigger->handlers, handler_id);
	if (!handler) {
		return -1;
	}
	EventTrigger_RemoveHandler(trigger, handler);
	return 0;
}

int EventTrigger_Unbind3(LCUI_EventTrigger trigger, int event_id,
			 int(*compare_func)(void*, void*), void *key)
{
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	list_node_t *node = NULL;
	record = rbtree_get_data_by_key(&trigger->events, event_id);
	if (!record) {
		return -1;
	}
	for (list_each(node, &record->handlers)) {
		handler = node->data;
		if (!compare_func(key, handler->data)) {
			continue;
		}
		EventTrigger_RemoveHandler(trigger, handler);
		return 0;
	}
	return -1;
}

int EventTrigger_Trigger(LCUI_EventTrigger trigger, int event_id, void *arg)
{
	int count = 0;
	LCUI_EventRec e;
	LCUI_EventRecord record;
	LCUI_EventHandler handler;
	list_node_t *node;
	record = rbtree_get_data_by_key(&trigger->events, event_id);
	if (!record) {
		return count;
	}
	e.type = event_id;
	record->blocked = TRUE;
	for (list_each(node, &record->handlers)) {
		handler = node->data;
		e.data = handler->data;
		handler->func(&e, arg);
		++count;
	}
	record->blocked = FALSE;
	while (record->trash.length > 0) {
		node = record->trash.head.next;
		handler = node->data;
		rbtree_delete_by_key(&trigger->handlers, handler->id);
		list_unlink(&record->handlers, &handler->node);
		list_delete_node(&record->trash, node);
		if (handler->destroy_data && handler->data) {
			handler->destroy_data(handler->data);
		}
		handler->data = NULL;
		free(handler);
	}
	if (record->handlers.length < 1) {
		rbtree_delete_by_key(&trigger->events, record->id);
	}
	return count;
}
