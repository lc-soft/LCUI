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

#define MEMCMP(A, B) memcmp(A, B, sizeof(*(A)))

typedef struct LCUI_WidgetTaskContextRec_ *LCUI_WidgetTaskContext;
typedef struct LCUI_WidgetLayoutTaskRec_ *LCUI_WidgetLayoutTask;

/** for check widget difference */
typedef struct LCUI_WidgetTaskDiffRec_ {
	int z_index;
	int display;
	float left;
	float right;
	float top;
	float bottom;
	float width;
	float height;
	float opacity;
	LCUI_BOOL visible;
	LCUI_Rect2F margin;
	LCUI_Rect2F padding;
	LCUI_StyleValue position;
	LCUI_BorderStyle border;
	LCUI_BoxShadowStyle shadow;
	LCUI_BackgroundStyle background;
	LCUI_WidgetBoxModelRec box;

	int invalid_box;
	LCUI_BOOL can_render;
	LCUI_BOOL sync_props_to_surface;
	LCUI_BOOL should_add_invalid_area;
} LCUI_WidgetTaskDiffRec, *LCUI_WidgetTaskDiff;

typedef struct LCUI_WidgetLayoutTaskRec_ {
	LCUI_Widget widget;
	LinkedList children;
	LinkedListNode node;
	LCUI_WidgetLayoutTask parent;
	LCUI_WidgetLayoutContextRec ctx;
} LCUI_WidgetLayoutTaskRec;

typedef struct LCUI_WidgetTaskContextRec_ {
	unsigned style_hash;
	Dict *style_cache;
	LCUI_WidgetTaskDiffRec diff;
	LCUI_WidgetTaskContext parent;
	LCUI_WidgetTasksProfile profile;
	LCUI_WidgetLayoutTask layout_task;
} LCUI_WidgetTaskContextRec;

static struct WidgetTaskModule {
	unsigned max_updates_per_frame;
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

static int ComputeStyleOption(LCUI_Widget w, int key, int default_value)
{
	if (!w->style->sheet[key].is_valid ||
	    w->style->sheet[key].type != LCUI_STYPE_STYLE) {
		return default_value;
	}
	return w->style->sheet[key].style;
}

static void Widget_ComputePaddingStyle(LCUI_Widget w)
{
	int i;
	LCUI_Style s;
	LCUI_BoundBox *pbox = &w->computed_style.padding;
	struct {
		LCUI_Style sval;
		float *ival;
		int key;
	} pd_map[4] = { { &pbox->top, &w->padding.top, key_padding_top },
			{ &pbox->right, &w->padding.right, key_padding_right },
			{ &pbox->bottom, &w->padding.bottom,
			  key_padding_bottom },
			{ &pbox->left, &w->padding.left, key_padding_left } };

	/* 内边距的单位暂时都用 px  */
	for (i = 0; i < 4; ++i) {
		s = &w->style->sheet[pd_map[i].key];
		if (!s->is_valid) {
			pd_map[i].sval->type = LCUI_STYPE_PX;
			pd_map[i].sval->px = 0.0;
			*pd_map[i].ival = 0.0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].ival = LCUIMetrics_Compute(s->value, s->type);
	}
}

static void Widget_ComputeMarginStyle(LCUI_Widget w)
{
	int i;
	LCUI_Style s;
	LCUI_BoundBox *mbox = &w->computed_style.margin;
	struct {
		LCUI_Style sval;
		float *fval;
		int key;
	} pd_map[4] = { { &mbox->top, &w->margin.top, key_margin_top },
			{ &mbox->right, &w->margin.right, key_margin_right },
			{ &mbox->bottom, &w->margin.bottom, key_margin_bottom },
			{ &mbox->left, &w->margin.left, key_margin_left } };

	for (i = 0; i < 4; ++i) {
		s = &w->style->sheet[pd_map[i].key];
		if (!s->is_valid) {
			pd_map[i].sval->type = LCUI_STYPE_PX;
			pd_map[i].sval->px = 0.0;
			*pd_map[i].fval = 0.0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].fval = LCUIMetrics_Compute(s->value, s->type);
	}
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

static void Widget_ComputeFlexLayoutStyle(LCUI_Widget w)
{
	LCUI_FlexLayoutStyle *data = &w->computed_style.flex;
	LCUI_Style s = StyleSheet_GetStyle(w->style, key_justify_content);

	if (s->type != LCUI_STYPE_STYLE || !s->is_valid) {
		data->justify_content = SV_FLEX_START;
		return;
	}
	data->justify_content = s->val_style;
}

static void Widget_OnUpdateBorder(LCUI_Widget w)
{
	Widget_ComputeBorderStyle(w);
}

static void Widget_OnUpdateVisible(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_visibility];

	if (w->computed_style.display == SV_NONE) {
		w->computed_style.visible = FALSE;
	} else if (s->is_valid && s->type == LCUI_STYPE_STRING &&
		   strcmp(s->val_string, "hidden") == 0) {
		w->computed_style.visible = FALSE;
	} else {
		w->computed_style.visible = TRUE;
	}
}

static void Widget_OnUpdateDisplay(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_display];
	LCUI_WidgetStyle *style = &w->computed_style;

