/*
 * widget_base.c -- The widget base operation set.
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
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>

static struct LCUI_WidgetModule {
	LCUI_Widget root; /**< 根级部件 */
	LinkedList trash; /**< 待删除的部件列表 */
} LCUIWidget;

LCUI_Widget LCUIWidget_GetRoot(void)
{
	return LCUIWidget.root;
}

size_t LCUIWidget_ClearTrash(void)
{
	size_t count;
	LinkedListNode *node;

	node = LCUIWidget.trash.head.next;
	count = LCUIWidget.trash.length;
	while (node) {
		LinkedListNode *next = node->next;
		LinkedList_Unlink(&LCUIWidget.trash, node);
		Widget_ExecDestroy(node->data);
		node = next;
	}
	return count;
}

static void Widget_AddToTrash(LCUI_Widget w)
{
	w->state = LCUI_WSTATE_DELETED;
	if (Widget_Unlink(w) != 0) {
		return;
	}
	LinkedList_AppendNode(&LCUIWidget.trash, &w->node);
	Widget_PostSurfaceEvent(w, LCUI_WEVENT_UNLINK, TRUE);
}

/** 构造函数 */
static void Widget_Init(LCUI_Widget widget)
{
	ZEROSET(widget, LCUI_Widget);
	widget->state = LCUI_WSTATE_CREATED;
	widget->style = StyleSheet();
	widget->computed_style.opacity = 1.0;
	widget->computed_style.visible = TRUE;
	widget->computed_style.focusable = FALSE;
	widget->computed_style.display = SV_BLOCK;
	widget->computed_style.position = SV_STATIC;
	widget->computed_style.pointer_events = SV_INHERIT;
	widget->computed_style.box_sizing = SV_CONTENT_BOX;
	widget->computed_style.margin.top.type = LCUI_STYPE_PX;
	widget->computed_style.margin.right.type = LCUI_STYPE_PX;
	widget->computed_style.margin.bottom.type = LCUI_STYPE_PX;
	widget->computed_style.margin.left.type = LCUI_STYPE_PX;
	widget->computed_style.padding.top.type = LCUI_STYPE_PX;
	widget->computed_style.padding.right.type = LCUI_STYPE_PX;
	widget->computed_style.padding.bottom.type = LCUI_STYPE_PX;
	widget->computed_style.padding.left.type = LCUI_STYPE_PX;
	LinkedList_Init(&widget->children);
	LinkedList_Init(&widget->children_show);
	widget->node.data = widget;
	widget->node_show.data = widget;
	widget->node.next = widget->node.prev = NULL;
	widget->node_show.next = widget->node_show.prev = NULL;
	Widget_InitBackground(widget);
}

LCUI_Widget LCUIWidget_NewWithPrototype(LCUI_WidgetPrototypeC proto)
{
	LCUI_Widget widget = malloc(sizeof(LCUI_WidgetRec));

	Widget_Init(widget);
	widget->proto = proto;
	widget->type = widget->proto->name;
	widget->proto->init(widget);
	Widget_AddTask(widget, LCUI_WTASK_REFRESH_STYLE);
	return widget;
}

LCUI_Widget LCUIWidget_New(const char *type)
{
	LCUI_Widget widget = malloc(sizeof(LCUI_WidgetRec));

	Widget_Init(widget);
	if (type) {
		widget->proto = LCUIWidget_GetPrototype(type);
		if (widget->proto) {
			widget->type = widget->proto->name;
			widget->proto->init(widget);
		} else {
			widget->type = strdup2(type);
		}
	}
	Widget_AddTask(widget, LCUI_WTASK_REFRESH_STYLE);
	return widget;
}

void Widget_ExecDestroy(LCUI_Widget w)
{
	if (w->parent) {
		Widget_AddTask(w->parent, LCUI_WTASK_REFLOW);
		Widget_Unlink(w);
	}
	Widget_DestroyBackground(w);
	Widget_DestroyEventTrigger(w);
	Widget_DestroyChildren(w);
	Widget_ClearPrototype(w);
	if (w->title) {
		free(w->title);
		w->title = NULL;
	}
	Widget_DestroyId(w);
	Widget_DestroyStyleSheets(w);
	Widget_DestroyAttributes(w);
	Widget_DestroyClasses(w);
	Widget_DestroyStatus(w);
	Widget_SetRules(w, NULL);
	free(w);
}

