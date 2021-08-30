
#include "../include/ui.h"
#include "private.h"

int ui_widget_append(ui_widget_t* parent, ui_widget_t* widget)
{
	ui_event_t ev = { 0 };

	if (!parent || !widget) {
		return -1;
	}
	if (parent == widget) {
		return -2;
	}
	ui_widget_unlink(widget);
	widget->parent = parent;
	widget->state = LCUI_WSTATE_CREATED;
	widget->index = parent->children.length;
	LinkedList_AppendNode(&parent->children, &widget->node);
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_LINK;
	Widget_UpdateStyle(widget, TRUE);
	Widget_UpdateChildrenStyle(widget, TRUE);
	ui_widget_emit_event(widget, &ev, NULL);
	ui_widget_post_surface_event(widget, UI_EVENT_LINK, TRUE);
	Widget_UpdateTaskStatus(widget);
	Widget_UpdateStatus(widget);
	Widget_AddTask(parent, UI_WIDGET_TASK_REFLOW);
	return 0;
}

int ui_widget_prepend(ui_widget_t* parent, ui_widget_t* widget)
{
	ui_widget_t* child;
	ui_event_t ev = { 0 };
	LinkedListNode *node;

	if (!parent || !widget) {
		return -1;
	}
	if (parent == widget) {
		return -2;
	}
	child = widget->parent;
	ui_widget_unlink(widget);
	widget->index = 0;
	widget->parent = parent;
	widget->state = LCUI_WSTATE_CREATED;
	node = &widget->node;
	LinkedList_InsertNode(&parent->children, 0, node);
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while (node) {
		child = node->data;
		child->index += 1;
		node = node->next;
	}
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_LINK;
	ui_widget_emit_event(widget, &ev, NULL);
	ui_widget_post_surface_event(widget, UI_EVENT_LINK, TRUE);
	Widget_AddTaskForChildren(widget, UI_WIDGET_TASK_REFRESH_STYLE);
	Widget_UpdateTaskStatus(widget);
	Widget_UpdateStatus(widget);
	Widget_AddTask(parent, UI_WIDGET_TASK_REFLOW);
	return 0;
}

int ui_widget_unwrap(ui_widget_t* widget)
{
	size_t len;
	ui_widget_t* child;
	ui_event_t ev = { 0 };
	LinkedList *children;
	LinkedListNode *target, *node, *prev;

	if (!widget->parent) {
		return -1;
	}
	children = &widget->parent->children;
	len = widget->children.length;
	if (len > 0) {
		node = LinkedList_GetNode(&widget->children, 0);
		Widget_RemoveStatus(node->data, "first-child");
		node = LinkedList_GetNodeAtTail(&widget->children, 0);
		Widget_RemoveStatus(node->data, "last-child");
	}
	node = &widget->node;
	target = node->prev;
	node = widget->children.tail.prev;
	ev.cancel_bubble = TRUE;
	while (len > 0) {
		assert(node != NULL);
		assert(node->data != NULL);
		prev = node->prev;
		child = node->data;
		ev.type = UI_EVENT_UNLINK;
		ui_widget_emit_event(child, &ev, NULL);
		LinkedList_Unlink(&widget->children, node);
		LinkedList_Link(children, target, node);
		child->parent = widget->parent;
		ev.type = UI_EVENT_LINK;
		ui_widget_emit_event(child, &ev, NULL);
		Widget_AddTaskForChildren(child, UI_WIDGET_TASK_REFRESH_STYLE);
		Widget_UpdateTaskStatus(child);
		node = prev;
		--len;
	}
	if (widget->index == 0) {
		Widget_AddStatus(target->next->data, "first-child");
	}
	if (widget->index == children->length - 1) {
		node = LinkedList_GetNodeAtTail(children, 0);
		Widget_AddStatus(node->data, "last-child");
	}
	ui_widget_remove(widget);
	return 0;
}

int ui_widget_unlink(ui_widget_t* w)
{
	ui_widget_t* child;
	ui_event_t ev = { 0 };
	LinkedListNode *node;

	if (!w->parent) {
		return -1;
	}
	node = &w->node;
	if (w->index == w->parent->children.length - 1) {
		Widget_RemoveStatus(w, "last-child");
		child = ui_widget_prev(w);
		if (child) {
			Widget_AddStatus(child, "last-child");
		}
	}
	if (w->index == 0) {
		Widget_RemoveStatus(w, "first-child");
		child = ui_widget_next(w);
		if (child) {
			Widget_AddStatus(child, "first-child");
		}
	}
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while (node) {
		child = node->data;
		child->index -= 1;
		node = node->next;
	}
	node = &w->node;
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_UNLINK;
	ui_widget_emit_event(w, &ev, NULL);
	LinkedList_Unlink(&w->parent->children, node);
	LinkedList_Unlink(&w->parent->children_show, &w->node_show);
	ui_widget_post_surface_event(w, UI_EVENT_UNLINK, TRUE);
	Widget_AddTask(w->parent, UI_WIDGET_TASK_REFLOW);
	w->parent = NULL;
	return 0;
}

