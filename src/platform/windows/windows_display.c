/*
 * windows_display.c -- Surface support for windows platform.
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

#include <stdio.h>
#include <LCUI_Build.h>
#define LCUI_SURFACE_C
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/painter.h>
#include <LCUI/platform.h>
#include LCUI_DISPLAY_H
#include LCUI_EVENTS_H

/* clang-format off */

#define MIN_WIDTH		320
#define MIN_HEIGHT		240
#define WIN32_WINDOW_STYLE	(WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX)

enum SurfaceTaskType {
	TASK_MOVE,
	TASK_RESIZE,
	TASK_RESIZE_FB,
	TASK_SHOW,
	TASK_SET_CAPTION,
	TASK_TOTAL_NUM
};

typedef struct LCUI_SurfaceTask {
	LCUI_BOOL is_valid;
	union {
		struct {
			int x, y;
		};
		struct {
			int width, height;
		};
		LCUI_BOOL show;
		wchar_t *caption;
	};
} LCUI_SurfaceTask;

/** 适用于 windows 平台的 surface 数据结构 */
struct LCUI_SurfaceRec_ {
	HWND hwnd;				/**< windows 窗口句柄 */
	int mode;				/**< 渲染模式 */
	int width, height;			/**< 窗口宽高 */
	HDC hdc_fb;				/**< 帧缓存的设备上下文 */
	HDC hdc_client;				/**< 窗口的设备上下文 */
	HBITMAP fb_bmp;				/**< 帧缓存 */
	LCUI_BOOL is_ready;			/**< 是否已经准备好 */
	LCUI_Graph fb;				/**< 帧缓存，保存当前窗口内呈现的图像内容 */
	LCUI_SurfaceTask tasks[TASK_TOTAL_NUM]; /**< 任务缓存 */
	LinkedListNode node;                    /**< 在链表中的结点 */
};

/** windows 下图形显示功能所需的数据 */
static struct WIN_Display {
	LCUI_BOOL active;		/**< 是否已经初始化 */
	LinkedList surfaces;		/**< surface 记录 */
	LCUI_EventTrigger trigger;	/**< 事件触发器 */
} win;

/* clang-format on */

/** 根据 hwnd 获取 Surface */
static LCUI_Surface GetSurfaceByHWND(HWND hwnd)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &win.surfaces)) {
		if (((LCUI_Surface)node->data)->hwnd == hwnd) {
			return node->data;
		}
	}
	return NULL;
}

static void WinSurface_ClearTasks(LCUI_Surface surface)
{
	LCUI_SurfaceTask *t;
	t = &surface->tasks[TASK_SET_CAPTION];
	if (t->caption) {
		free(t->caption);
		t->caption = NULL;
	}
	t->is_valid = FALSE;
}

static void WinSurface_ExecDestroy(LCUI_Surface surface)
{
	surface->width = 0;
	surface->height = 0;
	if (surface->fb_bmp) {
		DeleteObject(surface->fb_bmp);
	}
	if (surface->hwnd) {
		if (surface->hdc_fb) {
			/* DeleteDC only for CreateDC */
			DeleteDC(surface->hdc_fb);
		}
		if (surface->hdc_client) {
			/* ReleaseDC only for GetDC */
			ReleaseDC(surface->hwnd, surface->hdc_client);
		}
	}
	DestroyWindow(surface->hwnd);
	surface->hwnd = NULL;
	surface->fb_bmp = NULL;
	surface->hdc_fb = NULL;
	surface->hdc_client = NULL;
	surface->is_ready = FALSE;
	Graph_Free(&surface->fb);
	WinSurface_ClearTasks(surface);
	free(surface);
}

static void WinSurface_Destroy(LCUI_Surface surface)
{
	LinkedList_Unlink(&win.surfaces, &surface->node);
	WinSurface_ExecDestroy(surface);
}

static void WinSurface_OnDestroy(void *data)
{
	WinSurface_ExecDestroy(data);
}

static void WinSurface_Close(LCUI_Surface surface)
{
	if (surface->hwnd) {
		PostMessage(surface->hwnd, WM_CLOSE, 0, 0);
	}
}

