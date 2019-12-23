/* display.c -- Graphical display control
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

#include "config.h"

#ifdef USE_OPENMP
#include <omp.h>
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#ifdef LCUI_DISPLAY_H
#include LCUI_DISPLAY_H
#endif

/* clang-format off */

#define DEFAULT_WIDTH	800
#define DEFAULT_HEIGHT	600

#ifdef USE_OPENMP
/**
 * Parallel rendering threads
 * We recommend that you set it to half the number of CPU logical cores
 */
#define PARALLEL_RENDERING_THREADS 4
#else
#define PARALLEL_RENDERING_THREADS 1
#endif

#define FLASH_DURATION	1000.0

typedef struct FlashRectRec_ {
	int64_t paint_time;
	LCUI_Rect rect;
} FlashRectRec, *FlashRect;

typedef struct SurfaceRecordRec_ {
	/** whether new content has been rendered */
	LCUI_BOOL rendered;

	/** dirty rectangles for rendering */
	LinkedList rects;

	/** flashing rect list */
	LinkedList flash_rects;

	LCUI_Surface surface;
	LCUI_Widget widget;
} SurfaceRecordRec, *SurfaceRecord;

static struct LCUI_DisplayModule {
	unsigned width, height;		/**< 当前缓存的屏幕尺寸 */
	LCUI_DisplayMode mode;		/**< 显示模式 */
	LCUI_BOOL show_rect_border;	/**< 是否为重绘的区域显示边框 */
	LCUI_BOOL active;		/**< 当前模块是否处于工作状态 */
	LinkedList surfaces;		/**< surface 列表 */
	LinkedList rects;		/**< 无效区域列表 */
	LCUI_DisplayDriver driver;
} display;

/* clang-format on */

#define LCUIDisplay_CleanSurfaces() \
	LinkedList_Clear(&display.surfaces, OnDestroySurfaceRecord)

INLINE int is_rect_equals(const LCUI_Rect *a, const LCUI_Rect *b)
{
	return a->x == b->x && a->y == b->y && a->width == b->width &&
	       a->height == b->height;
}

static void OnDestroySurfaceRecord(void *data)
{
	SurfaceRecord record = data;

	Surface_Close(record->surface);
	LinkedList_Clear(&record->rects, free);
	LinkedList_Clear(&record->flash_rects, free);
	free(record);
}

static void DrawBorder(LCUI_Graph *mask)
{
	LCUI_Pos pos;
	LCUI_Color color;
	int end_x = mask->width - 1;
	int end_y = mask->height - 1;
	pos.x = pos.y = 0;
	color = RGB(124, 179, 5);
	Graph_DrawHorizLine(mask, color, 1, pos, end_x);
	Graph_DrawVertiLine(mask, color, 1, pos, end_y);
	pos.x = mask->width - 1;
	Graph_DrawVertiLine(mask, color, 1, pos, end_y);
	pos.x = 0;
	pos.y = mask->height - 1;
	Graph_DrawHorizLine(mask, color, 1, pos, end_x);
}

static size_t LCUIDisplay_UpdateFlashRects(SurfaceRecord record)
{
	int64_t period;
	size_t count = 0;
	LCUI_Graph mask;
	LCUI_PaintContext paint;
	FlashRect flash_rect;
	LinkedListNode *node, *prev;

	for (LinkedList_Each(node, &record->flash_rects)) {
		flash_rect = node->data;
		if (flash_rect->paint_time == 0) {
			prev = node->prev;
			free(node->data);
			LinkedList_DeleteNode(&record->flash_rects, node);
			node = prev;
			continue;
		}
		period = LCUI_GetTimeDelta(flash_rect->paint_time);
		if (period >= FLASH_DURATION) {
			flash_rect->paint_time = 0;
		} else {
			Graph_Init(&mask);
			mask.color_type = LCUI_COLOR_TYPE_ARGB;
			Graph_Create(&mask, flash_rect->rect.width,
				     flash_rect->rect.height);
			Graph_FillRect(&mask, ARGB(125, 124, 179, 5), NULL, TRUE);
			mask.opacity =
			    0.6 * (FLASH_DURATION - (float)period) / FLASH_DURATION;
		}
		paint = Surface_BeginPaint(record->surface, &flash_rect->rect);
		if (!paint) {
			continue;
		}
		count += Widget_Render(record->widget, paint);
		if (flash_rect->paint_time != 0) {
			DrawBorder(&mask);
			Graph_Mix(&paint->canvas, &mask, 0, 0, TRUE);
			Graph_Free(&mask);
		}
		Surface_EndPaint(record->surface, paint);
		record->rendered = TRUE;
	}
	return count;
}