	if (s->is_valid && s->type == LCUI_STYPE_STYLE) {
		style->display = s->style;
		if (style->display == SV_NONE) {
			w->computed_style.visible = FALSE;
		}
	} else {
		style->display = SV_BLOCK;
	}
	if (w->computed_style.display == SV_FLEX) {
		Widget_ComputeFlexLayoutStyle(w);
	}
	Widget_OnUpdateVisible(w);
}

static void Widget_OnUpdateOpacity(LCUI_Widget w)
{
	float opacity = 1.0;
	LCUI_Style s = &w->style->sheet[key_opacity];

	if (s->is_valid) {
		switch (s->type) {
		case LCUI_STYPE_INT:
			opacity = 1.0f * s->val_int;
			break;
		case LCUI_STYPE_SCALE:
			opacity = s->val_scale;
			break;
		default:
			opacity = 1.0f;
			break;
		}
		if (opacity > 1.0) {
			opacity = 1.0;
		} else if (opacity < 0.0) {
			opacity = 0.0;
		}
	}
	w->computed_style.opacity = opacity;
}

static void Widget_OnUpdateZIndex(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_z_index];

	if (s->is_valid && s->type == LCUI_STYPE_INT) {
		w->computed_style.z_index = s->val_int;
	} else {
		w->computed_style.z_index = 0;
	}
}

static void Widget_OnUpdateContentBox(LCUI_Widget w)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &w->children)) {
		Widget_AddTask(node->data, LCUI_WTASK_POSITION);
		Widget_AddTask(node->data, LCUI_WTASK_RESIZE);
	}
}

static void Widget_OnUpdatePosition(LCUI_Widget w)
{
	int position = ComputeStyleOption(w, key_position, SV_STATIC);
	int valign = ComputeStyleOption(w, key_vertical_align, SV_TOP);

	w->computed_style.vertical_align = valign;
	w->computed_style.left = Widget_ComputeXMetric(w, key_left);
	w->computed_style.right = Widget_ComputeXMetric(w, key_right);
	w->computed_style.top = Widget_ComputeYMetric(w, key_top);
	w->computed_style.bottom = Widget_ComputeYMetric(w, key_bottom);
	w->computed_style.position = position;
	Widget_OnUpdateZIndex(w);
}

static void Widget_OnUpdateMargin(LCUI_Widget w)
{
	Widget_ComputeMarginStyle(w);
}

static void Widget_OnUpdatePadding(LCUI_Widget w)
{
	Widget_ComputePaddingStyle(w);
}

