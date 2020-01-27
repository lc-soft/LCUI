/*
 * flexbox.h -- Flexible Box Layout
 *
 * Copyright (c) 2020, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include "flexbox.h"
#include "../widget_util.h"

typedef struct LCUI_FlexBoxLineRec_ {
	/** The size of it on the main axis */
	float main_size;

	/** The size of it on the cross axis */
	float cross_size;

	float sum_of_grow_value;
	float sum_of_shrink_value;
	size_t count_of_auto_margin_items;

	/** LinkedList<LCUI_Widget> elements */
	LinkedList elements;
} LCUI_FlexBoxLineRec, *LCUI_FlexBoxLine;

typedef struct LCUI_FlexBoxLayoutContextRec_ {
	LCUI_WidgetLayoutContext base;
	LCUI_Widget widget;

	float main_axis;
	float cross_axis;
	float main_size;
	float cross_size;

	/** LinkedList<LCUI_FlexBoxLine> lines */
	LinkedList lines;
	LCUI_FlexBoxLine line;

	LinkedList free_elements;
} LCUI_FlexBoxLayoutContextRec, *LCUI_FlexBoxLayoutContext;

static LCUI_FlexBoxLine FlexBoxLine_Create(void)
{
	LCUI_FlexBoxLine line;

	line = malloc(sizeof(LCUI_FlexBoxLineRec));
	line->main_size = 0;
	line->cross_size = 0;
	line->sum_of_grow_value = 0;
	line->sum_of_shrink_value = 0;
	line->count_of_auto_margin_items = 0;
	LinkedList_Init(&line->elements);
	return line;
}

static void FlexBoxLine_Destroy(void *arg)
{
	LCUI_FlexBoxLine line = arg;

	LinkedList_Clear(&line->elements, NULL);
	free(line);
}

static void FlexBoxLine_LoadElement(LCUI_FlexBoxLine line, LCUI_Widget w)
{
	if (w->computed_style.flex.grow > 0) {
		line->sum_of_grow_value += w->computed_style.flex.grow;
	}
	if (w->computed_style.flex.shrink > 0) {
		line->sum_of_shrink_value += w->computed_style.flex.shrink;
	}
	LinkedList_Append(&line->elements, w);
}

static void FlexBoxLayout_NextLine(LCUI_FlexBoxLayoutContext ctx)
{
	if (ctx->line) {
		ctx->main_size = max(ctx->main_size, ctx->line->main_size);
		ctx->cross_size += ctx->line->cross_size;
		ctx->cross_axis += ctx->line->cross_size;
	}
	ctx->main_axis = ctx->base->container->padding.left;
	ctx->line = FlexBoxLine_Create();
	LinkedList_Append(&ctx->lines, ctx->line);
}

static LCUI_FlexBoxLayoutContext FlexBoxLayout_Begin(
    LCUI_WidgetLayoutContext base)
{
	ASSIGN(ctx, LCUI_FlexBoxLayoutContext);

	ctx->line = NULL;
	ctx->base = base;
	ctx->widget = ctx->base->container;
	ctx->main_axis = ctx->widget->padding.left;
	ctx->cross_axis = ctx->widget->padding.right;
	ctx->main_size = 0;
	ctx->cross_size = 0;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->lines);
	FlexBoxLayout_NextLine(ctx);
	return ctx;
}

static void FlexBoxLayout_End(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget w = ctx->base->container;
	LCUI_WidgetEventRec ev = { 0 };

	ev.cancel_bubble = TRUE;
	ev.type = LCUI_WEVENT_AFTERLAYOUT;
	Widget_TriggerEvent(w, &ev, NULL);
	LinkedList_Clear(&ctx->lines, FlexBoxLine_Destroy);
	LinkedList_Clear(&ctx->free_elements, NULL);
	free(ctx);
}

