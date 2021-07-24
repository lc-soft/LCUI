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
#include "../widget_diff.h"

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
	LCUI_Widget widget;
	LCUI_LayoutRule rule;
	LCUI_BOOL is_initiative;

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
		ctx->cross_axis += ctx->line->cross_size;
		ctx->cross_size = ctx->cross_axis;
		if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
			ctx->cross_size -= ctx->widget->padding.left;
		} else {
			ctx->cross_size -= ctx->widget->padding.top;
		}
	}
	ctx->main_axis = ctx->widget->padding.left;
	ctx->line = FlexBoxLine_Create();
	LinkedList_Append(&ctx->lines, ctx->line);
}

static LCUI_FlexBoxLayoutContext FlexBoxLayout_Begin(LCUI_Widget w,
						     LCUI_LayoutRule rule)
{
	LCUI_WidgetStyle *style = &w->computed_style;
	ASSIGN(ctx, LCUI_FlexBoxLayoutContext);

	if (rule == LCUI_LAYOUT_RULE_AUTO) {
		ctx->is_initiative = TRUE;
		if (style->flex.direction == SV_COLUMN) {
			if (style->height_sizing == LCUI_SIZING_RULE_FIXED) {
				rule = LCUI_LAYOUT_RULE_FIXED_HEIGHT;
			} else {
				rule = LCUI_LAYOUT_RULE_MAX_CONTENT;
			}
		} else {
			if (style->width_sizing == LCUI_SIZING_RULE_FIXED) {
				rule = LCUI_LAYOUT_RULE_FIXED_WIDTH;
			} else {
				rule = LCUI_LAYOUT_RULE_MAX_CONTENT;
			}
		}
	} else {
		ctx->is_initiative = FALSE;
	}
	if (style->position == SV_ABSOLUTE) {
		if (rule == LCUI_LAYOUT_RULE_FIXED_HEIGHT &&
		    style->width_sizing == LCUI_SIZING_RULE_PERCENT) {
			rule = LCUI_LAYOUT_RULE_FIXED;
		} else if (rule == LCUI_LAYOUT_RULE_FIXED_WIDTH &&
			   style->height_sizing == LCUI_SIZING_RULE_PERCENT) {
			rule = LCUI_LAYOUT_RULE_FIXED;
		}
	}
	if (rule == LCUI_LAYOUT_RULE_FIXED_WIDTH) {
		if (style->height_sizing == LCUI_SIZING_RULE_FIXED) {
			rule = LCUI_LAYOUT_RULE_FIXED;
		}
	} else if (rule == LCUI_LAYOUT_RULE_FIXED_HEIGHT) {
		if (style->width_sizing == LCUI_SIZING_RULE_FIXED) {
			rule = LCUI_LAYOUT_RULE_FIXED;
		}
	}
	DEBUG_MSG("%s, rule: %d, is_initiative: %d\n", w->id, rule,
		  ctx->is_initiative);
	ctx->rule = rule;
	ctx->line = NULL;
	ctx->widget = w;
	if (style->flex.direction == SV_COLUMN) {
		ctx->main_axis = w->padding.left;
		ctx->cross_axis = w->padding.top;
	} else {
		ctx->main_axis = w->padding.top;
		ctx->cross_axis = w->padding.left;
	}
	ctx->main_size = 0;
	ctx->cross_size = 0;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->lines);
	FlexBoxLayout_NextLine(ctx);
	return ctx;
}

static void FlexBoxLayout_End(LCUI_FlexBoxLayoutContext ctx)
{
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
	float max_main_size = -1;

	if (ctx->rule == LCUI_LAYOUT_RULE_FIXED ||
	    ctx->rule == LCUI_LAYOUT_RULE_FIXED_WIDTH) {
		max_main_size = ctx->widget->box.content.width;
	}
	DEBUG_MSG("%s, max_main_size: %g\n", ctx->widget->id, max_main_size);
	for (LinkedList_Each(node, &ctx->widget->children)) {
		child = node->data;
		if (child->computed_style.display == SV_NONE) {
			continue;
		}
		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		/* Clears the auto margin calculated on the last layout */
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
			  ctx->lines.length, child->index, ctx->line->main_size,
			  basis);
		/* Check line wrap */
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0 &&
		    max_main_size != -1) {
			if (ctx->line->main_size + basis - max_main_size >
			    0.4f) {
				FlexBoxLayout_NextLine(ctx);
			}
		}
		if (child->box.outer.height > ctx->line->cross_size) {
			ctx->line->cross_size = child->box.outer.height;
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
	ctx->main_size = max(ctx->main_size, ctx->line->main_size);
	ctx->cross_size += ctx->line->cross_size;
	DEBUG_MSG("%s,end load, size: (%g, %g)\n", ctx->widget->id,
		  ctx->main_size, ctx->cross_size);
}

