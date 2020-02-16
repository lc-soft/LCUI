/*
 * widget_paint.c -- LCUI widget paint module.
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

//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/display.h>

//#define DEBUG_FRAME_RENDER
#define ComputeActualPX(VAL) LCUIMetrics_ComputeActual(VAL, LCUI_STYPE_PX)

#define MAX_VISIBLE_WIDTH 20000
#define MAX_VISIBLE_HEIGHT 20000

#ifdef DEBUG_FRAME_RENDER
#include <LCUI/image.h>
#endif

typedef struct LCUI_RectGroupRec_ {
	LCUI_Widget widget;
	LinkedList rects;
} LCUI_RectGroupRec, *LCUI_RectGroup;

typedef struct LCUI_WidgetRendererRec_ {
	/* target widget position, it relative to root canvas */
	float x, y;

	/* content area top spacing, it relative to widget canvas */
	float content_top;

	/* content area left spacing, it relative to widget canvas */
	float content_left;

	/* target widget */
	LCUI_Widget target;

	/* computed actual style */
	LCUI_WidgetActualStyle style;

	/* current target widget paint context */
	LCUI_PaintContext paint;

	/* root paint context */
	LCUI_PaintContext root_paint;

	/* content canvas */
	LCUI_Graph content_graph;

	/* target widget canvas */
	LCUI_Graph self_graph;

	/* layer canvas, used to mix content and self canvas with widget
	 * opacity */
	LCUI_Graph layer_graph;

	/* actual paint rectangle in widget canvas rectangle, it relative to
	 * root canvas */
	LCUI_Rect actual_paint_rect;

	/* actual paint rectangle in widget content rectangle, it relative to
	 * root canvas */
	LCUI_Rect actual_content_rect;

	/* Paint rectangle in widget content rectangle, it relative to
	 * root canvas */
	LCUI_RectF content_rect;

	LCUI_BOOL has_content_graph;
	LCUI_BOOL has_self_graph;
	LCUI_BOOL has_layer_graph;
	LCUI_BOOL can_render_self;
	LCUI_BOOL can_render_centent;
} LCUI_WidgetRendererRec, *LCUI_WidgetRenderer;

static struct LCUI_WidgetRenderModule {
	LCUI_BOOL active;
	RBTree groups;
	LinkedList rects;
} self = { 0 };

/** 判断部件是否有可绘制内容 */
static LCUI_BOOL Widget_IsPaintable(LCUI_Widget w)
{
	const LCUI_WidgetStyle *s = &w->computed_style;
	if (s->background.color.alpha > 0 ||
	    Graph_IsValid(&s->background.image) || s->border.top.width > 0 ||
	    s->border.right.width > 0 || s->border.bottom.width > 0 ||
	    s->border.left.width > 0 || s->shadow.blur > 0 ||
	    s->shadow.spread > 0) {
		return TRUE;
	}
	return w->proto && w->proto->paint;
}

static LCUI_BOOL Widget_HasRoundBorder(LCUI_Widget w)
{
	const LCUI_BorderStyle *s = &w->computed_style.border;

	return s->top_left_radius || s->top_right_radius ||
	       s->bottom_left_radius || s->bottom_right_radius;
}

/**
 * 根据所处框区域，调整矩形
 * @param[in] w		目标部件
 * @param[in] in_rect	矩形
 * @param[out] out_rect	调整后的矩形
 * @param[in] box_type	区域相对于何种框进行定位
 */
static void Widget_AdjustArea(LCUI_Widget w, LCUI_RectF *in_rect,
			      LCUI_RectF *out_rect, int box_type)
{
	LCUI_RectF *box;
	switch (box_type) {
	case SV_BORDER_BOX:
		box = &w->box.border;
		break;
	case SV_GRAPH_BOX:
		box = &w->box.canvas;
		break;
	case SV_PADDING_BOX:
		box = &w->box.padding;
		break;
	case SV_CONTENT_BOX:
	default:
		box = &w->box.content;
		break;
	}
	/* 如果为NULL，则视为使用整个部件区域 */
	if (!in_rect) {
		out_rect->x = out_rect->y = 0;
		out_rect->width = box->width;
		out_rect->height = box->height;
	} else {
		*out_rect = *in_rect;
		LCUIRectF_ValidateArea(out_rect, box->width, box->height);
	}
	/* 将坐标转换成相对于图像呈现区的坐标 */
	out_rect->x += box->x - w->box.canvas.x;
	out_rect->y += box->y - w->box.canvas.y;
}

