/*
 * widget_tree.c -- The widget tree node operation set.
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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

int Widget_Append(LCUI_Widget parent, LCUI_Widget widget)
{
	LCUI_WidgetEventRec ev = { 0 };

	if (!parent || !widget) {
		return -1;
	}
	if (parent == widget) {
		return -2;
	}
	Widget_Unlink(widget);
	widget->parent = parent;
	widget->state = LCUI_WSTATE_CREATED;
	widget->index = parent->children.length;
	LinkedList_AppendNode(&parent->children, &widget->node);
	ev.cancel_bubble = TRUE;
	ev.type = LCUI_WEVENT_LINK;
	Widget_UpdateStyle(widget, TRUE);
	Widget_UpdateChildrenStyle(widget, TRUE);
	Widget_TriggerEvent(widget, &ev, NULL);
	Widget_PostSurfaceEvent(widget, LCUI_WEVENT_LINK, TRUE);
	Widget_UpdateTaskStatus(widget);
	Widget_UpdateStatus(widget);
	Widget_AddTask(parent, LCUI_WTASK_LAYOUT);
	return 0;
}

int Widget_Prepend(LCUI_Widget parent, LCUI_Widget widget)
{
	LCUI_Widget child;
	LCUI_WidgetEventRec ev = { 0 };
	LinkedListNode *node;

	if (!parent || !widget) {
		return -1;
	}
	if (parent == widget) {
		return -2;
	}
	child = widget->parent;
	Widget_Unlink(widget);
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
	ev.type = LCUI_WEVENT_LINK;
	Widget_TriggerEvent(widget, &ev, NULL);
	Widget_PostSurfaceEvent(widget, LCUI_WEVENT_LINK, TRUE);
	Widget_AddTaskForChildren(widget, LCUI_WTASK_REFRESH_STYLE);
	Widget_UpdateTaskStatus(widget);
	Widget_UpdateStatus(widget);
	Widget_AddTask(parent, LCUI_WTASK_LAYOUT);
	return 0;
}

int Widget_Unwrap(LCUI_Widget widget)
{
	size_t len;
	LCUI_Widget child;
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
	while (len > 0) {
		assert(node != NULL);
		assert(node->data != NULL);
		prev = node->prev;
		child = node->data;
		LinkedList_Unlink(&widget->children, node);
		child->parent = widget->parent;
		LinkedList_Link(children, target, node);
		Widget_AddTaskForChildren(child, LCUI_WTASK_REFRESH_STYLE);
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
	Widget_Destroy(widget);
	return 0;
}

int Widget_Unlink(LCUI_Widget w)
{
	LCUI_Widget child;
	LCUI_WidgetEventRec ev = { 0 };
	LinkedListNode *node;

	if (!w->parent) {
		return -1;
	}
	node = &w->node;
	if (w->index == w->parent->children.length - 1) {
		Widget_RemoveStatus(w, "last-child");
		child = Widget_GetPrev(w);
		if (child) {
			Widget_AddStatus(child, "last-child");
		}
	}
	if (w->index == 0) {
		Widget_RemoveStatus(w, "first-child");
		child = Widget_GetNext(w);
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
	ev.type = LCUI_WEVENT_UNLINK;
	Widget_TriggerEvent(w, &ev, NULL);
	LinkedList_Unlink(&w->parent->children, node);
	LinkedList_Unlink(&w->parent->children_show, &w->node_show);
	Widget_PostSurfaceEvent(w, LCUI_WEVENT_UNLINK, TRUE);
	Widget_AddTask(w->parent, LCUI_WTASK_LAYOUT);
	w->parent = NULL;
	return 0;
}

LCUI_Widget Widget_GetPrev(LCUI_Widget w)
{
	LinkedListNode *node = &w->node;
	if (node->prev && node != w->parent->children.head.next) {
		return node->prev->data;
	}
	return NULL;
}

LCUI_Widget Widget_GetNext(LCUI_Widget w)
{
	LinkedListNode *node = &w->node;
	if (node->next) {
		return node->next->data;
	}
	return NULL;
}

LCUI_Widget Widget_GetChild(LCUI_Widget w, size_t index)
{
	LinkedListNode *node = LinkedList_GetNode(&w->children, index);
	if (node) {
		return node->data;
	}
	return NULL;
}

static void Widget_OnDestroy(void *arg)
{
	Widget_ExecDestroy(arg);
}

void Widget_DestroyChildren(LCUI_Widget w)
{
	/* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
	 * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
	LinkedList_ClearData(&w->children_show, NULL);
	LinkedList_ClearData(&w->children, Widget_OnDestroy);
}

static void _LCUIWidget_PrintTree(LCUI_Widget w, int depth, const char *prefix)
{
	size_t len;
	LCUI_Widget child;
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
		Logger_Debug(
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
		_LCUIWidget_PrintTree(child, depth + 1, child_prefix);
	}
}

void Widget_PrintTree(LCUI_Widget w)
{
	LCUI_SelectorNode node;
	w = w ? w : LCUIWidget_GetRoot();
	node = Widget_GetSelectorNode(w);
	Logger_Debug("%s, xy:(%g,%g), size:(%g,%g), visible: %s\n",
		     node->fullname, w->x, w->y, w->width, w->height,
		     w->computed_style.visible ? "true" : "false");
	SelectorNode_Delete(node);
	_LCUIWidget_PrintTree(w, 0, "  ");
}

size_t Widget_Each(LCUI_Widget w, void (*callback)(LCUI_Widget, void *),
		   void *arg)
{
	size_t count = 0;

	LCUI_Widget next;
	LCUI_Widget child = LinkedList_Get(&w->children, 0);

	while (child && child != w) {
		callback(child, arg);
		++count;
		next = LinkedList_Get(&child->children, 0);
		while (!next && child != w) {
			next = Widget_GetNext(child);
			child = child->parent;
		}
		child = next;
	}
	return count;
}

LCUI_Widget Widget_At(LCUI_Widget widget, int ix, int iy)
{
	float x, y;
	LCUI_BOOL is_hit;
	LinkedListNode *node;
	LCUI_Widget target = widget, c = NULL;

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