static void LCUIDisplay_AppendFlashRects(SurfaceRecord record, LCUI_Rect *rect)
{
	LinkedListNode *node;
	FlashRect flash_rect;

	for (LinkedList_Each(node, &record->flash_rects)) {
		flash_rect = node->data;
		if (is_rect_equals(&flash_rect->rect, rect)) {
			flash_rect->paint_time = LCUI_GetTime();
			return;
		}
	}

	flash_rect = NEW(FlashRectRec, 1);
	flash_rect->rect = *rect;
	flash_rect->paint_time = LCUI_GetTime();
	LinkedList_Append(&record->flash_rects, flash_rect);
}

static void SurfaceRecord_DumpRects(SurfaceRecord record, LinkedList *rects)
{
	typedef struct DirtyLayerRec {
		LinkedList rects;
		LCUI_Rect rect;
		int diry;
	} DirtyLayerRec, *DirtyLayer;

	int i;
	int max_dirty;
	float scale = LCUIMetrics_GetScale();
	int layer_width = LCUIDisplay_GetWidth() * scale;
	int layer_height = LCUIDisplay_GetHeight() * scale;

	LCUI_Rect rect;
	LCUI_Rect *sub_rect;
	DirtyLayer layer;
	DirtyLayerRec layers[PARALLEL_RENDERING_THREADS];
	LinkedListNode *node;

	layer_height = max(200, layer_height / PARALLEL_RENDERING_THREADS + 1);
	max_dirty = 0.8 * layer_width * layer_height;
	for (i = 0; i < PARALLEL_RENDERING_THREADS; ++i) {
		layer = &layers[i];
		layer->diry = 0;
		layer->rect.y = i * layer_height;
		layer->rect.x = 0;
		layer->rect.width = layer_width;
		layer->rect.height = layer_height;
		LinkedList_Init(&layer->rects);
	}
	sub_rect = malloc(sizeof(LCUI_Rect));
	for (LinkedList_Each(node, &record->rects)) {
		rect = *(LCUI_Rect *)node->data;
		for (i = 0; i < PARALLEL_RENDERING_THREADS; ++i) {
			layer = &layers[i];
			if (layer->diry >= max_dirty) {
				continue;
			}
			if (!LCUIRect_GetOverlayRect(&layer->rect, &rect,
						     sub_rect)) {
				continue;
			}
			LinkedList_Append(&layer->rects, sub_rect);
			rect.y += sub_rect->height;
			rect.height -= sub_rect->height;
			layer->diry += sub_rect->width * sub_rect->height;
			sub_rect = malloc(sizeof(LCUI_Rect));
			if (rect.height < 1) {
				break;
			}
		}
	}
	for (i = 0; i < PARALLEL_RENDERING_THREADS; ++i) {
		layer = &layers[i];
		if (layer->diry >= max_dirty) {
			RectList_AddEx(rects, &layer->rect, FALSE);
			RectList_Clear(&layer->rects);
		} else {
			LinkedList_Concat(rects, &layer->rects);
		}
	}
	RectList_Clear(&record->rects);
	free(sub_rect);
}

