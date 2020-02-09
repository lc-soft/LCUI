/*
 * block.c -- block layout
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
#include "block.h"
#include "../widget_util.h"
#include "../widget_diff.h"

typedef struct LCUI_BlockLayoutRowRec_ {
	float width;
	float height;
	LinkedList elements;
} LCUI_BlockLayoutRowRec, *LCUI_BlockLayoutRow;

typedef struct LCUI_BlockLayoutContextRec_ {
	LCUI_Widget widget;
	LCUI_LayoutRule rule;
	LCUI_BOOL is_initiative;

	float x, y;
	float content_width;
	float content_height;

	LCUI_Widget prev;
	int prev_display;

	/*
	 * LinkedList<LCUI_BlockLayoutRow> rows
	 * Element rows in the static layout flow
	 */
	LinkedList rows;
	LCUI_BlockLayoutRow row;

	/*
	 * LinkedList<LCUI_Widget> free_elements
	 * A list of elements that do not exist in the static layout flow
	 */
	LinkedList free_elements;
} LCUI_BlockLayoutContextRec, *LCUI_BlockLayoutContext;

static void BlockLayout_UpdateElementPosition(LCUI_BlockLayoutContext ctx,
					      LCUI_Widget w, float x, float y)
{
	if (w->computed_style.position != SV_ABSOLUTE &&
	    w->computed_style.display == SV_INLINE_BLOCK) {
		switch (w->computed_style.vertical_align) {
		case SV_MIDDLE:
			if (!w->parent) {
				break;
			}
			y += (ctx->row->height - w->height) / 2.f;
			break;
		case SV_BOTTOM:
			if (!w->parent) {
				break;
			}
			y += ctx->row->height - w->height;
		case SV_TOP:
		default:
			break;
		}
	}
	w->layout_x = x;
	w->layout_y = y;
	Widget_UpdateBoxPosition(w);
}

static LCUI_BlockLayoutRow BlockLayoutRow_Create(void)
{
	LCUI_BlockLayoutRow row;

	row = malloc(sizeof(LCUI_BlockLayoutRowRec));
	row->width = 0;
	row->height = 0;
	LinkedList_Init(&row->elements);
	return row;
}

static void BlockLayoutRow_Destroy(void *arg)
{
	LCUI_BlockLayoutRow row = arg;

	LinkedList_Clear(&row->elements, NULL);
	free(row);
}

static void BlockLayout_NextRow(LCUI_BlockLayoutContext ctx)
{
	if (ctx->row) {
		ctx->content_width = max(ctx->content_width, ctx->row->width);
		ctx->content_height += ctx->row->height;
		ctx->y += ctx->row->height;
	}
	ctx->prev_display = 0;
	ctx->x = ctx->widget->padding.left;
	ctx->row = BlockLayoutRow_Create();
	LinkedList_Append(&ctx->rows, ctx->row);
}

