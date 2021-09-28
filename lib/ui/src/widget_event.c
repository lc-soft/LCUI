﻿/*
 * widget_event.c -- LCUI widget event module.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>
#include <LCUI/input.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

/* clang-format off */

#define DBLCLICK_INTERVAL 500

typedef struct TouchCapturerRec_ {
	list_t points;
	LCUI_Widget widget;
	list_node_t node;
} TouchCapturerRec, *TouchCapturer;

typedef struct WidgetEventHandlerRec_ {
	LCUI_WidgetEventFunc func;
	void *data;
	void(*destroy_data)(void *);
} WidgetEventHandlerRec, *WidgetEventHandler;

typedef struct LCUI_WidgetEventPackRec_ {
	void *data;                   /**< 额外数据 */
	void(*destroy_data)(void *); /**< 数据的销毁函数 */
	LCUI_Widget widget;           /**< 当前处理该事件的部件 */
	LCUI_WidgetEventRec event;    /**< 事件数据 */
} LCUI_WidgetEventPackRec, *LCUI_WidgetEventPack;

enum WidgetStatusType {
	WST_HOVER, WST_ACTIVE, WST_FOCUS, WST_TOTAL
};

/** 部件事件记录结点 */
typedef struct WidgetEventRecordRec_ {
	LCUI_Widget widget; /**< 所属部件 */
	list_t records; /**< 事件记录列表 */
} WidgetEventRecordRec, *WidgetEventRecord;

/** 事件标识号与名称的映射记录 */
typedef struct EventMappingRec_ {
	int id;
	char *name;
} EventMappingRec, *EventMapping;

/** 鼠标点击记录 */
typedef struct ClickRecord_ {
	int64_t time;       /**< 时间 */
	int x, y;           /**< 坐标 */
	int interval;       /**< 与上次点击时的时间间隔 */
	LCUI_Widget widget; /**< 被点击的部件 */
} ClickRecord;

