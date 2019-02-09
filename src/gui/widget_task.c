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
	LCUI_WidgetFunction handlers[LCUI_WTASK_TOTAL_NUM];
	unsigned update_count;
} self;

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
}

void LCUIWidget_FreeTasks(void)
{
	LCUIWidget_ClearTrash();
}

LCUI_WidgetTaskContext Widget_BeginUpdate(LCUI_Widget w,
					  LCUI_WidgetTaskContext ctx)
{
	LCUI_CachedStyleSheet inherited_style;
	LCUI_WidgetTaskContext self_ctx;

	self_ctx = malloc(sizeof(LCUI_WidgetTaskContextRec));
	if (!self_ctx) {
		return NULL;
	}
	if (ctx) {
		self_ctx->selector = Selector_Copy(ctx->selector);
		if (0 != Selector_AppendNode(self_ctx->selector,
					     Widget_GetSelectorNode(w))) {
			return NULL;
		}
	} else {
		self_ctx->selector = Widget_GetSelector(w);
	}
	inherited_style = w->inherited_style;
	w->inherited_style = LCUI_GetCachedStyleSheet(self_ctx->selector);
	if (w->inherited_style != inherited_style) {
		Widget_AddTask(w, LCUI_WTASK_REFRESH_STYLE);
	}
	self_ctx->widget = w;
	return self_ctx;
}

void Widget_EndUpdate(LCUI_WidgetTaskContext ctx)
{
	Selector_Delete(ctx->selector);
	ctx->selector = NULL;
	ctx->widget = NULL;
	free(ctx);
}

size_t Widget_UpdateWithContext(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	int i;
	clock_t msec;
	size_t total = 0, update_count = 0, count;

	LCUI_BOOL *states;
	LCUI_Widget child;
	LCUI_WidgetTaskContext self_ctx;
	LCUI_WidgetRulesData data;
	LinkedListNode *node, *next;

	if (!w->task.for_self && !w->task.for_children) {
		return 0;
	}
	data = (LCUI_WidgetRulesData)w->rules;
	if (data) {
		msec = clock();
	}
	self_ctx = Widget_BeginUpdate(w, ctx);
	if (w->task.for_self) {
		w->task.for_self = FALSE;
		states = w->task.states;
		/* 如果有用户自定义任务 */
		if (states[LCUI_WTASK_USER] && w->proto && w->proto->runtask) {
			w->proto->runtask(w);
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
		total += 1;
	}
	if (w->task.for_children) {
		/* 如果子级部件中有待处理的部件，则递归进去 */
		w->task.for_children = FALSE;
		node = w->children.head.next;
		while (node) {
			child = node->data;
			/* 如果当前部件有销毁任务，结点空间会连同部件一起被
			 * 释放，为避免因访问非法空间而出现异常，预先保存下
			 * 个结点。
			 */
			next = node->next;
			/* 如果该级部件的任务需要留到下次再处理 */
			count = Widget_UpdateWithContext(child, self_ctx);
			if (child->task.for_self || child->task.for_children) {
				w->task.for_children = TRUE;
			}
			total += count;
			node = next;
			if (!data) {
				continue;
			}
			if (count > 0) {
				data->current_index = max(data->current_index, child->index);
				update_count += 1;
			}
			if (data->rules.max_update_children_count < 0) {
				continue;
			}
			if (data->rules.max_update_children_count > 0) {
				if (update_count >=
				    data->rules.max_update_children_count) {
					w->task.for_children = TRUE;
					break;
				}
			}
			if (update_count < data->default_max_update_count) {
				continue;
			}
			w->task.for_children = TRUE;
			/*
			 * Conversion from:
			 * (1000 / LCUI_MAX_FRAMES_PER_SEC /
			 * ((clock() - start) / CLOCKS_PER_SEC * 1000 /
			 * update_count);
			 */
			msec = (clock() - msec);
			if (msec < 1) {
				data->default_max_update_count += 2048;	
				continue;
			}
			data->default_max_update_count =
			update_count * CLOCKS_PER_SEC /
			LCUI_MAX_FRAMES_PER_SEC / msec;
			if (data->default_max_update_count < 1) {
				data->default_max_update_count = 32;
			}
			break;
		}
	}
	if (data && data->rules.on_update_progress) {
		data->rules.on_update_progress(w, data->current_index);
	}
	Widget_EndUpdate(self_ctx);
	return total;
}

size_t Widget_Update(LCUI_Widget w)
{
	return Widget_UpdateWithContext(w, NULL);
}

size_t LCUIWidget_Update(void)
{
	int i;
	size_t total, count;
	LCUI_Widget root;

	/* 前两次更新需要主动刷新所有部件的样式，主要是为了省去在应用程序里手动调用
	 * LCUIWidget_RefreshStyle() 的麻烦 */
	if (self.update_count < 2) {
		LCUIWidget_RefreshStyle();
		self.update_count += 1;
	}
	root = LCUIWidget_GetRoot();
	for (total = 0, i = 0; i < 2; ++i) {
		count = Widget_Update(root);
		if (count < 1) {
			break;
		}
		total += count;
	}
	LCUIWidget_ClearTrash();
	return total;
}

void LCUIWidget_RefreshStyle(void)
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	Widget_UpdateStyle(root, TRUE);
	Widget_AddTaskForChildren(root, LCUI_WTASK_REFRESH_STYLE);
}