static LCUI_BlockLayoutContext BlockLayout_Begin(LCUI_Widget w,
						 LCUI_LayoutRule rule)
{
	LCUI_WidgetStyle *style = &w->computed_style;
	ASSIGN(ctx, LCUI_BlockLayoutContext);

	if (rule == LCUI_LAYOUT_RULE_AUTO) {
		ctx->is_initiative = TRUE;
		if (style->width_sizing == LCUI_SIZING_RULE_FIXED) {
			if (style->height_sizing == LCUI_SIZING_RULE_FIXED) {
				rule = LCUI_LAYOUT_RULE_FIXED;
			} else {
				rule = LCUI_LAYOUT_RULE_FIXED_WIDTH;
			}
		} else if (style->height_sizing == LCUI_SIZING_RULE_FIXED) {
			rule = LCUI_LAYOUT_RULE_FIXED_HEIGHT;
		} else {
			rule = LCUI_LAYOUT_RULE_MAX_CONTENT;
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
	if (rule == LCUI_LAYOUT_RULE_MAX_CONTENT) {
		if (style->width_sizing == LCUI_SIZING_RULE_FIXED) {
			rule = LCUI_LAYOUT_RULE_FIXED_WIDTH;
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
	ctx->rule = rule;
	ctx->row = NULL;
	ctx->widget = w;
	ctx->x = w->padding.left;
	ctx->y = w->padding.right;
	ctx->content_width = 0;
	ctx->content_height = 0;
	ctx->prev_display = 0;
	ctx->prev = NULL;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->rows);
	BlockLayout_NextRow(ctx);
	return ctx;
}

static void UpdateBlockItemSize(LCUI_Widget w, LCUI_LayoutRule rule)
{
	float content_width = w->box.content.width;
	float content_height = w->box.content.height;
	LCUI_WidgetLayoutDiffRec diff;

	Widget_BeginLayoutDiff(w, &diff);
	Widget_ComputeWidthLimitStyle(w, LCUI_LAYOUT_RULE_FIXED);
	Widget_ComputeHeightLimitStyle(w, LCUI_LAYOUT_RULE_FIXED);
	Widget_ComputeWidthStyle(w);
	Widget_ComputeHeightStyle(w);
	Widget_UpdateBoxSize(w);
	if (content_width == w->box.content.width &&
	    content_height == w->box.content.height) {
		return;
	}
	Widget_Reflow(w, rule);
	Widget_EndLayoutDiff(w, &diff);
	w->task.states[LCUI_WTASK_REFLOW] = FALSE;
}

static void BlockLayout_Load(LCUI_BlockLayoutContext ctx)
{
	float max_row_width = -1;

	LCUI_Widget child;
	LCUI_Widget w = ctx->widget;
	LinkedListNode *node;

	if (ctx->rule == LCUI_LAYOUT_RULE_FIXED_WIDTH ||
	    ctx->rule == LCUI_LAYOUT_RULE_FIXED) {
		max_row_width = ctx->widget->box.content.width;
	} else {
		if (w->computed_style.max_width != -1) {
			max_row_width = w->computed_style.max_width -
					PaddingX(w) - BorderX(w);
		}
	}
	DEBUG_MSG("%s, start\n", ctx->widget->id);
	DEBUG_MSG("%s, max_row_width: %g\n", ctx->widget->id, max_row_width);
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;

		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		if (child->computed_style.width_sizing !=
			LCUI_SIZING_RULE_FIXED &&
		    child->computed_style.width_sizing !=
			LCUI_SIZING_RULE_FIT_CONTENT) {
			UpdateBlockItemSize(child,
					    LCUI_LAYOUT_RULE_MAX_CONTENT);
		}
		DEBUG_MSG(
		    "row %lu, child %lu, static size: (%g, %g), display: %d\n",
		    ctx->rows.length, child->index, child->box.outer.width,
		    child->box.outer.height, child->computed_style.display);
		switch (child->computed_style.display) {
		case SV_INLINE_BLOCK:
			if (ctx->prev_display &&
			    ctx->prev_display != SV_INLINE_BLOCK) {
				DEBUG_MSG("next row\n");
				BlockLayout_NextRow(ctx);
			}
			if (max_row_width != -1 &&
			    ctx->row->elements.length > 0 &&
			    ctx->row->width + child->box.outer.width -
				    max_row_width >
				0.4f) {
				DEBUG_MSG("next row\n");
				BlockLayout_NextRow(ctx);
			}
			break;
		case SV_FLEX:
		case SV_BLOCK:
			BlockLayout_NextRow(ctx);
			break;
		case SV_NONE:
		default:
			continue;
		}
		DEBUG_MSG("row %lu, xy: (%g, %g)\n", ctx->rows.length, ctx->x,
			  ctx->y);
		ctx->row->width += child->box.outer.width;
		if (child->box.outer.height > ctx->row->height) {
			ctx->row->height = child->box.outer.height;
		}
		LinkedList_Append(&ctx->row->elements, child);
		ctx->prev_display = child->computed_style.display;
		ctx->prev = child;
	}
	ctx->content_width = max(ctx->content_width, ctx->row->width);
	ctx->content_height += ctx->row->height;
	DEBUG_MSG("content_size: %g, %g\n", ctx->content_width,
		  ctx->content_height);
	DEBUG_MSG("%s, end\n", ctx->widget->id);
}

static void BlockLayout_UpdateElementMargin(LCUI_BlockLayoutContext ctx,
					    LCUI_Widget w)
{
	if (w->computed_style.display != SV_BLOCK) {
		return;
	}
	if (!Widget_HasAutoStyle(w, key_margin_left)) {
		return;
	}
	if (Widget_HasAutoStyle(w, key_margin_right)) {
		w->margin.left = (ctx->content_width - w->width) / 2.f;
		w->margin.right = w->margin.left;
		return;
	}
	w->margin.left = ctx->content_width - w->width;
	w->margin.left -= w->margin.right;
}

static void BlockLayout_ReflowRow(LCUI_BlockLayoutContext ctx, float row_y)
{
	float x = ctx->widget->padding.left;

	LCUI_Widget w;
	LinkedListNode *node;

	for (LinkedList_Each(node, &ctx->row->elements)) {
		w = node->data;
		UpdateBlockItemSize(w, LCUI_LAYOUT_RULE_FIXED);
		BlockLayout_UpdateElementMargin(ctx, w);
		BlockLayout_UpdateElementPosition(ctx, w, x, row_y);
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		x += w->box.outer.width;
	}
}

static void BlockLayout_ReflowFreeElements(LCUI_BlockLayoutContext ctx)
{
	LCUI_Widget w;
	LinkedListNode *node;

	for (LinkedList_Each(node, &ctx->free_elements)) {
		w = node->data;
		Widget_ComputeSizeStyle(w);
		Widget_UpdateBoxSize(w);
		Widget_UpdateBoxPosition(w);
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
		w->proto->resize(w, w->box.content.width,
				 w->box.content.height);
	}
}

static void BlockLayout_Reflow(LCUI_BlockLayoutContext ctx)
{
	float y;
	LCUI_Widget w = ctx->widget;
	LinkedListNode *node;

	y = w->padding.top;
	if (w->computed_style.display != SV_INLINE_BLOCK) {
		ctx->content_width = w->box.content.width;
	}
	for (LinkedList_Each(node, &ctx->rows)) {
		ctx->row = node->data;
		BlockLayout_ReflowRow(ctx, y);
		y += ctx->row->height;
	}
	ctx->content_height = y - w->padding.top;
}

static void BlockLayout_End(LCUI_BlockLayoutContext ctx)
{
	LinkedList_Clear(&ctx->rows, BlockLayoutRow_Destroy);
	LinkedList_Clear(&ctx->free_elements, NULL);
	free(ctx);
}

static void BlockLayout_ApplySize(LCUI_BlockLayoutContext ctx)
{
	float width = 0, height = 0;

	LCUI_Widget w = ctx->widget;

	switch (ctx->rule) {
	case LCUI_LAYOUT_RULE_FIXED_WIDTH:
		width = w->box.content.width;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = w->box.content.width;
		height = max(height, ctx->content_height);
		break;
	case LCUI_LAYOUT_RULE_FIXED_HEIGHT:
		height = w->box.content.height;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->content_width);
		height = w->box.content.height;
		break;
	case LCUI_LAYOUT_RULE_MAX_CONTENT:
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->content_width);
		height = max(height, ctx->content_height);
		break;
	default:
		width = w->box.content.width;
		height = w->box.content.height;
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

void LCUIBlockLayout_Reflow(LCUI_Widget w, LCUI_LayoutRule rule)
{
	LCUI_BlockLayoutContext ctx;

	ctx = BlockLayout_Begin(w, rule);
	BlockLayout_Load(ctx);
	BlockLayout_ApplySize(ctx);
	BlockLayout_Reflow(ctx);
	BlockLayout_ReflowFreeElements(ctx);
	BlockLayout_End(ctx);
}