static void Widget_OnUpdateSize(LCUI_Widget w)
{
	LCUI_WidgetStyle *style;
	const float border_spacing_x = w->padding.left + w->padding.right +
				       w->computed_style.border.left.width +
				       w->computed_style.border.right.width;
	const float border_spacing_y = w->padding.top + w->padding.bottom +
				       w->computed_style.border.top.width +
				       w->computed_style.border.bottom.width;

	style = &w->computed_style;
	style->max_width = -1;
	style->min_width = -1;
	style->max_height = -1;
	style->min_height = -1;
	style->box_sizing =
	    ComputeStyleOption(w, key_box_sizing, SV_CONTENT_BOX);
	if (Widget_CheckStyleValid(w, key_max_width)) {
		style->max_width = Widget_ComputeXMetric(w, key_max_width);
	}
	if (Widget_CheckStyleValid(w, key_min_width)) {
		style->min_width = Widget_ComputeXMetric(w, key_min_width);
	}
	if (Widget_CheckStyleValid(w, key_max_height)) {
		style->max_height = Widget_ComputeYMetric(w, key_max_height);
	}
	if (Widget_CheckStyleValid(w, key_min_height)) {
		style->min_height = Widget_ComputeYMetric(w, key_min_height);
	}
	if (w->computed_style.box_sizing != SV_BORDER_BOX) {
		if (style->max_width != -1) {
			style->max_width += border_spacing_x;
		}
		if (style->min_width != -1) {
			style->min_width += border_spacing_x;
		}
		if (style->max_height != -1) {
			style->max_height += border_spacing_y;
		}
		if (style->min_height != -1) {
			style->min_height += border_spacing_y;
		}
	}
	if (w->parent && Widget_HasFillAvailableWidth(w) &&
	    Widget_HasAutoStyle(w, key_width)) {
		w->width = w->parent->box.content.width;
		w->width -= w->margin.left + w->margin.right;
	} else {
		w->width = Widget_ComputeXMetric(w, key_width);
		if (w->computed_style.box_sizing == SV_CONTENT_BOX) {
			w->width += border_spacing_x;
		}
	}
	if (!Widget_HasAutoStyle(w, key_height)) {
		w->height = Widget_ComputeYMetric(w, key_height);
		if (w->computed_style.box_sizing == SV_CONTENT_BOX) {
			w->height += border_spacing_y;
		}
	}
	w->width = Widget_GetLimitedWidth(w, w->width);
	w->height = Widget_GetLimitedHeight(w, w->height);
}

static void Widget_OnUpdateBoxShadow(LCUI_Widget w)
{
	Widget_ComputeBoxShadowStyle(w);
}

static void Widget_OnUpdateBackground(LCUI_Widget w)
{
	Widget_ComputeBackgroundStyle(w);
}