ui_widget_t* ui_widget_prev(ui_widget_t* w)
{
	LinkedListNode *node = &w->node;
	if (node->prev && node != w->parent->children.head.next) {
		return node->prev->data;
	}
	return NULL;
}

ui_widget_t* ui_widget_next(ui_widget_t* w)
{
	LinkedListNode *node = &w->node;
	if (node->next) {
		return node->next->data;
	}
	return NULL;
}

ui_widget_t* ui_widget_get_child(ui_widget_t* w, size_t index)
{
	LinkedListNode *node = LinkedList_GetNode(&w->children, index);
	if (node) {
		return node->data;
	}
	return NULL;
}

size_t ui_widget_each(ui_widget_t* w, void (*callback)(ui_widget_t*, void *),
		   void *arg)
{
	size_t count = 0;

	ui_widget_t* next;
	ui_widget_t* child = LinkedList_Get(&w->children, 0);

	while (child && child != w) {
		callback(child, arg);
		++count;
		next = LinkedList_Get(&child->children, 0);
		while (!next && child != w) {
			next = ui_widget_next(child);
			child = child->parent;
		}
		child = next;
	}
	return count;
}

ui_widget_t* ui_widget_at(ui_widget_t* widget, int ix, int iy)
{
	float x, y;
	LCUI_BOOL is_hit;
	LinkedListNode *node;
	ui_widget_t* target = widget, c = NULL;

	if (!widget) {
		return NULL;
	}
	x = 1.0f * ix;
	y = 1.0f * iy;
	do {
		is_hit = FALSE;
		for (LinkedList_Each(node, &target->children_show)) {
			c = node->data;
			if (!c->computed_style.visible) {
				continue;
			}
			if (LCUIRect_HasPoint(&c->box.border, x, y)) {
				target = c;
				x -= c->box.padding.x;
				y -= c->box.padding.y;
				is_hit = TRUE;
				break;
			}
		}
	} while (is_hit);
	return target == widget ? NULL : target;
}

static void _ui_widget_print_tree(ui_widget_t* w, int depth, const char *prefix)
{
	size_t len;
	ui_widget_t* child;
	LinkedListNode *node;
	LCUI_SelectorNode snode;
	char str[16], child_prefix[512];

	len = strlen(prefix);
	strcpy(child_prefix, prefix);
	for (LinkedList_Each(node, &w->children)) {
		if (node == w->children.tail.prev) {
			strcpy(str, "└");
			strcpy(&child_prefix[len], "    ");
		} else {
			strcpy(str, "├");
			strcpy(&child_prefix[len], "│  ");
		}
		strcat(str, "─");
		child = node->data;
		if (child->children.length == 0) {
			strcat(str, "─");
		} else {
			strcat(str, "┬");
		}
		snode = Widget_GetSelectorNode(child);
		Logger_Error(
		    "%s%s %s, xy:(%g,%g), size:(%g,%g), "
		    "visible: %s, display: %d, padding: (%g,%g,%g,%g), margin: "
		    "(%g,%g,%g,%g)\n",
		    prefix, str, snode->fullname, child->x, child->y,
		    child->width, child->height,
		    child->computed_style.visible ? "true" : "false",
		    child->computed_style.display, child->padding.top,
		    child->padding.right, child->padding.bottom,
		    child->padding.left, child->margin.top, child->margin.right,
		    child->margin.bottom, child->margin.left);
		SelectorNode_Delete(snode);
		_ui_widget_print_tree(child, depth + 1, child_prefix);
	}
}

void ui_widget_print_tree(ui_widget_t* w)
{
	LCUI_SelectorNode node;
	w = w ? w : ui_root();
	node = Widget_GetSelectorNode(w);
	Logger_Error("%s, xy:(%g,%g), size:(%g,%g), visible: %s\n",
		     node->fullname, w->x, w->y, w->width, w->height,
		     w->computed_style.visible ? "true" : "false");
	SelectorNode_Delete(node);
	_ui_widget_print_tree(w, 0, "  ");
}

void ui_widget_destroy_children(ui_widget_t* w)
{
	/* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
	 * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
	LinkedList_ClearData(&w->children_show, NULL);
	LinkedList_ClearData(&w->children, ui_widget_destroy);
}
