/*
 * lib/ui-widgets/src/canvas.c: -- canvas, used to draw custom graphics
 *
 * Copyright (c) 2019-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <pandagl.h>
#include <ui.h>
#include <ui_widgets/canvas.h>

typedef struct ui_canvas_t {
	pd_canvas_t buffer;
	list_t contexts;
} ui_canvas_t;

static struct {
	ui_widget_prototype_t *proto;
} ui_canvas;

static void ui_canvas_on_resize(ui_widget_t *w, float width, float height)
{
	float scale = ui_get_actual_scale();

	pd_canvas_t buffer;
	ui_canvas_t *canvas = ui_widget_get_data(w, ui_canvas.proto);

	pd_canvas_init(&buffer);
	buffer.color_type = PD_COLOR_TYPE_ARGB;
	pd_canvas_create(&buffer, (unsigned)(width * scale),
			 (unsigned)(height * scale));
	pd_canvas_replace(&buffer, &canvas->buffer, 0, 0);
	pd_canvas_destroy(&canvas->buffer);
	canvas->buffer = buffer;
}

static void ui_canvas_on_init(ui_widget_t *w)
{
	ui_canvas_t *canvas;

	canvas = ui_widget_add_data(w, ui_canvas.proto, sizeof(ui_canvas_t));
	pd_canvas_init(&canvas->buffer);
	list_create(&canvas->contexts);
}

static void ui_canvas_on_destroy(ui_widget_t *w)
{
	list_node_t *node;
	ui_canvas_context_t *ctx;
	ui_canvas_t *canvas;

	canvas = ui_widget_add_data(w, ui_canvas.proto, sizeof(ui_canvas_t));
	for (list_each(node, &canvas->contexts)) {
		ctx = node->data;
		ctx->available = false;
	}
	list_destroy_without_node(&canvas->contexts, NULL);
	pd_canvas_destroy(&canvas->buffer);
}

static void ui_canvas_on_autosize(ui_widget_t *w, float *width, float *height)
{
	*width = UI_CANVAS_DEFAULT_WIDTH;
	*height = UI_CANVAS_DEFAULT_HEIGHT;
}

static void ui_canvas_on_paint(ui_widget_t *w, pd_context_t *paint,
			       ui_widget_actual_style_t *style)
{
	pd_canvas_t src, dest;
	pd_rect_t content_rect, rect;
	ui_canvas_t *canvas = ui_widget_get_data(w, ui_canvas.proto);

	content_rect.width = style->content_box.width;
	content_rect.height = style->content_box.height;
	content_rect.x = style->content_box.x - style->canvas_box.x;
	content_rect.y = style->content_box.y - style->canvas_box.y;
	if (!pd_rect_overlap(&content_rect, &paint->rect, &rect)) {
		return;
	}
	content_rect.x = rect.x - content_rect.x;
	content_rect.y = rect.y - content_rect.y;
	content_rect.width = rect.width;
	content_rect.height = rect.height;
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	pd_canvas_quote(&dest, &paint->canvas, &rect);
	pd_canvas_quote(&src, &canvas->buffer, &content_rect);
	pd_canvas_replace(&dest, &src, 0, 0);
}

static void ui_canvas_context_clear_rect(ui_canvas_context_t *ctx, int x, int y,
					 int width, int height)
{
	pd_rect_t rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	pd_canvas_fill_rect(&ctx->buffer, pd_argb(0, 0, 0, 0), rect);
}

static void ui_canvas_context_fill_rect(ui_canvas_context_t *ctx, int x, int y,
					int width, int height)
{
	pd_rect_t rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	pd_canvas_fill_rect(&ctx->buffer, ctx->fill_color, rect);
}

static void ui_canvas_context_release(ui_canvas_context_t *ctx)
{
	ui_canvas_t *canvas;

	if (ctx->available) {
		canvas = ui_widget_get_data(ctx->canvas, ui_canvas.proto);
		list_unlink(&canvas->contexts, &ctx->node);
	}
	free(ctx);
}

ui_canvas_context_t *ui_canvas_get_context(ui_widget_t *w)
{
	ui_canvas_context_t *ctx;
	ui_canvas_t *canvas = ui_widget_get_data(w, ui_canvas.proto);

	ctx = malloc(sizeof(ui_canvas_context_t));
	ctx->canvas = w;
	ctx->available = true;
	ctx->buffer = canvas->buffer;
	ctx->width = ctx->buffer.width;
	ctx->height = ctx->buffer.height;
	ctx->fill_color = pd_rgb(0, 0, 0);
	ctx->scale = ui_get_actual_scale();
	ctx->clear_rect = ui_canvas_context_clear_rect;
	ctx->fill_rect = ui_canvas_context_fill_rect;
	ctx->release = ui_canvas_context_release;
	ctx->node.data = ctx;
	ctx->node.next = ctx->node.prev = NULL;
	list_append_node(&canvas->contexts, &ctx->node);
	return ctx;
}

void ui_register_canvas(void)
{
	ui_canvas.proto = ui_create_widget_prototype("canvas", NULL);
	ui_canvas.proto->init = ui_canvas_on_init;
	ui_canvas.proto->destroy = ui_canvas_on_destroy;
	ui_canvas.proto->paint = ui_canvas_on_paint;
	ui_canvas.proto->autosize = ui_canvas_on_autosize;
	ui_canvas.proto->resize = ui_canvas_on_resize;
}
