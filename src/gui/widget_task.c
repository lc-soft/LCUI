/*
 * widget_task.c -- LCUI widget task module.
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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

static struct WidgetTaskModule {
	DictType style_cache_dict;
	unsigned max_updates_per_frame;
	LCUI_WidgetFunction handlers[LCUI_WTASK_TOTAL_NUM];
	unsigned update_count;
} self;

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

static void HandleRefreshStyle(LCUI_Widget w)
{
	int i;

	Widget_ExecUpdateStyle(w, TRUE);
	for (i = LCUI_WTASK_UPDATE_STYLE + 1; i < LCUI_WTASK_TOTAL_NUM; ++i) {
		w->task.states[i] = TRUE;
	}
	w->task.states[LCUI_WTASK_UPDATE_STYLE] = FALSE;
}

static void HandleUpdateStyle(LCUI_Widget w)
{
	Widget_ExecUpdateStyle(w, FALSE);
}

static void HandleSetTitle(LCUI_Widget w)
{
	Widget_PostSurfaceEvent(w, LCUI_WEVENT_TITLE, TRUE);
}

/** 处理主体刷新（标记主体区域为脏矩形，但不包括阴影区域） */
static void HandleBody(LCUI_Widget w)
{
	Widget_InvalidateArea(w, NULL, SV_BORDER_BOX);
}

/** 处理刷新（标记整个部件区域为脏矩形） */
static void HandleRefresh(LCUI_Widget w)
{
	DEBUG_MSG("refresh\n");
	Widget_InvalidateArea(w, NULL, SV_GRAPH_BOX);
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
	SetHandler(VISIBLE, Widget_UpdateVisibility);
	SetHandler(POSITION, Widget_UpdatePosition);
	SetHandler(RESIZE, Widget_UpdateSize);
	SetHandler(RESIZE_WITH_SURFACE, Widget_UpdateSizeWithSurface);
	SetHandler(SHADOW, Widget_UpdateBoxShadow);
	SetHandler(BORDER, Widget_UpdateBorder);
	SetHandler(OPACITY, Widget_UpdateOpacity);
	SetHandler(MARGIN, Widget_UpdateMargin);
	SetHandler(BODY, HandleBody);
	SetHandler(TITLE, HandleSetTitle);
	SetHandler(REFRESH, HandleRefresh);
	SetHandler(UPDATE_STYLE, HandleUpdateStyle);
	SetHandler(REFRESH_STYLE, HandleRefreshStyle);
	SetHandler(BACKGROUND, Widget_UpdateBackground);
	SetHandler(LAYOUT, Widget_ExecUpdateLayout);
	SetHandler(ZINDEX, Widget_ExecUpdateZIndex);
	SetHandler(DISPLAY, Widget_UpdateDisplay);
	SetHandler(PROPS, Widget_UpdateProps);
	InitStylesheetCacheDict();
	self.max_updates_per_frame = 4;
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
	LCUI_WidgetTaskContext parent;

	self_ctx = malloc(sizeof(LCUI_WidgetTaskContextRec));
	if (!self_ctx) {
		return NULL;
	}
	self_ctx->parent = ctx;
	self_ctx->style_cache = NULL;
	for (parent = ctx; parent; parent = parent->parent) {
		if (parent->style_cache) {
			self_ctx->style_cache = parent->style_cache;
			self_ctx->style_hash = parent->style_hash;
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
			DEBUG_MSG("first update visible children count: %zu\n",
				  total);
		}
	}
	if (!w->task.for_children) {
		return 0;
	}
	/* 如果子级部件中有待处理的部件，则递归进去 */
	w->task.for_children = FALSE;
	while (node) {
		child = node->data;
		/* 如果当前部件有销毁任务，结点空间会连同部件一起被
		 * 释放，为避免因访问非法空间而出现异常，预先保存下
		 * 个结点。
		 */
		next = node->next;
		/* 如果该级部件的任务需要留到下次再处理 */
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
						 self.max_updates_per_frame /
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

size_t Widget_UpdateWithContext(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	int i;
	size_t count = 0;
	LCUI_BOOL *states;
	LCUI_WidgetTaskContext self_ctx;

	if (!w->task.for_self && !w->task.for_children) {
		return 0;
	}
	self_ctx = Widget_BeginUpdate(w, ctx);
	if (w->task.for_self) {
		w->task.for_self = FALSE;
		states = w->task.states;
		/* 如果有用户自定义任务 */
		if (states[LCUI_WTASK_USER] && w->proto && w->proto->runtask) {
			w->proto->runtask(w);
			if (self_ctx->profile) {
				self_ctx->profile->user_task_count += 1;
			}
		}
		if (self_ctx->profile) {
			if (states[LCUI_WTASK_REFRESH_STYLE]) {
				self_ctx->profile->refresh_count += 1;
			}
			if (states[LCUI_WTASK_LAYOUT]) {
				self_ctx->profile->layout_count += 1;
			}
			self_ctx->profile->update_count += 1;
		}
		for (i = 0; i < LCUI_WTASK_USER; ++i) {
			if (states[i]) {
				states[i] = FALSE;
				if (self.handlers[i]) {
					self.handlers[i](w);
				}
			} else {
				states[i] = FALSE;
			}
		}
		Widget_AddState(w, LCUI_WSTATE_UPDATED);
		count += 1;
	}
	count += Widget_UpdateChildren(w, self_ctx);
	Widget_SortChildrenShow(w);
	Widget_EndUpdate(self_ctx);
	return count;
}

size_t Widget_Update(LCUI_Widget w)
{
	return Widget_UpdateWithContext(w, NULL);
}

size_t LCUIWidget_Update(void)
{
	size_t i, count;
	LCUI_Widget root;

	/* 前两次更新需要主动刷新所有部件的样式，主要是为了省去在应用程序里手动调用
	 * LCUIWidget_RefreshStyle() 的麻烦 */
	if (self.update_count < 2) {
		LCUIWidget_RefreshStyle();
		self.update_count += 1;
	}
	root = LCUIWidget_GetRoot();
	for (count = i = 0; i < self.max_updates_per_frame; ++i) {
		count = Widget_Update(root);
	}
	LCUIWidget_ClearTrash();
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
	size_t i;
	LCUI_Widget root;

	profile->time = clock();
	/* 初次更新时主动刷新所有部件的样式，为了省去在应用程序里手动调用
	 * LCUIWidget_RefreshStyle() 的麻烦 */
	if (self.update_count < 1) {
		LCUIWidget_RefreshStyle();
		self.update_count += 1;
	}
	root = LCUIWidget_GetRoot();
	for (i = 0; i < self.max_updates_per_frame; ++i) {
		Widget_UpdateWithProfile(root, profile);
	}
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