static void OnCreateSurface(void *arg1, void *arg2)
{
	HINSTANCE instance;
	LCUI_DisplayEventRec ev;
	LCUI_Surface surface = arg1;

	ev.type = LCUI_DEVENT_READY;
	ev.surface = surface;
	instance = LCUI_GetAppData();
	/* 为 Surface 创建窗口 */
	surface->hwnd = CreateWindow(
	    TEXT("LCUI"), TEXT("LCUI Surface"), WIN32_WINDOW_STYLE,
	    CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, instance, NULL);
#ifdef ENABLE_TOUCH_SUPPORT
	RegisterTouchWindow(surface->hwnd, 0);
#endif
	surface->hdc_client = GetDC(surface->hwnd);
	surface->hdc_fb = CreateCompatibleDC(surface->hdc_client);
	surface->is_ready = TRUE;
	DEBUG_MSG("surface: %p, surface->hwnd: %p\n", surface, surface->hwnd);
	LCUI_SetMainWindow(surface->hwnd);
	EventTrigger_Trigger(win.trigger, LCUI_DEVENT_READY, &ev);
}

/** 新建一个 Surface */
static LCUI_Surface WinSurface_New(void)
{
	int i;
	LCUI_Surface surface;
	surface = NEW(struct LCUI_SurfaceRec_, 1);
	surface->mode = RENDER_MODE_BIT_BLT;
	surface->hwnd = NULL;
	surface->hdc_fb = NULL;
	surface->hdc_client = NULL;
	surface->fb_bmp = NULL;
	surface->is_ready = FALSE;
	surface->node.data = surface;
	Graph_Init(&surface->fb);
	surface->fb.color_type = LCUI_COLOR_TYPE_ARGB;
	for (i = 0; i < TASK_TOTAL_NUM; ++i) {
		surface->tasks[i].is_valid = FALSE;
	}
	LinkedList_AppendNode(&win.surfaces, &surface->node);
	LCUI_PostSimpleTask(OnCreateSurface, surface, NULL);
	return surface;
}

static LCUI_BOOL WinSurface_IsReady(LCUI_Surface surface)
{
	return surface->is_ready;
}

static void WinSurface_ExecMove(LCUI_Surface surface, int x, int y)
{
	x += GetSystemMetrics(SM_CXFIXEDFRAME);
	y += GetSystemMetrics(SM_CYFIXEDFRAME);
	SetWindowPos(surface->hwnd, HWND_NOTOPMOST, x, y, 0, 0,
		     SWP_NOSIZE | SWP_NOZORDER);
	return;
}

static void WinSurface_Move(LCUI_Surface surface, int x, int y)
{
	/* 缓存任务，等获得窗口句柄后处理 */
	surface->tasks[TASK_MOVE].x = x;
	surface->tasks[TASK_MOVE].y = y;
	surface->tasks[TASK_MOVE].is_valid = TRUE;
}

static void WinSurface_ExecResizeFrameBuffer(LCUI_Surface surface, int w, int h)
{
	HBITMAP old_bmp;
	LCUI_Rect rect;

	if (surface->width == w && surface->height == h) {
		return;
	}
	Graph_Create(&surface->fb, w, h);
	surface->fb_bmp = CreateCompatibleBitmap(surface->hdc_client, w, h);
	old_bmp = (HBITMAP)SelectObject(surface->hdc_fb, surface->fb_bmp);
	if (old_bmp) {
		DeleteObject(old_bmp);
	}
	surface->width = w;
	surface->height = h;
	rect.x = rect.y = 0;
	rect.width = w;
	rect.height = 0;
	LCUIDisplay_InvalidateArea(&rect);
}

static void SurfaceSizeToWindowSize(LCUI_Surface surface, int *w, int *h)
{
	RECT rect_client, rect_window;
	GetClientRect(surface->hwnd, &rect_client);
	GetWindowRect(surface->hwnd, &rect_window);
	*w += rect_window.right - rect_window.left;
	*w -= rect_client.right - rect_client.left;
	*h += rect_window.bottom - rect_window.top;
	*h -= rect_client.bottom - rect_client.top;
}

