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

typedef struct LCUI_BlockLayoutRowRec_ {
	float width;
	float height;
	LinkedList elements;
} LCUI_BlockLayoutRowRec, *LCUI_BlockLayoutRow;

typedef struct LCUI_BlockLayoutContextRec_ {
	LCUI_WidgetLayoutContext base;

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

static void BlockLayout_ApplySize(LCUI_BlockLayoutContext ctx)
{
	Widget_SetContentSize(ctx->base->container, ctx->content_width,
			      ctx->content_height);
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
	ctx->x = ctx->base->container->padding.left;
	ctx->row = BlockLayoutRow_Create();
	LinkedList_Append(&ctx->rows, ctx->row);
}

static LCUI_BlockLayoutContext BlockLayout_Begin(LCUI_WidgetLayoutContext base)
{
	ASSIGN(ctx, LCUI_BlockLayoutContext);

	ctx->row = NULL;
	ctx->base = base;
	ctx->x = ctx->base->container->padding.left;
	ctx->y = ctx->base->container->padding.right;
	ctx->content_width = 0;
	ctx->content_height = 0;
	ctx->prev_display = SV_BLOCK;
	ctx->prev = NULL;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->rows);
	BlockLayout_NextRow(ctx);
	return ctx;
}

static void BlockLayout_Load(LCUI_BlockLayoutContext ctx)
{
	float max_row_width = -1;
	float static_width, static_height;

	LCUI_Widget child;
	LCUI_Widget w = ctx->base->container;
	LCUI_SizingRule width_sizing = Widget_GetWidthSizingRule(w);
	LCUI_SizingRule height_sizing = Widget_GetHeightSizingRule(w);
	LinkedListNode *node;

	if (w->computed_style.display != SV_INLINE_BLOCK) {
		max_row_width = w->box.content.width;
	}
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;

		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		static_width = child->box.outer.width;
		static_height = child->box.outer.height;
		if (width_sizing == LCUI_SIZING_RULE_FIT_CONTENT &&
		    !Widget_HasStaticWidth(child)) {
			static_width -= child->box.content.width;
			static_width += child->min_content_width;
		}
		if (height_sizing == LCUI_SIZING_RULE_FIT_CONTENT &&
		    !Widget_HasStaticHeight(child)) {
			static_height -= child->box.content.height;
			static_height += child->min_content_height;
		}
		switch (child->computed_style.display) {
		case SV_INLINE_BLOCK:
			if (ctx->prev_display != SV_INLINE_BLOCK) {
				BlockLayout_NextRow(ctx);
			}
			if (max_row_width != -1 &&
			    ctx->row->elements.length > 0 &&
			    ctx->row->width + static_width > max_row_width) {
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
		ctx->row->width += static_width;
		if (static_height > ctx->row->height) {
			ctx->row->height = static_height;
		}
		LinkedList_Append(&ctx->row->elements, child);
		ctx->prev_display = child->computed_style.display;
		ctx->prev = child;
	}
	ctx->content_width = max(ctx->content_width, ctx->row->width);
	ctx->content_height += ctx->row->height;
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
	float x = ctx->base->container->padding.left;

	LCUI_Widget w;
	LinkedListNode *node;

	for (LinkedList_Each(node, &ctx->row->elements)) {
		w = node->data;
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
		Widget_UpdateBoxSize(node->data);
		Widget_UpdateBoxPosition(node->data);
		Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
	}
}

static void BlockLayout_Reflow(LCUI_BlockLayoutContext ctx)
{
	float y;
	LCUI_Widget w = ctx->base->container;
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
	LCUI_Widget w = ctx->base->container;
	LCUI_WidgetEventRec ev = { 0 };

	ev.cancel_bubble = TRUE;
	ev.type = LCUI_WEVENT_AFTERLAYOUT;
	Widget_TriggerEvent(w, &ev, NULL);
	LinkedList_Clear(&ctx->rows, BlockLayoutRow_Destroy);
	LinkedList_Clear(&ctx->free_elements, NULL);
	free(ctx);
}

void LCUIBlockLayout_Reflow(LCUI_WidgetLayoutContext base_ctx)
{
	LCUI_BlockLayoutContext ctx;

	ctx = BlockLayout_Begin(base_ctx);
	BlockLayout_Load(ctx);
	BlockLayout_ApplySize(ctx);
	BlockLayout_Reflow(ctx);
	BlockLayout_ReflowFreeElements(ctx);
	BlockLayout_End(ctx);
}