static void FlexBoxLayout_LoadRows(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget child;
	LCUI_FlexBoxLayoutStyle *flex = &ctx->widget->computed_style.flex;
	LinkedListNode *node;

	float basis;
	float cross_size;
	float max_main_size = ctx->widget->box.content.width;

	DEBUG_MSG("max_main_size: %g\n", max_main_size);
	for (LinkedList_Each(node, &ctx->widget->children)) {
		child = node->data;
		if (child->computed_style.display == SV_NONE) {
			continue;
		}
		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		if (Widget_HasAutoStyle(child, key_margin_left)) {
			child->margin.left = 0;
			if (Widget_HasAutoStyle(child, key_margin_right)) {
				child->margin.right = 0;
			}
			Widget_UpdateBoxSize(child);
		} else if (Widget_HasAutoStyle(child, key_margin_right)) {
			child->margin.right = 0;
			Widget_UpdateBoxSize(child);
		}
		Widget_ComputeFlexBasisStyle(child);
		basis = MarginX(child) + child->computed_style.flex.basis;
		DEBUG_MSG("[line %lu][%lu] main_size: %g, basis: %g\n",
			   ctx->lines.length, child->index,
			   ctx->line->main_size, basis);
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0) {
			if (ctx->line->main_size + basis > max_main_size) {
				FlexBoxLayout_NextLine(ctx);
			}
		}
		cross_size = child->box.outer.height;
		if (!Widget_HasStaticHeight(child)) {
			cross_size -= child->box.content.height;
			cross_size += child->min_content_height;
		}
		if (cross_size > ctx->line->cross_size) {
			ctx->line->cross_size = cross_size;
		}
		if (Widget_HasAutoStyle(child, key_margin_left)) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (Widget_HasAutoStyle(child, key_margin_right)) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += basis;
		FlexBoxLine_LoadElement(ctx->line, child);
	}
	if (ctx->widget->computed_style.display == SV_FLEX) {
		ctx->main_size = ctx->widget->box.content.width;
	} else {
		ctx->main_size = max(ctx->main_size, ctx->line->main_size);
	}
	ctx->cross_size += ctx->line->cross_size;
}

static void FlexBoxLayout_LoadColumns(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget child;
	LCUI_FlexBoxLayoutStyle *flex = &ctx->widget->computed_style.flex;
	LinkedListNode *node;

	float basis;
	float max_main_size = -1;

	if (Widget_HasStaticHeight(ctx->widget)) {
		max_main_size = ctx->widget->box.content.height;
	}
	DEBUG_MSG("max_main_size: %g\n", max_main_size);
	for (LinkedList_Each(node, &ctx->widget->children)) {
		child = node->data;

		if (child->computed_style.display == SV_NONE) {
			continue;
		}
		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		Widget_ComputeFlexBasisStyle(child);
		basis = MarginY(child) + child->computed_style.flex.basis;
		DEBUG_MSG("[column %lu][%lu] main_size: %g, basis: %g\n",
			   ctx->lines.length, child->index,
			   ctx->line->main_size, basis);
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0 &&
		    max_main_size >= 0) {
			if (ctx->line->main_size + basis > max_main_size) {
				FlexBoxLayout_NextLine(ctx);
			}
		}
		if (child->box.outer.width > ctx->line->cross_size) {
			ctx->line->cross_size = child->box.outer.width;
		}
		if (Widget_HasAutoStyle(child, key_margin_top)) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (Widget_HasAutoStyle(child, key_margin_top)) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += basis;
		FlexBoxLine_LoadElement(ctx->line, child);
	}
	if (Widget_HasStaticHeight(ctx->widget)) {
		ctx->main_size = ctx->widget->box.content.height;
	} else {
		ctx->main_size = max(ctx->main_size, ctx->line->main_size);
	}
	ctx->cross_size += ctx->line->cross_size;
	DEBUG_MSG("main_size: %g\n", ctx->main_size);
}

static void FlexBoxLayout_Load(LCUI_FlexBoxLayoutContext ctx)
{
	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		FlexBoxLayout_LoadColumns(ctx);
	} else {
		FlexBoxLayout_LoadRows(ctx);
	}
}

static void FlexBoxLayout_ComputeJustifyContent(LCUI_FlexBoxLayoutContext ctx,
						float *start_axis, float *space)
{
	float free_space = ctx->main_size - ctx->line->main_size;

	switch (ctx->widget->computed_style.flex.justify_content) {
	case SV_SPACE_BETWEEN:
		if (ctx->line->elements.length > 1) {
			*space = free_space / (ctx->line->elements.length - 1);
		}
		*start_axis -= *space;
		break;
	case SV_SPACE_AROUND:
		*space = free_space / ctx->line->elements.length;
		*start_axis -= *space * 0.5f;
		break;
	case SV_SPACE_EVENLY:
		*space = free_space / (ctx->line->elements.length + 1);
		*start_axis += *space;
		break;
	case SV_RIGHT:
	case SV_FLEX_END:
		*start_axis += free_space;
		break;
	case SV_CENTER:
		*start_axis += free_space * 0.5f;
		break;
	case SV_LEFT:
	default:
		break;
	}
}