static void WinSurface_ExecResize(LCUI_Surface surface, int w, int h)
{
	if (surface->width == w && surface->height == h) {
		return;
	}
	WinSurface_ExecResizeFrameBuffer(surface, w, h);
	SurfaceSizeToWindowSize(surface, &w, &h);
	SetWindowPos(surface->hwnd, HWND_NOTOPMOST, 0, 0, w, h,
		     SWP_NOMOVE | SWP_NOZORDER);
}

static void WinSurface_Resize(LCUI_Surface surface, int w, int h)
{
	surface->tasks[TASK_RESIZE].width = w;
	surface->tasks[TASK_RESIZE].height = h;
	surface->tasks[TASK_RESIZE].is_valid = TRUE;
}

static void WinSurface_ResizeFrameBuffer(LCUI_Surface surface, int w, int h)
{
	surface->tasks[TASK_RESIZE_FB].width = w;
	surface->tasks[TASK_RESIZE_FB].height = h;
	surface->tasks[TASK_RESIZE_FB].is_valid = TRUE;
}

static void WinSurface_Show(LCUI_Surface surface)
{
	DEBUG_MSG("surface: %p, buffer show.\n", surface);
	surface->tasks[TASK_SHOW].show = TRUE;
	surface->tasks[TASK_SHOW].is_valid = TRUE;
}

static void WinSurface_Hide(LCUI_Surface surface)
{
	surface->tasks[TASK_SHOW].show = FALSE;
	surface->tasks[TASK_SHOW].is_valid = TRUE;
}

static void WinSurface_SetCaptionW(LCUI_Surface surface, const wchar_t *str)
{
	size_t len;
	wchar_t *caption = NULL;

	if (str) {
		len = wcslen(str) + 1;
		caption = (wchar_t *)malloc(sizeof(wchar_t) * len);
		wcsncpy(caption, str, len);
	}
	if (surface->tasks[TASK_SET_CAPTION].is_valid &&
	    surface->tasks[TASK_SET_CAPTION].caption) {
		free(surface->tasks[TASK_SET_CAPTION].caption);
	}
	surface->tasks[TASK_SET_CAPTION].caption = caption;
	surface->tasks[TASK_SET_CAPTION].is_valid = TRUE;
}

static void WinSurface_SetOpacity(LCUI_Surface surface, float opacity)
{
}

static int WinSurface_GetWidth(LCUI_Surface surface)
{
	return surface->width;
}

static int WinSurface_GetHeight(LCUI_Surface surface)
{
	return surface->height;
}

/** 设置 Surface 的渲染模式 */
static void WinSurface_SetRenderMode(LCUI_Surface surface, int mode)
{
	surface->mode = mode;
}

