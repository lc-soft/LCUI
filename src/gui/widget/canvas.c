/*
 * canvas.c -- canvas, used to draw custom graphics
 *
 * Copyright (c) 2019, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget/canvas.h>

typedef struct CanvasRec_ {
	LCUI_Graph buffer;
	LinkedList contexts;
} CanvasRec, *Canvas;

struct {
	LCUI_WidgetPrototype proto;
} self;

static void Canvas_OnResize(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	float scale = LCUIMetrics_GetScale();
	unsigned width = (unsigned)(w->box.content.width * scale);
	unsigned height = (unsigned)(w->box.content.height * scale);

	LCUI_Graph buffer;
	Canvas canvas = Widget_GetData(w, self.proto);

	Graph_Init(&buffer);
	buffer.color_type = LCUI_COLOR_TYPE_ARGB;
	Graph_Create(&buffer, width, height);
	Graph_Replace(&buffer, &canvas->buffer, 0, 0);
	Graph_Free(&canvas->buffer);
	canvas->buffer = buffer;
}

static void Canvas_OnInit(LCUI_Widget w)
{
	Canvas canvas = Widget_AddData(w, self.proto, sizeof(CanvasRec));

	Graph_Init(&canvas->buffer);
	LinkedList_Init(&canvas->contexts);
	Widget_BindEventById(w, LCUI_WEVENT_RESIZE, Canvas_OnResize, NULL,
			     NULL);
}

static void Canvas_OnDestroy(LCUI_Widget w)
{
	LinkedListNode *node;
	LCUI_CanvasContext ctx;
	Canvas canvas = Widget_AddData(w, self.proto, sizeof(CanvasRec));

	for (LinkedList_Each(node, &canvas->contexts)) {
		ctx = node->data;
		ctx->available = FALSE;
	}
	LinkedList_ClearData(&canvas->contexts, NULL);
	Graph_Free(&canvas->buffer);
}

static void Canvas_AutoSize(LCUI_Widget w, float *width, float *height)
{
	*width = 300;
	*height = 150;
}

static void Canvas_OnPaint(LCUI_Widget w, LCUI_PaintContext paint,
			   LCUI_WidgetActualStyle style)
{
	LCUI_Graph src, dest;
	LCUI_Rect content_rect, rect;
	Canvas canvas = Widget_GetData(w, self.proto);

	content_rect.width = style->content_box.width;
	content_rect.height = style->content_box.height;
	content_rect.x = style->content_box.x - style->canvas_box.x;
	content_rect.y = style->content_box.y - style->canvas_box.y;
	if (!LCUIRect_GetOverlayRect(&content_rect, &paint->rect, &rect)) {
		return;
	}
	content_rect.x = rect.x - content_rect.x;
	content_rect.y = rect.y - content_rect.y;
	content_rect.width = rect.width;
	content_rect.height = rect.height;
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	Graph_Quote(&dest, &paint->canvas, &rect);
	Graph_Quote(&src, &canvas->buffer, &content_rect);
	Graph_Replace(&dest, &src, 0, 0);
}

static void CanvasContext_ClearRect(LCUI_CanvasContext ctx, int x, int y,
				    int width, int height)
{
	LCUI_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	Graph_FillRect(&ctx->buffer, ARGB(0, 0, 0, 0), &rect, TRUE);
}

static void CanvasContext_FillRect(LCUI_CanvasContext ctx, int x, int y,
				   int width, int height)
{
	LCUI_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	Graph_FillRect(&ctx->buffer, ctx->fill_color, &rect, TRUE);
}

static void CanvasContext_Release(LCUI_CanvasContext ctx)
{
	Canvas canvas;

	if (ctx->available) {
		canvas = Widget_GetData(ctx->canvas, self.proto);
		LinkedList_Unlink(&canvas->contexts, &ctx->node);
	}
	free(ctx);
}

LCUI_CanvasContext Canvas_GetContext(LCUI_Widget w)
{
	ASSIGN(ctx, LCUI_CanvasRenderingContext);
	Canvas canvas = Widget_GetData(w, self.proto);

	ctx->canvas = w;
	ctx->available = TRUE;
	ctx->buffer = canvas->buffer;
	ctx->width = ctx->buffer.width;
	ctx->height = ctx->buffer.height;
	ctx->fill_color = RGB(0, 0, 0);
	ctx->scale = LCUIMetrics_GetScale();
	ctx->clearRect = CanvasContext_ClearRect;
	ctx->fillRect = CanvasContext_FillRect;
	ctx->release = CanvasContext_Release;
	ctx->node.data = ctx;
	ctx->node.next = ctx->node.prev = NULL;
	LinkedList_AppendNode(&canvas->contexts, &ctx->node);
	return ctx;
}

void LCUIWidget_AddCanvas(void)
{
	self.proto = LCUIWidget_NewPrototype("canvas", NULL);
	self.proto->init = Canvas_OnInit;
	self.proto->destroy = Canvas_OnDestroy;
	self.proto->paint = Canvas_OnPaint;
	self.proto->autosize = Canvas_AutoSize;
}