static void FlexBoxLayout_ReflowRow(LCUI_FlexBoxLayoutContext ctx)
{
	float k = 0;
	float main_axis = 0;
	float space = 0;
	float free_space = ctx->main_size - ctx->line->main_size;

	LCUI_Widget w;
	LCUI_FlexBoxLayoutStyle *flex;
	LinkedListNode *node;

	if (free_space >= 0) {
		if (ctx->line->sum_of_grow_value > 0) {
			k = free_space / ctx->line->sum_of_grow_value;
		}
	} else if (ctx->line->sum_of_shrink_value > 0) {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */
	DEBUG_MSG("free_space: %g\n", free_space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->width = flex->basis + k * flex->grow;
				Widget_UpdateBoxSize(w);
				LCUIWidgetLayout_ReflowChild(ctx->base, w);
			}
		} else if (flex->shrink > 0) {
			w->width = flex->basis + k * flex->shrink;
			Widget_UpdateBoxSize(w);
			LCUIWidgetLayout_ReflowChild(ctx->base, w);
		}
		if (!Widget_HasStaticHeight(w)) {
			Widget_ComputeHeightStyle(w);
			LCUIWidgetLayout_ReflowChild(ctx->base, w);
		}
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.width;
	}
	ctx->line->main_size = main_axis;
	free_space = ctx->main_size - main_axis;

	/* auto margin */
	DEBUG_MSG("free_space: %g, auto margin items: %lu\n", free_space,
		   ctx->line->count_of_auto_margin_items);
	if (free_space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = free_space / ctx->line->count_of_auto_margin_items;
		for (LinkedList_Each(node, &ctx->line->elements)) {
			w = node->data;
			if (Widget_HasAutoStyle(w, key_margin_left)) {
				w->margin.left = k;
				Widget_UpdateBoxSize(w);
				DEBUG_MSG("margin-left: %g\n", k);
			}
			if (Widget_HasAutoStyle(w, key_margin_right)) {
				w->margin.right = k;
				Widget_UpdateBoxSize(w);
				DEBUG_MSG("margin-right: %g\n", k);
			}
			DEBUG_MSG("basis: %g\n", w->box.outer.width);
			main_axis += w->box.outer.width;
		}
		ctx->line->main_size = main_axis;
	}

	/* update the position of each child widget */

	main_axis = ctx->widget->padding.left;
	FlexBoxLayout_ComputeJustifyContent(ctx, &main_axis, &space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		main_axis += space;
		w->layout_x = main_axis;
		Widget_UpdateBoxPosition(w);
		main_axis += w->box.outer.width;
	}
	ctx->line->main_size = ctx->main_size;
}

static void FlexBoxLayout_ReflowColumn(LCUI_FlexBoxLayoutContext ctx)
{
	float k;
	float main_axis = 0;
	float space = 0;
	float free_space = ctx->main_size - ctx->line->main_size;

	LCUI_Widget w;
	LCUI_FlexBoxLayoutStyle *flex;
	LinkedListNode *node;

	if (free_space >= 0) {
		k = free_space / ctx->line->sum_of_grow_value;
	} else {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */

	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->height = flex->basis + k * flex->grow;
				Widget_UpdateBoxSize(w);
				LCUIWidgetLayout_ReflowChild(ctx->base, w);
			}
		} else if (flex->shrink > 0) {
			w->height = flex->basis + k * flex->shrink;
			Widget_UpdateBoxSize(w);
			LCUIWidgetLayout_ReflowChild(ctx->base, w);
		}
		if (!Widget_HasStaticWidth(w)) {
			Widget_ComputeWidthStyle(w);
			LCUIWidgetLayout_ReflowChild(ctx->base, w);
		}
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.height;
	}
	ctx->line->main_size = main_axis;
	free_space = main_axis - ctx->main_size;

	/* auto margin */

	if (free_space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = free_space / ctx->line->count_of_auto_margin_items;
		for (LinkedList_Each(node, &ctx->line->elements)) {
			w = node->data;
			if (Widget_HasAutoStyle(w, key_margin_top)) {
				w->margin.top = k;
				Widget_UpdateBoxSize(w);
			}
			if (Widget_HasAutoStyle(w, key_margin_bottom)) {
				w->margin.bottom = k;
				Widget_UpdateBoxSize(w);
			}
			main_axis += w->box.outer.height;
		}
		ctx->line->main_size = main_axis;
	}

	/* justify-content */

	main_axis = ctx->widget->padding.top;
	FlexBoxLayout_ComputeJustifyContent(ctx, &main_axis, &space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		main_axis += space;
		w->layout_y = main_axis;
		Widget_UpdateBoxPosition(w);
		main_axis += w->box.outer.height;
	}
	ctx->line->main_size = ctx->main_size;
}

static void FlexBoxLayout_AlignItemsCenter(LCUI_FlexBoxLayoutContext ctx,
					   float base_cross_axis)
{
	LinkedListNode *node;
	LCUI_Widget child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x =
			    base_cross_axis +
			    (ctx->line->cross_size - child->box.outer.width) *
				0.5f;
			Widget_UpdateBoxPosition(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y =
		    base_cross_axis +
		    (ctx->line->cross_size - child->box.outer.height) * 0.5f;
		Widget_UpdateBoxPosition(child);
	}
}

