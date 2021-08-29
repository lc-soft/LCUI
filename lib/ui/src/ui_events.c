#include <LCUI.h>
#include <LCUI/thread.h>
#include "../include/ui.h"
#include "private.h"

/* clang-format off */

#define DBLCLICK_INTERVAL 500

typedef struct ui_touch_capturer_t {
	LinkedList points;
	ui_widget_t* widget;
	LinkedListNode node;
} ui_touch_capturer_t;

typedef struct ui_event_listener_t {
	ui_event_handler_t func;
	void *data;
	void(*destroy_data)(void *);
} ui_event_listener_t;

typedef struct ui_event_pack_t {
	void *data;
	void(*destroy_data)(void *);
	ui_widget_t* widget;
	ui_event_t event;
} ui_event_pack_t;

enum ui_widget_status_type_t {
	UI_WIDGET_STATUS_HOVER, UI_WIDGET_STATUS_ACTIVE, UI_WIDGET_STATUS_FOCUS, UI_WIDGET_STATUS_TOTAL
};

typedef struct ui_event_record_t {
	ui_widget_t* widget;

	/**< LinkedList<ui_event_pack_t> */
	LinkedList records;
} ui_event_record_t;

typedef struct ui_event_mapping_t {
	int id;
	char *name;
} ui_event_mapping_t;

/** 鼠标点击记录 */
typedef struct ui_widget_click_record_t {
	int64_t time;       /**< 时间 */
	int x, y;           /**< 坐标 */
	int interval;       /**< 与上次点击时的时间间隔 */
	ui_widget_t* widget; /**< 被点击的部件 */
} ui_widget_click_record_t;

/** 当前功能模块的相关数据 */
static struct ui_events_t {
	ui_widget_t* mouse_capturer;     /**< 占用鼠标的部件 */
	LinkedList touch_capturers;     /**< 触点占用记录 */
	ui_widget_t* targets[UI_WIDGET_STATUS_TOTAL]; /**< 相关的部件 */
	LinkedList events;              /**< 已绑定的事件 */
	LinkedList event_mappings;	/**< 事件标识号和名称映射记录列表  */
	RBTree event_records;		/**< 当前正执行的事件的记录 */

	/** RBTree<int, event_mapping_t> */
	RBTree event_names;

	/** Dict<string, event_mapping_t> */
	Dict *event_ids;
	DictType event_ids_type;
	int base_event_id;
	ui_widget_click_record_t click;
	LCUI_Mutex mutex;
} ui_events;

/* clang-format on */

static void ui_event_mapping_destroy(void* data)
{
	ui_event_mapping_t* mapping = data;
	free(mapping->name);
	free(mapping);
}

static int ui_use_widget_event_id(const char* event_name)
{
	int id = ui_get_widget_event_id(event_name);
	if (id < 0) {
		id = ui_alloc_widget_event_id();
		ui_set_widget_event_id(id, event_name);
	}
	return id;
}

void ui_event_init(ui_event_t* e, const char* name)
{
	e->target = NULL;
	e->type = ui_use_widget_event_id(name);
	e->cancel_bubble = FALSE;
	e->data = NULL;
}