void RectFToInvalidArea(const LCUI_RectF *rect, LCUI_Rect *area)
{
	LCUIMetrics_ComputeRectActual(area, rect);
}

void RectToInvalidArea(const LCUI_Rect *rect, LCUI_Rect *area)
{
	LCUI_RectF rectf;
	LCUIRect_ToRectF(rect, &rectf, 1.0f);
	LCUIMetrics_ComputeRectActual(area, &rectf);
}

LCUI_BOOL Widget_InvalidateArea(LCUI_Widget w, LCUI_RectF *in_rect,
				int box_type)
{
	LCUI_RectF rect;

	if (!w) {
		w = LCUIWidget_GetRoot();
	}
	if (!w->computed_style.visible ||
	    w->invalid_area_type > LCUI_INVALID_AREA_TYPE_CUSTOM) {
		return FALSE;
	}
	Widget_AdjustArea(w, in_rect, &rect, box_type);
	rect.x += w->box.canvas.x;
	rect.y += w->box.canvas.y;
	if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_CUSTOM) {
		LCUIRectF_MergeRect(&w->invalid_area, &rect, &w->invalid_area);
	} else {
		w->invalid_area = rect;
	}
	w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CUSTOM;
	while (w->parent) {
		w->parent->has_child_invalid_area = TRUE;
		w = w->parent;
	}
	return TRUE;
}

#define AddInvalidArea()                                               \
	do {                                                           \
		rect.x += x;                                           \
		rect.y += y;                                           \
		LCUIRectF_GetOverlayRect(&rect, &visible_area, &rect); \
		if (rect.width > 0 && rect.height > 0) {               \
			actual_rect = malloc(sizeof(LCUI_Rect));       \
			RectFToInvalidArea(&rect, actual_rect);        \
			LinkedList_Append(rects, actual_rect);         \
		}                                                      \
	} while (0)

static void Widget_CollectInvalidArea(LCUI_Widget w, LinkedList *rects, float x,
				      float y, LCUI_RectF visible_area)
{
	LCUI_RectF rect;
	LCUI_Rect *actual_rect;
	LinkedListNode *node;

	if (w->parent && w->parent->invalid_area_type >=
			     LCUI_INVALID_AREA_TYPE_PADDING_BOX) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	} else if (w->invalid_area_type >= LCUI_INVALID_AREA_TYPE_PADDING_BOX) {
		switch (w->invalid_area_type) {
		case LCUI_INVALID_AREA_TYPE_PADDING_BOX:
			rect = w->box.padding;
			break;
		case LCUI_INVALID_AREA_TYPE_BORDER_BOX:
			rect = w->box.border;
			break;
		default:
			rect = w->box.canvas;
			break;
		}
		if (!LCUIRectF_IsCoverRect(&rect, &w->invalid_area)) {
			AddInvalidArea();
			rect = w->invalid_area;
			AddInvalidArea();
		} else {
			LCUIRectF_MergeRect(&rect, &rect, &w->invalid_area);
			AddInvalidArea();
		}
	} else if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_CUSTOM) {
		rect = w->invalid_area;
		AddInvalidArea();
	}
	if (w->has_child_invalid_area) {
		visible_area.x -= x;
		visible_area.y -= y;
		LCUIRectF_GetOverlayRect(&visible_area, &w->box.padding,
					 &visible_area);
		visible_area.x += x;
		visible_area.y += y;
		for (LinkedList_Each(node, &w->children_show)) {
			Widget_CollectInvalidArea(
			    node->data, rects, x + w->box.padding.x,
			    y + w->box.padding.y, visible_area);
		}
	}
	w->invalid_area_type = LCUI_INVALID_AREA_TYPE_NONE;
	w->has_child_invalid_area = FALSE;
}

size_t Widget_GetInvalidArea(LCUI_Widget w, LinkedList *rects)
{
	LCUI_Rect *rect;
	LinkedListNode *node;

	float scale = LCUIMetrics_GetScale();
	int x = iround(w->box.padding.x * scale);
	int y = iround(w->box.padding.y * scale);

	Widget_CollectInvalidArea(w, rects, 0, 0, w->box.padding);
	for (LinkedList_Each(node, rects)) {
		rect = node->data;
		rect->x -= x;
		rect->y -= y;
	}
	return rects->length;
}

static int OnCompareGroup(void *data, const void *keydata)
{
	LCUI_RectGroup group = data;
	return (int)((char *)group->widget - (char *)keydata);
}