static size_t LCUIDisplay_RenderSurface(SurfaceRecord record)
{
	int i = 0;
	size_t count = 0;
	LCUI_BOOL can_render;
	LCUI_Rect **rect_array;
	LCUI_SysEventRec ev;
	LinkedList rects;
	LinkedListNode *node;

	ev.type = LCUI_PAINT;
	can_render = record->widget && record->surface &&
		     Surface_IsReady(record->surface);
	record->rendered = FALSE;
	LinkedList_Init(&rects);
	SurfaceRecord_DumpRects(record, &rects);
	if (rects.length < 1) {
		return 0;
	}

	rect_array = (LCUI_Rect **)malloc(sizeof(LCUI_Rect *) * rects.length);
	for (LinkedList_Each(node, &rects)) {
		rect_array[i] = node->data;
		i++;
	}
#ifdef USE_OPENMP
#pragma omp parallel for \
	default(none) \
	shared(can_render, display, rects, rect_array) \
	firstprivate(record, ev) \
	reduction(+:count)
#endif
	for (i = 0; i < rects.length; ++i) {
		LCUI_Rect *rect;
		LCUI_PaintContext paint;

		rect = rect_array[i];
		ev.paint.rect = *rect;
		LCUI_TriggerEvent(&ev, NULL);
		if (!can_render) {
			continue;
		}
		paint = Surface_BeginPaint(record->surface, rect);
		if (!paint) {
			continue;
		}
		DEBUG_MSG("rect: (%d,%d,%d,%d)\n", paint->rect.x, paint->rect.y,
			  paint->rect.width, paint->rect.height);
		count += Widget_Render(record->widget, paint);
		if (display.show_rect_border) {
			LCUIDisplay_AppendFlashRects(record, &paint->rect);
		}
		if (display.mode != LCUI_DMODE_SEAMLESS) {
			LCUICursor_Paint(paint);
		}
		Surface_EndPaint(record->surface, paint);
	}
	RectList_Clear(&rects);
	record->rendered = count > 0;
	count += LCUIDisplay_UpdateFlashRects(record);
	return count;
}

void LCUIDisplay_Update(void)
{
	LCUI_Surface surface;
	LinkedListNode *node;
	SurfaceRecord record = NULL;

	if (!display.active) {
		return;
	}
	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		surface = record->surface;
		if (record->widget && surface && Surface_IsReady(surface)) {
			Surface_Update(surface);
		}
		Widget_GetInvalidArea(record->widget, &record->rects);
	}
	if (display.mode == LCUI_DMODE_SEAMLESS || !record) {
		return;
	}
	LinkedList_Concat(&record->rects, &display.rects);
}

size_t LCUIDisplay_Render(void)
{
	size_t count = 0;
	LinkedListNode *node;

	if (!display.active) {
		return 0;
	}
	for (LinkedList_Each(node, &display.surfaces)) {
		count += LCUIDisplay_RenderSurface(node->data);
		count += LCUIDisplay_UpdateFlashRects(node->data);
	}
	return count;
}

void LCUIDisplay_Present(void)
{
	LinkedListNode *sn;

	if (!display.active) {
		return;
	}
	for (LinkedList_Each(sn, &display.surfaces)) {
		SurfaceRecord record = sn->data;
		LCUI_Surface surface = record->surface;
		if (!surface || !Surface_IsReady(surface)) {
			continue;
		}
		if (record->rendered) {
			Surface_Present(surface);
		}
	}
}

void LCUIDisplay_InvalidateArea(LCUI_Rect *rect)
{
	LCUI_Rect area;

	if (!display.active) {
		return;
	}
	if (!rect) {
		area.x = 0;
		area.y = 0;
		area.width = LCUIDisplay_GetWidth();
		area.height = LCUIDisplay_GetHeight();
		rect = &area;
	}
	RectToInvalidArea(rect, &area);
	RectList_Add(&display.rects, &area);
}

static LCUI_Widget LCUIDisplay_GetBindWidget(LCUI_Surface surface)
{
	SurfaceRecord record;
	LinkedListNode *node;

	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		if (record && record->surface == surface) {
			return record->widget;
		}
	}
	return NULL;
}