void ui_event_destroy(ui_event_t* e)
{
	switch (e->type) {
	case UI_EVENT_TOUCH:
		if (e->touch.points) {
			free(e->touch.points);
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case UI_EVENT_TEXTINPUT:
		if (e->text.text) {
			free(e->text.text);
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	}
}

static void ui_event_pack_destroy(void* data)
{
	ui_event_pack_t* pack = data;
	if (pack->data && pack->destroy_data) {
		pack->destroy_data(pack->data);
	}
	ui_event_destroy(&pack->event);
	pack->data = NULL;
	free(pack);
}

static void ui_event_record_destroy(void* data)
{
	ui_event_record_t* record = data;
	LinkedList_Clear(&record->records, ui_event_pack_destroy);
	free(record);
}

static int ui_event_record_compare(void* data, const void* keydata)
{
	ui_event_record_t* record = data;
	if (record->widget == (ui_widget_t*)keydata) {
		return 0;
	} else if (record->widget < (ui_widget_t*)keydata) {
		return -1;
	} else {
		return 1;
	}
}

static void ui_event_listener_destroy(void* arg)
{
	ui_event_listener_t* handler = arg;
	if (handler->data && handler->destroy_data) {
		handler->destroy_data(handler->data);
	}
	handler->data = NULL;
	free(handler);
}

/**
 * 添加事件记录
 * 记录当前待处理的事件和目标部件，方便在部件被销毁时清除待处理的事件
 */
static void Widget_AddEventRecord(ui_widget_t* widget, ui_event_pack_t* pack)
{
	ui_event_record_t* record;

	LCUIMutex_Lock(&ui_events.mutex);
	record = RBTree_CustomGetData(&ui_events.event_records, widget);
	if (!record) {
		record = malloc(sizeof(ui_event_record_t));
		if (!record) {
			return;
		}
		LinkedList_Init(&record->records);
		record->widget = widget;
		RBTree_CustomInsert(&ui_events.event_records, widget, record);
	}
	LinkedList_Append(&record->records, pack);
	LCUIMutex_Unlock(&ui_events.mutex);
}

/** 删除事件记录 */
static int ui_widget_destroy_event_record(ui_widget_t* widget,
					  ui_event_pack_t* pack)
{
	int ret = 0;
	ui_event_record_t* record;
	LinkedListNode *node, *prev;

	LCUIMutex_Lock(&ui_events.mutex);
	record = RBTree_CustomGetData(&ui_events.event_records, widget);
	if (!record) {
		LCUIMutex_Unlock(&ui_events.mutex);
		return -1;
	}
	for (LinkedList_Each(node, &record->records)) {
		prev = node->prev;
		if (node->data == pack) {
			LinkedList_DeleteNode(&record->records, node);
			node = prev;
			ret = 1;
		}
	}
	LCUIMutex_Unlock(&ui_events.mutex);
	return ret;
}

/** 将原始事件转换成部件事件 */
static void WidgetEventTranslator(LCUI_Event e, ui_event_pack_t* pack)
{
	ui_event_listener_t* handler = e->data;
	ui_widget_t* w = pack->widget;

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
static int ui_copy_widget_event(ui_event_t* dst, const ui_event_t* src)
{
	int n;
	size_t size;

	*dst = *src;
	switch (src->type) {
	case UI_EVENT_TOUCH:
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
	case UI_EVENT_TEXTINPUT:
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

static void ui_event_pack_delete(void* arg)
{
	ui_event_pack_t* pack = arg;
	/* 如果删除成功则说明有记录，需要销毁数据 */
	if (ui_widget_destroy_event_record(pack->event.target, pack) == 1) {
		ui_event_pack_destroy(pack);
	}
}

static void ui_touch_capturer_destroy(void* arg)
{
	ui_touch_capturer_t* tc = arg;
	LinkedList_Clear(&tc->points, free);
	tc->widget = NULL;
	free(tc);
}

INLINE ui_clear_touch_capturers(LinkedList* list)
{
	LinkedList_ClearData(list, ui_touch_capturer_destroy);
}

static int ui_add_touch_capturer(LinkedList* list, ui_widget_t* w, int point_id)
{
	int* data;
	ui_touch_capturer_t* tc = NULL;
	LinkedListNode *node, *ptnode;

	if (point_id < 0) {
		tc = malloc(sizeof(ui_touch_capturer_t));
		if (!tc) {
			return -1;
		}
		tc->widget = w;
		LinkedList_Init(&tc->points);
		ui_clear_touch_capturers(list);
		LinkedList_Append(list, tc);
		return 0;
	}
	/* 获取该部件的触点捕捉记录 */
	for (LinkedList_Each(node, list)) {
		tc = node->data;
		/* 清除与该触点绑定的其它捕捉记录 */
		for (LinkedList_Each(ptnode, &tc->points)) {
			if (point_id == *(int*)ptnode->data) {
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
		tc = NEW(ui_touch_capturer_t, 1);
		tc->widget = w;
		tc->node.data = tc;
		LinkedList_Init(&tc->points);
		LinkedList_AppendNode(list, &tc->node);
	}
	/* 追加触点捕捉记录 */
	data = NEW(int, 1);
	*data = point_id;
	LinkedList_Append(&tc->points, data);
	return 0;
}

static int ui_remove_touch_capturer(LinkedList* list, ui_widget_t* w,
				    int point_id)
{
	ui_touch_capturer_t* tc = NULL;
	LinkedListNode *node, *ptnode;
	for (LinkedList_Each(node, list)) {
		tc = node->data;
		if (tc->widget == w) {
			break;
		}
	}
	if (!tc || tc->widget != w) {
		return -1;
	}
	if (point_id < 0) {
		LinkedList_Clear(&tc->points, free);
	} else {
		for (LinkedList_Each(ptnode, &tc->points)) {
			if (*(int*)ptnode->data == point_id) {
				free(node->data);
				LinkedList_DeleteNode(&tc->points, ptnode);
			}
		}
	}
	if (tc->points.length == 0) {
		LinkedList_Unlink(&ui_events.touch_capturers, &tc->node);
		free(tc);
	}
	return 0;
}

int ui_set_widget_event_id(int event_id, const char* event_name)
{
	int ret;
	ui_event_mapping_t* mapping;
	LCUIMutex_Lock(&ui_events.mutex);
	if (Dict_FetchValue(ui_events.event_ids, event_name)) {
		LCUIMutex_Unlock(&ui_events.mutex);
		return -1;
	}
	mapping = malloc(sizeof(ui_event_mapping_t));
	if (!mapping) {
		return -2;
	}
	mapping->name = strdup2(event_name);
	mapping->id = event_id;
	LinkedList_Append(&ui_events.event_mappings, mapping);
	RBTree_Insert(&ui_events.event_names, event_id, mapping);
	ret = Dict_Add(ui_events.event_ids, mapping->name, mapping);
	LCUIMutex_Unlock(&ui_events.mutex);
	return ret;
}

int ui_alloc_widget_event_id(void)
{
	return ui_events.base_event_id++;
}

const char* ui_get_widget_event_name(int event_id)
{
	ui_event_mapping_t* mapping;
	LCUIMutex_Lock(&ui_events.mutex);
	mapping = RBTree_GetData(&ui_events.event_names, event_id);
	LCUIMutex_Unlock(&ui_events.mutex);
	return mapping ? mapping->name : NULL;
}

int ui_get_widget_event_id(const char* event_name)
{
	ui_event_mapping_t* mapping;
	LCUIMutex_Lock(&ui_events.mutex);
	mapping = Dict_FetchValue(ui_events.event_ids, event_name);
	LCUIMutex_Unlock(&ui_events.mutex);
	return mapping ? mapping->id : -1;
}

int ui_widget_add_event_listener(ui_widget_t* widget, int event_id,
				 ui_event_handler_t func, void* data,
				 void (*destroy_data)(void*))
{
	ui_event_listener_t* listener;
	listener = malloc(sizeof(ui_event_listener_t));
	if (!listener) {
		return -1;
	}
	listener->func = func;
	listener->data = data;
	listener->destroy_data = destroy_data;
	if (!widget->trigger) {
		widget->trigger = EventTrigger();
	}
	return EventTrigger_Bind(widget->trigger, event_id,
				 (LCUI_EventFunc)WidgetEventTranslator,
				 listener, ui_event_listener_destroy);
}

int ui_widget_on(ui_widget_t* widget, const char* event_name,
		 ui_event_handler_t handler, void* data,
		 void (*destroy_data)(void*))
{
	return ui_widget_add_event_listener(widget,
					    ui_use_widget_event_id(event_name),
					    handler, data, destroy_data);
}

int ui_widget_remove_event_listener(ui_widget_t* w, int event_id,
				    ui_event_handler_t handler)
{
	return EventTrigger_Unbind(w->trigger, event_id, handler);
}

int ui_widget_off(ui_widget_t* w, const char* event_name,
		  ui_event_handler_t handler)
{
	return EventTrigger_Unbind(w->trigger,
				   ui_use_widget_event_id(event_name), handler);
}

static ui_widget_t* ui_widget_get_next_at(ui_widget_t* widget, int x, int y)
{
	ui_widget_t* w;
	LinkedListNode* node;

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

static int ui_widget_trigger_event_ex(ui_widget_t* widget,
				      ui_event_pack_t* pack)
{
	ui_event_t* e = &pack->event;

	pack->widget = widget;
	switch (e->type) {
	case UI_EVENT_CLICK:
	case UI_EVENT_MOUSEDOWN:
	case UI_EVENT_MOUSEUP:
	case UI_EVENT_MOUSEMOVE:
	case UI_EVENT_MOUSEOVER:
	case UI_EVENT_MOUSEOUT:
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
		return ui_widget_trigger_event_ex(widget->parent, pack);
	}
	if (!widget->parent || e->cancel_bubble) {
		return -1;
	}
	while (widget->trigger &&
	       widget->computed_style.pointer_events == SV_NONE) {
		ui_widget_t* w;
		LCUI_BOOL is_pointer_event = TRUE;
		int pointer_x, pointer_y;
		float x, y;

		switch (e->type) {
		case UI_EVENT_CLICK:
		case UI_EVENT_MOUSEDOWN:
		case UI_EVENT_MOUSEUP:
			pointer_x = e->button.x;
			pointer_y = e->button.y;
			break;
		case UI_EVENT_MOUSEMOVE:
		case UI_EVENT_MOUSEOVER:
		case UI_EVENT_MOUSEOUT:
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
		ui_widget_get_offset(widget->parent, NULL, &x, &y);
		/* 转换成相对于父级部件内容框的坐标 */
		x = pointer_x - x;
		y = pointer_y - y;
		/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
		w = ui_widget_get_next_at(widget, iround(x), iround(y));
		if (!w) {
			break;
		}
		return EventTrigger_Trigger(w->trigger, e->type, pack);
	}
	return ui_widget_trigger_event_ex(widget->parent, pack);
}

static void ui_widget_on_event(LCUI_Event e, ui_event_pack_t* pack)
{
	if (pack->widget) {
		ui_widget_trigger_event_ex(pack->widget, pack);
	}
}

LCUI_BOOL ui_widget_post_event(ui_widget_t* widget, ui_event_t* e, void* data,
			       void (*destroy_data)(void*))
{
	LCUI_Event e;
	LCUI_TaskRec task;
	ui_event_pack_t* pack;

	if (widget->state == LCUI_WSTATE_DELETED) {
		return FALSE;
	}
	if (!e->target) {
		e->target = widget;
	}
	/* 准备任务 */
	task.func = (LCUI_TaskFunc)ui_widget_on_event;
	task.arg[0] = malloc(sizeof(LCUI_EventRec));
	task.arg[1] = malloc(sizeof(ui_event_pack_t));
	/* 这两个参数都需要在任务执行完后释放 */
	task.destroy_arg[1] = ui_event_pack_delete;
	task.destroy_arg[0] = free;
	e = task.arg[0];
	pack = task.arg[1];
	e->data = pack;
	e->type = e->type;
	pack->data = data;
	pack->widget = widget;
	pack->destroy_data = destroy_data;
	ui_copy_widget_event(&pack->event, e);
	Widget_AddEventRecord(widget, pack);
	/* 把任务扔给当前跑主循环的线程 */
	if (!LCUI_PostTask(&task)) {
		LCUITask_Destroy(&task);
		return FALSE;
	}
	return TRUE;
}

int ui_widget_trigger_event(ui_widget_t* widget, ui_event_t* e, void* data)
{
	ui_event_pack_t pack;

	if (!e->target) {
		e->target = widget;
	}
	pack.event = *e;
	pack.data = data;
	pack.widget = widget;
	pack.destroy_data = NULL;
	return ui_widget_trigger_event_ex(widget, &pack);
}

int ui_widget_stop_event_propagation(ui_widget_t* widget)
{
	LinkedListNode* node;
	ui_event_record_t* record;
	ui_event_pack_t* pack;

	if (ui_events.event_records.total_node <= 1) {
		return 0;
	}
	LCUIMutex_Lock(&ui_events.mutex);
	record = RBTree_CustomGetData(&ui_events.event_records, widget);
	if (!record) {
		LCUIMutex_Unlock(&ui_events.mutex);
		return -1;
	}
	for (LinkedList_Each(node, &record->records)) {
		pack = node->data;
		pack->event.cancel_bubble = TRUE;
	}
	LCUIMutex_Unlock(&ui_events.mutex);
	return 0;
}

static ui_widget_t* ui_get_same_parent(ui_widget_t* a, ui_widget_t* b)
{
	int depth = 0, i;
	ui_widget_t* w;

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

static ui_widget_t* ui_widget_get_event_target(ui_widget_t* widget, float x,
					       float y,
					       int inherited_pointer_events)
{
	int pointer_events;

	ui_widget_t* child;
	ui_widget_t* target = NULL;
	LinkedListNode* node;

	for (LinkedList_Each(node, &widget->children_show)) {
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
		target = ui_widget_get_event_target(
		    child, x - child->box.padding.x, y - child->box.padding.y,
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

static void ui_widget_trigger_mouseover_event(ui_widget_t* widget,
					      ui_widget_t* parent)
{
	ui_widget_t* w;
	ui_event_t e = { 0 };

	e.cancel_bubble = FALSE;
	e.type = UI_EVENT_MOUSEOVER;
	for (w = widget; w && w != parent; w = w->parent) {
		e.target = w;
		Widget_AddStatus(w, "hover");
		ui_widget_trigger_event(w, &e, NULL);
	}
}

static void ui_widget_trigger_mouseout_event(ui_widget_t* widget,
					     ui_widget_t* parent)
{
	ui_widget_t* w;
	ui_event_t e = { 0 };

	e.cancel_bubble = FALSE;
	e.type = UI_EVENT_MOUSEOUT;
	for (w = widget; w && w != parent; w = w->parent) {
		e.target = w;
		Widget_RemoveStatus(w, "hover");
		ui_widget_trigger_event(w, &e, NULL);
	}
}

static void ui_widget_on_mouseover_event(ui_widget_t* widget)
{
	ui_widget_t* parent = NULL;

	if (ui_events.targets[UI_WIDGET_STATUS_HOVER] == widget) {
		return;
	}
	parent = ui_get_same_parent(widget,
				    ui_events.targets[UI_WIDGET_STATUS_HOVER]);
	if (widget) {
		ui_widget_trigger_mouseover_event(widget, parent);
	}
	if (ui_events.targets[UI_WIDGET_STATUS_HOVER]) {
		ui_widget_trigger_mouseout_event(
		    ui_events.targets[UI_WIDGET_STATUS_HOVER], parent);
	}
	ui_events.targets[UI_WIDGET_STATUS_HOVER] = widget;
}

static void ui_clear_mousedown_target(ui_widget_t* widget)
{
	ui_widget_t* parent;
	ui_widget_t* w = ui_events.targets[UI_WIDGET_STATUS_ACTIVE];

	if (w == widget) {
		return;
	}
	parent = ui_get_same_parent(widget, w);
	for (; w && w != parent; w = w->parent) {
		Widget_RemoveStatus(w, "active");
	}
	for (w = widget; w && w != parent; w = w->parent) {
		Widget_AddStatus(w, "active");
	}
	ui_events.targets[UI_WIDGET_STATUS_ACTIVE] = widget;
}

static void ui_clear_mouseover_target(ui_widget_t* target)
{
	ui_widget_t* w;

	if (!target) {
		ui_widget_on_mouseover_event(NULL);
		return;
	}
	for (w = ui_events.targets[UI_WIDGET_STATUS_HOVER]; w; w = w->parent) {
		if (w == target) {
			ui_widget_on_mouseover_event(NULL);
			break;
		}
	}
}

static void ui_clear_mousedown_target(ui_widget_t* target)
{
	ui_widget_t* w;

	if (!target) {
		ui_clear_mousedown_target(NULL);
		return;
	}
	for (w = ui_events.targets[UI_WIDGET_STATUS_ACTIVE]; w; w = w->parent) {
		if (w == target) {
			ui_clear_mousedown_target(NULL);
			break;
		}
	}
}

static void ui_clear_focus_target(ui_widget_t* target)
{
	ui_widget_t* w;

	if (!target) {
		ui_events.targets[UI_WIDGET_STATUS_FOCUS] = NULL;
		return;
	}
	for (w = ui_events.targets[UI_WIDGET_STATUS_FOCUS]; w; w = w->parent) {
		if (w == target) {
			ui_events.targets[UI_WIDGET_STATUS_FOCUS] = NULL;
			break;
		}
	}
}

void ui_clear_event_target(ui_widget_t* widget)
{
	LinkedListNode* node;
	ui_event_record_t* record;
	ui_event_pack_t* pack;

	if (ui_events.event_records.total_node <= 1) {
		return;
	}
	LCUIMutex_Lock(&ui_events.mutex);
	record = RBTree_CustomGetData(&ui_events.event_records, widget);
	if (record) {
		for (LinkedList_Each(node, &record->records)) {
			pack = node->data;
			pack->widget = NULL;
			pack->event.cancel_bubble = TRUE;
		}
	}
	LCUIMutex_Unlock(&ui_events.mutex);
	ui_clear_mouseover_target(widget);
	ui_clear_mousedown_target(widget);
	ui_clear_focus_target(widget);
}

INLINE LCUI_BOOL ui_widget_check_focusable(ui_widget_t* w)
{
	return w && w->computed_style.pointer_events != SV_NONE &&
	       w->computed_style.focusable && !w->disabled;
}

ui_widget_t* ui_get_focus(void)
{
	return ui_events.targets[UI_WIDGET_STATUS_FOCUS];
}

int ui_set_focus(ui_widget_t* widget)
{
	ui_widget_t* w;
	ui_event_t e = { 0 };

	for (w = widget; w; w = w->parent) {
		if (ui_widget_check_focusable(w)) {
			break;
		}
	}
	if (ui_events.targets[UI_WIDGET_STATUS_FOCUS] == w) {
		return 0;
	}
	if (ui_events.targets[UI_WIDGET_STATUS_FOCUS]) {
		e.type = UI_EVENT_BLUR;
		e.target = ui_events.targets[UI_WIDGET_STATUS_FOCUS];
		Widget_RemoveStatus(e.target, "focus");
		ui_widget_post_event(e.target, &e, NULL, NULL);
		ui_events.targets[UI_WIDGET_STATUS_FOCUS] = NULL;
	}
	if (!ui_widget_check_focusable(w)) {
		return -1;
	}
	e.target = w;
	e.type = UI_EVENT_FOCUS;
	e.cancel_bubble = FALSE;
	ui_events.targets[UI_WIDGET_STATUS_FOCUS] = w;
	Widget_AddStatus(e.target, "focus");
	ui_widget_post_event(e.target, &e, NULL, NULL);
	return 0;
}

static ui_widget_t* ui_resolve_event_target(float x, float y)
{
	ui_widget_t* w;
	ui_widget_t* root;
	ui_widget_t* target;

	root = ui_root();
	if (ui_events.mouse_capturer) {
		target = ui_events.mouse_capturer;
	} else {
		target = ui_widget_get_event_target(root, x, y, SV_AUTO);
	}
	for (w = target; w; w = w->parent) {
		if (w->event_blocked) {
			return NULL;
		}
	}
	if (!target) {
		target = root;
	}
	return target;
}

static int ui_on_mouse_event(ui_event_t* origin_event)
{
	float scale;
	LCUI_Pos pos;
	ui_widget_t *target, *w;
	ui_event_t e = *origin_event;

	e.cancel_bubble = FALSE;
	switch (e.type) {
	case LCUI_MOUSEDOWN:
		e.target = ui_resolve_event_target(e.button.x, e.button.y);
		e.type = UI_EVENT_MOUSEDOWN;
		e.button.x = pos.x;
		e.button.y = pos.y;
		ui_widget_trigger_event(e.target, &e, NULL);
		ui_events.click.interval = DBLCLICK_INTERVAL;
		if (e.button.button == LCUI_KEY_LEFTBUTTON &&
		    ui_events.click.widget == e.target) {
			int delta;
			delta = (int)LCUI_GetTimeDelta(ui_events.click.time);
			ui_events.click.interval = delta;
		} else if (e.button.button == LCUI_KEY_LEFTBUTTON &&
			   ui_events.click.widget != target) {
			ui_events.click.x = pos.x;
			ui_events.click.y = pos.y;
		}
		ui_events.click.time = LCUI_GetTime();
		ui_events.click.widget = target;
		ui_clear_mousedown_target(target);
		ui_set_focus(target);
		break;
	case LCUI_MOUSEUP:
		e.type = UI_EVENT_MOUSEUP;
		e.button.x = pos.x;
		e.button.y = pos.y;
		e.button.button = e->button.button;
		ui_widget_trigger_event(target, &e, NULL);
		if (ui_events.targets[UI_WIDGET_STATUS_ACTIVE] != target ||
		    e.button.button != LCUI_KEY_LEFTBUTTON) {
			ui_events.click.x = 0;
			ui_events.click.y = 0;
			ui_events.click.time = 0;
			ui_events.click.widget = NULL;
			ui_clear_mousedown_target(NULL);
			break;
		}
		e.type = UI_EVENT_CLICK;
		ui_widget_trigger_event(target, &e, NULL);
		ui_clear_mousedown_target(NULL);
		if (ui_events.click.widget != target) {
			ui_events.click.x = 0;
			ui_events.click.y = 0;
			ui_events.click.time = 0;
			ui_events.click.widget = NULL;
			break;
		}
		if (ui_events.click.interval < DBLCLICK_INTERVAL) {
			e.type = UI_EVENT_DBLCLICK;
			ui_events.click.x = 0;
			ui_events.click.y = 0;
			ui_events.click.time = 0;
			ui_events.click.widget = NULL;
			ui_widget_trigger_event(target, &e, NULL);
		}
		ui_clear_mousedown_target(NULL);
		break;
	case LCUI_MOUSEMOVE:
		if (abs(ui_events.click.x - pos.x) >= 8 ||
		    abs(ui_events.click.y - pos.y) >= 8) {
			ui_events.click.time = 0;
			ui_events.click.widget = NULL;
		}
		ui_widget_trigger_event(target, &e, NULL);
		break;
	case LCUI_MOUSEWHEEL:
		ui_widget_trigger_event(target, &e, NULL);
	default:
		return;
	}
	ui_widget_on_mouseover_event(target);
}

static int ui_on_keyboard_event(ui_event_t* origin_event)
{
	ui_event_t e = *origin_event;

	e.target = ui_events.targets[UI_WIDGET_STATUS_FOCUS];
	if (!e.target) {
		return -1;
	}
	e.cancel_bubble = FALSE;
	ui_widget_trigger_event(e.target, &e, NULL);
	return 0;
}

static int ui_on_text_input(ui_event_t* origin_event)
{
	ui_event_t e = *origin_event;

	e.target = ui_events.targets[UI_WIDGET_STATUS_FOCUS];
	if (!e.target) {
		return -1;
	}
	e.cancel_bubble = FALSE;
	ui_widget_trigger_event(e.target, &e, NULL);
	return 0;
}

static void ui_convert_touch_point(LCUI_TouchPoint point)
{
	float scale;
	switch (point->state) {
	case LCUI_TOUCHDOWN:
		point->state = UI_EVENT_TOUCHDOWN;
		break;
	case LCUI_TOUCHUP:
		point->state = UI_EVENT_TOUCHUP;
		break;
	case LCUI_TOUCHMOVE:
		point->state = UI_EVENT_TOUCHMOVE;
		break;
	default:
		break;
	}
	scale = LCUIMetrics_GetScale();
	point->x = iround(point->x / scale);
	point->y = iround(point->y / scale);
}

/** 分发触控事件给对应的部件 */
static int ui_dispatch_touch_event(LinkedList* capturers,
				   LCUI_TouchPoint points, int n_points)
{
	int i, count;
	float scale;
	ui_event_t e = { 0 };
	ui_widget_t *target, *root, *w;
	LinkedListNode *node, *ptnode;

	root = ui_root();
	scale = LCUIMetrics_GetScale();
	e.type = UI_EVENT_TOUCH;
	e.cancel_bubble = FALSE;
	e.touch.points = NEW(LCUI_TouchPointRec, n_points);
	/* 先将各个触点按命中的部件进行分组 */
	for (i = 0; i < n_points; ++i) {
		target = ui_widget_at(root, iround(points[i].x / scale),
				      iround(points[i].y / scale));
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
		ui_add_touch_capturer(capturers, target, points[i].id);
	}
	count = 0;
	e.touch.n_points = 0;
	/* 然后向命中的部件发送触控事件 */
	for (LinkedList_Each(node, capturers)) {
		ui_touch_capturer_t* tc = node->data;
		for (i = 0; i < n_points; ++i) {
			for (LinkedList_Each(ptnode, &tc->points)) {
				LCUI_TouchPoint point;
				if (points[i].id != *(int*)ptnode->data) {
					continue;
				}
				point = &e.touch.points[e.touch.n_points];
				*point = points[i];
				ui_convert_touch_point(point);
				++e.touch.n_points;
			}
		}
		if (e.touch.n_points == 0) {
			continue;
		}
		ui_widget_post_event(tc->widget, &e, NULL, NULL);
		e.touch.n_points = 0;
		++count;
	}
	free(e.touch.points);
	return count;
}

static int ui_on_touch_event(ui_event_t* e)
{
	int i, n;
	LinkedList capturers;
	LCUI_TouchPoint points;
	LinkedListNode *node, *ptnode;

	n = e->touch.n_points;
	points = e->touch.points;
	LinkedList_Init(&capturers);
	LCUIMutex_Lock(&ui_events.mutex);
	/* 合并现有的触点捕捉记录 */
	for (LinkedList_Each(node, &ui_events.touch_capturers)) {
		ui_touch_capturer_t* tc = node->data;
		for (i = 0; i < n; ++i) {
			/* 如果没有触点记录，则说明是捕获全部触点 */
			if (tc->points.length == 0) {
				ui_add_touch_capturer(&capturers, tc->widget,
						      points[i].id);
				continue;
			}
			for (LinkedList_Each(ptnode, &tc->points)) {
				if (points[i].id != *(int*)ptnode->data) {
					continue;
				}
				ui_add_touch_capturer(&capturers, tc->widget,
						      points[i].id);
			}
		}
	}
	ui_dispatch_touch_event(&capturers, points, n);
	ui_clear_touch_capturers(&capturers);
	LCUIMutex_Unlock(&ui_events.mutex);
	return 0;
}

void ui_widget_set_mouse_capture(ui_widget_t* w)
{
	ui_events.mouse_capturer = w;
}

void ui_widget_release_mouse_capture(ui_widget_t* w)
{
	ui_events.mouse_capturer = NULL;
}

int ui_widget_set_touch_capture(ui_widget_t* w, int point_id)
{
	int ret;
	LCUIMutex_Lock(&ui_events.mutex);
	ret = ui_add_touch_capturer(&ui_events.touch_capturers, w, point_id);
	LCUIMutex_Unlock(&ui_events.mutex);
	return ret;
}

int ui_widget_release_touch_capture(ui_widget_t* w, int point_id)
{
	int ret;

	if (ui_events.touch_capturers.length <= 1) {
		return 0;
	}
	LCUIMutex_Lock(&ui_events.mutex);
	ret = ui_remove_touch_capturer(&ui_events.touch_capturers, w, point_id);
	LCUIMutex_Unlock(&ui_events.mutex);
	return ret;
}

int ui_widget_post_surface_event(ui_widget_t* w, int event_type,
				 LCUI_BOOL sync_props)
{
	int* data;
	ui_event_t e = { 0 };
	ui_widget_t* root = ui_root();

	if (w->parent != root && w != root) {
		return -1;
	}
	e.target = w;
	e.type = UI_EVENT_SURFACE;
	e.cancel_bubble = TRUE;
	data = malloc(sizeof(int) * 2);
	if (!data) {
		return -ENOMEM;
	}
	data[0] = event_type;
	data[1] = sync_props;
	return ui_widget_post_event(root, &e, data, free);
}

void ui_widget_destroy_event_trigger(ui_widget_t* w)
{
	ui_event_t e = { UI_EVENT_DESTROY, 0 };

	ui_widget_trigger_event(w, &e, NULL);
	ui_widget_release_mouse_capture(w);
	ui_widget_release_touch_capture(w, -1);
	ui_widget_stop_event_propagation(w);
	ui_clear_event_target(w);
	if (w->trigger) {
		EventTrigger_Destroy(w->trigger);
		w->trigger = NULL;
	}
}

void ui_init_events(void)
{
	int i, n;
	struct EventNameMapping {
		int id;
		const char* name;
	} mappings[] = { { UI_EVENT_LINK, "link" },
			 { UI_EVENT_UNLINK, "unlink" },
			 { UI_EVENT_READY, "ready" },
			 { UI_EVENT_DESTROY, "destroy" },
			 { UI_EVENT_MOUSEDOWN, "mousedown" },
			 { UI_EVENT_MOUSEUP, "mouseup" },
			 { UI_EVENT_MOUSEMOVE, "mousemove" },
			 { UI_EVENT_MOUSEWHEEL, "mousewheel" },
			 { UI_EVENT_CLICK, "click" },
			 { UI_EVENT_DBLCLICK, "dblclick" },
			 { UI_EVENT_MOUSEOUT, "mouseout" },
			 { UI_EVENT_MOUSEOVER, "mouseover" },
			 { UI_EVENT_KEYDOWN, "keydown" },
			 { UI_EVENT_KEYUP, "keyup" },
			 { UI_EVENT_KEYPRESS, "keypress" },
			 { UI_EVENT_TOUCH, "touch" },
			 { UI_EVENT_TEXTINPUT, "textinput" },
			 { UI_EVENT_TOUCHDOWN, "touchdown" },
			 { UI_EVENT_TOUCHMOVE, "touchmove" },
			 { UI_EVENT_TOUCHUP, "touchup" },
			 { UI_EVENT_RESIZE, "resize" },
			 { UI_EVENT_AFTERLAYOUT, "afterlayout" },
			 { UI_EVENT_FOCUS, "focus" },
			 { UI_EVENT_BLUR, "blur" },
			 { UI_EVENT_SHOW, "show" },
			 { UI_EVENT_HIDE, "hide" },
			 { UI_EVENT_SURFACE, "surface" },
			 { UI_EVENT_TITLE, "title" } };

	LCUIMutex_Init(&ui_events.mutex);
	RBTree_Init(&ui_events.event_names);
	RBTree_Init(&ui_events.event_records);
	LinkedList_Init(&ui_events.events);
	LinkedList_Init(&ui_events.event_mappings);
	ui_events.targets[UI_WIDGET_STATUS_ACTIVE] = NULL;
	ui_events.targets[UI_WIDGET_STATUS_HOVER] = NULL;
	ui_events.targets[UI_WIDGET_STATUS_FOCUS] = NULL;
	ui_events.mouse_capturer = NULL;
	ui_events.click.x = 0;
	ui_events.click.y = 0;
	ui_events.click.time = 0;
	ui_events.click.widget = NULL;
	ui_events.click.interval = DBLCLICK_INTERVAL;
	ui_events.base_event_id = UI_EVENT_USER + 1000;
	Dict_InitStringKeyType(&ui_events.event_ids_type);
	ui_events.event_ids = Dict_Create(&ui_events.event_ids_type, NULL);
	n = sizeof(mappings) / sizeof(mappings[0]);
	for (i = 0; i < n; ++i) {
		ui_set_widget_event_id(mappings[i].id, mappings[i].name);
	}
	RBTree_OnCompare(&ui_events.event_records, ui_event_record_compare);
	RBTree_OnDestroy(&ui_events.event_records, ui_event_record_destroy);
	LinkedList_Init(&ui_events.touch_capturers);
}

int ui_dispatch_event(ui_event_t* e)
{
	switch (e->type) {
	case LCUI_MOUSEWHEEL:
	case LCUI_MOUSEDOWN:
	case LCUI_MOUSEMOVE:
	case LCUI_MOUSEUP:
		return ui_on_mouse_event(e);
	case LCUI_KEYPRESS:
	case LCUI_KEYDOWN:
	case LCUI_KEYUP:
		return ui_on_keyboard_event(e);
	case LCUI_TOUCH:
		return ui_on_touch_event(e);
	case LCUI_TEXTINPUT:
		return ui_on_text_input(e);
	default:
		break;
	}
	return -1;
}

void ui_process_events(void)
{
}

void ui_destroy_events(void)
{
	LinkedListNode* node;
	LCUIMutex_Lock(&ui_events.mutex);
	for (LinkedList_Each(node, &ui_events.events)) {
		int* id = node->data;
		LCUI_UnbindEvent(*id);
	}
	RBTree_Destroy(&ui_events.event_names);
	RBTree_Destroy(&ui_events.event_records);
	Dict_Release(ui_events.event_ids);
	ui_clear_touch_capturers(&ui_events.touch_capturers);
	LinkedList_Clear(&ui_events.events, free);
	LinkedList_Clear(&ui_events.event_mappings, ui_event_mapping_destroy);
	LCUIMutex_Unlock(&ui_events.mutex);
	LCUIMutex_Destroy(&ui_events.mutex);
	ui_events.event_ids = NULL;
}