static void FlexBoxLayout_AlignItemsStretch(LCUI_FlexBoxLayoutContext ctx,
					    float base_cross_axis)
{
	LinkedListNode *node;
	LCUI_Widget child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis;
			if (Widget_HasAutoStyle(child, key_width)) {
				child->width = ctx->line->cross_size;
				Widget_UpdateBoxSize(child);
			}
			Widget_UpdateBoxPosition(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis;
		if (Widget_HasAutoStyle(child, key_height)) {
			child->height = ctx->line->cross_size;
			Widget_UpdateBoxSize(child);
		}
		Widget_UpdateBoxPosition(child);
	}
}

static void FlexBoxLayout_AlignItemsStart(LCUI_FlexBoxLayoutContext ctx,
					  float base_cross_axis)
{
	LinkedListNode *node;
	LCUI_Widget child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis;
			Widget_UpdateBoxPosition(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis;
		Widget_UpdateBoxPosition(child);
	}
}

static void FlexBoxLayout_AlignItemsEnd(LCUI_FlexBoxLayoutContext ctx,
					float base_cross_axis)
{
	LinkedListNode *node;
	LCUI_Widget child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis +
					  ctx->line->cross_size -
					  child->box.outer.width;
			Widget_UpdateBoxPosition(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis + ctx->line->cross_size -
				  child->box.outer.height;
		Widget_UpdateBoxPosition(child);
	}
}

static void FlexBoxLayout_AlignItems(LCUI_FlexBoxLayoutContext ctx)
{
	float cross_axis;
	float free_space = 0;

	LCUI_Widget w = ctx->widget;
	LinkedListNode *node;

	if (w->computed_style.flex.direction == SV_COLUMN) {
		cross_axis = w->padding.left;
		free_space = w->box.content.width - ctx->cross_size;
	} else {
		cross_axis = w->padding.top;
		free_space = w->box.content.height - ctx->cross_size;
	}
	if (free_space < 0) {
		free_space = 0;
	}
	for (LinkedList_Each(node, &ctx->lines)) {
		ctx->line = node->data;
		ctx->line->cross_size += free_space / ctx->lines.length;
		switch (w->computed_style.flex.align_items) {
		case SV_CENTER:
			FlexBoxLayout_AlignItemsCenter(ctx, cross_axis);
			break;
		case SV_FLEX_START:
			FlexBoxLayout_AlignItemsStart(ctx, cross_axis);
			break;
		case SV_FLEX_END:
			FlexBoxLayout_AlignItemsEnd(ctx, cross_axis);
			break;
		case SV_NORMAL:
		case SV_STRETCH:
		default:
			FlexBoxLayout_AlignItemsStretch(ctx, cross_axis);
			break;
		}
		cross_axis += ctx->line->cross_size;
	}
	if (w->computed_style.flex.direction == SV_COLUMN) {
		ctx->cross_size = cross_axis - w->padding.left;
	} else {
		ctx->cross_size = cross_axis - w->padding.top;
	}
}

static void FlexBoxLayout_ApplySize(LCUI_FlexBoxLayoutContext ctx)
{
	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		Widget_SetContentSize(ctx->widget, ctx->cross_size,
				      ctx->main_size);
	} else {
		Widget_SetContentSize(ctx->widget, ctx->main_size,
				      ctx->cross_size);
	}
}

static void FlexBoxLayout_Reflow(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget w = ctx->widget;
	LinkedListNode *node;

	for (LinkedList_Each(node, &ctx->lines)) {
		ctx->line = node->data;
		if (w->computed_style.flex.direction == SV_COLUMN) {
			DEBUG_MSG("column %lu\n", i++);
			FlexBoxLayout_ReflowColumn(ctx);
		} else {
			DEBUG_MSG("row %lu\n", i++);
			FlexBoxLayout_ReflowRow(ctx);
		}
	}
	FlexBoxLayout_AlignItems(ctx);
}

static void FlexBoxLayout_ReflowFreeElements(LCUI_FlexBoxLayoutContext ctx)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &ctx->free_elements)) {
		Widget_UpdateBoxSize(node->data);
		Widget_UpdateBoxPosition(node->data);
		Widget_AddState(node->data, LCUI_WSTATE_LAYOUTED);
	}
}

void LCUIFlexBoxLayout_Reflow(LCUI_WidgetLayoutContext base_ctx)
{
	LCUI_FlexBoxLayoutContext ctx;

	ctx = FlexBoxLayout_Begin(base_ctx);
	FlexBoxLayout_Load(ctx);
	FlexBoxLayout_ApplySize(ctx);
	FlexBoxLayout_Reflow(ctx);
	FlexBoxLayout_ReflowFreeElements(ctx);
	FlexBoxLayout_End(ctx);
}