static LCUI_Surface LCUIDisplay_GetBindSurface(LCUI_Widget widget)
{
	SurfaceRecord record;
	LinkedListNode *node;

	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		if (record && record->widget == widget) {
			return record->surface;
		}
	}
	return NULL;
}

LCUI_Surface LCUIDisplay_GetSurfaceOwner(LCUI_Widget w)
{
	if (LCUIDisplay_GetMode() == LCUI_DMODE_SEAMLESS) {
		while (w->parent) {
			w = w->parent;
		}
	} else {
		w = LCUIWidget_GetRoot();
	}
	return LCUIDisplay_GetBindSurface(w);
}

LCUI_Surface LCUIDisplay_GetSurfaceByHandle(void *handle)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
		if (Surface_GetHandle(record->surface) == handle) {
			return record->surface;
		}
	}
	return NULL;
}

/** 将 widget 与 sruface 进行绑定 */
static void LCUIDisplay_BindSurface(LCUI_Widget widget)
{
	LCUI_Rect rect;
	SurfaceRecord record;

	if (LCUIDisplay_GetBindSurface(widget)) {
		return;
	}
	record = NEW(SurfaceRecordRec, 1);
	record->surface = Surface_New();
	record->widget = widget;
	record->rendered = FALSE;
	LinkedList_Init(&record->flash_rects);
	LCUIMetrics_ComputeRectActual(&rect, &widget->box.canvas);
	if (Widget_CheckStyleValid(widget, key_top) &&
	    Widget_CheckStyleValid(widget, key_left)) {
		Surface_Move(record->surface, rect.x, rect.y);
	}
	Surface_SetCaptionW(record->surface, widget->title);
	Surface_Resize(record->surface, rect.width, rect.height);
	if (widget->computed_style.visible) {
		Surface_Show(record->surface);
	} else {
		Surface_Hide(record->surface);
	}
	Widget_InvalidateArea(widget, NULL, SV_GRAPH_BOX);
	LinkedList_Append(&display.surfaces, record);
}

/** 解除 widget 与 sruface 的绑定 */
static void LCUIDisplay_UnbindSurface(LCUI_Widget widget)
{
	SurfaceRecord record;
	LinkedListNode *node;

	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		if (record && record->widget == widget) {
			Surface_Close(record->surface);
			LinkedList_DeleteNode(&display.surfaces, node);
			break;
		}
	}
}

static int LCUIDisplay_Windowed(void)
{
	LCUI_Widget root;

	root = LCUIWidget_GetRoot();
	switch (display.mode) {
	case LCUI_DMODE_WINDOWED:
		return 0;
	case LCUI_DMODE_FULLSCREEN:
		LCUIDisplay_GetBindSurface(root);
		break;
	case LCUI_DMODE_SEAMLESS:
	default:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface(root);
		break;
	}
	LCUIDisplay_SetSize(display.width, display.height);
	display.mode = LCUI_DMODE_WINDOWED;
	return 0;
}

static int LCUIDisplay_FullScreen(void)
{
	LCUI_Widget root;

	root = LCUIWidget_GetRoot();
	switch (display.mode) {
	case LCUI_DMODE_SEAMLESS:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface(root);
	case LCUI_DMODE_WINDOWED:
	default:
		break;
	case LCUI_DMODE_FULLSCREEN:
		return 0;
	}
	display.mode = LCUI_DMODE_FULLSCREEN;
	display.width = LCUIDisplay_GetWidth();
	display.height = LCUIDisplay_GetHeight();
	LCUIDisplay_SetSize(display.width, display.height);
	return 0;
}

/* FIXME: improve the seamless display mode
 * the seamless display mode has not been updated for a long time, we not
 * sure if it can work.
 */