static void OnDestroyGroup(void *data)
{
	LCUI_RectGroup group = data;
	RectList_Clear(&group->rects);
	group->widget = NULL;
}

void LCUIWidget_InitRenderer(void)
{
	RBTree_Init(&self.groups);
	RBTree_OnCompare(&self.groups, OnCompareGroup);
	RBTree_OnDestroy(&self.groups, OnDestroyGroup);
	LinkedList_Init(&self.rects);
	self.active = TRUE;
}

void LCUIWidget_FreeRenderer(void)
{
	self.active = FALSE;
	RectList_Clear(&self.rects);
	RBTree_Destroy(&self.groups);
}

/** 当前部件的绘制函数 */
static void Widget_OnPaint(LCUI_Widget w, LCUI_PaintContext paint,
			   LCUI_WidgetActualStyle style)
{
	Widget_PaintBakcground(w, paint, style);
	Widget_PaintBorder(w, paint, style);
	Widget_PaintBoxShadow(w, paint, style);
	if (w->proto && w->proto->paint) {
		w->proto->paint(w, paint, style);
	}
}

int Widget_ConvertArea(LCUI_Widget w, LCUI_Rect *in_rect, LCUI_Rect *out_rect,
		       int box_type)
{
	LCUI_RectF rect;
	if (!in_rect) {
		return -1;
	}
	switch (box_type) {
	case SV_CONTENT_BOX:
		rect = w->box.content;
		break;
	case SV_PADDING_BOX:
		rect = w->box.content;
		rect.x -= w->padding.left;
		rect.y -= w->padding.top;
		rect.width += w->padding.left;
		rect.width += w->padding.right;
		rect.height += w->padding.top;
		rect.height += w->padding.bottom;
		break;
	case SV_BORDER_BOX:
		rect = w->box.border;
		break;
	case SV_GRAPH_BOX:
	default:
		return -2;
	}
	/* 转换成相对坐标 */
	rect.x -= w->box.canvas.x;
	rect.y -= w->box.canvas.y;
	out_rect->x = in_rect->x - (int)rect.x;
	out_rect->y = in_rect->y - (int)rect.y;
	out_rect->width = in_rect->width;
	out_rect->height = in_rect->height;
	/* 裁剪掉超出范围的区域 */
	if (out_rect->x < 0) {
		out_rect->width += out_rect->x;
		out_rect->x = 0;
	}
	if (out_rect->y < 0) {
		out_rect->height += out_rect->y;
		out_rect->y = 0;
	}
	if (out_rect->x + out_rect->width > (int)rect.width) {
		out_rect->width = (int)rect.width - out_rect->x;
	}
	if (out_rect->y + out_rect->height > (int)rect.height) {
		out_rect->height = (int)rect.height - out_rect->y;
	}
	return 0;
}

