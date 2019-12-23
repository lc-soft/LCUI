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

/** for check widget difference */
typedef struct LCUI_WidgetTaskDiffRec_ {
	int z_index;
	int display;
	float opacity;
	LCUI_BOOL visible;
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

typedef struct LCUI_WidgetTaskContextRec_ {
	unsigned style_hash;
	Dict *style_cache;

	LCUI_WidgetTaskDiffRec diff;
	LCUI_WidgetTaskContext parent;
	LCUI_WidgetTasksProfile profile;
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
	if (!w->style->sheet[key].is_valid) {
		return default_value;
	}
	if (w->style->sheet[key].type != LCUI_STYPE_STYLE) {
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
	/* 如果有父级部件，则处理 margin-left 和 margin-right 的值 */
	if (w->parent) {
		float width = w->parent->box.content.width;
		if (Widget_HasAutoStyle(w, key_margin_left)) {
			if (Widget_HasAutoStyle(w, key_margin_right)) {
				w->margin.left = (width - w->width) / 2;
				if (w->margin.left < 0) {
					w->margin.left = 0;
				}
				w->margin.right = w->margin.left;
			} else {
				w->margin.left = width - w->width;
				w->margin.left -= w->margin.right;
				if (w->margin.left < 0) {
					w->margin.left = 0;
				}
			}
		} else if (Widget_HasAutoStyle(w, key_margin_right)) {
			w->margin.right = width - w->width;
			w->margin.right -= w->margin.left;
			if (w->margin.right < 0) {
				w->margin.right = 0;
			}
		}
	}
}

static void Widget_OnRefreshStyle(LCUI_Widget w)
{
	int i;

	Widget_ExecUpdateStyle(w, TRUE);
	for (i = LCUI_WTASK_UPDATE_STYLE + 1; i < LCUI_WTASK_TOTAL_NUM; ++i) {
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

static void Widget_ClearComputedSize(LCUI_Widget w)
{
	if (Widget_HasAutoStyle(w, key_width)) {
		w->width = 0;
		w->box.canvas.width = 0;
		w->box.content.width = 0;
	}
	if (Widget_HasAutoStyle(w, key_height)) {
		w->height = 0;
		w->box.canvas.height = 0;
		w->box.content.height = 0;
	}
}

static void Widget_UpdateChildrenSize(LCUI_Widget w)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &w->children)) {
		LCUI_Widget child = node->data;
		LCUI_StyleSheet s = child->style;

		if (Widget_HasFillAvailableWidth(child)) {
			Widget_AddTask(child, LCUI_WTASK_RESIZE);
		} else if (Widget_HasScaleSize(child)) {
			Widget_AddTask(child, LCUI_WTASK_RESIZE);
		}
		if (Widget_HasAbsolutePosition(child)) {
			if (s->sheet[key_right].is_valid ||
			    s->sheet[key_bottom].is_valid ||
			    CheckStyleType(s, key_left, scale) ||
			    CheckStyleType(s, key_top, scale)) {
				Widget_AddTask(child, LCUI_WTASK_POSITION);
			}
		}
		if (Widget_HasAutoStyle(child, key_margin_left) ||
		    Widget_HasAutoStyle(child, key_margin_right)) {
			Widget_AddTask(child, LCUI_WTASK_MARGIN);
		}
		if (child->computed_style.vertical_align != SV_TOP) {
			Widget_AddTask(child, LCUI_WTASK_POSITION);
		}
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
	if (w->parent && w->computed_style.position != position) {
		w->computed_style.position = position;
		Widget_AddTask(w->parent, LCUI_WTASK_LAYOUT);
		Widget_ClearComputedSize(w);
		Widget_UpdateChildrenSize(w);
		/* 当部件尺寸是按百分比动态计算的时候需要重新计算尺寸 */
		if (Widget_CheckStyleType(w, key_width, scale) ||
		    Widget_CheckStyleType(w, key_height, scale)) {
			Widget_AddTask(w, LCUI_WTASK_RESIZE);
		}
	}
	w->computed_style.position = position;
	Widget_OnUpdateZIndex(w);
	w->x = w->origin_x;
	w->y = w->origin_y;
	switch (position) {
	case SV_ABSOLUTE:
		w->x = w->y = 0;
		if (!Widget_HasAutoStyle(w, key_left)) {
			w->x = w->computed_style.left;
		} else if (!Widget_HasAutoStyle(w, key_right)) {
			if (w->parent) {
				w->x = w->parent->box.border.width;
				w->x -= w->width;
			}
			w->x -= w->computed_style.right;
		}
		if (!Widget_HasAutoStyle(w, key_top)) {
			w->y = w->computed_style.top;
		} else if (!Widget_HasAutoStyle(w, key_bottom)) {
			if (w->parent) {
				w->y = w->parent->box.border.height;
				w->y -= w->height;
			}
			w->y -= w->computed_style.bottom;
		}
		break;
	case SV_RELATIVE:
		if (!Widget_HasAutoStyle(w, key_left)) {
			w->x += w->computed_style.left;
		} else if (!Widget_HasAutoStyle(w, key_right)) {
			w->x -= w->computed_style.right;
		}
		if (!Widget_HasAutoStyle(w, key_top)) {
			w->y += w->computed_style.top;
		} else if (!Widget_HasAutoStyle(w, key_bottom)) {
			w->y -= w->computed_style.bottom;
		}
	default:
		if (w->parent) {
			w->x += w->parent->padding.left;
			w->y += w->parent->padding.top;
		}
		break;
	}
	switch (valign) {
	case SV_MIDDLE:
		if (!w->parent) {
			break;
		}
		w->y += (w->parent->box.content.height - w->height) / 2;
		break;
	case SV_BOTTOM:
		if (!w->parent) {
			break;
		}
		w->y += w->parent->box.content.height - w->height;
	case SV_TOP:
	default:
		break;
	}
	w->box.outer.x = w->x;
	w->box.outer.y = w->y;
	w->x += w->margin.left;
	w->y += w->margin.top;
	/* 以x、y为基础 */
	w->box.padding.x = w->x;
	w->box.padding.y = w->y;
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.canvas.x = w->x;
	w->box.canvas.y = w->y;
	/* 计算各个框的坐标 */
	w->box.padding.x += w->computed_style.border.left.width;
	w->box.padding.y += w->computed_style.border.top.width;
	w->box.content.x = w->box.padding.x + w->padding.left;
	w->box.content.y = w->box.padding.y + w->padding.top;
	w->box.canvas.x -= Widget_GetBoxShadowOffsetX(w);
	w->box.canvas.y -= Widget_GetBoxShadowOffsetY(w);
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
	int box_sizing;
	LCUI_RectF rect = w->box.canvas;

	box_sizing = ComputeStyleOption(w, key_box_sizing, SV_CONTENT_BOX);
	w->computed_style.box_sizing = box_sizing;
	Widget_ComputePaddingStyle(w);
	Widget_ComputeSizeStyle(w);
	/* 如果左右外间距是 auto 类型的，则需要计算外间距 */
	if (w->style->sheet[key_margin_left].is_valid &&
	    w->style->sheet[key_margin_left].type == LCUI_STYPE_AUTO) {
		Widget_ComputeMarginStyle(w);
	} else if (w->style->sheet[key_margin_right].is_valid &&
		   w->style->sheet[key_margin_right].type == LCUI_STYPE_AUTO) {
		Widget_ComputeMarginStyle(w);
	}
	/* 若在变化前后的宽高中至少有一个为 0，则不继续处理 */
	if ((w->box.canvas.width <= 0 || w->box.canvas.height <= 0) &&
	    (rect.width <= 0 || rect.height <= 0)) {
		return;
	}
	/* 如果垂直对齐方式不为顶部对齐 */
	if (w->computed_style.vertical_align != SV_TOP) {
		Widget_AddTask(w, LCUI_WTASK_POSITION);
	} else if (w->computed_style.position == SV_ABSOLUTE) {
		/* 如果是绝对定位，且指定了右间距或底间距 */
		if (!Widget_HasAutoStyle(w, key_right) ||
		    !Widget_HasAutoStyle(w, key_bottom)) {
			Widget_AddTask(w, LCUI_WTASK_POSITION);
		}
	}
}

static void Widget_OnUpdateBoxShadow(LCUI_Widget w)
{
	Widget_ComputeBoxShadowStyle(w);
}

static void Widget_OnUpdateBackground(LCUI_Widget w)
{
	Widget_ComputeBackgroundStyle(w);
}

static void Widget_OnUpdateLayout(LCUI_Widget w)
{
	if (w->computed_style.display == SV_FLEX) {
		Widget_ComputeFlexLayoutStyle(w);
	}
	Widget_DoLayout(w);
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
	SetHandler(LAYOUT, UpdateLayout);
	SetHandler(ZINDEX, UpdateZIndex);
	SetHandler(DISPLAY, UpdateDisplay);
	SetHandler(PROPS, UpdateProps);
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
		ctx->diff.box = w->box;
		ctx->diff.display = style->display;
		ctx->diff.z_index = style->z_index;
		ctx->diff.visible = style->visible;
		ctx->diff.opacity = style->opacity;
		ctx->diff.position = style->position;
		ctx->diff.shadow = style->shadow;
		ctx->diff.border = style->border;
		ctx->diff.background = style->background;
	}
}

static int Widget_EndDiff(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	LCUI_RectF rect;
	LCUI_WidgetEventRec e;
	LCUI_BOOL widget_box_moved = FALSE;
	const LCUI_WidgetTaskDiff diff = &ctx->diff;
	const LCUI_WidgetStyle *style = &w->computed_style;

	if (!diff->can_render) {
		return 0;
	}
	diff->can_render = style->visible;
	/* hidden widget do not need to be rendered */
	if (style->visible == diff->visible && !style->visible) {
		return 0;
	}
	/* Check for layout related property changes */

	if (diff->visible != style->visible) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (style->visible) {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_SHOW, TRUE);
		} else {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_HIDE, TRUE);
		}
	}
	if (style->position == SV_ABSOLUTE) {
		if (MEMCMP(&diff->box.canvas, &w->box.canvas)) {
			diff->invalid_box = SV_GRAPH_BOX;
			widget_box_moved = TRUE;
		}
	} else if (MEMCMP(&diff->box.outer, &w->box.outer)) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (w->parent) {
			Widget_AddTask(w->parent, LCUI_WTASK_LAYOUT);
		}
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_MOVE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
		widget_box_moved = TRUE;
	} else if (MEMCMP(&diff->box.canvas, &w->box.canvas)) {
		diff->invalid_box = SV_GRAPH_BOX;
		widget_box_moved = TRUE;
	}

	if (diff->box.border.width != w->box.border.width ||
	    diff->box.border.height != w->box.border.height) {
		e.target = w;
		e.data = NULL;
		e.type = LCUI_WEVENT_RESIZE;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent(w, &e, NULL);
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_RESIZE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
		Widget_AddTask(w, LCUI_WTASK_POSITION);
		diff->invalid_box = SV_GRAPH_BOX;
	}
	if (diff->position != style->position) {
		if (w->parent && style->position != SV_ABSOLUTE) {
			Widget_AddTask(w->parent, LCUI_WTASK_LAYOUT);
		}
	}
	if (MEMCMP(&diff->box.padding, &w->box.padding)) {
		diff->invalid_box = max(diff->invalid_box, SV_PADDING_BOX);
		Widget_UpdateChildrenSize(w);
		Widget_AddTask(w, LCUI_WTASK_LAYOUT);
	}
	if (!diff->should_add_invalid_area) {
		return 0;
	}

	/* Check for canvas related property changes */

	if (diff->invalid_box == SV_GRAPH_BOX) {
	} else if (diff->visible != style->visible) {
		diff->invalid_box = SV_GRAPH_BOX;
		if (style->visible) {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_SHOW, TRUE);
		} else {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_HIDE, TRUE);
		}
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

	/* Processing invalid area */

	if (widget_box_moved && w->parent) {
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
	Widget_InvalidateArea(w, NULL, diff->invalid_box);
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
			DEBUG_MSG("first update visible children "
				  "count: %zu\n",
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

static void Widget_UpdateSelf(LCUI_Widget w, LCUI_WidgetTaskContext ctx)
{
	int i;
	int self_updates;
	LCUI_BOOL *states;

	Widget_BeginDiff(w, ctx);
	states = w->task.states;
	w->task.for_self = FALSE;
	for (i = 0; i < LCUI_WTASK_REFLOW; ++i) {
		if (states[i]) {
			states[i] = FALSE;
			if (self.handlers[i]) {
				self.handlers[i](w);
			}
		}
	}
	if (states[LCUI_WTASK_USER] && w->proto && w->proto->runtask) {
		states[LCUI_WTASK_USER] = FALSE;
		w->proto->runtask(w);
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
	if (w->task.for_children) {
		count += Widget_UpdateChildren(w, self_ctx);
	}
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