void Widget_Destroy(LCUI_Widget w)
{
	LCUI_Widget root = w;

	assert(w->state != LCUI_WSTATE_DELETED);
	while (root->parent) {
		root = root->parent;
	}
	/* If this widget is not mounted in the root widget tree */
	if (root != LCUIWidget.root) {
		w->state = LCUI_WSTATE_DELETED;
		Widget_ExecDestroy(w);
		return;
	}
	if (w->parent) {
		LCUI_Widget child;
		LinkedListNode *node;

		/* Update the index of the siblings behind it */
		node = w->node.next;
		while (node) {
			child = node->data;
			child->index -= 1;
			node = node->next;
		}
		if (w->computed_style.position != SV_ABSOLUTE) {
			Widget_AddTask(w->parent, LCUI_WTASK_REFLOW);
		}
		Widget_InvalidateArea(w, NULL, SV_GRAPH_BOX);
		Widget_AddToTrash(w);
	}
}

void Widget_Empty(LCUI_Widget w)
{
	LCUI_Widget root = w;
	LCUI_Widget child;
	LinkedListNode *node;
	LCUI_WidgetEventRec ev;

	while (root->parent) {
		root = root->parent;
	}
	if (root != LCUIWidget.root) {
		Widget_DestroyChildren(w);
		return;
	}
	LCUI_InitWidgetEvent(&ev, "unlink");
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;
		Widget_TriggerEvent(child, &ev, NULL);
		if (child->parent == root) {
			Widget_PostSurfaceEvent(child, LCUI_WEVENT_UNLINK,
						TRUE);
		}
		child->state = LCUI_WSTATE_DELETED;
		child->parent = NULL;
	}
	LinkedList_ClearData(&w->children_show, NULL);
	LinkedList_Concat(&LCUIWidget.trash, &w->children);
	Widget_InvalidateArea(w, NULL, SV_GRAPH_BOX);
	Widget_UpdateStyle(w, TRUE);
}

void Widget_GetOffset(LCUI_Widget w, LCUI_Widget parent, float *offset_x,
		      float *offset_y)
{
	float x = 0, y = 0;
	while (w != parent) {
		x += w->box.border.x;
		y += w->box.border.y;
		w = w->parent;
		if (w) {
			x += w->box.padding.x - w->box.border.x;
			y += w->box.padding.y - w->box.border.y;
		} else {
			break;
		}
	}
	*offset_x = x;
	*offset_y = y;
}

/* FIXME: rename this function */
int Widget_Top(LCUI_Widget w)
{
	return Widget_Append(LCUIWidget.root, w);
}

void Widget_SetTitleW(LCUI_Widget w, const wchar_t *title)
{
	size_t len;
	wchar_t *new_title, *old_title;

	len = wcslen(title) + 1;
	new_title = (wchar_t *)malloc(sizeof(wchar_t) * len);
	if (!new_title) {
		return;
	}
	wcsncpy(new_title, title, len);
	old_title = w->title;
	w->title = new_title;
	if (old_title) {
		free(old_title);
	}
	Widget_AddTask(w, LCUI_WTASK_TITLE);
}

LCUI_BOOL Widget_InVisibleArea(LCUI_Widget w)
{
	LinkedListNode *node;
	LCUI_RectF rect;
	LCUI_Widget self, parent, child;
	LCUI_WidgetStyle *style;

	rect = w->box.padding;
	/* If the size of the widget is not fixed, then set the maximum size to
	 * avoid it being judged invisible all the time. */
	if (rect.width < 1 && Widget_HasAutoStyle(w, key_width)) {
		rect.width = w->parent->box.padding.width;
	}
	if (rect.height < 1 && Widget_HasAutoStyle(w, key_height)) {
		rect.height = w->parent->box.padding.height;
	}
	for (self = w, parent = w->parent; parent;
	     self = parent, parent = parent->parent) {
		if (!Widget_IsVisible(parent)) {
			return FALSE;
		}
		for (node = self->node_show.prev; node && node->prev;
		     node = node->prev) {
			child = node->data;
			style = &child->computed_style;
			if (child->state < LCUI_WSTATE_LAYOUTED ||
			    child == self || !Widget_IsVisible(child)) {
				continue;
			}
			DEBUG_MSG("rect: (%g,%g,%g,%g), child rect: "
				  "(%g,%g,%g,%g), child: %s %s\n",
				  rect.x, rect.y, rect.width, rect.height,
				  child->box.border.x, child->box.border.y,
				  child->box.border.width,
				  child->box.border.height, child->type,
				  child->id);
			if (!LCUIRectF_IsIncludeRect(&child->box.border,
						     &rect)) {
				continue;
			}
			if (style->opacity == 1.0f &&
			    style->background.color.alpha == 255) {
				return FALSE;
			}
		}
		rect.x += parent->box.padding.x;
		rect.y += parent->box.padding.y;
		LCUIRectF_ValidateArea(&rect, parent->box.padding.width,
				       parent->box.padding.height);
		if (rect.width < 1 || rect.height < 1) {
			return FALSE;
		}
	}
	return TRUE;
}

