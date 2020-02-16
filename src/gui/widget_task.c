/*
 * widget_task.c -- LCUI widget task module.
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>
#include "widget_diff.h"
#include "widget_border.h"

typedef struct LCUI_WidgetTaskContextRec_ *LCUI_WidgetTaskContext;

typedef struct LCUI_WidgetTaskContextRec_ {
	unsigned style_hash;
	Dict *style_cache;
	LCUI_WidgetStyleDiffRec style_diff;
	LCUI_WidgetLayoutDiffRec layout_diff;
	LCUI_WidgetTaskContext parent;
	LCUI_WidgetTasksProfile profile;
} LCUI_WidgetTaskContextRec;

static struct WidgetTaskModule {
	DictType style_cache_dict;
	LCUI_MetricsRec metrics;
	LCUI_BOOL refresh_all;
	LCUI_WidgetFunction handlers[LCUI_WTASK_TOTAL_NUM];
} self;

static size_t Widget_UpdateWithContext(LCUI_Widget w,
				       LCUI_WidgetTaskContext ctx);

static unsigned int IntKeyDict_HashFunction(const void *key)
{
	return Dict_IdentityHashFunction(*(unsigned int *)key);
}

static int IntKeyDict_KeyCompare(void *privdata, const void *key1,
				 const void *key2)
{
	return *(unsigned int *)key1 == *(unsigned int *)key2;
}

static void IntKeyDict_KeyDestructor(void *privdata, void *key)
{
	free(key);
}

static void *IntKeyDict_KeyDup(void *privdata, const void *key)
{
	unsigned int *newkey = malloc(sizeof(unsigned int));
	*newkey = *(unsigned int *)key;
	return newkey;
}

static void StyleSheetCacheDestructor(void *privdata, void *val)
{
	StyleSheet_Delete(val);
}

static void InitStylesheetCacheDict(void)
{
	DictType *dt = &self.style_cache_dict;

	dt->valDup = NULL;
	dt->keyDup = IntKeyDict_KeyDup;
	dt->keyCompare = IntKeyDict_KeyCompare;
	dt->hashFunction = IntKeyDict_HashFunction;
	dt->keyDestructor = IntKeyDict_KeyDestructor;
	dt->valDestructor = StyleSheetCacheDestructor;
	dt->keyDestructor = IntKeyDict_KeyDestructor;
}

static void Widget_OnRefreshStyle(LCUI_Widget w)
{
	int i;

	Widget_ExecUpdateStyle(w, TRUE);
	for (i = LCUI_WTASK_UPDATE_STYLE + 1; i < LCUI_WTASK_REFLOW; ++i) {
		w->task.states[i] = TRUE;
	}
	w->task.states[LCUI_WTASK_UPDATE_STYLE] = FALSE;
}

static void Widget_OnUpdateStyle(LCUI_Widget w)
{
	Widget_ExecUpdateStyle(w, FALSE);
}

static void Widget_OnSetTitle(LCUI_Widget w)
{
	Widget_PostSurfaceEvent(w, LCUI_WEVENT_TITLE, TRUE);
}

void Widget_UpdateTaskStatus(LCUI_Widget widget)
{
	int i;

	for (i = 0; i < LCUI_WTASK_TOTAL_NUM; ++i) {
		if (widget->task.states[i]) {
			break;
		}
	}
	if (i >= LCUI_WTASK_TOTAL_NUM) {
		return;
	}
	widget->task.for_self = TRUE;
	while (widget && !widget->task.for_children) {
		widget->task.for_children = TRUE;
		widget = widget->parent;
	}
}

void Widget_AddTaskForChildren(LCUI_Widget widget, int task)
{
	LCUI_Widget child;
	LinkedListNode *node;

	widget->task.for_children = TRUE;
	for (LinkedList_Each(node, &widget->children)) {
		child = node->data;
		Widget_AddTask(child, task);
		Widget_AddTaskForChildren(child, task);
	}
}

void Widget_AddTask(LCUI_Widget widget, int task)
{
	if (widget->state == LCUI_WSTATE_DELETED) {
		return;
	}
	DEBUG_MSG("[%lu] %s, %d\n", widget->index, widget->type, task);
	widget->task.for_self = TRUE;
	widget->task.states[task] = TRUE;
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while (widget && !widget->task.for_children) {
		widget->task.for_children = TRUE;
		widget = widget->parent;
	}
}

void LCUIWidget_InitTasks(void)
{
#define SetHandler(NAME, HANDLER) self.handlers[LCUI_WTASK_##NAME] = HANDLER
	SetHandler(VISIBLE, Widget_ComputeVisibilityStyle);
	SetHandler(POSITION, Widget_ComputePositionStyle);
	SetHandler(RESIZE, Widget_ComputeSizeStyle);
	SetHandler(SHADOW, Widget_ComputeBoxShadowStyle);
	SetHandler(BORDER, Widget_ComputeBorderStyle);
	SetHandler(OPACITY, Widget_ComputeOpacityStyle);
	SetHandler(MARGIN, Widget_ComputeMarginStyle);
	SetHandler(PADDING, Widget_ComputePaddingStyle);
	SetHandler(BACKGROUND, Widget_ComputeBackgroundStyle);
	SetHandler(ZINDEX, Widget_ComputeZIndexStyle);
	SetHandler(DISPLAY, Widget_ComputeDisplayStyle);
	SetHandler(FLEX, Widget_ComputeFlexBoxStyle);
	SetHandler(PROPS, Widget_ComputeProperties);
	SetHandler(UPDATE_STYLE, Widget_OnUpdateStyle);
	SetHandler(REFRESH_STYLE, Widget_OnRefreshStyle);
	SetHandler(TITLE, Widget_OnSetTitle);
	self.handlers[LCUI_WTASK_REFLOW] = NULL;
	InitStylesheetCacheDict();
	self.refresh_all = TRUE;
}

void LCUIWidget_FreeTasks(void)
{
	LCUIWidget_ClearTrash();
}

LCUI_WidgetTaskContext Widget_BeginUpdate(LCUI_Widget w,
					  LCUI_WidgetTaskContext ctx)
{
	unsigned hash;
	LCUI_Selector selector;
	LCUI_StyleSheet style;
	LCUI_WidgetRulesData data;
	LCUI_CachedStyleSheet inherited_style;
	LCUI_WidgetTaskContext self_ctx;
	LCUI_WidgetTaskContext parent_ctx;

	self_ctx = malloc(sizeof(LCUI_WidgetTaskContextRec));
	if (!self_ctx) {
		return NULL;
	}
	self_ctx->parent = ctx;
	self_ctx->style_cache = NULL;
	for (parent_ctx = ctx; parent_ctx; parent_ctx = parent_ctx->parent) {
		if (parent_ctx->style_cache) {
			self_ctx->style_cache = parent_ctx->style_cache;
			self_ctx->style_hash = parent_ctx->style_hash;
			break;
		}
	}
	if (ctx && ctx->profile) {
		self_ctx->profile = ctx->profile;
	} else {
		self_ctx->profile = NULL;
	}
	if (w->hash && w->task.states[LCUI_WTASK_REFRESH_STYLE]) {
		Widget_GenerateSelfHash(w);
	}
	if (!self_ctx->style_cache && w->rules &&
	    w->rules->cache_children_style) {
		data = (LCUI_WidgetRulesData)w->rules;
		if (!data->style_cache) {
			data->style_cache =
			    Dict_Create(&self.style_cache_dict, NULL);
		}
		Widget_GenerateSelfHash(w);
		self_ctx->style_hash = w->hash;
		self_ctx->style_cache = data->style_cache;
	}
	inherited_style = w->inherited_style;
	if (self_ctx->style_cache && w->hash) {
		hash = self_ctx->style_hash;
		hash = ((hash << 5) + hash) + w->hash;
		style = Dict_FetchValue(self_ctx->style_cache, &hash);
		if (!style) {
			style = StyleSheet();
			selector = Widget_GetSelector(w);
			LCUI_GetStyleSheet(selector, style);
			Dict_Add(self_ctx->style_cache, &hash, style);
			Selector_Delete(selector);
		}
		w->inherited_style = style;
	} else {
		selector = Widget_GetSelector(w);
		w->inherited_style = LCUI_GetCachedStyleSheet(selector);
		Selector_Delete(selector);
	}
	if (w->inherited_style != inherited_style) {
		Widget_AddTask(w, LCUI_WTASK_REFRESH_STYLE);
	}
	return self_ctx;
}

void Widget_EndUpdate(LCUI_WidgetTaskContext ctx)
{
	ctx->style_cache = NULL;
	ctx->parent = NULL;
	free(ctx);
}

static size_t Widget_UpdateVisibleChildren(LCUI_Widget w,
					   LCUI_WidgetTaskContext ctx)
{
	size_t total = 0, count;
	LCUI_BOOL found = FALSE;
	LCUI_RectF rect, visible_rect;
	LCUI_Widget child, parent;
	LinkedListNode *node, *next;

	rect = w->box.padding;
	if (rect.width < 1 && Widget_HasAutoStyle(w, key_width)) {
		rect.width = w->parent->box.padding.width;
	}
	if (rect.height < 1 && Widget_HasAutoStyle(w, key_height)) {
		rect.height = w->parent->box.padding.height;
	}
	for (child = w, parent = w->parent; parent;
	     child = parent, parent = parent->parent) {
		if (child == w) {
			continue;
		}
		rect.x += child->box.padding.x;
		rect.y += child->box.padding.y;
		LCUIRectF_ValidateArea(&rect, parent->box.padding.width,
				       parent->box.padding.height);
	}
	visible_rect = rect;
	rect = w->box.padding;
	Widget_GetOffset(w, NULL, &rect.x, &rect.y);
	if (!LCUIRectF_GetOverlayRect(&visible_rect, &rect, &visible_rect)) {
		return 0;
	}
	visible_rect.x -= w->box.padding.x;
	visible_rect.y -= w->box.padding.y;
	for (node = w->children.head.next; node; node = next) {
		child = node->data;
		next = node->next;
		if (!LCUIRectF_GetOverlayRect(&visible_rect, &child->box.border,
					      &rect)) {
			if (found) {
				break;
			}
			continue;
		}
		found = TRUE;
		count = Widget_UpdateWithContext(child, ctx);
		if (child->task.for_self || child->task.for_children) {
			w->task.for_children = TRUE;
		}
		total += count;
		node = next;
	}
	return total;
}

static size_t Widget_UpdateChildren(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	clock_t msec = 0;
	LCUI_Widget child;
	LCUI_WidgetRulesData data;
	LinkedListNode *node, *next;
	size_t total = 0, update_count = 0, count;

	if (!w->task.for_children) {
		return 0;
	}
	data = (LCUI_WidgetRulesData)w->rules;
	node = w->children.head.next;
	if (data) {
		msec = clock();
		if (data->rules.only_on_visible) {
			if (!Widget_InVisibleArea(w)) {
				DEBUG_MSG("%s %s: is not visible\n", w->type,
					  w->id);
				return 0;
			}
		}
		DEBUG_MSG("%s %s: is visible\n", w->type, w->id);
		if (data->rules.first_update_visible_children) {
			total += Widget_UpdateVisibleChildren(w, ctx);
			DEBUG_MSG("first update visible children "
				  "count: %zu\n",
				  total);
		}
	}
	if (!w->task.for_children) {
		return 0;
	}
	w->task.for_children = FALSE;
	while (node) {
		child = node->data;
		next = node->next;
		count = Widget_UpdateWithContext(child, ctx);
		if (child->task.for_self || child->task.for_children) {
			w->task.for_children = TRUE;
		}
		total += count;
		node = next;
		if (!data) {
			continue;
		}
		if (count > 0) {
			data->progress = max(child->index, data->progress);
			if (data->progress > w->children_show.length) {
				data->progress = child->index;
			}
			update_count += 1;
		}
		if (data->rules.max_update_children_count < 0) {
			continue;
		}
		if (data->rules.max_update_children_count > 0) {
			if (update_count >=
			    (size_t)data->rules.max_update_children_count) {
				w->task.for_children = TRUE;
				break;
			}
		}
		if (update_count < data->default_max_update_count) {
			continue;
		}
		w->task.for_children = TRUE;
		msec = (clock() - msec);
		if (msec < 1) {
			data->default_max_update_count += 128;
			continue;
		}
		data->default_max_update_count = update_count * CLOCKS_PER_SEC /
						 LCUI_MAX_FRAMES_PER_SEC / msec;
		if (data->default_max_update_count < 1) {
			data->default_max_update_count = 32;
		}
		break;
	}
	if (data) {
		if (!w->task.for_children) {
			data->progress = w->children_show.length;
		}
		if (data->rules.on_update_progress) {
			data->rules.on_update_progress(w, data->progress);
		}
	}
	return total;
}

static void Widget_UpdateSelf(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	int i;
	LCUI_BOOL *states;

	states = w->task.states;
	w->task.for_self = FALSE;
	for (i = 0; i < LCUI_WTASK_REFLOW; ++i) {
		if (states[i]) {
			if (w->proto && w->proto->runtask) {
				w->proto->runtask(w, i);
			}
			states[i] = FALSE;
			if (self.handlers[i]) {
				self.handlers[i](w);
			}
		}
	}
	if (states[LCUI_WTASK_USER] && w->proto && w->proto->runtask) {
		states[LCUI_WTASK_USER] = FALSE;
		w->proto->runtask(w, LCUI_WTASK_USER);
	}
	Widget_AddState(w, LCUI_WSTATE_UPDATED);
}

static size_t Widget_UpdateWithContext(LCUI_Widget w,
				       LCUI_WidgetTaskContext ctx)
{
	size_t count = 0;
	LCUI_WidgetTaskContext self_ctx;

	if (!w->task.for_self && !w->task.for_children) {
		return 0;
	}
	if (self.refresh_all) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	}
	self_ctx = Widget_BeginUpdate(w, ctx);
	Widget_BeginLayoutDiff(w, &self_ctx->layout_diff);
	if (w->task.for_self) {
		if (self.refresh_all) {
			memset(&self_ctx->style_diff, 0,
			       sizeof(LCUI_WidgetStyleDiffRec));
			Widget_InitStyleDiff(w, &self_ctx->style_diff);
		} else {
			Widget_InitStyleDiff(w, &self_ctx->style_diff);
			Widget_BeginStyleDiff(w, &self_ctx->style_diff);
		}
		Widget_UpdateSelf(w, self_ctx);
		Widget_EndStyleDiff(w, &self_ctx->style_diff);
	}
	if (w->task.for_children) {
		count += Widget_UpdateChildren(w, self_ctx);
	}
	if (w->task.states[LCUI_WTASK_REFLOW]) {
		Widget_Reflow(w, LCUI_LAYOUT_RULE_AUTO);
		w->task.states[LCUI_WTASK_REFLOW] = FALSE;
	}
	Widget_EndLayoutDiff(w, &self_ctx->layout_diff);
	Widget_EndUpdate(self_ctx);
	Widget_SortChildrenShow(w);
	return count;
}

size_t Widget_Update(LCUI_Widget w)
{
	size_t count;
	LCUI_WidgetTaskContext ctx;

	ctx = Widget_BeginUpdate(w, NULL);
	count = Widget_UpdateWithContext(w, ctx);
	Widget_EndUpdate(ctx);
	return count;
}

size_t LCUIWidget_Update(void)
{
	size_t count;
	LCUI_Widget root;
	const LCUI_MetricsRec *metrics;

	metrics = LCUI_GetMetrics();
	if (memcmp(metrics, &self.metrics, sizeof(LCUI_MetricsRec))) {
		self.refresh_all = TRUE;
	}
	if (self.refresh_all) {
		LCUIWidget_RefreshStyle();
	}
	root = LCUIWidget_GetRoot();
	count = Widget_Update(root);
	root->state = LCUI_WSTATE_NORMAL;
	LCUIWidget_ClearTrash();
	self.metrics = *metrics;
	self.refresh_all = FALSE;
	return count;
}

void Widget_UpdateWithProfile(LCUI_Widget w, LCUI_WidgetTasksProfile profile)
{
	LCUI_WidgetTaskContext ctx;

	ctx = Widget_BeginUpdate(w, NULL);
	ctx->profile = profile;
	Widget_UpdateWithContext(w, ctx);
	Widget_EndUpdate(ctx);
}

void LCUIWidget_UpdateWithProfile(LCUI_WidgetTasksProfile profile)
{
	LCUI_Widget root;
	const LCUI_MetricsRec *metrics;

	profile->time = clock();
	metrics = LCUI_GetMetrics();
	if (memcmp(metrics, &self.metrics, sizeof(LCUI_MetricsRec))) {
		self.refresh_all = TRUE;
	}
	if (self.refresh_all) {
		LCUIWidget_RefreshStyle();
	}
	if (self.refresh_all) {
		LCUIWidget_RefreshStyle();
	}
	root = LCUIWidget_GetRoot();
	Widget_UpdateWithProfile(root, profile);
	root->state = LCUI_WSTATE_NORMAL;
	profile->time = clock() - profile->time;
	profile->destroy_time = clock();
	profile->destroy_count = LCUIWidget_ClearTrash();
	profile->destroy_time = clock() - profile->destroy_time;
}

void LCUIWidget_RefreshStyle(void)
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	Widget_UpdateStyle(root, TRUE);
	Widget_AddTaskForChildren(root, LCUI_WTASK_REFRESH_STYLE);
}