static void FlexBoxLayout_LoadColumns(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget child;
	LCUI_FlexBoxLayoutStyle *flex = &ctx->widget->computed_style.flex;
	LinkedListNode *node;

	float basis;
	float max_main_size = -1;

	if (ctx->rule == LCUI_LAYOUT_RULE_FIXED ||
	    ctx->rule == LCUI_LAYOUT_RULE_FIXED_HEIGHT) {
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
			  ctx->lines.length, child->index, ctx->line->main_size,
			  basis);
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0 &&
		    max_main_size != -1) {
			if (ctx->line->main_size + basis - max_main_size >
			    0.4f) {
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
	ctx->main_size = max(ctx->main_size, ctx->line->main_size);
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
	float free_space;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		free_space = ctx->widget->box.content.height;
	} else {
		free_space = ctx->widget->box.content.width;
	}
	free_space -= ctx->line->main_size;
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

static void UpdateFlexItemSize(LCUI_Widget w, LCUI_LayoutRule rule)
{
	float content_width = w->box.padding.width;
	float content_height = w->box.padding.height;
	LCUI_WidgetLayoutDiffRec diff;

	Widget_BeginLayoutDiff(w, &diff);
	/*
	 * As a widget of a flex item, its size is calculated by the flexbox
	 * layout engine, so here we only need to calculate its width and
	 * height limits.
	 */
	Widget_ComputeWidthLimitStyle(w, LCUI_LAYOUT_RULE_FIXED);
	Widget_ComputeHeightLimitStyle(w, LCUI_LAYOUT_RULE_FIXED);
	Widget_UpdateBoxSize(w);
	if (content_width == w->box.padding.width &&
	    content_height == w->box.padding.height) {
		return;
	}
	if (rule == LCUI_LAYOUT_RULE_FIXED_WIDTH ||
	    rule == LCUI_LAYOUT_RULE_FIXED_HEIGHT) {
		rule = LCUI_LAYOUT_RULE_FIXED;
	}
	Widget_Reflow(w, rule);
	Widget_EndLayoutDiff(w, &diff);
	w->task.states[LCUI_WTASK_REFLOW] = FALSE;
}

static void FlexBoxLayout_ReflowRow(LCUI_FlexBoxLayoutContext ctx)
{
	float k = 0;
	float main_axis = 0;
	float space = 0;
	float free_space;

	LCUI_Widget w;
	LCUI_FlexBoxLayoutStyle *flex;
	LinkedListNode *node;

	free_space = ctx->widget->box.content.width - ctx->line->main_size;
	if (free_space >= 0) {
		if (ctx->line->sum_of_grow_value > 0) {
			k = free_space / ctx->line->sum_of_grow_value;
		}
	} else if (ctx->line->sum_of_shrink_value > 0) {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */
	DEBUG_MSG("%s, free_space: %g\n", ctx->widget->id, free_space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (w->computed_style.height_sizing != LCUI_SIZING_RULE_FIXED) {
			Widget_ComputeHeightStyle(w);
		}
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->width = flex->basis + k * flex->grow;
				UpdateFlexItemSize(
				    w, LCUI_LAYOUT_RULE_FIXED_WIDTH);
			} else {
				UpdateFlexItemSize(
				    w, LCUI_LAYOUT_RULE_FIXED_WIDTH);
			}
		} else if (flex->shrink > 0) {
			w->width = flex->basis + k * flex->shrink;
			UpdateFlexItemSize(w, LCUI_LAYOUT_RULE_FIXED_WIDTH);
		} else {
			UpdateFlexItemSize(w, LCUI_LAYOUT_RULE_FIXED_WIDTH);
		}
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.width;
	}
	ctx->line->main_size = main_axis;
	free_space = ctx->widget->box.content.width - main_axis;

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
			}
			if (Widget_HasAutoStyle(w, key_margin_right)) {
				w->margin.right = k;
				Widget_UpdateBoxSize(w);
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
	float free_space;

	LCUI_Widget w;
	LCUI_FlexBoxLayoutStyle *flex;
	LinkedListNode *node;

	free_space = ctx->widget->box.content.height - ctx->line->main_size;
	if (free_space >= 0) {
		k = free_space / ctx->line->sum_of_grow_value;
	} else {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */

	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (w->computed_style.width_sizing != LCUI_SIZING_RULE_FIXED) {
			Widget_ComputeWidthStyle(w);
		}
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->height = flex->basis + k * flex->grow;
				UpdateFlexItemSize(
				    w, LCUI_LAYOUT_RULE_FIXED_HEIGHT);
			} else {
				UpdateFlexItemSize(
				    w, LCUI_LAYOUT_RULE_FIXED_HEIGHT);
			}
		} else if (flex->shrink > 0) {
			w->height = flex->basis + k * flex->shrink;
			UpdateFlexItemSize(w, LCUI_LAYOUT_RULE_FIXED_HEIGHT);
		} else {
			UpdateFlexItemSize(w, LCUI_LAYOUT_RULE_FIXED_HEIGHT);
		}
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.height;
	}
	free_space = ctx->widget->box.content.height - ctx->line->main_size;
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
				child->width =
				    ctx->line->cross_size - MarginX(child);
				UpdateFlexItemSize(child,
						   LCUI_LAYOUT_RULE_FIXED);
			}
			Widget_UpdateBoxPosition(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis;
		if (Widget_HasAutoStyle(child, key_height)) {
			child->height = ctx->line->cross_size - MarginY(child);
			UpdateFlexItemSize(child, LCUI_LAYOUT_RULE_FIXED);
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

static void FlexBoxLayout_Reflow(LCUI_FlexBoxLayoutContext ctx)
{
	LCUI_Widget w = ctx->widget;
	LinkedListNode *node;

	DEBUG_MSG("widget: %s, start\n", w->id);
	for (LinkedList_Each(node, &ctx->lines)) {
		ctx->line = node->data;
		if (w->computed_style.flex.direction == SV_COLUMN) {
			FlexBoxLayout_ReflowColumn(ctx);
		} else {
			FlexBoxLayout_ReflowRow(ctx);
		}
	}
	DEBUG_MSG("widget: %s, end\n", w->id);
	FlexBoxLayout_AlignItems(ctx);
}

static void FlexBoxLayout_ReflowFreeElements(LCUI_FlexBoxLayoutContext ctx)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &ctx->free_elements)) {
		Widget_AutoReflow(node->data, LCUI_LAYOUT_RULE_FIXED);
	}
}

