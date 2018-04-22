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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
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

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

/** surface 记录 */
typedef struct SurfaceRecordRec_ {
	LCUI_BOOL rendered;   /**< 是否已渲染了新内容 */
	LinkedList rects;     /**< 需重绘的区域列表 */
	LCUI_Surface surface; /**< surface */
	LCUI_Widget widget;   /**< surface 所映射的 widget */
} SurfaceRecordRec, *SurfaceRecord;

/** 图形显示功能的上下文数据 */
static struct DisplayContext {
	int mode;                   /**< 显示模式 */
	size_t width, height;       /**< 当前缓存的屏幕尺寸 */
	LCUI_BOOL show_rect_border; /**< 是否为重绘的区域显示边框 */
	LCUI_BOOL is_working; /**< 标志，指示当前模块是否处于工作状态 */
	LCUI_Thread thread;  /**< 线程，负责画面更新工作 */
	LinkedList surfaces; /**< surface 列表 */
	LinkedList rects;    /**< 无效区域列表 */
	LCUI_DisplayDriver driver;
} display;

#define LCUIDisplay_CleanSurfaces() \
	LinkedList_Clear(&display.surfaces, OnDestroySurfaceRecord)

static void OnDestroySurfaceRecord(void *data)
{
	SurfaceRecord record = data;
	Surface_Close(record->surface);
	LinkedList_Clear(&record->rects, free);
	free(record);
}

static void DrawBorder(LCUI_PaintContext paint)
{
	LCUI_Pos pos;
	LCUI_Color color;
	int end_x = paint->rect.width - 1;
	int end_y = paint->rect.height - 1;
	pos.x = pos.y = 0;
	color = RGB(255, 0, 0);
	Graph_DrawHorizLine(&paint->canvas, color, 1, pos, end_x);
	Graph_DrawVertiLine(&paint->canvas, color, 1, pos, end_y);
	pos.x = paint->rect.width - 1;
	Graph_DrawVertiLine(&paint->canvas, color, 1, pos, end_y);
	pos.x = 0;
	pos.y = paint->rect.height - 1;
	Graph_DrawHorizLine(&paint->canvas, color, 1, pos, end_x);
}

void LCUIDisplay_Update(void)
{
	LCUI_Surface surface;
	LinkedListNode *node;
	SurfaceRecord record = NULL;

	if (!display.is_working) {
		return;
	}
	LCUICursor_Update();
	LCUIWidget_Update();
	/* 遍历当前的 surface 记录列表 */
	for (LinkedList_Each(node, &display.surfaces)) {
		record = node->data;
		surface = record->surface;
		if (record->widget && surface && Surface_IsReady(surface)) {
			Surface_Update(surface);
		}
		/* 收集无效区域记录 */
		Widget_GetInvalidArea(record->widget, &record->rects);
	}
	if (display.mode == LCDM_SEAMLESS || !record) {
		return;
	}
	for (LinkedList_Each(node, &display.rects)) {
		RectList_Add(&record->rects, node->data);
	}
	RectList_Clear(&display.rects);
}

size_t LCUIDisplay_Render(void)
{
	size_t count = 0;
	LCUI_Rect *rect;
	LCUI_Surface s;
	LCUI_SysEventRec ev;
	LCUI_BOOL can_render;
	LCUI_PaintContext paint;
	LinkedListNode *sn, *rn;
	SurfaceRecord record;

	if (!display.is_working) {
		return 0;
	}
	ev.type = LCUI_PAINT;
	/* 遍历当前的 surface 记录列表 */
	for (LinkedList_Each(sn, &display.surfaces)) {
		record = sn->data;
		s = record->surface;
		can_render = record->widget && s && Surface_IsReady(s);
		record->rendered = FALSE;
		/* 在 surface 上逐个重绘无效区域 */
		for (LinkedList_Each(rn, &record->rects)) {
			rect = rn->data;
			ev.paint.rect = *rect;
			LCUI_TriggerEvent(&ev, NULL);
			if (!can_render) {
				continue;
			}
			paint = Surface_BeginPaint(s, rect);
			if (!paint) {
				continue;
			}
			DEBUG_MSG("rect: (%d,%d,%d,%d)\n", paint->rect.x,
				  paint->rect.y, paint->rect.width,
				  paint->rect.height);
			count += Widget_Render(record->widget, paint);
			if (display.show_rect_border) {
				DrawBorder(paint);
			}
			if (display.mode != LCDM_SEAMLESS) {
				LCUICursor_Paint(paint);
			}
			Surface_EndPaint(s, paint);
			record->rendered = TRUE;
		}
		RectList_Clear(&record->rects);
	}
	return count;
}

void LCUIDisplay_Present(void)
{
	LinkedListNode *sn;
	if (!display.is_working) {
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
	if (!display.is_working) {
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
	LinkedListNode *node;
	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
		if (record && record->surface == surface) {
			return record->widget;
		}
	}
	return NULL;
}

static LCUI_Surface LCUIDisplay_GetBindSurface(LCUI_Widget widget)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
		if (record && record->widget == widget) {
			return record->surface;
		}
	}
	return NULL;
}

LCUI_Surface LCUIDisplay_GetSurfaceOwner(LCUI_Widget w)
{
	if (LCUIDisplay_GetMode() == LCDM_SEAMLESS) {
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
	LinkedList_Init(&record->rects);
	Surface_SetCaptionW(record->surface, widget->title);
	LCUIMetrics_ComputeRectActual(&rect, &widget->box.canvas);
	if (widget->style->sheet[key_top].is_valid &&
	    widget->style->sheet[key_left].is_valid) {
		Surface_Move(record->surface, rect.x, rect.y);
	}
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
	LinkedListNode *node;
	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
		if (record && record->widget == widget) {
			Surface_Close(record->surface);
			LinkedList_DeleteNode(&display.surfaces, node);
			break;
		}
	}
}