static int LCUIDisplay_Seamless(void)
{
	LinkedListNode *node;
	LCUI_Widget root = LCUIWidget_GetRoot();

	switch (display.mode) {
	case LCUI_DMODE_SEAMLESS:
		return 0;
	case LCUI_DMODE_FULLSCREEN:
	case LCUI_DMODE_WINDOWED:
	default:
		LCUIDisplay_CleanSurfaces();
		break;
	}
	for (LinkedList_Each(node, &root->children)) {
		LCUIDisplay_BindSurface(node->data);
	}
	display.mode = LCUI_DMODE_SEAMLESS;
	return 0;
}

/* 设置呈现模式 */
int LCUIDisplay_SetMode(int mode)
{
	int ret;

	switch (mode) {
	case LCUI_DMODE_WINDOWED:
		ret = LCUIDisplay_Windowed();
		break;
	case LCUI_DMODE_SEAMLESS:
		ret = LCUIDisplay_Seamless();
		break;
	case LCUI_DMODE_FULLSCREEN:
	default:
		ret = LCUIDisplay_FullScreen();
		break;
	}
	return ret;
}

/* 获取呈现模式 */
int LCUIDisplay_GetMode(void)
{
	return display.mode;
}

void LCUIDisplay_ShowRectBorder(void)
{
	display.show_rect_border = TRUE;
}

void LCUIDisplay_HideRectBorder(void)
{
	display.show_rect_border = FALSE;
}

/** 设置显示区域的尺寸，仅在窗口化、全屏模式下有效 */
void LCUIDisplay_SetSize(int width, int height)
{
	float scale;
	LCUI_Widget root;
	LCUI_Surface surface;

	if (display.mode == LCUI_DMODE_SEAMLESS) {
		return;
	}
	root = LCUIWidget_GetRoot();
	scale = LCUIMetrics_GetScale();
	surface = LCUIDisplay_GetBindSurface(root);
	Surface_Resize(surface, width, height);
	Widget_Resize(root, width / scale, height / scale);
}

int LCUIDisplay_GetWidth(void)
{
	if (!display.active) {
		return 0;
	}
	if (display.mode == LCUI_DMODE_WINDOWED ||
	    display.mode == LCUI_DMODE_FULLSCREEN) {
		return iround(LCUIWidget_GetRoot()->width);
	}
	return display.driver->getWidth();
}

int LCUIDisplay_GetHeight(void)
{
	if (!display.active) {
		return 0;
	}
	if (display.mode == LCUI_DMODE_WINDOWED ||
	    display.mode == LCUI_DMODE_FULLSCREEN) {
		return iround(LCUIWidget_GetRoot()->height);
	}
	return display.driver->getHeight();
}

void Surface_Close(LCUI_Surface surface)
{
	if (display.active) {
		display.driver->close(surface);
	}
}

void Surface_Destroy(LCUI_Surface surface)
{
	LinkedListNode *node;

	if (!display.active) {
		return;
	}
	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
		if (record && record->surface == surface) {
			LinkedList_DeleteNode(&display.surfaces, node);
			display.driver->destroy(surface);
			free(record);
			break;
		}
	}
}

LCUI_Surface Surface_New(void)
{
	if (display.driver) {
		return display.driver->create();
	}
	return NULL;
}

LCUI_BOOL Surface_IsReady(LCUI_Surface surface)
{
	if (display.driver) {
		return display.driver->isReady(surface);
	}
	return TRUE;
}

void Surface_Move(LCUI_Surface surface, int x, int y)
{
	if (display.driver) {
		display.driver->move(surface, x, y);
	}
}

int Surface_GetWidth(LCUI_Surface surface)
{
	if (display.driver) {
		return display.driver->getSurfaceWidth(surface);
	}
	return 0;
}

int Surface_GetHeight(LCUI_Surface surface)
{
	if (display.driver) {
		return display.driver->getSurfaceHeight(surface);
	}
	return 0;
}