static LCUI_WidgetRenderer WidgetRenderer(LCUI_Widget w,
					  LCUI_PaintContext paint,
					  LCUI_WidgetActualStyle style,
					  LCUI_WidgetRenderer parent)
{
	ASSIGN(that, LCUI_WidgetRenderer);

	that->target = w;
	that->style = style;
	that->paint = paint;
	that->has_self_graph = FALSE;
	that->has_layer_graph = FALSE;
	that->has_content_graph = FALSE;
	if (parent) {
		that->root_paint = parent->root_paint;
		that->x = parent->x + parent->content_left + w->box.canvas.x;
		that->y = parent->y + parent->content_top + w->box.canvas.y;
	} else {
		that->x = that->y = 0;
		that->root_paint = that->paint;
	}
	if (w->computed_style.opacity < 1.0) {
		that->has_self_graph = TRUE;
		that->has_content_graph = TRUE;
		that->has_layer_graph = TRUE;
	} else if (Widget_HasRoundBorder(w)) {
		that->has_content_graph = TRUE;
	}
	Graph_Init(&that->self_graph);
	Graph_Init(&that->layer_graph);
	Graph_Init(&that->content_graph);
	that->layer_graph.color_type = LCUI_COLOR_TYPE_ARGB;
	that->can_render_self = Widget_IsPaintable(w);
	if (that->can_render_self) {
		that->self_graph.color_type = LCUI_COLOR_TYPE_ARGB;
		Graph_Create(&that->self_graph, that->paint->rect.width,
			     that->paint->rect.height);
	}
	/* get content rectangle left spacing and top */
	that->content_left = w->box.padding.x - w->box.canvas.x;
	that->content_top = w->box.padding.y - w->box.canvas.y;
	/* convert position of paint rectangle to root canvas relative */
	that->actual_paint_rect.x =
	    that->style->canvas_box.x + that->paint->rect.x;
	that->actual_paint_rect.y =
	    that->style->canvas_box.y + that->paint->rect.y;
	that->actual_paint_rect.width = that->paint->rect.width;
	that->actual_paint_rect.height = that->paint->rect.height;
	DEBUG_MSG("[%s] paint_rect: (%d, %d, %d, %d)\n", w->id,
		  that->actual_paint_rect.x, that->actual_paint_rect.y,
		  that->actual_paint_rect.width,
		  that->actual_paint_rect.height);
	/* get actual paint rectangle in widget content rectangle */
	that->can_render_centent = LCUIRect_GetOverlayRect(
	    &that->style->padding_box, &that->actual_paint_rect,
	    &that->actual_content_rect);
	;
	LCUIRect_ToRectF(&that->actual_content_rect, &that->content_rect,
			 1.0f / LCUIMetrics_GetScale());
	DEBUG_MSG("[%s] content_rect: (%d, %d, %d, %d)\n", w->id,
		  that->actual_content_rect.x, that->actual_content_rect.y,
		  that->actual_content_rect.width,
		  that->actual_content_rect.height);
	DEBUG_MSG("[%s] content_box: (%d, %d, %d, %d)\n", w->id,
		  style->content_box.x, style->content_box.y,
		  style->content_box.width, style->content_box.height);
	DEBUG_MSG("[%s] border_box: (%d, %d, %d, %d)\n", w->id,
		  style->border_box.x, style->border_box.y,
		  style->border_box.width, style->border_box.height);
	DEBUG_MSG(
	    "[%s][%d/%d] content_rect: (%d,%d,%d,%d), "
	    "canvas_rect: (%d,%d,%d,%d)\n",
	    w->id, w->index, w->parent ? w->parent->children_show.length : 1,
	    that->actual_content_rect.x, that->actual_content_rect.y,
	    that->actual_content_rect.width, that->actual_content_rect.height,
	    that->paint->canvas.quote.left, that->paint->canvas.quote.top,
	    that->paint->canvas.width, that->paint->canvas.height);
	if (!that->can_render_centent) {
		return that;
	}
	if (that->has_content_graph) {
		that->content_graph.color_type = LCUI_COLOR_TYPE_ARGB;
		Graph_Create(&that->content_graph,
			     that->actual_content_rect.width,
			     that->actual_content_rect.height);
	}
	return that;
}

static void WidgetRenderer_Delete(LCUI_WidgetRenderer renderer)
{
	Graph_Free(&renderer->layer_graph);
	Graph_Free(&renderer->self_graph);
	Graph_Free(&renderer->content_graph);
	free(renderer);
}

static size_t WidgetRenderer_Render(LCUI_WidgetRenderer renderer);

static void Widget_ComputeActualBorderBox(LCUI_Widget w,
					  LCUI_WidgetActualStyle s)
{
	LCUI_RectF rect;
	rect.x = s->x + w->box.border.x;
	rect.y = s->y + w->box.border.y;
	rect.width = w->box.border.width;
	rect.height = w->box.border.height;
	Widget_ComputeBorder(w, &s->border);
	LCUIMetrics_ComputeRectActual(&s->border_box, &rect);
}

static void Widget_ComputeActualCanvasBox(LCUI_Widget w,
					  LCUI_WidgetActualStyle s)
{
	Widget_ComputeBoxShadow(w, &s->shadow);
	BoxShadow_GetCanvasRect(&s->shadow, &s->border_box, &s->canvas_box);
}

static void Widget_ComputeActualPaddingBox(LCUI_Widget w,
					   LCUI_WidgetActualStyle s)
{
	Widget_ComputeBackground(w, &s->background);
	s->padding_box.x = s->border_box.x + s->border.left.width;
	s->padding_box.y = s->border_box.y + s->border.top.width;
	s->padding_box.width = s->border_box.width - s->border.left.width;
	s->padding_box.width -= s->border.right.width;
	s->padding_box.height = s->border_box.height - s->border.top.width;
	s->padding_box.height -= s->border.bottom.width;
}

