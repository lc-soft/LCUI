#include <LCUI.h>
#include <LCUI/input.h>
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
	LinkedListNode node;
	int event_id;
	ui_event_handler_t handler;
	void *data;
	void(*destroy_data)(void *);
} ui_event_listener_t;

typedef struct ui_event_pack_t {
	LinkedListNode node;
	void *data;
	void(*destroy_data)(void *);
	ui_widget_t* widget;
	ui_event_t event;
} ui_event_pack_t;

enum ui_widget_status_type_t {
	UI_WIDGET_STATUS_HOVER,
	UI_WIDGET_STATUS_ACTIVE,
	UI_WIDGET_STATUS_FOCUS,
	UI_WIDGET_STATUS_TOTAL
};

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
	ui_widget_t* mouse_capturer;
	LinkedList touch_capturers;
	ui_widget_t* targets[UI_WIDGET_STATUS_TOTAL];

	/** LinkedList<ui_event_pack_t> */
	LinkedList queue;

	/** LinkedList<event_mapping_t> */
	LinkedList event_mappings;

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

static int ui_use_widget_event_id(const char* event_name)
{
	int id = ui_get_event_id(event_name);
	if (id < 0) {
		id = ui_alloc_event_id();
		ui_set_event_id(id, event_name);
	}
	return id;
}

int ui_set_event_id(int event_id, const char* event_name)
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

int ui_alloc_event_id(void)
{
	return ui_events.base_event_id++;
}

const char* ui_get_event_name(int event_id)
{
	ui_event_mapping_t* mapping;
	LCUIMutex_Lock(&ui_events.mutex);
	mapping = RBTree_GetData(&ui_events.event_names, event_id);
	LCUIMutex_Unlock(&ui_events.mutex);
	return mapping ? mapping->name : NULL;
}