static void Widget_OnUpdateProps(LCUI_Widget w)
{
	LCUI_Style s;
	LCUI_WidgetStyle *style = &w->computed_style;

	s = &w->style->sheet[key_focusable];
	style->pointer_events =
	    ComputeStyleOption(w, key_pointer_events, SV_INHERIT);
	if (s->is_valid && s->type == LCUI_STYPE_BOOL && s->val_bool == 0) {
		style->focusable = FALSE;
	} else {
		style->focusable = TRUE;
	}
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
	DEBUG_MSG("%s, %d\n", widget->type, task);
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
#define SetHandler(NAME, HANDLER) \
	self.handlers[LCUI_WTASK_##NAME] = Widget_On##HANDLER
	SetHandler(VISIBLE, UpdateVisible);
	SetHandler(POSITION, UpdatePosition);
	SetHandler(RESIZE, UpdateSize);
	SetHandler(SHADOW, UpdateBoxShadow);
	SetHandler(BORDER, UpdateBorder);
	SetHandler(OPACITY, UpdateOpacity);
	SetHandler(MARGIN, UpdateMargin);
	SetHandler(PADDING, UpdatePadding);
	SetHandler(TITLE, SetTitle);
	SetHandler(UPDATE_STYLE, UpdateStyle);
	SetHandler(REFRESH_STYLE, RefreshStyle);
	SetHandler(BACKGROUND, UpdateBackground);
	SetHandler(ZINDEX, UpdateZIndex);
	SetHandler(DISPLAY, UpdateDisplay);
	SetHandler(PROPS, UpdateProps);
	self.handlers[LCUI_WTASK_REFLOW] = NULL;
	InitStylesheetCacheDict();
	self.refresh_all = TRUE;
	self.max_updates_per_frame = 4;
}

void LCUIWidget_FreeTasks(void)
{
	LCUIWidget_ClearTrash();
}

static void Widget_InitDiff(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	ctx->diff.can_render = TRUE;
	ctx->diff.invalid_box = self.refresh_all ? SV_GRAPH_BOX : 0;
	ctx->diff.should_add_invalid_area = FALSE;
	ctx->diff.box = w->box;
	if (ctx->parent) {
		if (!ctx->parent->diff.can_render) {
			ctx->diff.can_render = FALSE;
			return;
		}
		if (ctx->parent->diff.invalid_box >= SV_PADDING_BOX) {
			ctx->diff.invalid_box = SV_GRAPH_BOX;
			return;
		}
	}
	if (w->state < LCUI_WSTATE_LAYOUTED) {
		ctx->diff.invalid_box = SV_GRAPH_BOX;
	}
	ctx->diff.should_add_invalid_area = TRUE;
}

static void Widget_BeginDiff(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	const LCUI_WidgetStyle *style = &w->computed_style;

	if (self.refresh_all) {
		memset(&ctx->diff, 0, sizeof(ctx->diff));
		Widget_InitDiff(w, ctx);
	} else {
		ctx->diff.left = w->computed_style.left;
		ctx->diff.right = w->computed_style.right;
		ctx->diff.top = w->computed_style.top;
		ctx->diff.bottom = w->computed_style.bottom;
		ctx->diff.width = w->width;
		ctx->diff.height = w->height;
		ctx->diff.margin = w->margin;
		ctx->diff.padding = w->padding;
		ctx->diff.display = style->display;
		ctx->diff.z_index = style->z_index;
		ctx->diff.visible = style->visible;
		ctx->diff.opacity = style->opacity;
		ctx->diff.position = style->position;
		ctx->diff.shadow = style->shadow;
		ctx->diff.border = style->border;
		ctx->diff.background = style->background;
		ctx->diff.box = w->box;
	}
}

INLINE void Widget_AddReflowTask(LCUI_Widget w)
{
	if (w) {
		Widget_AddTask(w, LCUI_WTASK_REFLOW);
	}
}

static int Widget_EndDiff(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	LCUI_RectF rect;
	const LCUI_WidgetTaskDiff diff = &ctx->diff;
	const LCUI_WidgetStyle *style = &w->computed_style;

	if (!diff->can_render) {
		return 0;
	}
	diff->can_render = style->visible;
	if (style->visible != diff->visible) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (style->visible) {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_SHOW, TRUE);
		} else {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_HIDE, TRUE);
		}
	}

	/* check layout related property changes */

	if (w->width != diff->width || w->height != diff->height ||
	    MEMCMP(&diff->padding, &w->padding)) {
		diff->invalid_box = SV_GRAPH_BOX;
		Widget_UpdateBoxSize(w);
		Widget_UpdateBoxPosition(w);
		Widget_OnUpdateContentBox(w);
		Widget_AddTask(w, LCUI_WTASK_REFLOW);
	} else if (style->position != SV_STATIC &&
		   (diff->left != style->left || diff->right != style->right ||
		    diff->top != style->top || diff->bottom != style->bottom)) {
		diff->invalid_box = SV_GRAPH_BOX;
		Widget_UpdateBoxPosition(w);
	}
	if (diff->display != style->display) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (style->position != SV_ABSOLUTE) {
			Widget_AddReflowTask(w->parent);
		}
		if (style->display != SV_NONE) {
			Widget_AddTask(w, LCUI_WTASK_REFLOW);
		}
	} else if (diff->position != style->position) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (diff->position == SV_ABSOLUTE ||
		    style->position == SV_ABSOLUTE) {
			Widget_AddTask(w, LCUI_WTASK_REFLOW);
		}
		Widget_AddReflowTask(w->parent);
	} else if (MEMCMP(&diff->margin, &w->margin)) {
		Widget_AddReflowTask(w->parent);
	}

	/* check repaint related property changes */

	if (!diff->should_add_invalid_area) {
		return 0;
	}
	if (diff->invalid_box == SV_GRAPH_BOX) {
	} else if (diff->z_index != style->z_index &&
		   style->position != SV_STATIC) {
		diff->invalid_box = SV_GRAPH_BOX;
	} else if (MEMCMP(&diff->shadow, &style->shadow)) {
		diff->invalid_box = SV_GRAPH_BOX;
	} else if (diff->invalid_box == SV_BORDER_BOX) {
	} else if (MEMCMP(&diff->border, &style->border)) {
		diff->invalid_box = SV_BORDER_BOX;
	} else if (MEMCMP(&diff->background, &style->background)) {
		diff->invalid_box = SV_BORDER_BOX;
	} else {
		return 0;
	}

	/* invalid area will be processed after reflow */
	if (w->task.states[LCUI_WTASK_REFLOW]) {
		return 0;
	}
	if (diff->invalid_box >= SV_BORDER_BOX) {
		Widget_UpdateCanvasBox(w);
	}
	if (!w->parent) {
		Widget_InvalidateArea(w, NULL, diff->invalid_box);
		return 1;
	}
	if (!LCUIRectF_IsCoverRect(&diff->box.canvas, &w->box.canvas)) {
		Widget_InvalidateArea(w->parent, &diff->box.canvas,
				      SV_PADDING_BOX);
		Widget_InvalidateArea(w, NULL, diff->invalid_box);
		return 1;
	}
	LCUIRectF_MergeRect(&rect, &diff->box.canvas, &w->box.canvas);
	Widget_InvalidateArea(w->parent, &rect, SV_PADDING_BOX);
	return 1;
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
	self_ctx->layout_task = NULL;
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