void Surface_Resize(LCUI_Surface surface, int w, int h)
{
	if (display.driver) {
		LCUI_Rect rect;
		display.driver->resize(surface, w, h);
		rect.x = rect.y = 0;
		rect.width = w;
		rect.height = h;
		LCUIDisplay_InvalidateArea(&rect);
	}
}

void Surface_SetCaptionW(LCUI_Surface surface, const wchar_t *str)
{
	if (display.driver) {
		display.driver->setCaptionW(surface, str);
	}
}

void Surface_Show(LCUI_Surface surface)
{
	if (display.driver) {
		display.driver->show(surface);
	}
}

void Surface_Hide(LCUI_Surface surface)
{
	if (display.driver) {
		display.driver->hide(surface);
	}
}

void *Surface_GetHandle(LCUI_Surface surface)
{
	if (display.driver) {
		return display.driver->getHandle(surface);
	}
	return NULL;
}

void Surface_SetRenderMode(LCUI_Surface surface, int mode)
{
	if (display.driver) {
		display.driver->setRenderMode(surface, mode);
	}
}

void Surface_Update(LCUI_Surface surface)
{
	if (display.driver) {
		display.driver->update(surface);
	}
}

LCUI_PaintContext Surface_BeginPaint(LCUI_Surface surface, LCUI_Rect *rect)
{
	if (display.driver) {
		return display.driver->beginPaint(surface, rect);
	}
	return NULL;
}

void Surface_EndPaint(LCUI_Surface surface, LCUI_PaintContext paint)
{
	if (display.driver) {
		display.driver->endPaint(surface, paint);
	}
}

void Surface_Present(LCUI_Surface surface)
{
	if (display.driver) {
		display.driver->present(surface);
	}
}

/** 响应顶级部件的各种事件 */
static void OnSurfaceEvent(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	LCUI_Widget root;
	LCUI_RectF *rect;
	LCUI_Surface surface;
	int *data, event_type, sync_props;

	data = (int *)arg;
	event_type = data[0];
	sync_props = data[1];
	root = LCUIWidget_GetRoot();
	surface = LCUIDisplay_GetBindSurface(e->target);
	if (display.mode == LCUI_DMODE_SEAMLESS) {
		if (!surface && event_type != LCUI_WEVENT_LINK) {
			return;
		}
	} else if (e->target == root) {
		if (!surface && event_type != LCUI_WEVENT_LINK) {
			return;
		}
	} else {
		return;
	}
	rect = &e->target->box.canvas;
	switch (event_type) {
	case LCUI_WEVENT_LINK:
		LCUIDisplay_BindSurface(e->target);
		break;
	case LCUI_WEVENT_UNLINK:
	case LCUI_WEVENT_DESTROY:
		LCUIDisplay_UnbindSurface(e->target);
		break;
	case LCUI_WEVENT_SHOW:
		Surface_Show(surface);
		break;
	case LCUI_WEVENT_HIDE:
		Surface_Hide(surface);
		break;
	case LCUI_WEVENT_RESIZE: {
		LCUI_Rect area;
		RectFToInvalidArea(rect, &area);
		if (sync_props) {
			Surface_Resize(surface, area.width, area.height);
		}
		LCUIDisplay_InvalidateArea(&area);
		break;
	}
	case LCUI_WEVENT_TITLE:
		Surface_SetCaptionW(surface, e->target->title);
		break;
	default:
		break;
	}
}

/** 在 surface 主动产生无效区域并需要绘制的时候 */
static void OnPaint(LCUI_Event e, void *arg)
{
	LCUI_RectF rect;
	LinkedListNode *node;
	SurfaceRecord record;
	LCUI_DisplayEvent dpy_ev = arg;

	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		if (record && record->surface != dpy_ev->surface) {
			continue;
		}
		LCUIRect_ToRectF(&dpy_ev->paint.rect, &rect, 1.0f);
		Widget_InvalidateArea(record->widget, &rect, SV_GRAPH_BOX);
	}
}

