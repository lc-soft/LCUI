/*
 * widget_layout.c -- the widget layout processing module.
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

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

 /** 布局环境（上下文）数据 */
typedef struct LCUI_LayoutContextRec_ {
	float x, y;		/**< 坐标 */
	float line_height;	/**< 当前行的高度 */
	float line_width;	/**< 当前行的宽度 */
	LCUI_Widget container;	/**< 容器 */
	LCUI_Widget start;	/**< 当前行第一个部件 */
	LCUI_Widget current;	/**< 当前处理的部件 */
	LCUI_Widget prev;	/**< 上一个部件 */
	int prev_display;	/**< 上一个部件的显示方式 */
	float max_width;	/**< 容器的最大宽度 */
} LCUI_LayoutContextRec, *LCUI_LayoutContext;

/** 布局当前行的所有元素 */
static void LCUILayout_HandleCurrentLine(LCUI_LayoutContext ctx)
{
	float offset_x = 0;
	LinkedListNode *node;
	LCUI_WidgetStyle *style;
	LCUI_Widget child;

	style = &ctx->container->computed_style;
	if (style->display != SV_FLEX || !ctx->start) {
		goto finish_layout;
	}
	switch (style->flex.justify_content) {
	case SV_CENTER:
		offset_x = (ctx->max_width - ctx->line_width) / 2;
		break;
	case SV_FLEX_END:
		offset_x = ctx->max_width - ctx->line_width;
		break;
	case SV_FLEX_START:
	default:
		goto finish_layout;
	}
	node = &ctx->start->node;
	while (node) {
		if (ctx->current) {
			if (node == &ctx->current->node) {
				break;
			}
		}
		child = node->data;
		child->origin_x += offset_x;
		Widget_AddTask(child, LCUI_WTASK_POSITION);
		node = node->next;
	}
	ctx->start = ctx->current;

finish_layout:
	ctx->x = 0;
	ctx->y += ctx->line_height;
	ctx->line_width = 0;
	ctx->line_height = 0;
}

/** 布局当前内联块级元素 */
static void LCUILayout_HandleInlineBlock(LCUI_LayoutContext ctx)
{
	ctx->current->origin_x = ctx->x;
	ctx->x += ctx->current->box.outer.width;
	ctx->line_width += ctx->current->box.outer.width;
	/* 只考虑小数点后两位 */
	if (ctx->x - ctx->max_width >= 0.01) {
		LCUILayout_HandleCurrentLine(ctx);
		ctx->current->origin_x = 0;
		ctx->x += ctx->current->box.outer.width;
	}
	ctx->current->origin_y = ctx->y;
	if (ctx->current->box.outer.height > ctx->line_height) {
		ctx->line_height = ctx->current->box.outer.height;
	}
}

/** 布局当前块级元素 */
static void LCUILayout_HandleBlock(LCUI_LayoutContext ctx)
{
	ctx->x = 0;
	if (ctx->prev && ctx->prev_display != SV_BLOCK) {
		LCUILayout_HandleCurrentLine(ctx);
	}
	ctx->current->origin_x = ctx->x;
	ctx->current->origin_y = ctx->y;
	ctx->line_height = 0;
	ctx->y += ctx->current->box.outer.height;
	ctx->start = Widget_GetNext(ctx->current);
}

static LCUI_LayoutContext LCUILayout_Begin(LCUI_Widget w)
{
	ASSIGN(ctx, LCUI_LayoutContext);
	ctx->container = w;
	ctx->x = ctx->y = 0;
	ctx->line_height = ctx->line_width = 0;
	ctx->max_width = Widget_ComputeMaxContentWidth(w);
	ctx->current = LinkedList_Get(&w->children, 0);
	ctx->start = ctx->current;
	ctx->prev_display = SV_BLOCK;
	ctx->prev = NULL;
	return ctx;
}

static void LCUILayout_End(LCUI_LayoutContext ctx)
{
	if (ctx->current) {
		ctx->current = Widget_GetNext(ctx->current);
	}
	LCUILayout_HandleCurrentLine(ctx);
	free(ctx);
}

void Widget_DoLayout(LCUI_Widget w)
{
	LinkedListNode *node;
	LCUI_LayoutContext ctx;
	LCUI_WidgetEventRec ev = { 0 };

	ctx = LCUILayout_Begin(w);
	for (LinkedList_Each(node, &w->children)) {
		ctx->current = node->data;
		if (ctx->current->computed_style.position != SV_STATIC &&
		    ctx->current->computed_style.position != SV_RELATIVE) {
			Widget_AddState(ctx->current, LCUI_WSTATE_LAYOUTED);
			continue;
		}
		switch (ctx->current->computed_style.display) {
		case SV_INLINE_BLOCK:
			LCUILayout_HandleInlineBlock(ctx);
			break;
		case SV_FLEX:
		case SV_BLOCK:
			LCUILayout_HandleBlock(ctx);
			break;
		case SV_NONE:
		default:
			continue;
		}
		Widget_AddTask(ctx->current, LCUI_WTASK_POSITION);
		Widget_AddState(ctx->current, LCUI_WSTATE_LAYOUTED);
		ctx->prev_display = ctx->current->computed_style.display;
		ctx->prev = ctx->current;
	}
	LCUILayout_End(ctx);
	if (w->style->sheet[key_width].type == LCUI_STYPE_AUTO ||
	    w->style->sheet[key_height].type == LCUI_STYPE_AUTO) {
		Widget_AddTask(w, LCUI_WTASK_RESIZE);
	}
	ev.cancel_bubble = TRUE;
	ev.type = LCUI_WEVENT_AFTERLAYOUT;
	Widget_TriggerEvent(w, &ev, NULL);
}