void Widget_GenerateSelfHash(LCUI_Widget widget)
{
	int i;
	unsigned hash = 1080;
	LCUI_Widget w;

	for (w = widget; w; w = w->parent) {
		if (w != widget) {
			hash = strhash(hash, " ");
		}
		if (w->type) {
			hash = strhash(hash, w->type);
		} else {
			hash = strhash(hash, "*");
		}
		if (w->id) {
			hash = strhash(hash, "#");
			hash = strhash(hash, w->id);
		}
		if (w->classes) {
			for (i = 0; w->classes[i]; ++i) {
				hash = strhash(hash, ".");
				hash = strhash(hash, w->classes[i]);
			}
		}
		if (w->status) {
			for (i = 0; w->status[i]; ++i) {
				hash = strhash(hash, ":");
				hash = strhash(hash, w->status[i]);
			}
		}
		if (w->rules && w->rules->cache_children_style) {
			break;
		}
	}
	widget->hash = hash;
}

void Widget_GenerateHash(LCUI_Widget w)
{
	LinkedListNode *node;

	Widget_GenerateSelfHash(w);
	for (LinkedList_Each(node, &w->children)) {
		Widget_GenerateHash(node->data);
	}
}

size_t Widget_SetHashList(LCUI_Widget w, unsigned *hash_list, size_t len)
{
	size_t count = 0;
	LCUI_Widget child;

	child = w;
	if (hash_list) {
		child->hash = hash_list[count];
	}
	++count;
	if (len > 0 && count >= len) {
		return count;
	}
	while (child->children.length > 0) {
		child = child->children.head.next->data;
	}
	while (child != w) {
		while (child->children.length > 0) {
			child = child->children.head.next->data;
		}
		if (hash_list) {
			child->hash = hash_list[count];
		}
		++count;
		if (len > 0 && count >= len) {
			break;
		}
		if (child->node.next) {
			child = child->node.next->data;
			continue;
		}
		do {
			child = child->parent;
			if (child == w) {
				break;
			}
			if (child->node.next) {
				child = child->node.next->data;
				break;
			}
		} while (1);
	}
	return count;
}

size_t Widget_GetHashList(LCUI_Widget w, unsigned *hash_list, size_t maxlen)
{
	size_t count = 0;
	LCUI_Widget child;

	child = w;
	if (hash_list) {
		hash_list[count] = child->hash;
	}
	++count;
	if (maxlen > 0 && count >= maxlen) {
		return count;
	}
	while (child->children.length > 0) {
		child = child->children.head.next->data;
	}
	while (child != w) {
		while (child->children.length > 0) {
			child = child->children.head.next->data;
		}
		if (hash_list) {
			hash_list[count] = child->hash;
		}
		++count;
		if (maxlen > 0 && count >= maxlen) {
			break;
		}
		if (child->node.next) {
			child = child->node.next->data;
			continue;
		}
		do {
			child = child->parent;
			if (child == w) {
				break;
			}
			if (child->node.next) {
				child = child->node.next->data;
				break;
			}
		} while (1);
	}
	return count;
}

