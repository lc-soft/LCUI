﻿#include <assert.h>
#include <string.h>
#include <LCUI/util.h>
#include "../include/ui.h"
#include "internal.h"

#define TYPE_CHILD_LIST UI_MUTATION_RECORD_TYPE_CHILD_LIST

int ui_widget_append(ui_widget_t* parent, ui_widget_t* widget)
{
	ui_event_t ev = { 0 };
	ui_mutation_record_t* record;

	if (!parent || !widget) {
		return -1;
	}
	if (parent == widget) {
		return -2;
	}
	ui_widget_unlink(widget);
	widget->parent = parent;
	widget->state = UI_WIDGET_STATE_CREATED;
	widget->index = parent->children.length;
	widget->parent->update.for_children = TRUE;
	list_append_node(&parent->children, &widget->node);
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_LINK;
	ui_widget_refresh_style(widget);
	ui_widget_refresh_children_style(widget);
	ui_widget_emit_event(widget, ev, NULL);
	ui_widget_update_status(widget);
	ui_widget_add_task(parent, UI_TASK_REFLOW);
	if (ui_widget_has_observer(parent, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(widget, TYPE_CHILD_LIST);
		list_append(&record->added_widgets, widget);
		ui_widget_add_mutation_recrod(parent, record);
		ui_mutation_record_destroy(record);
	}
	return 0;
}

int ui_widget_prepend(ui_widget_t* parent, ui_widget_t* widget)
{
	ui_widget_t* child;
	ui_mutation_record_t* record;
	ui_event_t ev = { 0 };
	list_node_t* node;

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
	widget->state = UI_WIDGET_STATE_CREATED;
	widget->parent->update.for_children = TRUE;
	node = &widget->node;
	list_insert_node(&parent->children, 0, node);
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while (node) {
		child = node->data;
		child->index += 1;
		node = node->next;
	}
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_LINK;
	ui_widget_emit_event(widget, ev, NULL);
	ui_widget_add_task_for_children(widget, UI_TASK_REFRESH_STYLE);
	ui_widget_update_status(widget);
	ui_widget_add_task(parent, UI_TASK_REFLOW);
	if (ui_widget_has_observer(parent, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(widget, TYPE_CHILD_LIST);
		list_append(&record->added_widgets, widget);
		ui_widget_add_mutation_recrod(parent, record);
		ui_mutation_record_destroy(record);
	}
	return 0;
}

int ui_widget_unwrap(ui_widget_t* w)
{
	size_t len;
	ui_widget_t* child;
	ui_event_t ev = { 0 };
	ui_mutation_record_t* record;
	list_t* children;
	list_node_t *target, *node, *prev;

	if (!w->parent) {
		return -1;
	}
	if (ui_widget_has_observer(w, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(w, TYPE_CHILD_LIST);
		for (list_each(node, &w->children)) {
			list_append(&record->removed_widgets, node->data);
		}
		ui_widget_add_mutation_recrod(w, record);
		ui_mutation_record_destroy(record);
	}
	if (ui_widget_has_observer(w->parent, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
		for (list_each(node, &w->children)) {
			list_append(&record->added_widgets, node->data);
		}
		ui_widget_add_mutation_recrod(w->parent, record);
		ui_mutation_record_destroy(record);
	}
	children = &w->parent->children;
	len = w->children.length;
	if (len > 0) {
		node = list_get_node(&w->children, 0);
		ui_widget_remove_status(node->data, "first-child");
		node = list_get_last_node(&w->children);
		ui_widget_remove_status(node->data, "last-child");
	}
	node = &w->node;
	target = node->prev;
	node = w->children.tail.prev;
	ev.cancel_bubble = TRUE;
	while (len > 0) {
		assert(node != NULL);
		assert(node->data != NULL);
		prev = node->prev;
		child = node->data;
		ev.type = UI_EVENT_UNLINK;
		ui_widget_emit_event(child, ev, NULL);
		list_unlink(&w->children, node);
		list_link(children, target, node);
		child->parent = w->parent;
		ev.type = UI_EVENT_LINK;
		ui_widget_emit_event(child, ev, NULL);
		ui_widget_add_task_for_children(child, UI_TASK_REFRESH_STYLE);
		node = prev;
		--len;
	}
	if (w->index == 0) {
		ui_widget_add_status(target->next->data, "first-child");
	}
	if (w->index == children->length - 1) {
		node = list_get_last_node(children);
		ui_widget_add_status(node->data, "last-child");
	}
	ui_widget_remove(w);
	return 0;
}

int ui_widget_unlink(ui_widget_t* w)
{
	ui_widget_t* child;
	ui_event_t ev = { 0 };
	ui_mutation_record_t* record;
	list_node_t* node;

	if (!w->parent) {
		return -1;
	}
	node = &w->node;
	if (w->index == w->parent->children.length - 1) {
		ui_widget_remove_status(w, "last-child");
		child = ui_widget_prev(w);
		if (child) {
			ui_widget_add_status(child, "last-child");
		}
	}
	if (w->index == 0) {
		ui_widget_remove_status(w, "first-child");
		child = ui_widget_next(w);
		if (child) {
			ui_widget_add_status(child, "first-child");
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
	ui_widget_emit_event(w, ev, NULL);
	list_unlink(&w->parent->children, node);
	list_unlink(&w->parent->stacking_context, &w->node_show);
	ui_widget_add_task(w->parent, UI_TASK_REFLOW);
	if (ui_widget_has_observer(w->parent, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
		list_append(&record->removed_widgets, w->parent);
		ui_widget_add_mutation_recrod(w->parent, record);
		ui_mutation_record_destroy(record);
	}
	w->parent = NULL;
	return 0;
}

void ui_widget_empty(ui_widget_t* w)
{
	ui_widget_t* child;
	list_node_t* node;
	ui_event_t ev = { 0 };
	ui_mutation_record_t *record;

	ui_event_init(&ev, "unlink");
	if (ui_widget_has_observer(w, TYPE_CHILD_LIST)) {
		record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
		for (list_each(node, &w->children)) {
			list_append(&record->removed_widgets, node->data);
		}
		ui_widget_add_mutation_recrod(w, record);
		ui_mutation_record_destroy(record);
	}
	while ((node = list_get_first_node(&w->children)) != NULL) {
		child = node->data;
		list_unlink(&w->children, node);
		ui_widget_emit_event(child, ev, NULL);
		ui_trash_add(child);
	}
	list_destroy_without_node(&w->stacking_context, NULL);
	ui_widget_mark_dirty_rect(w, NULL, CSS_KEYWORD_GRAPH_BOX);
	ui_widget_refresh_style(w);
}

void ui_widget_remove(ui_widget_t* w)
{
	ui_widget_t* child;
	list_node_t* node;

	assert(w->state != UI_WIDGET_STATE_DELETED);
	if (!w->parent) {
		ui_trash_add(w);
		return;
	}
	/* Update the index of the siblings behind it */
	node = w->node.next;
	while (node) {
		child = node->data;
		child->index -= 1;
		node = node->next;
	}
	if (w->computed_style.position != CSS_KEYWORD_ABSOLUTE) {
		ui_widget_add_task(w->parent, UI_TASK_REFLOW);
	}
	ui_widget_mark_dirty_rect(w->parent, &w->box.canvas, CSS_KEYWORD_CONTENT_BOX);
	ui_widget_unlink(w);
	ui_trash_add(w);
}

ui_widget_t* ui_widget_prev(ui_widget_t* w)
{
	list_node_t* node = &w->node;
	if (node->prev && node != w->parent->children.head.next) {
		return node->prev->data;
	}
	return NULL;
}

ui_widget_t* ui_widget_next(ui_widget_t* w)
{
	list_node_t* node = &w->node;
	if (node->next) {
		return node->next->data;
	}
	return NULL;
}

ui_widget_t* ui_widget_get_child(ui_widget_t* w, size_t index)
{
	list_node_t* node = list_get_node(&w->children, index);
	if (node) {
		return node->data;
	}
	return NULL;
}

size_t ui_widget_each(ui_widget_t* w, void (*callback)(ui_widget_t*, void*),
		      void* arg)
{
	size_t count = 0;

	ui_widget_t* next;
	ui_widget_t* child = list_get(&w->children, 0);

	while (child && child != w) {
		callback(child, arg);
		++count;
		next = list_get(&child->children, 0);
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
	list_node_t* node;
	ui_widget_t *target = widget, *c = NULL;

	if (!widget) {
		return NULL;
	}
	x = 1.0f * ix;
	y = 1.0f * iy;
	do {
		is_hit = FALSE;
		for (list_each(node, &target->stacking_context)) {
			c = node->data;
			if (!c->computed_style.visible) {
				continue;
			}
			if (LCUIRectF_HasPoint(&c->box.border, x, y)) {
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

static void _ui_print_tree(ui_widget_t* w, int depth, const char* prefix)
{
	size_t len;
	ui_widget_t* child;
	list_node_t* node;
	css_selector_node_t *snode;
	char str[16], child_prefix[512];

	len = strlen(prefix);
	strcpy(child_prefix, prefix);
	for (list_each(node, &w->children)) {
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
		snode = ui_widget_create_selector_node(child);
		logger_error(
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
		css_selector_node_destroy(snode);
		_ui_print_tree(child, depth + 1, child_prefix);
	}
}

void ui_print_tree(ui_widget_t* w)
{
	css_selector_node_t *node;
	w = w ? w : ui_root();
	node = ui_widget_create_selector_node(w);
	logger_error("%s, xy:(%g,%g), size:(%g,%g), visible: %s\n",
		     node->fullname, w->x, w->y, w->width, w->height,
		     w->computed_style.visible ? "true" : "false");
	css_selector_node_destroy(node);
	_ui_print_tree(w, 0, "  ");
}