static LCUI_WidgetLayoutTask WidgetLayoutTask_Create(LCUI_Widget w,
						     LCUI_WidgetTaskContext ctx)
{
	LCUI_WidgetLayoutTask t;
	LCUI_WidgetTaskContext parent_ctx;

	t = malloc(sizeof(LCUI_WidgetLayoutTaskRec));
	t->widget = w;
	t->node.data = t;
	t->node.prev = t->node.next = NULL;
	t->ctx.container = w;
	t->ctx.box = ctx->diff.box;
	t->ctx.can_render = ctx->diff.can_render;
	t->ctx.invalid_box = ctx->diff.invalid_box;
	LinkedList_Init(&t->children);
	parent_ctx = ctx;
	while (parent_ctx && !parent_ctx->layout_task) {
		parent_ctx = parent_ctx->parent;
	}
	if (!parent_ctx) {
		t->parent = NULL;
		t->ctx.should_add_invalid_area = TRUE;
		return t;
	}
	t->parent = parent_ctx->layout_task;
	t->ctx.should_add_invalid_area = ctx->diff.should_add_invalid_area &&
					 t->parent->ctx.should_add_invalid_area;
	LinkedList_AppendNode(&t->parent->children, &t->node);
	return t;
}

static void Widget_UpdateSelf(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	int i;
	LCUI_BOOL *states;

	Widget_BeginDiff(w, ctx);
	states = w->task.states;
	if (states[LCUI_WTASK_USER] && w->proto && w->proto->runtask) {
		states[LCUI_WTASK_USER] = FALSE;
		w->proto->runtask(w);
	}
	w->task.for_self = FALSE;
	for (i = 0; i < LCUI_WTASK_REFLOW; ++i) {
		if (states[i]) {
			states[i] = FALSE;
			if (self.handlers[i]) {
				self.handlers[i](w);
			}
		}
	}
	Widget_EndDiff(w, ctx);
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
	self_ctx = Widget_BeginUpdate(w, ctx);
	Widget_InitDiff(w, self_ctx);
	if (w->task.for_self) {
		Widget_UpdateSelf(w, self_ctx);
	}
	if (w->task.states[LCUI_WTASK_REFLOW]) {
		self_ctx->layout_task = WidgetLayoutTask_Create(w, self_ctx);
		w->task.states[LCUI_WTASK_REFLOW] = FALSE;
	}
	if (w->task.for_children) {
		count += Widget_UpdateChildren(w, self_ctx);
	}
	Widget_SortChildrenShow(w);
	Widget_EndUpdate(self_ctx);
	return count;
}

static size_t WidgetLayoutTask_Run(LCUI_WidgetLayoutTask task)
{
	static int depth = 0;
	size_t count = 0;
	LCUI_WidgetLayoutTask child;

	++depth;
	DEBUG_MSG("[%d] layout start\n", depth);
	while (task->children.length > 0) {
		child = task->children.head.next->data;
		LinkedList_Unlink(&task->children, &child->node);
		count += WidgetLayoutTask_Run(child);
	}
	if (task->widget) {
		LCUIWidgetLayout_Reflow(&task->ctx);
		++count;
	}
	free(task);
	--depth;
	DEBUG_MSG("[%d] layout end\n", depth);
	return count;
}

size_t Widget_Update(LCUI_Widget w)
{
	size_t count;
	LCUI_WidgetTaskContext ctx;
	LCUI_WidgetLayoutTask task;

	ctx = Widget_BeginUpdate(w, NULL);
	Widget_InitDiff(w, ctx);
	task = WidgetLayoutTask_Create(w, ctx);
	task->widget = NULL;
	ctx->layout_task = task;
	count = Widget_UpdateWithContext(w, ctx);
	Widget_EndUpdate(ctx);
	WidgetLayoutTask_Run(task);
	// Widget_PrintTree(w);
	return count;
}

size_t LCUIWidget_Update(void)
{
	unsigned i;
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
	for (count = i = 0; i < 4; ++i) {
		count += Widget_Update(root);
	}
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
	unsigned i;
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
	for (i = 0; i < 4; ++i) {
		Widget_UpdateWithProfile(root, profile);
	}
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