int Widget_SetRules(LCUI_Widget w, const LCUI_WidgetRulesRec *rules)
{
	LCUI_WidgetRulesData data;

	data = (LCUI_WidgetRulesData)w->rules;
	if (data) {
		Dict_Release(data->style_cache);
		free(data);
		w->rules = NULL;
	}
	if (!rules) {
		return 0;
	}
	data = malloc(sizeof(LCUI_WidgetRulesDataRec));
	if (!data) {
		return -ENOMEM;
	}
	data->rules = *rules;
	data->progress = 0;
	data->style_cache = NULL;
	data->default_max_update_count = 2048;
	w->rules = (LCUI_WidgetRules)data;
	return 0;
}

void Widget_AddState(LCUI_Widget w, LCUI_WidgetState state)
{
	/* 如果部件还处于未准备完毕的状态 */
	if (w->state < LCUI_WSTATE_READY) {
		w->state |= state;
		/* 如果部件已经准备完毕则触发 ready 事件 */
		if (w->state == LCUI_WSTATE_READY) {
			LCUI_WidgetEventRec e = { 0 };
			e.type = LCUI_WEVENT_READY;
			e.cancel_bubble = TRUE;
			Widget_TriggerEvent(w, &e, NULL);
			w->state = LCUI_WSTATE_NORMAL;
		}
	}
}

float Widget_ComputeXMetric(LCUI_Widget w, int key)
{
	LCUI_Style s = &w->style->sheet[key];

	if (s->type == LCUI_STYPE_SCALE) {
		if (!w->parent) {
			return 0;
		}
		if (Widget_HasAbsolutePosition(w)) {
			return w->parent->box.padding.width * s->scale;
		}
		return w->parent->box.content.width * s->scale;
	}
	return LCUIMetrics_Compute(s->value, s->type);
}

float Widget_ComputeYMetric(LCUI_Widget w, int key)
{
	LCUI_Style s = &w->style->sheet[key];

	if (s->type == LCUI_STYPE_SCALE) {
		if (!w->parent) {
			return 0;
		}
		if (Widget_HasAbsolutePosition(w)) {
			return w->parent->box.padding.height * s->scale;
		}
		return w->parent->box.content.height * s->scale;
	}
	return LCUIMetrics_Compute(s->value, s->type);
}

void Widget_SortChildrenShow(LCUI_Widget w)
{
	LCUI_Widget child, target;
	LCUI_WidgetStyle *s, *ts;
	LinkedListNode *node, *target_node;
	LinkedList *list;

	list = &w->children_show;
	LinkedList_ClearData(list, NULL);
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;
		s = &child->computed_style;
		if (child->state < LCUI_WSTATE_READY) {
			continue;
		}
		for (LinkedList_Each(target_node, list)) {
			target = target_node->data;
			ts = &target->computed_style;
			if (s->z_index == ts->z_index) {
				if (s->position == ts->position) {
					if (child->index < target->index) {
						continue;
					}
				} else if (s->position < ts->position) {
					continue;
				}
			} else if (s->z_index < ts->z_index) {
				continue;
			}
			LinkedList_Link(list, target_node->prev,
					&child->node_show);
			break;
		}
		if (!target_node) {
			LinkedList_AppendNode(list, &child->node_show);
		}
	}
}

LCUI_BOOL Widget_HasAutoStyle(LCUI_Widget w, int key)
{
	return !Widget_CheckStyleValid(w, key) ||
	       Widget_CheckStyleType(w, key, AUTO);
}

LCUI_BOOL Widget_HasStaticWidthParent(LCUI_Widget widget)
{
	LCUI_Widget w;
	for (w = widget->parent; w; w = w->parent) {
		if (w->computed_style.max_width >= 0) {
			return TRUE;
		}
		if (!Widget_HasAutoStyle(w, key_width)) {
			return TRUE;
		}
		if (Widget_HasAbsolutePosition(w) ||
		    Widget_HasInlineBlockDisplay(w)) {
			return FALSE;
		}
	}
	return FALSE;
}

LCUI_BOOL Widget_HasFitContentWidth(LCUI_Widget w)
{
	if (!Widget_HasAutoStyle(w, key_width)) {
		return FALSE;
	}
	if (Widget_HasInlineBlockDisplay(w)) {
		return TRUE;
	}
	if (Widget_HasAbsolutePosition(w) || !Widget_HasStaticWidthParent(w)) {
		return TRUE;
	}
	return FALSE;
}

void Widget_SetText(LCUI_Widget w, const char *text)
{
	if (w->proto && w->proto->settext) {
		w->proto->settext(w, text);
	}
}