static void OnResize(LCUI_Event e, void *arg)
{
	LCUI_Widget widget;
	LCUI_DisplayEvent dpy_ev = arg;
	float scale = LCUIMetrics_GetScale();
	float width = dpy_ev->resize.width / scale;
	float height = dpy_ev->resize.height / scale;

	widget = LCUIDisplay_GetBindWidget(dpy_ev->surface);
	if (widget) {
		Widget_Resize(widget, width, height);
		widget->task.skip_surface_props_sync = TRUE;
	}
	LCUI_RunFrame();
}

static void OnMinMaxInfo(LCUI_Event e, void *arg)
{
	LCUI_BOOL resizable = FALSE;
	LCUI_DisplayEvent dpy_ev = arg;
	LCUI_Surface s = dpy_ev->surface;
	LCUI_Widget widget = LCUIDisplay_GetBindWidget(s);
	LCUI_WidgetStyle *style = &widget->computed_style;
	int width = Surface_GetWidth(s);
	int height = Surface_GetHeight(s);

	if (style->min_width >= 0) {
		dpy_ev->minmaxinfo.min_width = iround(style->min_width);
		resizable = resizable || width < style->min_width;
	}
	if (style->max_width >= 0) {
		dpy_ev->minmaxinfo.max_width = iround(style->max_width);
		resizable = resizable || width > style->max_width;
	}
	if (style->min_height >= 0) {
		dpy_ev->minmaxinfo.min_height = iround(style->min_height);
		resizable = resizable || height < style->min_height;
	}
	if (style->max_height >= 0) {
		dpy_ev->minmaxinfo.max_height = iround(style->max_height);
		resizable = resizable || height > style->max_height;
	}
	if (resizable) {
		LCUI_Rect area;
		RectFToInvalidArea(&widget->box.canvas, &area);
		Surface_Resize(s, area.width, area.height);
	}
}

int LCUIDisplay_BindEvent(int event_id, LCUI_EventFunc func, void *arg,
			  void *data, void (*destroy_data)(void *))
{
	if (display.active) {
		return display.driver->bindEvent(event_id, func, data,
						 destroy_data);
	}
	return -1;
}

int LCUI_InitDisplay(LCUI_DisplayDriver driver)
{
	LCUI_Widget root;
	if (display.active) {
		return -1;
	}
	Logger_Info("[display] init ...\n");
	display.mode = 0;
	display.driver = driver;
	display.active = TRUE;
	display.width = DEFAULT_WIDTH;
	display.height = DEFAULT_HEIGHT;
	LinkedList_Init(&display.rects);
	LinkedList_Init(&display.surfaces);
	if (!display.driver) {
		display.driver = LCUI_CreateDisplayDriver();
	}
	if (!display.driver) {
		Logger_Warning("[display] init failed\n");
		LCUIDisplay_SetMode(LCUI_DMODE_DEFAULT);
		LCUIDisplay_Update();
		return -2;
	}
	root = LCUIWidget_GetRoot();
	display.driver->bindEvent(LCUI_DEVENT_RESIZE, OnResize, NULL, NULL);
	display.driver->bindEvent(LCUI_DEVENT_MINMAXINFO, OnMinMaxInfo, NULL,
				  NULL);
	display.driver->bindEvent(LCUI_DEVENT_PAINT, OnPaint, NULL, NULL);
	Widget_BindEvent(root, "surface", OnSurfaceEvent, NULL, NULL);
	LCUIDisplay_SetMode(LCUI_DMODE_DEFAULT);
	LCUIDisplay_Update();
	Logger_Info("[display] init ok, driver name: %s\n",
		    display.driver->name);
	return 0;
}

/** 停用图形输出模块 */
int LCUI_FreeDisplay(void)
{
	if (!display.active) {
		return -1;
	}
	display.active = FALSE;
	RectList_Clear(&display.rects);
	LCUIDisplay_CleanSurfaces();
	if (display.driver) {
		LCUI_DestroyDisplayDriver(display.driver);
	}
	return 0;
}
