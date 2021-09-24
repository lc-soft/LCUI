﻿/*
 * widget_base.c -- The widget base operation set.
 *
 * Copyright (c) 2018-2020, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>
#include "widget_background.h"
#include "widget_shadow.h"

static struct LCUI_WidgetModule {
	LCUI_Widget root; /**< 根级部件 */
	list_t trash; /**< 待删除的部件列表 */
} LCUIWidget;

LCUI_Widget LCUIWidget_GetRoot(void)
{
	return LCUIWidget.root;
}

size_t LCUIWidget_ClearTrash(void)
{
	size_t count;
	list_node_t *node;

	node = LCUIWidget.trash.head.next;
	count = LCUIWidget.trash.length;
	while (node) {
		list_node_t *next = node->next;
		list_unlink(&LCUIWidget.trash, node);
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
	list_append_node(&LCUIWidget.trash, &w->node);
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
	list_create(&widget->children);
	list_create(&widget->children_show);
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
	widget->proto = LCUIWidget_GetPrototype(type);
	if (widget->proto->name) {
		widget->type = widget->proto->name;
	} else if (type) {
		widget->type = strdup2(type);
	}
	widget->proto->init(widget);
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
		list_node_t *node;

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
		Widget_InvalidateArea(w->parent, &w->box.canvas,
				      SV_CONTENT_BOX);
		Widget_AddToTrash(w);
	}
}

void Widget_Empty(LCUI_Widget w)
{
	LCUI_Widget root = w;
	LCUI_Widget child;
	list_node_t *node;
	LCUI_WidgetEventRec ev;

	while (root->parent) {
		root = root->parent;
	}
	if (root != LCUIWidget.root) {
		Widget_DestroyChildren(w);
		return;
	}
	LCUI_InitWidgetEvent(&ev, "unlink");
	for (list_each(node, &w->children)) {
		child = node->data;
		Widget_TriggerEvent(child, &ev, NULL);
		if (child->parent == root) {
			Widget_PostSurfaceEvent(child, LCUI_WEVENT_UNLINK,
						TRUE);
		}
		child->state = LCUI_WSTATE_DELETED;
		child->parent = NULL;
	}
	list_destroy_without_node(&w->children_show, NULL);
	list_concat(&LCUIWidget.trash, &w->children);
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
	list_node_t *node;
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

int Widget_SetRules(LCUI_Widget w, const LCUI_WidgetRulesRec *rules)
{
	LCUI_WidgetRulesData data;

	data = (LCUI_WidgetRulesData)w->rules;
	if (data) {
		dict_destroy(data->style_cache);
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
	list_node_t *node, *target_node;
	list_t *list;

	list = &w->children_show;
	list_destroy_without_node(list, NULL);
	for (list_each(node, &w->children)) {
		child = node->data;
		s = &child->computed_style;
		if (child->state < LCUI_WSTATE_READY) {
			continue;
		}
		for (list_each(target_node, list)) {
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
			list_link(list, target_node->prev,
					&child->node_show);
			break;
		}
		if (!target_node) {
			list_append_node(list, &child->node_show);
		}
	}
}

LCUI_BOOL Widget_HasAutoStyle(LCUI_Widget w, int key)
{
	return !Widget_CheckStyleValid(w, key) ||
	       Widget_CheckStyleType(w, key, AUTO);
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
	if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_NONE &&
	    (w->x != w->box.border.x || w->y != w->box.border.y)) {
		LCUI_Widget parent = w->parent;

		w->invalid_area = w->box.canvas;
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		while (parent) {
			parent->has_child_invalid_area = TRUE;
			parent = parent->parent;
		}
	}
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
	w->width = Widget_GetLimitedWidth(w, w->width);
	w->height = Widget_GetLimitedHeight(w, w->height);
	if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_NONE &&
	    (w->width != w->box.border.width ||
	     w->height != w->box.border.height)) {
		LCUI_Widget parent = w->parent;

		w->invalid_area = w->box.canvas;
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		while (parent) {
			parent->has_child_invalid_area = TRUE;
			parent = parent->parent;
		}
	}
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.padding.width = w->box.border.width - Widget_BorderX(w);
	w->box.padding.height = w->box.border.height - Widget_BorderY(w);
	w->box.content.width = w->box.padding.width - Widget_PaddingX(w);
	w->box.content.height = w->box.padding.height - Widget_PaddingY(w);
	w->box.outer.width = w->box.border.width + Widget_MarginX(w);
	w->box.outer.height = w->box.border.height + Widget_MarginY(w);
	w->box.canvas.width = Widget_GetCanvasWidth(w);
	w->box.canvas.height = Widget_GetCanvasHeight(w);
}

void LCUIWidget_InitBase(void)
{
	list_create(&LCUIWidget.trash);
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