static int LCUIDisplay_Windowed(void)
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	switch (display.mode) {
	case LCDM_WINDOWED:
		return 0;
	case LCDM_FULLSCREEN:
		LCUIDisplay_GetBindSurface(root);
		break;
	case LCDM_SEAMLESS:
	default:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface(root);
		break;
	}
	LCUIDisplay_SetSize(display.width, display.height);
	display.mode = LCDM_WINDOWED;
	return 0;
}

static int LCUIDisplay_FullScreen(void)
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	switch (display.mode) {
	case LCDM_SEAMLESS:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface(root);
	case LCDM_WINDOWED:
	default:
		break;
	case LCDM_FULLSCREEN:
		return 0;
	}
	display.mode = LCDM_FULLSCREEN;
	display.width = LCUIDisplay_GetWidth();
	display.height = LCUIDisplay_GetHeight();
	LCUIDisplay_SetSize(display.width, display.height);
	return 0;
}

static int LCUIDisplay_Seamless(void)
{
	LinkedListNode *node;
	LCUI_Widget root = LCUIWidget_GetRoot();
	DEBUG_MSG("display.mode: %d\n", display.mode);
	switch (display.mode) {
	case LCDM_SEAMLESS:
		return 0;
	case LCDM_FULLSCREEN:
	case LCDM_WINDOWED:
	default:
		LCUIDisplay_CleanSurfaces();
		break;
	}
	for (LinkedList_Each(node, &root->children)) {
		LCUIDisplay_BindSurface(node->data);
	}
	display.mode = LCDM_SEAMLESS;
	return 0;
}

/* 设置呈现模式 */
int LCUIDisplay_SetMode(int mode)
{
	int ret;
	DEBUG_MSG("mode: %d\n", mode);
	switch (mode) {
	case LCDM_WINDOWED:
		ret = LCUIDisplay_Windowed();
		break;
	case LCDM_SEAMLESS:
		ret = LCUIDisplay_Seamless();
		break;
	case LCDM_FULLSCREEN:
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
	if (display.mode == LCDM_SEAMLESS) {
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
	if (!display.is_working) {
		return 0;
	}
	if (display.mode == LCDM_WINDOWED || display.mode == LCDM_FULLSCREEN) {
		return iround(LCUIWidget_GetRoot()->width);
	}
	return display.driver->getWidth();
}

int LCUIDisplay_GetHeight(void)
{
	if (!display.is_working) {
		return 0;
	}
	if (display.mode == LCDM_WINDOWED || display.mode == LCDM_FULLSCREEN) {
		return iround(LCUIWidget_GetRoot()->height);
	}
	return display.driver->getHeight();
}

void Surface_Close(LCUI_Surface surface)
{
	if (display.is_working) {
		display.driver->close(surface);
	}
}

void Surface_Destroy(LCUI_Surface surface)
{
	if (!display.is_working) {
		return;
	}
	LinkedListNode *node;
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
	if (display.mode == LCDM_SEAMLESS) {
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
	LCUI_DisplayEvent dpy_ev = arg;
	for (LinkedList_Each(node, &display.surfaces)) {
		SurfaceRecord record = node->data;
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
	}
	LOG("[display] resize: (%d,%d)\n", dpy_ev->resize.width,
	    dpy_ev->resize.height);
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
	if (display.is_working) {
		return display.driver->bindEvent(event_id, func, data,
						 destroy_data);
	}
	return -1;
}

int LCUI_InitDisplay(LCUI_DisplayDriver driver)
{
	LCUI_Widget root;
	if (display.is_working) {
		return -1;
	}
	LOG("[display] init ...\n");
	display.mode = 0;
	display.driver = driver;
	display.is_working = TRUE;
	display.width = DEFAULT_WIDTH;
	display.height = DEFAULT_HEIGHT;
	LinkedList_Init(&display.rects);
	LinkedList_Init(&display.surfaces);
	if (!display.driver) {
		display.driver = LCUI_CreateDisplayDriver();
	}
	if (!display.driver) {
		LOG("[display] init failed\n");
		LCUIDisplay_SetMode(LCDM_DEFAULT);
		LCUIDisplay_Update();
		return -2;
	}
	root = LCUIWidget_GetRoot();
	display.driver->bindEvent(DET_RESIZE, OnResize, NULL, NULL);
	display.driver->bindEvent(DET_MINMAXINFO, OnMinMaxInfo, NULL, NULL);
	display.driver->bindEvent(DET_PAINT, OnPaint, NULL, NULL);
	Widget_BindEvent(root, "surface", OnSurfaceEvent, NULL, NULL);
	LCUIDisplay_SetMode(LCDM_DEFAULT);
	LCUIDisplay_Update();
	LOG("[display] init ok, driver name: %s\n", display.driver->name);
	return 0;
}

/** 停用图形输出模块 */
int LCUI_FreeDisplay(void)
{
	if (!display.is_working) {
		return -1;
	}
	display.is_working = FALSE;
	RectList_Clear(&display.rects);
	LCUIDisplay_CleanSurfaces();
	if (display.driver) {
		LCUI_DestroyDisplayDriver(display.driver);
	}
	return 0;
}