/** 当前功能模块的相关数据 */
static struct LCUIWidgetEvnetModule {
	LCUI_Widget mouse_capturer;     /**< 占用鼠标的部件 */
	list_t touch_capturers;     /**< 触点占用记录 */
	LCUI_Widget targets[WST_TOTAL]; /**< 相关的部件 */
	list_t events;              /**< 已绑定的事件 */
	list_t event_mappings;	/**< 事件标识号和名称映射记录列表  */
	rbtree_t event_records;		/**< 当前正执行的事件的记录 */
	rbtree_t event_names;		/**< 事件名称表，以标识号作为索引 */
	dict_type_t event_ids_type;
	dict_t *event_ids;		/**< 事件标识号表，以事件名称作为索引 */
	int base_event_id;		/**< 事件标识号计数器 */
	ClickRecord click;		/**< 上次鼠标点击记录 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
} self;

/* clang-format on */

static void DestroyEventMapping(void *data)
{
	EventMapping mapping = data;
	free(mapping->name);
	free(mapping);
}

static void DestroyWidgetEvent(LCUI_WidgetEvent e)
{
	switch (e->type) {
	case LCUI_WEVENT_TOUCH:
		if (e->touch.points) {
			free(e->touch.points);
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case LCUI_WEVENT_TEXTINPUT:
		if (e->text.text) {
			free(e->text.text);
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	}
}

static void DirectDestroyWidgetEventPack(void *data)
{
	LCUI_WidgetEventPack pack = data;
	if (pack->data && pack->destroy_data) {
		pack->destroy_data(pack->data);
	}
	DestroyWidgetEvent(&pack->event);
	pack->data = NULL;
	free(pack);
}

static void DestoryWidgetEventRecord(void *data)
{
	WidgetEventRecord record = data;
	list_destroy(&record->records, DirectDestroyWidgetEventPack);
	free(record);
}

static int CompareWidgetEventRecord(void *data, const void *keydata)
{
	WidgetEventRecord record = data;
	if (record->widget == (LCUI_Widget)keydata) {
		return 0;
	} else if (record->widget < (LCUI_Widget)keydata) {
		return -1;
	} else {
		return 1;
	}
}

static void DestroyWidgetEventHandler(void *arg)
{
	WidgetEventHandler handler = arg;
	if (handler->data && handler->destroy_data) {
		handler->destroy_data(handler->data);
	}
	handler->data = NULL;
	free(handler);
}

static int GetEventId(const char *event_name)
{
	int id = LCUIWidget_GetEventId(event_name);
	if (id < 0) {
		id = LCUIWidget_AllocEventId();
		LCUIWidget_SetEventName(id, event_name);
	}
	return id;
}

void LCUI_InitWidgetEvent(LCUI_WidgetEvent e, const char *name)
{
	e->target = NULL;
	e->type = GetEventId(name);
	e->cancel_bubble = FALSE;
	e->data = NULL;
}

/**
 * 添加事件记录
 * 记录当前待处理的事件和目标部件，方便在部件被销毁时清除待处理的事件
 */
static void Widget_AddEventRecord(LCUI_Widget widget, LCUI_WidgetEventPack pack)
{
	WidgetEventRecord record;

	LCUIMutex_Lock(&self.mutex);
	record = rbtree_get_data_by_keydata(&self.event_records, widget);
	if (!record) {
		record = NEW(WidgetEventRecordRec, 1);
		list_create(&record->records);
		record->widget = widget;
		rbtree_insert_by_keydata(&self.event_records, widget, record);
	}
	list_append(&record->records, pack);
	LCUIMutex_Unlock(&self.mutex);
}

/** 删除事件记录 */
static int Widget_DeleteEventRecord(LCUI_Widget widget,
				    LCUI_WidgetEventPack pack)
{
	int ret = 0;
	WidgetEventRecord record;
	list_node_t *node, *prev;

	LCUIMutex_Lock(&self.mutex);
	record = rbtree_get_data_by_keydata(&self.event_records, widget);
	if (!record) {
		LCUIMutex_Unlock(&self.mutex);
		return -1;
	}
	for (list_each(node, &record->records)) {
		prev = node->prev;
		if (node->data == pack) {
			list_delete_node(&record->records, node);
			node = prev;
			ret = 1;
		}
	}
	LCUIMutex_Unlock(&self.mutex);
	return ret;
}

/** 将原始事件转换成部件事件 */
static void WidgetEventTranslator(LCUI_Event e, LCUI_WidgetEventPack pack)
{
	WidgetEventHandler handler = e->data;
	LCUI_Widget w = pack->widget;

	if (!w) {
		return;
	}
	pack->event.type = e->type;
	pack->event.data = handler->data;
	handler->func(w, &pack->event, pack->data);
	while (!pack->event.cancel_bubble && w->parent) {
		w = w->parent;
		if (w->trigger) {
			pack->widget = w;
			/** 向父级部件冒泡传递事件 */
			EventTrigger_Trigger(w->trigger, e->type, pack);
		}
	}
}

/** 复制部件事件 */
static int CopyWidgetEvent(LCUI_WidgetEvent dst, const LCUI_WidgetEvent src)
{
	int n;
	size_t size;

	*dst = *src;
	switch (src->type) {
	case LCUI_WEVENT_TOUCH:
		if (dst->touch.n_points <= 0) {
			break;
		}
		n = dst->touch.n_points;
		size = sizeof(LCUI_TouchPointRec) * n;
		dst->touch.points = malloc(size);
		if (!dst->touch.points) {
			return -ENOMEM;
		}
		memcpy(dst->touch.points, src->touch.points, size);
		break;
	case LCUI_WEVENT_TEXTINPUT:
		if (!dst->text.text) {
			break;
		}
		dst->text.text = NEW(wchar_t, dst->text.length + 1);
		if (!dst->text.text) {
			return -ENOMEM;
		}
		wcsncpy(dst->text.text, src->text.text, dst->text.length + 1);
	default:
		break;
	}
	return 0;
}

/** 销毁部件事件包 */
static void DestroyWidgetEventPack(void *arg)
{
	LCUI_WidgetEventPack pack = arg;
	/* 如果删除成功则说明有记录，需要销毁数据 */
	if (Widget_DeleteEventRecord(pack->event.target, pack) == 1) {
		DirectDestroyWidgetEventPack(pack);
	}
}

static void DestroyTouchCapturer(void *arg)
{
	TouchCapturer tc = arg;
	list_destroy(&tc->points, free);
	tc->widget = NULL;
	free(tc);
}

#define TouchCapturers_Clear(LIST) \
                                   \
	list_destroy_without_node(LIST, DestroyTouchCapturer)

static int TouchCapturers_Add(list_t *list, LCUI_Widget w, int point_id)
{
	int *data;
	TouchCapturer tc = NULL;
	list_node_t *node, *ptnode;
	if (point_id < 0) {
		tc = NEW(TouchCapturerRec, 1);
		tc->widget = w;
		list_create(&tc->points);
		TouchCapturers_Clear(list);
		list_append(list, tc);
		return 0;
	}
	/* 获取该部件的触点捕捉记录 */
	for (list_each(node, list)) {
		tc = node->data;
		/* 清除与该触点绑定的其它捕捉记录 */
		for (list_each(ptnode, &tc->points)) {
			if (point_id == *(int *)ptnode->data) {
				if (tc->widget == w) {
					return 0;
				}
				return -1;
			}
		}
		if (tc->widget == w) {
			break;
		}
	}
	/* 如果没有该部件的触点捕捉记录 */
	if (!tc || tc->widget != w) {
		tc = NEW(TouchCapturerRec, 1);
		tc->widget = w;
		tc->node.data = tc;
		list_create(&tc->points);
		list_append_node(list, &tc->node);
	}
	/* 追加触点捕捉记录 */
	data = NEW(int, 1);
	*data = point_id;
	list_append(&tc->points, data);
	return 0;
}

static int TouchCapturers_Delete(list_t *list, LCUI_Widget w, int point_id)
{
	TouchCapturer tc = NULL;
	list_node_t *node, *ptnode;
	for (list_each(node, list)) {
		tc = node->data;
		if (tc->widget == w) {
			break;
		}
	}
	if (!tc || tc->widget != w) {
		return -1;
	}
	if (point_id < 0) {
		list_destroy(&tc->points, free);
	} else {
		for (list_each(ptnode, &tc->points)) {
			if (*(int *)ptnode->data == point_id) {
				free(node->data);
				list_delete_node(&tc->points, ptnode);
			}
		}
	}
	if (tc->points.length == 0) {
		list_unlink(&self.touch_capturers, &tc->node);
		free(tc);
	}
	return 0;
}

int LCUIWidget_SetEventName(int event_id, const char *event_name)
{
	int ret;
	EventMapping mapping;
	LCUIMutex_Lock(&self.mutex);
	if (dict_fetch_value(self.event_ids, event_name)) {
		LCUIMutex_Unlock(&self.mutex);
		return -1;
	}
	mapping = malloc(sizeof(EventMappingRec));
	mapping->name = strdup2(event_name);
	mapping->id = event_id;
	list_append(&self.event_mappings, mapping);
	rbtree_insert_by_key(&self.event_names, event_id, mapping);
	ret = dict_add(self.event_ids, mapping->name, mapping);
	LCUIMutex_Unlock(&self.mutex);
	return ret;
}

int LCUIWidget_AllocEventId(void)
{
	return self.base_event_id++;
}

const char *LCUIWidget_GetEventName(int event_id)
{
	EventMapping mapping;
	LCUIMutex_Lock(&self.mutex);
	mapping = rbtree_get_data_by_key(&self.event_names, event_id);
	LCUIMutex_Unlock(&self.mutex);
	return mapping ? mapping->name : NULL;
}

int LCUIWidget_GetEventId(const char *event_name)
{
	EventMapping mapping;
	LCUIMutex_Lock(&self.mutex);
	mapping = dict_fetch_value(self.event_ids, event_name);
	LCUIMutex_Unlock(&self.mutex);
	return mapping ? mapping->id : -1;
}

int Widget_BindEventById(LCUI_Widget widget, int event_id,
			 LCUI_WidgetEventFunc func, void *data,
			 void (*destroy_data)(void *))
{
	WidgetEventHandler handler;
	handler = NEW(WidgetEventHandlerRec, 1);
	handler->func = func;
	handler->data = data;
	handler->destroy_data = destroy_data;
	if (!widget->trigger) {
		widget->trigger = EventTrigger();
	}
	return EventTrigger_Bind(widget->trigger, event_id,
				 (LCUI_EventFunc)WidgetEventTranslator, handler,
				 DestroyWidgetEventHandler);
}

int Widget_BindEvent(LCUI_Widget widget, const char *event_name,
		     LCUI_WidgetEventFunc func, void *data,
		     void (*destroy_data)(void *))
{
	return Widget_BindEventById(widget, GetEventId(event_name), func, data,
				    destroy_data);
}

static int CompareEventHandlerKey(void *key, void *func_data)
{
	WidgetEventHandler handler = func_data;
	if (key == handler->func) {
		return 1;
	}
	return 0;
}

int Widget_UnbindEventById(LCUI_Widget widget, int event_id,
			   LCUI_WidgetEventFunc func)
{
	if (!widget->trigger) {
		return -1;
	}
	return EventTrigger_Unbind3(widget->trigger, event_id,
				    CompareEventHandlerKey, func);
}

int Widget_UnbindEventByHandlerId(LCUI_Widget widget, int handler_id)
{
	if (!widget->trigger) {
		return -1;
	}
	return EventTrigger_Unbind2(widget->trigger, handler_id);
}

int Widget_UnbindEvent(LCUI_Widget widget, const char *event_name,
		       LCUI_WidgetEventFunc func)
{
	return Widget_UnbindEventById(widget, GetEventId(event_name), func);
}

static LCUI_Widget Widget_GetNextAt(LCUI_Widget widget, int x, int y)
{
	LCUI_Widget w;
	list_node_t *node;

	node = &widget->node;
	for (node = node->next; node; node = node->next) {
		w = node->data;
		/* 如果忽略事件处理，则向它底层的兄弟部件传播事件 */
		if (w->computed_style.pointer_events == SV_NONE) {
			continue;
		}
		if (!w->computed_style.visible) {
			continue;
		}
		if (!LCUIRect_HasPoint(&w->box.border, x, y)) {
			continue;
		}
		return w;
	}
	return NULL;
}

static int Widget_TriggerEventEx(LCUI_Widget widget, LCUI_WidgetEventPack pack)
{
	LCUI_WidgetEvent e = &pack->event;

	pack->widget = widget;
	switch (e->type) {
	case LCUI_WEVENT_CLICK:
	case LCUI_WEVENT_MOUSEDOWN:
	case LCUI_WEVENT_MOUSEUP:
	case LCUI_WEVENT_MOUSEMOVE:
	case LCUI_WEVENT_MOUSEOVER:
	case LCUI_WEVENT_MOUSEOUT:
		if (widget->computed_style.pointer_events == SV_NONE) {
			break;
		}
	default:
		if (widget->trigger &&
		    0 < EventTrigger_Trigger(widget->trigger, e->type, pack)) {
			return 0;
		}
		if (!widget->parent || e->cancel_bubble) {
			return -1;
		}
		/* 如果事件投递失败，则向父级部件冒泡 */
		return Widget_TriggerEventEx(widget->parent, pack);
	}
	if (!widget->parent || e->cancel_bubble) {
		return -1;
	}
	while (widget->trigger &&
	       widget->computed_style.pointer_events == SV_NONE) {
		LCUI_Widget w;
		LCUI_BOOL is_pointer_event = TRUE;
		int pointer_x, pointer_y;
		float x, y;

		switch (e->type) {
		case LCUI_WEVENT_CLICK:
		case LCUI_WEVENT_MOUSEDOWN:
		case LCUI_WEVENT_MOUSEUP:
			pointer_x = e->button.x;
			pointer_y = e->button.y;
			break;
		case LCUI_WEVENT_MOUSEMOVE:
		case LCUI_WEVENT_MOUSEOVER:
		case LCUI_WEVENT_MOUSEOUT:
			pointer_x = e->motion.x;
			pointer_y = e->motion.y;
			break;
		default:
			is_pointer_event = FALSE;
			break;
		}
		if (!is_pointer_event) {
			break;
		}
		Widget_GetOffset(widget->parent, NULL, &x, &y);
		/* 转换成相对于父级部件内容框的坐标 */
		x = pointer_x - x;
		y = pointer_y - y;
		/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
		w = Widget_GetNextAt(widget, y_iround(x), y_iround(y));
		if (!w) {
			break;
		}
		return EventTrigger_Trigger(w->trigger, e->type, pack);
	}
	return Widget_TriggerEventEx(widget->parent, pack);
}

static void OnWidgetEvent(LCUI_Event e, LCUI_WidgetEventPack pack)
{
	if (pack->widget) {
		Widget_TriggerEventEx(pack->widget, pack);
	}
}

LCUI_BOOL Widget_PostEvent(LCUI_Widget widget, LCUI_WidgetEvent ev, void *data,
			   void (*destroy_data)(void *))
{
	LCUI_Event sys_ev;
	LCUI_TaskRec task;
	LCUI_WidgetEventPack pack;

	if (widget->state == LCUI_WSTATE_DELETED) {
		return FALSE;
	}
	if (!ev->target) {
		ev->target = widget;
	}
	/* 准备任务 */
	task.func = (LCUI_TaskFunc)OnWidgetEvent;
	task.arg[0] = malloc(sizeof(LCUI_EventRec));
	task.arg[1] = malloc(sizeof(LCUI_WidgetEventPackRec));
	/* 这两个参数都需要在任务执行完后释放 */
	task.destroy_arg[1] = DestroyWidgetEventPack;
	task.destroy_arg[0] = free;
	sys_ev = task.arg[0];
	pack = task.arg[1];
	sys_ev->data = pack;
	sys_ev->type = ev->type;
	pack->data = data;
	pack->widget = widget;
	pack->destroy_data = destroy_data;
	CopyWidgetEvent(&pack->event, ev);
	Widget_AddEventRecord(widget, pack);
	/* 把任务扔给当前跑主循环的线程 */
	if (!LCUI_PostTask(&task)) {
		LCUITask_Destroy(&task);
		return FALSE;
	}
	return TRUE;
}

int Widget_TriggerEvent(LCUI_Widget widget, LCUI_WidgetEvent e, void *data)
{
	LCUI_WidgetEventPackRec pack;

	if (!e->target) {
		e->target = widget;
	}
	pack.event = *e;
	pack.data = data;
	pack.widget = widget;
	pack.destroy_data = NULL;
	return Widget_TriggerEventEx(widget, &pack);
}

int Widget_StopEventPropagation(LCUI_Widget widget)
{
	list_node_t *node;
	WidgetEventRecord record;
	LCUI_WidgetEventPack pack;

	if (self.event_records.total_node <= 1) {
		return 0;
	}
	LCUIMutex_Lock(&self.mutex);
	record = rbtree_get_data_by_keydata(&self.event_records, widget);
	if (!record) {
		LCUIMutex_Unlock(&self.mutex);
		return -1;
	}
	for (list_each(node, &record->records)) {
		pack = node->data;
		pack->event.cancel_bubble = TRUE;
	}
	LCUIMutex_Unlock(&self.mutex);
	return 0;
}

static LCUI_Widget GetSameParent(LCUI_Widget a, LCUI_Widget b)
{
	int depth = 0, i;
	LCUI_Widget w;

	for (w = a; w; w = w->parent) {
		++depth;
	}
	for (w = b; w; w = w->parent) {
		--depth;
	}
	if (depth > 0) {
		for (i = 0; i < depth; ++i) {
			a = a->parent;
		}
	} else {
		for (i = 0; i < -depth; ++i) {
			b = b->parent;
		}
	}
	while (a && b && a != b) {
		a = a->parent;
		b = b->parent;
	}
	if (a && a == b) {
		return a;
	}
	return NULL;
}

static LCUI_Widget Widget_GetEventTarget(LCUI_Widget widget, float x, float y,
					 int inherited_pointer_events)
{
	int pointer_events;

	LCUI_Widget child;
	LCUI_Widget target = NULL;
	list_node_t *node;

	for (list_each(node, &widget->children_show)) {
		child = node->data;
		if (!child->computed_style.visible ||
		    child->state != LCUI_WSTATE_NORMAL ||
		    !LCUIRect_HasPoint(&child->box.border, x, y)) {
			continue;
		}
		pointer_events = child->computed_style.pointer_events;
		if (pointer_events == SV_INHERIT) {
			pointer_events = inherited_pointer_events;
		}
		target = Widget_GetEventTarget(child, x - child->box.padding.x,
					       y - child->box.padding.y,
					       pointer_events);
		if (target) {
			return target;
		}
		if (pointer_events == SV_AUTO) {
			return child;
		}
	}
	return target;
}

static void Widget_TriggerMouseOverEvent(LCUI_Widget widget, LCUI_Widget parent)
{
	LCUI_Widget w;
	LCUI_WidgetEventRec ev = { 0 };

	ev.cancel_bubble = FALSE;
	ev.type = LCUI_WEVENT_MOUSEOVER;
	for (w = widget; w && w != parent; w = w->parent) {
		ev.target = w;
		Widget_AddStatus(w, "hover");
		Widget_TriggerEvent(w, &ev, NULL);
	}
}

static void Widget_TriggerMouseOutEvent(LCUI_Widget widget, LCUI_Widget parent)
{
	LCUI_Widget w;
	LCUI_WidgetEventRec ev = { 0 };

	ev.cancel_bubble = FALSE;
	ev.type = LCUI_WEVENT_MOUSEOUT;
	for (w = widget; w && w != parent; w = w->parent) {
		ev.target = w;
		Widget_RemoveStatus(w, "hover");
		Widget_TriggerEvent(w, &ev, NULL);
	}
}

static void Widget_OnMouseOverEvent(LCUI_Widget widget)
{
	LCUI_Widget parent = NULL;

	if (self.targets[WST_HOVER] == widget) {
		return;
	}
	parent = GetSameParent(widget, self.targets[WST_HOVER]);
	if (widget) {
		Widget_TriggerMouseOverEvent(widget, parent);
	}
	if (self.targets[WST_HOVER]) {
		Widget_TriggerMouseOutEvent(self.targets[WST_HOVER], parent);
	}
	self.targets[WST_HOVER] = widget;
}

static void Widget_OnMouseDownEvent(LCUI_Widget widget)
{
	LCUI_Widget parent;
	LCUI_Widget w = self.targets[WST_ACTIVE];

	if (w == widget) {
		return;
	}
	parent = GetSameParent(widget, w);
	for (; w && w != parent; w = w->parent) {
		Widget_RemoveStatus(w, "active");
	}
	for (w = widget; w && w != parent; w = w->parent) {
		Widget_AddStatus(w, "active");
	}
	self.targets[WST_ACTIVE] = widget;
}

static void ClearMouseOverTarget(LCUI_Widget target)
{
	LCUI_Widget w;

	if (!target) {
		Widget_OnMouseOverEvent(NULL);
		return;
	}
	for (w = self.targets[WST_HOVER]; w; w = w->parent) {
		if (w == target) {
			Widget_OnMouseOverEvent(NULL);
			break;
		}
	}
}

static void ClearMouseDownTarget(LCUI_Widget target)
{
	LCUI_Widget w;

	if (!target) {
		Widget_OnMouseDownEvent(NULL);
		return;
	}
	for (w = self.targets[WST_ACTIVE]; w; w = w->parent) {
		if (w == target) {
			Widget_OnMouseDownEvent(NULL);
			break;
		}
	}
}

static void ClearFocusTarget(LCUI_Widget target)
{
	LCUI_Widget w;

	if (!target) {
		self.targets[WST_FOCUS] = NULL;
		return;
	}
	for (w = self.targets[WST_FOCUS]; w; w = w->parent) {
		if (w == target) {
			self.targets[WST_FOCUS] = NULL;
			break;
		}
	}
}

void LCUIWidget_ClearEventTarget(LCUI_Widget widget)
{
	list_node_t *node;
	WidgetEventRecord record;
	LCUI_WidgetEventPack pack;

	if (self.event_records.total_node <= 1) {
		return;
	}
	LCUIMutex_Lock(&self.mutex);
	record = rbtree_get_data_by_keydata(&self.event_records, widget);
	if (record) {
		for (list_each(node, &record->records)) {
			pack = node->data;
			pack->widget = NULL;
			pack->event.cancel_bubble = TRUE;
		}
	}
	LCUIMutex_Unlock(&self.mutex);
	ClearMouseOverTarget(widget);
	ClearMouseDownTarget(widget);
	ClearFocusTarget(widget);
}

static LCUI_BOOL Widget_Focusable(LCUI_Widget w)
{
	return w && w->computed_style.pointer_events != SV_NONE &&
	       w->computed_style.focusable && !w->disabled;
}

LCUI_Widget LCUIWidget_GetFocus(void)
{
	return self.targets[WST_FOCUS];
}

int LCUIWidget_SetFocus(LCUI_Widget widget)
{
	LCUI_Widget w;
	LCUI_WidgetEventRec ev = { 0 };

	for (w = widget; w; w = w->parent) {
		if (Widget_Focusable(w)) {
			break;
		}
	}
	if (self.targets[WST_FOCUS] == w) {
		return 0;
	}
	if (self.targets[WST_FOCUS]) {
		ev.type = LCUI_WEVENT_BLUR;
		ev.target = self.targets[WST_FOCUS];
		Widget_RemoveStatus(ev.target, "focus");
		Widget_PostEvent(ev.target, &ev, NULL, NULL);
		self.targets[WST_FOCUS] = NULL;
	}
	if (!Widget_Focusable(w)) {
		return -1;
	}
	ev.target = w;
	ev.type = LCUI_WEVENT_FOCUS;
	ev.cancel_bubble = FALSE;
	self.targets[WST_FOCUS] = w;
	Widget_AddStatus(ev.target, "focus");
	Widget_PostEvent(ev.target, &ev, NULL, NULL);
	return 0;
}

/** 响应系统的鼠标移动事件，向目标部件投递相关鼠标事件 */
static void OnMouseEvent(LCUI_SysEvent sys_ev, void *arg)
{
	float scale;
	pd_pos_t pos;
	LCUI_Widget root;
	LCUI_Widget target, w;
	LCUI_WidgetEventRec ev = { 0 };

	root = LCUIWidget_GetRoot();
	LCUICursor_GetPos(&pos);
	scale = LCUIMetrics_GetScale();
	pos.x = y_iround(pos.x / scale);
	pos.y = y_iround(pos.y / scale);
	if (self.mouse_capturer) {
		target = self.mouse_capturer;
	} else {
		target = Widget_GetEventTarget(root, 1.f * pos.x, 1.f * pos.y,
					       SV_AUTO);
	}
	for (w = target; w; w = w->parent) {
		if (w->event_blocked) {
			return;
		}
	}
	if (!target) {
		target = root;
	}
	ev.target = target;
	ev.cancel_bubble = FALSE;
	switch (sys_ev->type) {
	case LCUI_MOUSEDOWN:
		ev.type = LCUI_WEVENT_MOUSEDOWN;
		ev.button.x = pos.x;
		ev.button.y = pos.y;
		ev.button.button = sys_ev->button.button;
		Widget_TriggerEvent(target, &ev, NULL);
		self.click.interval = DBLCLICK_INTERVAL;
		if (ev.button.button == LCUI_KEY_LEFTBUTTON &&
		    self.click.widget == target) {
			int delta;
			delta = (int)get_time_delta(self.click.time);
			self.click.interval = delta;
		} else if (ev.button.button == LCUI_KEY_LEFTBUTTON &&
			   self.click.widget != target) {
			self.click.x = pos.x;
			self.click.y = pos.y;
		}
		self.click.time = get_time_ms();
		self.click.widget = target;
		Widget_OnMouseDownEvent(target);
		LCUIWidget_SetFocus(target);
		break;
	case LCUI_MOUSEUP:
		ev.type = LCUI_WEVENT_MOUSEUP;
		ev.button.x = pos.x;
		ev.button.y = pos.y;
		ev.button.button = sys_ev->button.button;
		Widget_TriggerEvent(target, &ev, NULL);
		if (self.targets[WST_ACTIVE] != target ||
		    ev.button.button != LCUI_KEY_LEFTBUTTON) {
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			Widget_OnMouseDownEvent(NULL);
			break;
		}
		ev.type = LCUI_WEVENT_CLICK;
		Widget_TriggerEvent(target, &ev, NULL);
		Widget_OnMouseDownEvent(NULL);
		if (self.click.widget != target) {
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			break;
		}
		if (self.click.interval < DBLCLICK_INTERVAL) {
			ev.type = LCUI_WEVENT_DBLCLICK;
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			Widget_TriggerEvent(target, &ev, NULL);
		}
		Widget_OnMouseDownEvent(NULL);
		break;
	case LCUI_MOUSEMOVE:
		ev.type = LCUI_WEVENT_MOUSEMOVE;
		ev.motion.x = pos.x;
		ev.motion.y = pos.y;
		if (abs(self.click.x - pos.x) >= 8 ||
		    abs(self.click.y - pos.y) >= 8) {
			self.click.time = 0;
			self.click.widget = NULL;
		}
		Widget_TriggerEvent(target, &ev, NULL);
		break;
	case LCUI_MOUSEWHEEL:
		ev.type = LCUI_WEVENT_MOUSEWHEEL;
		ev.wheel.x = pos.x;
		ev.wheel.y = pos.y;
		ev.wheel.delta = sys_ev->wheel.delta;
		Widget_TriggerEvent(target, &ev, NULL);
	default:
		return;
	}
	Widget_OnMouseOverEvent(target);
}

static void OnKeyboardEvent(LCUI_SysEvent e, void *arg)
{
	LCUI_WidgetEventRec ev = { 0 };
	if (!self.targets[WST_FOCUS]) {
		return;
	}
	switch (e->type) {
	case LCUI_KEYDOWN:
		ev.type = LCUI_WEVENT_KEYDOWN;
		break;
	case LCUI_KEYUP:
		ev.type = LCUI_WEVENT_KEYUP;
		break;
	case LCUI_KEYPRESS:
		ev.type = LCUI_WEVENT_KEYPRESS;
		break;
	default:
		return;
	}
	ev.target = self.targets[WST_FOCUS];
	ev.key.code = e->key.code;
	ev.cancel_bubble = FALSE;
	Widget_TriggerEvent(ev.target, &ev, NULL);
}

/** 响应输入法的输入 */
static void OnTextInput(LCUI_SysEvent e, void *arg)
{
	LCUI_WidgetEventRec ev = { 0 };
	LCUI_Widget target = self.targets[WST_FOCUS];
	if (!target) {
		return;
	}
	ev.target = target;
	ev.type = LCUI_WEVENT_TEXTINPUT;
	ev.cancel_bubble = FALSE;
	ev.text.length = e->text.length;
	ev.text.text = NEW(wchar_t, e->text.length + 1);
	if (!ev.text.text) {
		return;
	}
	wcsncpy(ev.text.text, e->text.text, e->text.length + 1);
	Widget_TriggerEvent(ev.target, &ev, NULL);
	free(ev.text.text);
	ev.text.text = NULL;
	ev.text.length = 0;
}

static void ConvertTouchPoint(LCUI_TouchPoint point)
{
	float scale;
	switch (point->state) {
	case LCUI_TOUCHDOWN:
		point->state = LCUI_WEVENT_TOUCHDOWN;
		break;
	case LCUI_TOUCHUP:
		point->state = LCUI_WEVENT_TOUCHUP;
		break;
	case LCUI_TOUCHMOVE:
		point->state = LCUI_WEVENT_TOUCHMOVE;
		break;
	default:
		break;
	}
	scale = LCUIMetrics_GetScale();
	point->x = y_iround(point->x / scale);
	point->y = y_iround(point->y / scale);
}

/** 分发触控事件给对应的部件 */
static int DispatchTouchEvent(list_t *capturers, LCUI_TouchPoint points,
			      int n_points)
{
	int i, count;
	float scale;
	LCUI_WidgetEventRec ev = { 0 };
	LCUI_Widget target, root, w;
	list_node_t *node, *ptnode;

	root = LCUIWidget_GetRoot();
	scale = LCUIMetrics_GetScale();
	ev.type = LCUI_WEVENT_TOUCH;
	ev.cancel_bubble = FALSE;
	ev.touch.points = NEW(LCUI_TouchPointRec, n_points);
	/* 先将各个触点按命中的部件进行分组 */
	for (i = 0; i < n_points; ++i) {
		target = Widget_At(root, y_iround(points[i].x / scale),
				   y_iround(points[i].y / scale));
		if (!target) {
			continue;
		}
		for (w = target; w; w = w->parent) {
			if (w->event_blocked) {
				break;
			}
		}
		if (w && w->event_blocked) {
			continue;
		}
		TouchCapturers_Add(capturers, target, points[i].id);
	}
	count = 0;
	ev.touch.n_points = 0;
	/* 然后向命中的部件发送触控事件 */
	for (list_each(node, capturers)) {
		TouchCapturer tc = node->data;
		for (i = 0; i < n_points; ++i) {
			for (list_each(ptnode, &tc->points)) {
				LCUI_TouchPoint point;
				if (points[i].id != *(int *)ptnode->data) {
					continue;
				}
				point = &ev.touch.points[ev.touch.n_points];
				*point = points[i];
				ConvertTouchPoint(point);
				++ev.touch.n_points;
			}
		}
		if (ev.touch.n_points == 0) {
			continue;
		}
		Widget_PostEvent(tc->widget, &ev, NULL, NULL);
		ev.touch.n_points = 0;
		++count;
	}
	free(ev.touch.points);
	return count;
}

/** 响应系统触控事件 */
static void OnTouch(LCUI_SysEvent sys_ev, void *arg)
{
	int i, n;
	list_t capturers;
	LCUI_TouchPoint points;
	list_node_t *node, *ptnode;

	n = sys_ev->touch.n_points;
	points = sys_ev->touch.points;
	list_create(&capturers);
	LCUIMutex_Lock(&self.mutex);
	/* 合并现有的触点捕捉记录 */
	for (list_each(node, &self.touch_capturers)) {
		TouchCapturer tc = node->data;
		for (i = 0; i < n; ++i) {
			/* 如果没有触点记录，则说明是捕获全部触点 */
			if (tc->points.length == 0) {
				TouchCapturers_Add(&capturers, tc->widget,
						   points[i].id);
				continue;
			}
			for (list_each(ptnode, &tc->points)) {
				if (points[i].id != *(int *)ptnode->data) {
					continue;
				}
				TouchCapturers_Add(&capturers, tc->widget,
						   points[i].id);
			}
		}
	}
	DispatchTouchEvent(&capturers, points, n);
	TouchCapturers_Clear(&capturers);
	LCUIMutex_Unlock(&self.mutex);
}

void Widget_SetMouseCapture(LCUI_Widget w)
{
	self.mouse_capturer = w;
}

void Widget_ReleaseMouseCapture(LCUI_Widget w)
{
	self.mouse_capturer = NULL;
}

int Widget_SetTouchCapture(LCUI_Widget w, int point_id)
{
	int ret;
	LCUIMutex_Lock(&self.mutex);
	ret = TouchCapturers_Add(&self.touch_capturers, w, point_id);
	LCUIMutex_Unlock(&self.mutex);
	return ret;
}

int Widget_ReleaseTouchCapture(LCUI_Widget w, int point_id)
{
	int ret;

	if (self.touch_capturers.length <= 1) {
		return 0;
	}
	LCUIMutex_Lock(&self.mutex);
	ret = TouchCapturers_Delete(&self.touch_capturers, w, point_id);
	LCUIMutex_Unlock(&self.mutex);
	return ret;
}

int Widget_PostSurfaceEvent(LCUI_Widget w, int event_type, LCUI_BOOL sync_props)
{
	int *data;
	LCUI_WidgetEventRec e = { 0 };
	LCUI_Widget root = LCUIWidget_GetRoot();

	if (w->parent != root && w != root) {
		return -1;
	}
	e.target = w;
	e.type = LCUI_WEVENT_SURFACE;
	e.cancel_bubble = TRUE;
	data = malloc(sizeof(int) * 2);
	if (!data) {
		return -ENOMEM;
	}
	data[0] = event_type;
	data[1] = sync_props;
	return Widget_PostEvent(root, &e, data, free);
}

void Widget_DestroyEventTrigger(LCUI_Widget w)
{
	LCUI_WidgetEventRec e = { LCUI_WEVENT_DESTROY, 0 };

	Widget_TriggerEvent(w, &e, NULL);
	Widget_ReleaseMouseCapture(w);
	Widget_ReleaseTouchCapture(w, -1);
	Widget_StopEventPropagation(w);
	LCUIWidget_ClearEventTarget(w);
	if (w->trigger) {
		EventTrigger_Destroy(w->trigger);
		w->trigger = NULL;
	}
}

static void BindSysEvent(int e, LCUI_SysEventFunc func)
{
	int *id = malloc(sizeof(int));
	*id = LCUI_BindEvent(e, func, NULL, NULL);
	list_append(&self.events, id);
}

void LCUIWidget_InitEvent(void)
{
	int i, n;
	struct EventNameMapping {
		int id;
		const char *name;
	} mappings[] = { { LCUI_WEVENT_LINK, "link" },
			 { LCUI_WEVENT_UNLINK, "unlink" },
			 { LCUI_WEVENT_READY, "ready" },
			 { LCUI_WEVENT_DESTROY, "destroy" },
			 { LCUI_WEVENT_MOUSEDOWN, "mousedown" },
			 { LCUI_WEVENT_MOUSEUP, "mouseup" },
			 { LCUI_WEVENT_MOUSEMOVE, "mousemove" },
			 { LCUI_WEVENT_MOUSEWHEEL, "mousewheel" },
			 { LCUI_WEVENT_CLICK, "click" },
			 { LCUI_WEVENT_DBLCLICK, "dblclick" },
			 { LCUI_WEVENT_MOUSEOUT, "mouseout" },
			 { LCUI_WEVENT_MOUSEOVER, "mouseover" },
			 { LCUI_WEVENT_KEYDOWN, "keydown" },
			 { LCUI_WEVENT_KEYUP, "keyup" },
			 { LCUI_WEVENT_KEYPRESS, "keypress" },
			 { LCUI_WEVENT_TOUCH, "touch" },
			 { LCUI_WEVENT_TEXTINPUT, "textinput" },
			 { LCUI_WEVENT_TOUCHDOWN, "touchdown" },
			 { LCUI_WEVENT_TOUCHMOVE, "touchmove" },
			 { LCUI_WEVENT_TOUCHUP, "touchup" },
			 { LCUI_WEVENT_RESIZE, "resize" },
			 { LCUI_WEVENT_AFTERLAYOUT, "afterlayout" },
			 { LCUI_WEVENT_FOCUS, "focus" },
			 { LCUI_WEVENT_BLUR, "blur" },
			 { LCUI_WEVENT_SHOW, "show" },
			 { LCUI_WEVENT_HIDE, "hide" },
			 { LCUI_WEVENT_SURFACE, "surface" },
			 { LCUI_WEVENT_TITLE, "title" } };

	LCUIMutex_Init(&self.mutex);
	rbtree_init(&self.event_names);
	rbtree_init(&self.event_records);
	list_create(&self.events);
	list_create(&self.event_mappings);
	self.targets[WST_ACTIVE] = NULL;
	self.targets[WST_HOVER] = NULL;
	self.targets[WST_FOCUS] = NULL;
	self.mouse_capturer = NULL;
	self.click.x = 0;
	self.click.y = 0;
	self.click.time = 0;
	self.click.widget = NULL;
	self.click.interval = DBLCLICK_INTERVAL;
	self.base_event_id = LCUI_WEVENT_USER + 1000;
	dict_init_string_key_type(&self.event_ids_type);
	self.event_ids = dict_create(&self.event_ids_type, NULL);
	n = sizeof(mappings) / sizeof(mappings[0]);
	for (i = 0; i < n; ++i) {
		LCUIWidget_SetEventName(mappings[i].id, mappings[i].name);
	}
	BindSysEvent(LCUI_MOUSEWHEEL, OnMouseEvent);
	BindSysEvent(LCUI_MOUSEDOWN, OnMouseEvent);
	BindSysEvent(LCUI_MOUSEMOVE, OnMouseEvent);
	BindSysEvent(LCUI_MOUSEUP, OnMouseEvent);
	BindSysEvent(LCUI_KEYPRESS, OnKeyboardEvent);
	BindSysEvent(LCUI_KEYDOWN, OnKeyboardEvent);
	BindSysEvent(LCUI_KEYUP, OnKeyboardEvent);
	BindSysEvent(LCUI_TOUCH, OnTouch);
	BindSysEvent(LCUI_TEXTINPUT, OnTextInput);
	rbtree_set_compare_func(&self.event_records, CompareWidgetEventRecord);
	rbtree_set_destroy_func(&self.event_records, DestoryWidgetEventRecord);
	list_create(&self.touch_capturers);
}

void LCUIWidget_FreeEvent(void)
{
	list_node_t *node;
	LCUIMutex_Lock(&self.mutex);
	for (list_each(node, &self.events)) {
		int *id = node->data;
		LCUI_UnbindEvent(*id);
	}
	rbtree_destroy(&self.event_names);
	rbtree_destroy(&self.event_records);
	dict_destroy(self.event_ids);
	TouchCapturers_Clear(&self.touch_capturers);
	list_destroy(&self.events, free);
	list_destroy(&self.event_mappings, DestroyEventMapping);
	LCUIMutex_Unlock(&self.mutex);
	LCUIMutex_Destroy(&self.mutex);
	self.event_ids = NULL;
}