/**
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
static LCUI_PaintContext WinSurface_BeginPaint(LCUI_Surface surface,
					       LCUI_Rect *rect)
{
	LCUI_PaintContext paint = LCUIPainter_Begin(&surface->fb, rect);
	Graph_FillRect(&paint->canvas, RGB(255, 255, 255), NULL, TRUE);
	return paint;
}

/**
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
static void WinSurface_EndPaint(LCUI_Surface surface, LCUI_PaintContext paint)
{
	LCUIPainter_End(paint);
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
static void WinSurface_Present(LCUI_Surface surface)
{
	RECT client_rect;

	DEBUG_MSG("surface: %p, hwnd: %p\n", surface, surface->hwnd);
	SetBitmapBits(surface->fb_bmp, (DWORD)surface->fb.mem_size,
		      surface->fb.bytes);
	switch (surface->mode) {
	case RENDER_MODE_STRETCH_BLT:
		GetClientRect(surface->hwnd, &client_rect);
		StretchBlt(surface->hdc_client, 0, 0, client_rect.right,
			   client_rect.bottom, surface->hdc_fb, 0, 0,
			   surface->width, surface->height, SRCCOPY);
		break;
	case RENDER_MODE_BIT_BLT:
	default:
		BitBlt(surface->hdc_client, 0, 0, surface->width,
		       surface->height, surface->hdc_fb, 0, 0, SRCCOPY);
		break;
	}
	ValidateRect(surface->hwnd, NULL);
}

/** 更新 surface，应用缓存的变更 */
static void WinSurface_Update(LCUI_Surface surface)
{
	LCUI_SurfaceTask *t;
	if (!surface->hwnd) {
		WinSurface_ClearTasks(surface);
		return;
	}
	DEBUG_MSG("surface: %p\n", surface);
	t = &surface->tasks[TASK_MOVE];
	if (t->is_valid) {
		WinSurface_ExecMove(surface, t->x, t->y);
		t->is_valid = FALSE;
	}
	t = &surface->tasks[TASK_RESIZE];
	if (t->is_valid) {
		WinSurface_ExecResize(surface, t->width, t->height);
		t->is_valid = FALSE;
	}
	t = &surface->tasks[TASK_RESIZE_FB];
	if (t->is_valid) {
		WinSurface_ExecResizeFrameBuffer(surface, t->width, t->height);
		t->is_valid = FALSE;
	}
	t = &surface->tasks[TASK_SET_CAPTION];
	if (t->is_valid) {
		SetWindowTextW(surface->hwnd, t->caption);
	}
	t->is_valid = FALSE;
	t = &surface->tasks[TASK_SHOW];
	DEBUG_MSG("surface: %p, hwnd: %p, is_valid: %d, show: %d\n", surface,
		  surface->hwnd, t->is_valid, t->show);
	if (t->is_valid) {
		if (t->show) {
			ShowWindow(surface->hwnd, SW_SHOWNORMAL);
		} else {
			ShowWindow(surface->hwnd, SW_HIDE);
		}
	}
	t->is_valid = FALSE;
	WinSurface_ClearTasks(surface);
}

static void OnWMPaint(LCUI_Event e, void *arg)
{
	MSG *msg = arg;
	PAINTSTRUCT ps;
	LCUI_Rect area;
	LCUI_Surface surface;
	LCUI_DisplayEventRec dpy_ev;
	BeginPaint(msg->hwnd, &ps);
	/* 获取区域坐标及尺寸 */
	area.x = ps.rcPaint.left;
	area.y = ps.rcPaint.top;
	area.width = ps.rcPaint.right - area.x;
	area.height = ps.rcPaint.bottom - area.y;
	EndPaint(msg->hwnd, &ps);
	surface = GetSurfaceByHWND(msg->hwnd);
	if (!surface) {
		return;
	}
	dpy_ev.type = LCUI_DEVENT_PAINT;
	dpy_ev.surface = surface;
	dpy_ev.paint.rect = area;
	EventTrigger_Trigger(win.trigger, LCUI_DEVENT_PAINT, &dpy_ev);
}

static void OnWMGetMinMaxInfo(LCUI_Event e, void *arg)
{
	int style;
	MSG *msg = arg;
	LCUI_DisplayEventRec dpy_ev;
	LCUI_MinMaxInfo info = &dpy_ev.minmaxinfo;
	LCUI_Surface surface = GetSurfaceByHWND(msg->hwnd);
	MINMAXINFO *mminfo = (PMINMAXINFO)msg->lParam;

	if (!surface) {
		return;
	}

	dpy_ev.surface = surface;
	dpy_ev.type = LCUI_DEVENT_MINMAXINFO;

	info->min_width = MIN_WIDTH;
	info->min_height = MIN_HEIGHT;
	info->max_width = GetSystemMetrics(SM_CXMAXTRACK);
	info->max_height = GetSystemMetrics(SM_CYMAXTRACK);

	EventTrigger_Trigger(win.trigger, LCUI_DEVENT_MINMAXINFO, &dpy_ev);
	SurfaceSizeToWindowSize(surface, &info->min_width, &info->min_height);
	SurfaceSizeToWindowSize(surface, &info->max_width, &info->max_height);

	mminfo->ptMinTrackSize.x = info->min_width;
	mminfo->ptMinTrackSize.y = info->min_height;
	mminfo->ptMaxTrackSize.x = info->max_width;
	mminfo->ptMaxTrackSize.y = info->max_height;

	style = GetWindowLong(msg->hwnd, GWL_STYLE);
	if (info->min_width == info->max_width &&
	    info->min_height == info->max_height) {
		style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
	} else if (info->min_width == info->max_width ||
		   info->min_height == info->max_height) {
		style &= ~WS_MAXIMIZEBOX;
	}
	SetWindowLong(msg->hwnd, GWL_STYLE, style);
}

