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
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>
#include "layout/block.h"
#include "layout/flexbox.h"

#define MEMCMP(A, B) memcmp(A, B, sizeof(*(A)))

static void LCUIWidgetLayout_ApplyChanges(LCUI_WidgetLayoutContext ctx)
{
	LCUI_RectF rect;
	LCUI_WidgetEventRec e;
	LCUI_Widget w = ctx->container;

	if (ctx->box.outer.x != w->box.outer.x ||
	    ctx->box.outer.y != w->box.outer.y) {
		ctx->invalid_box = SV_GRAPH_BOX;
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_MOVE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
	}
	if (ctx->box.outer.width != w->box.outer.width ||
	    ctx->box.outer.height != w->box.outer.height) {
		ctx->invalid_box = SV_GRAPH_BOX;
		e.target = w;
		e.data = NULL;
		e.type = LCUI_WEVENT_RESIZE;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent(w, &e, NULL);
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_RESIZE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
		if (w->parent) {
			Widget_AddTask(w->parent, LCUI_WTASK_REFLOW);
		}
	}
	if (!ctx->should_add_invalid_area || ctx->invalid_box == 0) {
		return;
	}
	if (ctx->invalid_box == SV_GRAPH_BOX) {
	} else if (MEMCMP(&ctx->box.canvas, &w->box.canvas)) {
		ctx->invalid_box = SV_GRAPH_BOX;
	} else if (MEMCMP(&ctx->box.padding, &w->box.padding)) {
		ctx->invalid_box = max(ctx->invalid_box, SV_PADDING_BOX);
	}
	if (w->parent) {
		if (!LCUIRectF_IsCoverRect(&ctx->box.canvas, &w->box.canvas)) {
			Widget_InvalidateArea(w->parent, &ctx->box.canvas,
					      SV_PADDING_BOX);
			Widget_InvalidateArea(w, NULL, ctx->invalid_box);
			return;
		}
		LCUIRectF_MergeRect(&rect, &ctx->box.canvas, &w->box.canvas);
		Widget_InvalidateArea(w->parent, &rect, SV_PADDING_BOX);
		return;
	}
	Widget_InvalidateArea(w, NULL, ctx->invalid_box);
}

void LCUIWidgetLayout_Reflow(LCUI_WidgetLayoutContext ctx)
{
	switch (ctx->container->computed_style.display) {
	case SV_BLOCK:
	case SV_INLINE_BLOCK:
		LCUIBlockLayout_Reflow(ctx);
		break;
	case SV_FLEX:
		LCUIFlexBoxLayout_Reflow(ctx);
		break;
	case SV_NONE:
	default:
		break;
	}
	LCUIWidgetLayout_ApplyChanges(ctx);
}

void LCUIWidgetLayout_ReflowChild(LCUI_WidgetLayoutContext ctx,
				  LCUI_Widget child)
{
	LCUI_WidgetLayoutContextRec child_ctx;

	child_ctx.container = child;
	child_ctx.box = child->box;
	child_ctx.invalid_box = 0;
	child_ctx.should_add_invalid_area =
	    ctx->invalid_box == 0 && ctx->should_add_invalid_area;
	LCUIWidgetLayout_Reflow(&child_ctx);
}