static void Widget_ComputeActualContentBox(LCUI_Widget w,
					   LCUI_WidgetActualStyle s)
{
	LCUI_RectF rect;
	rect.x = s->x + w->box.content.x;
	rect.y = s->y + w->box.content.y;
	rect.width = w->box.content.width;
	rect.height = w->box.content.height;
	LCUIMetrics_ComputeRectActual(&s->content_box, &rect);
}

static size_t WidgetRenderer_RenderChildren(LCUI_WidgetRenderer that)
{
	size_t total = 0, count = 0;
	LCUI_Widget child;
	LCUI_Rect paint_rect;
	LCUI_RectF child_rect;
	LinkedListNode *node;
	LCUI_PaintContextRec child_paint;
	LCUI_WidgetRenderer renderer;
	LCUI_WidgetActualStyleRec style;

	/* 按照显示顺序，从底到顶，递归遍历子级部件 */
	for (LinkedList_EachReverse(node, &that->target->children_show)) {
		child = node->data;
		if (!child->computed_style.visible ||
		    child->state != LCUI_WSTATE_NORMAL) {
			continue;
		}
		if (that->target->rules &&
		    that->target->rules->max_render_children_count &&
		    count > that->target->rules->max_render_children_count) {
			break;
		}
		/*
		 * The actual style calculation is time consuming, so here we
		 * use the existing properties to determine whether we need to
		 * render.
		 */
		style.x = that->x + that->content_left;
		style.y = that->y + that->content_top;
		child_rect.x = style.x + child->box.canvas.x;
		child_rect.y = style.y + child->box.canvas.y;
		child_rect.width = child->box.canvas.width;
		child_rect.height = child->box.canvas.height;
		if (!LCUIRectF_GetOverlayRect(&that->content_rect, &child_rect,
					      &child_rect)) {
			continue;
		}
		Widget_ComputeActualBorderBox(child, &style);
		Widget_ComputeActualCanvasBox(child, &style);
		DEBUG_MSG("content: %g, %g\n", that->content_left,
			  that->content_top);
		DEBUG_MSG("content rect: (%d, %d, %d, %d)\n",
			  that->actual_content_rect.x,
			  that->actual_content_rect.y,
			  that->actual_content_rect.width,
			  that->actual_content_rect.height);
		DEBUG_MSG("child canvas rect: (%d, %d, %d, %d)\n",
			  style.canvas_box.x, style.canvas_box.y,
			  style.canvas_box.width, style.canvas_box.height);
		if (!LCUIRect_GetOverlayRect(&that->actual_content_rect,
					     &style.canvas_box, &paint_rect)) {
			continue;
		}
		++count;
		Widget_ComputeActualPaddingBox(child, &style);
		Widget_ComputeActualContentBox(child, &style);
		child_paint.rect = paint_rect;
		child_paint.rect.x -= style.canvas_box.x;
		child_paint.rect.y -= style.canvas_box.y;
		if (that->has_content_graph) {
			child_paint.with_alpha = TRUE;
			paint_rect.x -= that->actual_content_rect.x;
			paint_rect.y -= that->actual_content_rect.y;
			Graph_Quote(&child_paint.canvas, &that->content_graph,
				    &paint_rect);
		} else {
			child_paint.with_alpha = that->paint->with_alpha;
			paint_rect.x -= that->actual_paint_rect.x;
			paint_rect.y -= that->actual_paint_rect.y;
			Graph_Quote(&child_paint.canvas, &that->paint->canvas,
				    &paint_rect);
		}
		DEBUG_MSG("child paint rect: (%d, %d, %d, %d)\n", paint_rect.x,
			  paint_rect.y, paint_rect.width, paint_rect.height);
		renderer = WidgetRenderer(child, &child_paint, &style, that);
		total += WidgetRenderer_Render(renderer);
		WidgetRenderer_Delete(renderer);
	}
	return total;
}