static void OnWMSize(LCUI_Event e, void *arg)
{
	MSG *msg = arg;
	LCUI_Surface surface;
	LCUI_DisplayEventRec dpy_ev;

	surface = GetSurfaceByHWND(msg->hwnd);
	if (!surface) {
		return;
	}
	dpy_ev.surface = surface;
	dpy_ev.type = LCUI_DEVENT_RESIZE;
	dpy_ev.resize.width = LOWORD(msg->lParam);
	dpy_ev.resize.height = HIWORD(msg->lParam);
	WinSurface_ResizeFrameBuffer(surface, dpy_ev.resize.width,
				     dpy_ev.resize.height);
	EventTrigger_Trigger(win.trigger, LCUI_DEVENT_RESIZE, &dpy_ev);
}

static int WinDisplay_BindEvent(int event_id, LCUI_EventFunc func, void *data,
				void (*destroy_data)(void *))
{
	return EventTrigger_Bind(win.trigger, event_id, func, data,
				 destroy_data);
}

static void *WinSurface_GetHandle(LCUI_Surface s)
{
	return s->hwnd;
}

static int WinDisplay_GetWidth(void)
{
	return GetSystemMetrics(SM_CXSCREEN);
}

static int WinDisplay_GetHeight(void)
{
	return GetSystemMetrics(SM_CYSCREEN);
}

LCUI_DisplayDriver LCUI_CreateWinDisplay(void)
{
	ASSIGN(driver, LCUI_DisplayDriver);
	strcpy(driver->name, "windows");
	driver->getWidth = WinDisplay_GetWidth;
	driver->getHeight = WinDisplay_GetHeight;
	driver->getSurfaceWidth = WinSurface_GetWidth;
	driver->getSurfaceHeight = WinSurface_GetHeight;
	driver->create = WinSurface_New;
	driver->close = WinSurface_Close;
	driver->destroy = WinSurface_Destroy;
	driver->isReady = WinSurface_IsReady;
	driver->show = WinSurface_Show;
	driver->hide = WinSurface_Hide;
	driver->move = WinSurface_Move;
	driver->resize = WinSurface_Resize;
	driver->update = WinSurface_Update;
	driver->present = WinSurface_Present;
	driver->setCaptionW = WinSurface_SetCaptionW;
	driver->setRenderMode = WinSurface_SetRenderMode;
	driver->setOpacity = WinSurface_SetOpacity;
	driver->getHandle = WinSurface_GetHandle;
	driver->beginPaint = WinSurface_BeginPaint;
	driver->endPaint = WinSurface_EndPaint;
	driver->bindEvent = WinDisplay_BindEvent;
	LCUI_BindSysEvent(WM_SIZE, OnWMSize, NULL, NULL);
	LCUI_BindSysEvent(WM_PAINT, OnWMPaint, NULL, NULL);
	LCUI_BindSysEvent(WM_GETMINMAXINFO, OnWMGetMinMaxInfo, NULL, NULL);
	LinkedList_Init(&win.surfaces);
	win.trigger = EventTrigger();
	win.active = TRUE;
	return driver;
}

void LCUI_DestroyWinDisplay(LCUI_DisplayDriver driver)
{
	win.active = FALSE;
	LCUI_UnbindSysEvent(WM_SIZE, OnWMSize);
	LCUI_UnbindSysEvent(WM_PAINT, OnWMPaint);
	LCUI_UnbindSysEvent(WM_GETMINMAXINFO, OnWMGetMinMaxInfo);
	LinkedList_ClearData(&win.surfaces, WinSurface_OnDestroy);
	EventTrigger_Destroy(win.trigger);
	free(driver);
}

#endif