static void FlexBoxLayout_ApplySize(LCUI_FlexBoxLayoutContext ctx)
{
	float width = 0, height = 0;

	LCUI_Widget w = ctx->widget;

	DEBUG_MSG("widget: %s, main_size: %g, cross_size: %g\n", w->id,
		  ctx->main_size, ctx->cross_size);
	if (w->computed_style.flex.direction == SV_COLUMN) {
		switch (ctx->rule) {
		case LCUI_LAYOUT_RULE_FIXED:
			width = w->box.content.width;
			height = w->box.content.height;
			break;
		case LCUI_LAYOUT_RULE_FIXED_WIDTH:
			width = w->box.content.width;
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = w->box.content.width;
			height = max(height, ctx->main_size);
			break;
		case LCUI_LAYOUT_RULE_FIXED_HEIGHT:
			height = w->box.content.height;
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = w->box.content.height;
			width = max(width, ctx->cross_size);
			break;
		default:
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = max(height, ctx->main_size);
			width = max(width, ctx->cross_size);
			break;
		}
		w->width = ToBorderBoxWidth(w, width);
		w->height = ToBorderBoxHeight(w, height);
		Widget_UpdateBoxSize(w);
		if (ctx->is_initiative) {
			w->max_content_width = w->box.content.width;
			w->max_content_height = w->box.content.height;
		}
		w->proto->resize(w, w->box.content.width,
				 w->box.content.height);
		return;
	}
	switch (ctx->rule) {
	case LCUI_LAYOUT_RULE_FIXED:
		width = w->box.content.width;
		height = w->box.content.height;
		break;
	case LCUI_LAYOUT_RULE_FIXED_WIDTH:
		width = w->box.content.width;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = w->box.content.width;
		height = max(height, ctx->cross_size);
		break;
	case LCUI_LAYOUT_RULE_FIXED_HEIGHT:
		height = w->box.content.height;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->main_size);
		height = w->box.content.height;
		break;
	default:
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->main_size);
		height = max(height, ctx->cross_size);
		break;
	}
	w->width = ToBorderBoxWidth(w, width);
	w->height = ToBorderBoxHeight(w, height);
	Widget_UpdateBoxSize(w);
	if (ctx->is_initiative) {
		w->max_content_width = w->box.content.width;
		w->max_content_height = w->box.content.height;
	}
	w->proto->resize(w, w->box.content.width, w->box.content.height);
}

void LCUIFlexBoxLayout_Reflow(LCUI_Widget w, LCUI_LayoutRule rule)
{
	LCUI_FlexBoxLayoutContext ctx;

	ctx = FlexBoxLayout_Begin(w, rule);
	FlexBoxLayout_Load(ctx);
	FlexBoxLayout_ApplySize(ctx);
	FlexBoxLayout_Reflow(ctx);
	FlexBoxLayout_ReflowFreeElements(ctx);
	FlexBoxLayout_End(ctx);
}