static size_t WidgetRenderer_Render(LCUI_WidgetRenderer renderer)
{
	size_t count = 0;
	LCUI_PaintContextRec self_paint;
	LCUI_WidgetRenderer that = renderer;

	int content_x = that->actual_content_rect.x - that->actual_paint_rect.x;
	int content_y = that->actual_content_rect.y - that->actual_paint_rect.y;

#ifdef DEBUG_FRAME_RENDER
	char filename[256];
	static size_t frame = 0;
#endif
	DEBUG_MSG("[%d] %s: start render\n", that->target->index,
		  that->target->type);
	/* 如果部件有需要绘制的内容 */
	if (that->can_render_self) {
		count += 1;
		self_paint = *that->paint;
		self_paint.with_alpha = TRUE;
		self_paint.canvas = that->self_graph;
		Widget_OnPaint(that->target, &self_paint, that->style);
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename,
			"frame-%lu-L%d-%s-self-paint-(%d,%d,%d,%d).png",
			frame++, __LINE__, renderer->target->id,
			self_paint.rect.x, self_paint.rect.y,
			self_paint.rect.width, self_paint.rect.height);
		LCUI_WritePNGFile(filename, &self_paint.canvas);
#endif
		/* 若不需要缓存自身位图则直接绘制到画布上 */
		if (!that->has_self_graph) {
			Graph_Mix(&that->paint->canvas, &that->self_graph, 0, 0,
				  that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
			Graph_PrintInfo(&that->paint->canvas);
			sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png",
				frame++, __LINE__, renderer->target->id);
			LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
		}
	}
	if (that->can_render_centent) {
		count += WidgetRenderer_RenderChildren(that);
	}
	if (that->has_content_graph && Widget_HasRoundBorder(that->target)) {
		self_paint.rect = that->actual_content_rect;
		self_paint.rect.x -= that->style->canvas_box.x;
		self_paint.rect.y -= that->style->canvas_box.y;
		self_paint.canvas = that->content_graph;
		Widget_CropContent(that->target, &self_paint, that->style);
	}
	if (!that->has_layer_graph) {
		if (that->has_content_graph) {
			Graph_Mix(&that->paint->canvas, &that->content_graph,
				  content_x, content_y, TRUE);
		}
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename, "frame-%lu-L%d-%s-canvas.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->paint->canvas);
		sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
		DEBUG_MSG("[%d] %s: end render, count: %lu\n",
			  that->target->index, that->target->type, count);
		return count;
	}
	/* 若需要绘制的是当前部件图层，则先混合部件自身位图和内容位图，得出当
	 * 前部件的图层，然后将该图层混合到输出的位图中
	 */
	if (that->can_render_self) {
		Graph_Copy(&that->layer_graph, &that->self_graph);
		Graph_Mix(&that->layer_graph, &that->content_graph, content_x,
			  content_y, TRUE);
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename, "frame-%lu-L%d-%s-content-grpah-%d-%d.png",
			frame++, __LINE__, renderer->target->id, content_x,
			content_y);
		LCUI_WritePNGFile(filename, &that->content_graph);
		sprintf(filename, "frame-%lu-L%d-%s-self-graph.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->layer_graph);
#endif
	} else {
		Graph_Create(&that->layer_graph, that->paint->rect.width,
			     that->paint->rect.height);
		Graph_Replace(&that->layer_graph, &that->content_graph,
			      content_x, content_y);
	}
	that->layer_graph.opacity = that->target->computed_style.opacity;
	Graph_Mix(&that->paint->canvas, &that->layer_graph, 0, 0,
		  that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
	sprintf(filename, "frame-%lu-%s-layer.png", frame++,
		renderer->target->id);
	LCUI_WritePNGFile(filename, &that->layer_graph);
	sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png", frame++, __LINE__,
		renderer->target->id);
	LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
	DEBUG_MSG("[%d] %s: end render, count: %lu\n", that->target->index,
		  that->target->type, count);
	return count;
}

size_t Widget_Render(LCUI_Widget w, LCUI_PaintContext paint)
{
	size_t count;
	LCUI_WidgetRenderer renderer;
	LCUI_WidgetActualStyleRec style;

	/* compute actual canvas box */
	style.x = style.y = 0;
	Widget_ComputeActualBorderBox(w, &style);
	Widget_ComputeActualCanvasBox(w, &style);
	/* reset widget position to relative paint rect */
	style.x = (float)-style.canvas_box.x;
	style.y = (float)-style.canvas_box.y;
	Widget_ComputeActualBorderBox(w, &style);
	Widget_ComputeActualCanvasBox(w, &style);
	Widget_ComputeActualPaddingBox(w, &style);
	Widget_ComputeActualContentBox(w, &style);
	renderer = WidgetRenderer(w, paint, &style, NULL);
	DEBUG_MSG("[%d] %s: start render\n", renderer->target->index,
		  renderer->target->type);
	count = WidgetRenderer_Render(renderer);
	DEBUG_MSG("[%d] %s: end render, count: %lu\n", renderer->target->index,
		  renderer->target->type, count);
	WidgetRenderer_Delete(renderer);
	return count;
}