void Widget_BindProperty(LCUI_Widget w, const char *name, LCUI_Object value)
{
	if (w->proto && w->proto->bindprop) {
		w->proto->bindprop(w, name, value);
	}
}

void Widget_UpdateBoxPosition(LCUI_Widget w)
{
	float x = w->layout_x;
	float y = w->layout_y;

	switch (w->computed_style.position) {
	case SV_ABSOLUTE:
		if (!Widget_HasAutoStyle(w, key_left)) {
			x = w->computed_style.left;
		} else if (!Widget_HasAutoStyle(w, key_right)) {
			if (w->parent) {
				x = w->parent->box.border.width - w->width;
			}
			x -= w->computed_style.right;
		}
		if (!Widget_HasAutoStyle(w, key_top)) {
			y = w->computed_style.top;
		} else if (!Widget_HasAutoStyle(w, key_bottom)) {
			if (w->parent) {
				y = w->parent->box.border.height - w->height;
			}
			y -= w->computed_style.bottom;
		}
		break;
	case SV_RELATIVE:
		if (!Widget_HasAutoStyle(w, key_left)) {
			x += w->computed_style.left;
		} else if (!Widget_HasAutoStyle(w, key_right)) {
			x -= w->computed_style.right;
		}
		if (!Widget_HasAutoStyle(w, key_top)) {
			y += w->computed_style.top;
		} else if (!Widget_HasAutoStyle(w, key_bottom)) {
			y -= w->computed_style.bottom;
		}
	case SV_STATIC:
	default:
		break;
	}
	w->box.outer.x = x;
	w->box.outer.y = y;
	w->x = x + w->margin.left;
	w->y = y + w->margin.top;
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.padding.x = w->x + w->computed_style.border.left.width;
	w->box.padding.y = w->y + w->computed_style.border.top.width;
	w->box.content.x = w->box.padding.x + w->padding.left;
	w->box.content.y = w->box.padding.y + w->padding.top;
	w->box.canvas.x = w->x - Widget_GetBoxShadowOffsetX(w);
	w->box.canvas.y = w->y - Widget_GetBoxShadowOffsetY(w);
}

void Widget_UpdateCanvasBox(LCUI_Widget w)
{
	w->box.canvas.x = w->box.border.x - Widget_GetBoxShadowOffsetX(w);
	w->box.canvas.y = w->box.border.y - Widget_GetBoxShadowOffsetY(w);
	w->box.canvas.width = Widget_GetCanvasWidth(w);
	w->box.canvas.height = Widget_GetCanvasHeight(w);
}

void Widget_UpdateBoxSize(LCUI_Widget w)
{
	const float padding_x = w->padding.left + w->padding.right;
	const float padding_y = w->padding.top + w->padding.bottom;
	const float border_x = w->computed_style.border.left.width +
			       w->computed_style.border.right.width;
	const float border_y = w->computed_style.border.top.width +
			       w->computed_style.border.bottom.width;
	const float margin_x = w->margin.left + w->margin.right;
	const float margin_y = w->margin.top + w->margin.bottom;

	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.padding.width = w->box.border.width - border_x;
	w->box.padding.height = w->box.border.height - border_y;
	w->box.content.width = w->box.padding.width - padding_x;
	w->box.content.height = w->box.padding.height - padding_y;
	w->box.outer.width = w->box.border.width + margin_x;
	w->box.outer.height = w->box.border.height + margin_y;
	w->box.canvas.width = Widget_GetCanvasWidth(w);
	w->box.canvas.height = Widget_GetCanvasHeight(w);
}

void Widget_SetBorderBoxSize(LCUI_Widget w, float width, float height)
{
	w->width = Widget_GetLimitedWidth(w, width);
	w->height = Widget_GetLimitedHeight(w, height);
	Widget_UpdateBoxSize(w);
}

void LCUIWidget_InitBase(void)
{
	LinkedList_Init(&LCUIWidget.trash);
	LCUIWidget.root = LCUIWidget_New("root");
	Widget_SetTitleW(LCUIWidget.root, L"LCUI Display");
}

void LCUIWidget_FreeRoot(void)
{
	Widget_ExecDestroy(LCUIWidget.root);
}

void LCUIWidget_FreeBase(void)
{
	LCUIWidget.root = NULL;
}