int ui_get_event_id(const char* event_name)
{
	ui_event_mapping_t* mapping;
	LCUIMutex_Lock(&ui_events.mutex);
	mapping = Dict_FetchValue(ui_events.event_ids, event_name);
	LCUIMutex_Unlock(&ui_events.mutex);
	return mapping ? mapping->id : -1;
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

/** 复制部件事件 */
static int ui_event_copy(const ui_event_t* src, ui_event_t* dst)
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

static void ui_event_mapping_destroy(void* data)
{
	ui_event_mapping_t* mapping = data;
	free(mapping->name);
	free(mapping);
}

static void ui_event_pack_destroy(ui_event_pack_t* pack)
{
	if (pack->data && pack->destroy_data) {
		pack->destroy_data(pack->data);
	}
	ui_event_destroy(&pack->event);
	pack->data = NULL;
	free(pack);
}

static void ui_event_listener_destroy(ui_event_listener_t* listener)
{
	if (listener->data && listener->destroy_data) {
		listener->destroy_data(listener->data);
	}
	listener->data = NULL;
	free(listener);
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

int ui_widget_add_event_listener(ui_widget_t* w, int event_id,
				 ui_event_handler_t handler, void* data,
				 void (*destroy_data)(void*))
{
	ui_event_listener_t* listener;

	listener = malloc(sizeof(ui_event_listener_t));
	if (!listener) {
		return -ENOMEM;
	}
	listener->handler = handler;
	listener->data = data;
	listener->event_id = event_id;
	listener->destroy_data = destroy_data;
	if (!w->listeners) {
		w->listeners = malloc(sizeof(ui_widget_listeners_t));
		if (!w->listeners) {
			return -ENOMEM;
		}
	}
	LinkedList_AppendNode(w->listeners, &listener->node);
	return 0;
}

int ui_widget_on(ui_widget_t* w, const char* event_name,
		 ui_event_handler_t handler, void* data,
		 void (*destroy_data)(void*))
{
	int id = ui_use_widget_event_id(event_name);
	return ui_widget_add_event_listener(w, id, handler, data, destroy_data);
}

int ui_widget_remove_event_listener(ui_widget_t* w, int event_id,
				    ui_event_handler_t handler)
{
	int count = 0;
	LinkedListNode *node, *prev;
	ui_event_listener_t* listener;

	if (!w->listeners) {
		return 0;
	}
	for (LinkedList_Each(node, w->listeners)) {
		listener = node->data;
		if (listener->event_id != event_id ||
		    (handler && handler != listener->handler)) {
			continue;
		}
		prev = node->prev;
		LinkedList_Unlink(w->listeners, node);
		ui_event_listener_destroy(listener);
		node = prev;
		count++;
	}
	return count;
}

int ui_widget_off(ui_widget_t* w, const char* event_name,
		  ui_event_handler_t handler)
{
	int id = ui_use_widget_event_id(event_name);
	return ui_widget_remove_event_listener(w, id, handler);
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

static int ui_widget_call_listeners(ui_widget_t* w, ui_event_t e,
				    void* arg)
{
	int count = 0;
	LinkedListNode* node;
	ui_event_listener_t* listener;

	if (!w->listeners) {
		return count;
	}
	for (LinkedList_Each(node, w->listeners)) {
		listener = node->data;
		if (listener->event_id != e.type) {
			continue;
		}
		listener->handler(w, &e, arg);
		if (!e.cancel_bubble && w->parent) {
			ui_widget_call_listeners(w->parent, e, arg);
		}
		count++;
	}
	return count;
}

int ui_widget_post_event(ui_widget_t* w, const ui_event_t* e, void* data,
			 void (*destroy_data)(void*))
{
	ui_event_pack_t* pack;

	if (w->state == LCUI_WSTATE_DELETED) {
		return -EPERM;
	}
	pack = malloc(sizeof(ui_event_pack_t));
	if (!pack) {
		return -ENOMEM;
	}
	ui_event_copy(e, &pack->event);
	if (!pack->event.target) {
		pack->event.target = w;
	}
	pack->widget = w;
	pack->data = data;
	pack->destroy_data = destroy_data;
	LinkedList_AppendNode(&ui_events.queue, &pack->node);
	return 0;
}

int ui_widget_emit_event(ui_widget_t* w, ui_event_t e, void* arg)
{
	if (!e.target) {
		e.target = w;
	}
	switch (e.type) {
	case UI_EVENT_CLICK:
	case UI_EVENT_MOUSEDOWN:
	case UI_EVENT_MOUSEUP:
	case UI_EVENT_MOUSEMOVE:
	case UI_EVENT_MOUSEOVER:
	case UI_EVENT_MOUSEOUT:
		if (w->computed_style.pointer_events == SV_NONE) {
			break;
		}
	default:
		if (ui_widget_call_listeners(w, e, NULL) > 0) {
			return 0;
		}
		if (!w->parent || e.cancel_bubble) {
			return -1;
		}
		return ui_widget_emit_event(w->parent, e, arg);
	}
	if (!w->parent || e.cancel_bubble) {
		return -1;
	}
	while (w->listeners && w->computed_style.pointer_events == SV_NONE) {
		ui_widget_t* w;
		LCUI_BOOL is_pointer_event = TRUE;
		int pointer_x, pointer_y;
		float x, y;

		switch (e.type) {
		case UI_EVENT_CLICK:
		case UI_EVENT_MOUSEDOWN:
		case UI_EVENT_MOUSEUP:
			pointer_x = e.button.x;
			pointer_y = e.button.y;
			break;
		case UI_EVENT_MOUSEMOVE:
		case UI_EVENT_MOUSEOVER:
		case UI_EVENT_MOUSEOUT:
			pointer_x = e.motion.x;
			pointer_y = e.motion.y;
			break;
		default:
			is_pointer_event = FALSE;
			break;
		}
		if (!is_pointer_event) {
			break;
		}
		ui_widget_get_offset(w->parent, NULL, &x, &y);
		/* 转换成相对于父级部件内容框的坐标 */
		x = pointer_x - x;
		y = pointer_y - y;
		/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
		w = ui_widget_get_next_at(w, iround(x), iround(y));
		if (!w) {
			break;
		}
		return ui_widget_call_listeners(w, e, arg);
	}
	return ui_widget_emit_event(w->parent, e, arg);
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

	for (LinkedList_Each(node, &widget->stacking_context)) {
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
		ui_widget_add_status(w, "hover");
		ui_widget_emit_event(w, e, NULL);
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
		ui_widget_remove_status(w, "hover");
		ui_widget_emit_event(w, e, NULL);
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

static void ui_widget_on_mousedown_event(ui_widget_t* widget)
{
	ui_widget_t* parent;
	ui_widget_t* w = ui_events.targets[UI_WIDGET_STATUS_ACTIVE];

	if (w == widget) {
		return;
	}
	parent = ui_get_same_parent(widget, w);
	for (; w && w != parent; w = w->parent) {
		ui_widget_remove_status(w, "active");
	}
	for (w = widget; w && w != parent; w = w->parent) {
		ui_widget_add_status(w, "active");
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
		ui_widget_on_mousedown_event(NULL);
		return;
	}
	for (w = ui_events.targets[UI_WIDGET_STATUS_ACTIVE]; w; w = w->parent) {
		if (w == target) {
			ui_widget_on_mousedown_event(NULL);
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

void ui_clear_event_target(ui_widget_t* w)
{
	LinkedListNode *node, *prev;
	ui_event_pack_t* pack;

	LCUIMutex_Lock(&ui_events.mutex);
	for (LinkedList_Each(node, &ui_events.queue)) {
		prev = node->prev;
		pack = node->data;
		if (pack->widget == w) {
			LinkedList_Unlink(&ui_events.queue, node);
			ui_event_pack_destroy(pack);
		}
	}
	LCUIMutex_Unlock(&ui_events.mutex);
	ui_clear_mouseover_target(w);
	ui_clear_mousedown_target(w);
	ui_clear_focus_target(w);
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
		ui_widget_remove_status(e.target, "focus");
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
	ui_widget_add_status(e.target, "focus");
	ui_widget_post_event(e.target, &e, NULL, NULL);
	return 0;
}

static ui_widget_t* ui_resolve_event_target(int ix, int iy)
{
	float scale = ui_get_scale();
	float x = iround(ix / scale);
	float y = iround(iy / scale);

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

// TODO: Convert LCUI Event to UI Event

static int ui_on_mouse_event(ui_event_t* origin_event)
{
	float scale;
	ui_widget_t *target, *w;
	ui_event_t e = *origin_event;

	e.cancel_bubble = FALSE;
	switch (e.type) {
	case LCUI_MOUSEDOWN:
		e.type = UI_EVENT_MOUSEDOWN;
		e.target = ui_resolve_event_target(e.button.x, e.button.y);
		if (!e.target) {
			return -1;
		}
		ui_widget_emit_event(e.target, e, NULL);
		ui_events.click.interval = DBLCLICK_INTERVAL;
		if (e.button.button == LCUI_KEY_LEFTBUTTON &&
		    ui_events.click.widget == e.target) {
			int delta;
			delta = (int)LCUI_GetTimeDelta(ui_events.click.time);
			ui_events.click.interval = delta;
		} else if (e.button.button == LCUI_KEY_LEFTBUTTON &&
			   ui_events.click.widget != target) {
			ui_events.click.x = e.button.x;
			ui_events.click.y = e.button.y;
		}
		ui_events.click.time = LCUI_GetTime();
		ui_events.click.widget = target;
		ui_clear_mousedown_target(target);
		ui_set_focus(target);
		break;
	case LCUI_MOUSEUP:
		e.type = UI_EVENT_MOUSEUP;
		e.target = ui_resolve_event_target(e.button.x, e.button.y);
		if (!e.target) {
			return -1;
		}
		ui_widget_emit_event(target, e, NULL);
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
		ui_widget_emit_event(target, e, NULL);
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
			ui_widget_emit_event(target, e, NULL);
		}
		ui_clear_mousedown_target(NULL);
		break;
	case LCUI_MOUSEMOVE:
		e.target = ui_resolve_event_target(e.motion.x, e.motion.y);
		if (!e.target) {
			return -1;
		}
		if (abs(ui_events.click.x - e.motion.x) >= 8 ||
		    abs(ui_events.click.y - e.motion.y) >= 8) {
			ui_events.click.time = 0;
			ui_events.click.widget = NULL;
		}
		ui_widget_emit_event(target, e, NULL);
		break;
	case LCUI_MOUSEWHEEL:
		e.target = ui_resolve_event_target(e.wheel.x, e.wheel.y);
		if (!e.target) {
			return -1;
		}
		ui_widget_emit_event(target, e, NULL);
	default:
		return -1;
	}
	ui_widget_on_mouseover_event(target);
	return 0;
}

static int ui_on_keyboard_event(ui_event_t* origin_event)
{
	ui_event_t e = *origin_event;

	e.target = ui_events.targets[UI_WIDGET_STATUS_FOCUS];
	if (!e.target) {
		return -1;
	}
	e.cancel_bubble = FALSE;
	ui_widget_emit_event(e.target, e, NULL);
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
	ui_widget_emit_event(e.target, e, NULL);
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
	scale = ui_get_scale();
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
	scale = ui_get_scale();
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

void ui_widget_destroy_listeners(ui_widget_t* w)
{
	ui_event_t e = { UI_EVENT_DESTROY, 0 };

	ui_widget_emit_event(w, e, NULL);
	ui_widget_release_mouse_capture(w);
	ui_widget_release_touch_capture(w, -1);
	ui_clear_event_target(w);
	if (w->listeners) {
		LinkedList_ClearData(w->listeners, ui_event_listener_destroy);
		free(w->listeners);
		w->listeners = NULL;
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
			 { UI_EVENT_TITLE, "title" },
			 { UI_EVENT_FONT_FACE_LOAD, "font_face_load" } };

	LCUIMutex_Init(&ui_events.mutex);
	RBTree_Init(&ui_events.event_names);
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
		ui_set_event_id(mappings[i].id, mappings[i].name);
	}
	LinkedList_Init(&ui_events.touch_capturers);
}

int ui_dispatch_event(ui_event_t* e)
{
	switch (e->type) {
	case UI_EVENT_MOUSEWHEEL:
	case UI_EVENT_MOUSEDOWN:
	case UI_EVENT_MOUSEMOVE:
	case UI_EVENT_MOUSEUP:
		return ui_on_mouse_event(e);
	case UI_EVENT_KEYPRESS:
	case UI_EVENT_KEYDOWN:
	case UI_EVENT_KEYUP:
		return ui_on_keyboard_event(e);
	case UI_EVENT_TOUCH:
		return ui_on_touch_event(e);
	case UI_EVENT_TEXTINPUT:
		return ui_on_text_input(e);
	default:
		break;
	}
	return -1;
}

void ui_process_events(void)
{
	LinkedList queue;
	LinkedListNode* node;
	ui_event_pack_t* pack;

	LinkedList_Init(&queue);
	LinkedList_Concat(&queue, &ui_events.queue);
	for (LinkedList_Each(node, &queue)) {
		pack = node->data;
		if (pack->widget) {
			ui_widget_emit_event(pack->widget, pack->event,
					     pack->data);
		}
	}
	LinkedList_ClearData(&queue, ui_event_pack_destroy);
}

void ui_destroy_events(void)
{
	LCUIMutex_Lock(&ui_events.mutex);
	RBTree_Destroy(&ui_events.event_names);
	Dict_Release(ui_events.event_ids);
	ui_clear_touch_capturers(&ui_events.touch_capturers);
	LinkedList_ClearData(&ui_events.queue, ui_event_pack_destroy);
	LinkedList_Clear(&ui_events.event_mappings, ui_event_mapping_destroy);
	LCUIMutex_Unlock(&ui_events.mutex);
	LCUIMutex_Destroy(&ui_events.mutex);
	ui_events.event_ids = NULL;
}
