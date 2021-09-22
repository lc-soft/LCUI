/*
 * linux_framebuffer.c -- surface support for linux framebuffer.
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

#define LCUI_SURFACE_C
#include "config.h"
#include <LCUI_Build.h>

#if defined(LCUI_BUILD_IN_LINUX) && defined(LCUI_VIDEO_DRIVER_FRAMEBUFFER)
#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include <LCUI/painter.h>
#include LCUI_DISPLAY_H
#include LCUI_EVENTS_H

#define MIN_WIDTH 320
#define MIN_HEIGHT 240

enum SurfaceTaskType { TASK_RESIZE, TASK_DELETE, TASK_TOTAL_NUM };

typedef struct LCUI_SurfaceTaskRec_ {
	LCUI_BOOL is_valid;
	struct {
		int width, height;
	};
} LCUI_SurfaceTaskRec, *LCUI_SurfaceTask;

typedef LCUI_SurfaceTaskRec LCUI_SurfaceTasks[TASK_TOTAL_NUM];

typedef struct LCUI_SurfaceRec_ {
	int x;
	int y;
	unsigned width;
	unsigned height;
	LCUI_Rect rect;
	LCUI_Rect actual_rect;
	LCUI_Mutex mutex;
	LCUI_Graph canvas;
	LinkedList rects;
	LCUI_SurfaceTasks tasks;
} LCUI_SurfaceRec;

static struct LCUI_DisplayModule {
	struct LCUI_FrameBuffer {
		int dev_fd;
		const char *dev_path;

		unsigned char *mem;
		size_t mem_len;

		struct fb_var_screeninfo var_info;
		struct fb_fix_screeninfo fix_info;
		struct fb_cmap cmap;
	} fb;

	unsigned width;
	unsigned height;

	LCUI_SurfaceRec surface;
	unsigned surface_count;

	LCUI_BOOL active;
	LCUI_Graph canvas;
	LCUI_EventTrigger trigger;
} display;

static void FBSurface_OnResize(LCUI_Surface s, int width, int height)
{
	s->width = width;
	s->height = height;
	s->x = (display.width - width) / 2;
	s->y = (display.height - height) / 2;
	s->rect.x = s->x;
	s->rect.y = s->y;
	s->rect.width = s->width;
	s->rect.height = s->height;
	s->actual_rect = s->rect;
	LCUIRect_ValidateArea(&s->actual_rect, display.width, display.height);
	Graph_Create(&s->canvas, width, height);
}

static void FBSurface_RunTask(LCUI_Surface surface, int type)
{
	LCUI_SurfaceTask task = &surface->tasks[type];
	switch (type) {
	case TASK_RESIZE:
		LCUIMutex_Lock(&surface->mutex);
		task->width = max(task->width, MIN_WIDTH);
		task->height = max(task->height, MIN_HEIGHT);
		FBSurface_OnResize(surface, task->width, task->height);
		LCUIMutex_Unlock(&surface->mutex);
		break;
	case TASK_DELETE:
		display.surface_count = 0;
	default:
		break;
	}
}

static void FBSurface_Delete(LCUI_Surface surface)
{
	surface->tasks[TASK_DELETE].is_valid = TRUE;
}

static void FBSurface_Show(LCUI_Surface surface)
{
}

static void FBSurface_Hide(LCUI_Surface surface)
{
}

static LCUI_Surface FBSurface_New(void)
{
	if (display.surface_count > 0) {
		return NULL;
	}
	display.surface_count += 1;
	return &display.surface;
}

static LCUI_BOOL FBSurface_IsReady(LCUI_Surface surface)
{
	return TRUE;
}

static void FBSurface_Move(LCUI_Surface surface, int x, int y)
{
}

static void FBSurface_Resize(LCUI_Surface surface, int width, int height)
{
	surface->tasks[TASK_RESIZE].width = width;
	surface->tasks[TASK_RESIZE].height = height;
	surface->tasks[TASK_RESIZE].is_valid = TRUE;
}

static void FBSurface_SetCaptionW(LCUI_Surface surface, const wchar_t *wstr)
{
}

void FBSurface_SetOpacity(LCUI_Surface surface, float opacity)
{
}

static void FBSurface_SetRenderMode(LCUI_Surface surface, int mode)
{
}

static LCUI_PaintContext FBSurface_BeginPaint(LCUI_Surface surface,
					      LCUI_Rect *rect)
{
	LCUI_PaintContext paint;
	LCUI_Rect actual_rect = *rect;
	LCUIRect_ValidateArea(&actual_rect, surface->width, surface->height);
	actual_rect.x += surface->rect.x;
	actual_rect.y += surface->rect.y;
	LCUIRect_GetOverlayRect(&actual_rect, &surface->actual_rect,
				&actual_rect);
	actual_rect.x -= surface->rect.x;
	actual_rect.y -= surface->rect.y;
	paint = LCUIPainter_Begin(&surface->canvas, &actual_rect);
	Graph_FillRect(&paint->canvas, RGB(255, 255, 255), NULL, TRUE);
	RectList_Add(&surface->rects, rect);
	return paint;
}

static void FBSurface_EndPaint(LCUI_Surface surface, LCUI_PaintContext paint)
{
	LCUIPainter_End(paint);
}

static void FBDisplay_SyncRect16(LCUI_Graph *canvas, int x, int y)
{
	uint32_t iy, ix;
	LCUI_Rect rect;
	LCUI_ARGB *pixel, *pixel_row;
	unsigned char *dst, *dst_row;

	Graph_GetValidRect(canvas, &rect);
	pixel_row = canvas->argb + rect.y * canvas->width + rect.x;
	dst_row = display.fb.mem + y * display.canvas.bytes_per_row + x * 2;
	for (iy = 0; iy < rect.width; ++iy) {
		dst = dst_row;
		pixel = pixel_row;
		for (ix = 0; ix < rect.width; ++ix) {
			dst[0] = (pixel->r & 0xF8) | (pixel->b >> 5);
			dst[1] = ((pixel->g & 0x1C) << 3) | (pixel->b >> 3);
		}
		pixel_row += canvas->width;
		dst_row += display.canvas.bytes_per_row;
	}
}

static void FBDisplay_SyncRect8(LCUI_Graph *canvas, int x, int y)
{
	uint32_t ix, iy;
	LCUI_Rect rect;
	LCUI_ARGB *pixel, *pixel_row;
	unsigned char *dst, *dst_row;
	unsigned int r, g, b, i;
	struct fb_cmap cmap;
	__u16 cmap_buf[256 * 3] = { 0 };

	cmap.start = 0;
	cmap.len = 255;
	cmap.transp = 0;
	cmap.red = cmap_buf;
	cmap.green = cmap_buf + 256;
	cmap.blue = cmap_buf + 512;

	Graph_GetValidRect(canvas, &rect);
	pixel_row = canvas->argb + rect.y * canvas->width + rect.x;
	dst_row = display.fb.mem + y * display.canvas.bytes_per_row + x;
	for (iy = 0; iy < rect.height; ++iy) {
		dst = dst_row;
		pixel = pixel_row;
		for (ix = 0; ix < rect.width; ++ix, ++dst) {
			r = pixel->r * 0.92;
			g = pixel->g * 0.92;
			b = pixel->b * 0.92;

			i = ((r & 0xc0)) + ((g & 0xf0) >> 2) +
			    ((b & 0xc0) >> 6);

			cmap.red[i] = r * 256;
			cmap.green[i] = g * 256;
			cmap.blue[i] = b * 256;
			*dst = (((r & 0xc0)) + ((g & 0xf0) >> 2) +
				((b & 0xc0) >> 6));
		}
		pixel_row += canvas->width;
		dst_row += display.canvas.bytes_per_row;
	}
	ioctl(display.fb.dev_fd, FBIOPUTCMAP, &cmap);
}

static void FBDisplay_SyncRect24(LCUI_Graph *canvas, int x, int y)
{
	Graph_Replace(&display.canvas, canvas, x, y);
}

#include <LCUI/image.h>

static void FBDisplay_SyncRect32(LCUI_Graph *canvas, int x, int y)
{
	Graph_Replace(&display.canvas, canvas, x, y);
}

static void FBDisplay_SyncRect(LCUI_Surface surface, LCUI_Rect *rect)
{
	int x, y;
	LCUI_Graph canvas;
	LCUI_Rect actual_rect;
	/* Get actual write rectangle */
	actual_rect.x = rect->x + surface->x;
	actual_rect.y = rect->y + surface->y;
	actual_rect.width = rect->width;
	actual_rect.height = rect->height;
	LCUIRect_ValidateArea(&actual_rect, display.width, display.height);
	/* Convert this rectangle to surface canvas related rectangle */
	x = actual_rect.x;
	y = actual_rect.y;
	actual_rect.x -= surface->x;
	actual_rect.y -= surface->y;
	Graph_Init(&canvas);
	/* Use this rectangle as a canvas rectangle to write pixels */
	Graph_Quote(&canvas, &surface->canvas, &actual_rect);
	/* Write pixels to the framebuffer by pixel format */
	switch (display.fb.var_info.bits_per_pixel) {
	case 32:
		FBDisplay_SyncRect32(&canvas, x, y);
		break;
	case 24:
		FBDisplay_SyncRect24(&canvas, x, y);
		break;
	case 16:
		FBDisplay_SyncRect16(&canvas, x, y);
		break;
	case 8:
		FBDisplay_SyncRect8(&canvas, x, y);
		break;
	default:
		break;
	}
}

static void FBSurface_Present(LCUI_Surface surface)
{
	LinkedListNode *node;
	LCUIMutex_Lock(&surface->mutex);
	for (LinkedList_Each(node, &surface->rects)) {
		FBDisplay_SyncRect(surface, node->data);
	}
	LinkedList_Clear(&surface->rects, free);
	LCUIMutex_Unlock(&surface->mutex);
}

/** 更新 surface，应用缓存的变更 */
static void FBSurface_Update(LCUI_Surface surface)
{
	int i;
	for (i = 0; i < TASK_TOTAL_NUM; ++i) {
		if (surface->tasks[i].is_valid) {
			FBSurface_RunTask(surface, i);
		}
	}
}

static int FBDisplay_BindEvent(int event_id, LCUI_EventFunc func, void *data,
			       void (*destroy_data)(void *))
{
	return EventTrigger_Bind(display.trigger, event_id, func, data,
				 destroy_data);
}

static void *FBSurface_GetHandle(LCUI_Surface s)
{
	return NULL;
}

static int FBDisplay_GetWidth(void)
{
	return display.width;
}

static int FBDisplay_GetHeight(void)
{
	return display.height;
}

static void FBDisplay_PrintInfo(void)
{
	char visual[256], type[256];

	switch (display.fb.fix_info.type) {
	case FB_TYPE_PACKED_PIXELS:
		strcpy(type, "packed pixels");
		break;
	case FB_TYPE_PLANES:
		strcpy(type, "non interleaved planes");
		break;
	case FB_TYPE_INTERLEAVED_PLANES:
		strcpy(type, "interleaved planes");
		break;
	case FB_TYPE_TEXT:
		strcpy(type, "text/attributes");
		break;
	case FB_TYPE_VGA_PLANES:
		strcpy(type, "EGA/VGA planes");
		break;
	default:
		strcpy(type, "unkown");
		break;
	}
	switch (display.fb.fix_info.visual) {
	case FB_VISUAL_MONO01:
		strcpy(visual, "Monochr. 1=Black 0=White");
		break;
	case FB_VISUAL_MONO10:
		strcpy(visual, "Monochr. 1=White 0=Black");
		break;
	case FB_VISUAL_TRUECOLOR:
		strcpy(visual, "true color");
		break;
	case FB_VISUAL_PSEUDOCOLOR:
		strcpy(visual, "pseudo color (like atari)");
		break;
	case FB_VISUAL_DIRECTCOLOR:
		strcpy(visual, "direct color");
		break;
	case FB_VISUAL_STATIC_PSEUDOCOLOR:
		strcpy(visual, "pseudo color readonly");
		break;
	default:
		strcpy(type, "unkown");
		break;
	}
	printf(
	    "============== screen info =============\n"
	    "FB mem start  : 0x%08lX\n"
	    "FB mem length : %d\n"
	    "FB type       : %s\n"
	    "FB visual     : %s\n"
	    "accel         : %d\n"
	    "geometry      : %d %d %d %d %d\n"
	    "timings       : %d %d %d %d %d %d\n"
	    "rgba          : %d/%d, %d/%d, %d/%d, %d/%d\n"
	    "========================================\n",
	    display.fb.fix_info.smem_start, display.fb.fix_info.smem_len, type,
	    visual, display.fb.fix_info.accel, display.fb.var_info.xres,
	    display.fb.var_info.yres, display.fb.var_info.xres_virtual,
	    display.fb.var_info.yres_virtual,
	    display.fb.var_info.bits_per_pixel,
	    display.fb.var_info.upper_margin, display.fb.var_info.lower_margin,
	    display.fb.var_info.left_margin, display.fb.var_info.right_margin,
	    display.fb.var_info.hsync_len, display.fb.var_info.vsync_len,
	    display.fb.var_info.red.length, display.fb.var_info.red.offset,
	    display.fb.var_info.green.length, display.fb.var_info.green.offset,
	    display.fb.var_info.blue.length, display.fb.var_info.blue.offset,
	    display.fb.var_info.transp.length,
	    display.fb.var_info.transp.offset);
}

static void FBDisplay_InitCanvas(void)
{
	display.canvas.width = display.width;
	display.canvas.height = display.height;
	display.canvas.bytes = display.fb.mem;
	display.canvas.bytes_per_row = display.fb.fix_info.line_length;
	display.canvas.mem_size = display.fb.mem_len;
	switch (display.fb.var_info.bits_per_pixel) {
	case 32:
		display.canvas.color_type = LCUI_COLOR_TYPE_ARGB8888;
		break;
	case 24:
		display.canvas.color_type = LCUI_COLOR_TYPE_RGB888;
		break;
	case 8:
		ioctl(display.fb.dev_fd, FBIOGETCMAP, &display.fb.cmap);
	default:
		break;
	}
	memset(display.canvas.bytes, 0, display.canvas.mem_size);
}

static void FBDisplay_InitSurface(void)
{
	LCUI_Surface surface = &display.surface;

	Graph_Init(&surface->canvas);
	LCUIMutex_Init(&surface->mutex);
	LinkedList_Init(&surface->rects);
	display.surface_count = 0;
	surface->canvas.color_type = LCUI_COLOR_TYPE_ARGB;
	FBSurface_Resize(surface, display.width, display.height);
}

static int FBDisplay_Init(void)
{
	display.fb.dev_path = getenv("LCUI_FRAMEBUFFER_DEVICE");
	if (!display.fb.dev_path) {
		display.fb.dev_path = "/dev/fb0";
	}
	Logger_Debug("[display] open framebuffer device: %s\n", display.fb.dev_path);
	display.fb.dev_fd = open(display.fb.dev_path, O_RDWR);
	if (display.fb.dev_fd == -1) {
		Logger_Error("[display] open framebuffer device failed\n");
		return -1;
	}
	ioctl(display.fb.dev_fd, FBIOGET_VSCREENINFO, &display.fb.var_info);
	ioctl(display.fb.dev_fd, FBIOGET_FSCREENINFO, &display.fb.fix_info);
	display.width = display.fb.var_info.xres;
	display.height = display.fb.var_info.yres;
	display.fb.mem_len = display.fb.fix_info.smem_len;
	display.fb.mem = mmap(NULL, display.fb.mem_len, PROT_READ | PROT_WRITE,
			      MAP_SHARED, display.fb.dev_fd, 0);
	if ((void *)-1 == display.fb.mem) {
		Logger_Error("[display] framebuffer mmap failed\n");
		return -1;
	}
	FBDisplay_PrintInfo();
	FBDisplay_InitCanvas();
	FBDisplay_InitSurface();
	return 0;
}

LCUI_DisplayDriver LCUI_CreateLinuxFBDisplayDriver(void)
{
	ASSIGN(driver, LCUI_DisplayDriver);
	if (FBDisplay_Init() != 0) {
		free(driver);
		return NULL;
	}
	strcpy(driver->name, "framebuffer");
	driver->getWidth = FBDisplay_GetWidth;
	driver->getHeight = FBDisplay_GetHeight;
	driver->create = FBSurface_New;
	driver->destroy = FBSurface_Delete;
	driver->isReady = FBSurface_IsReady;
	driver->show = FBSurface_Show;
	driver->hide = FBSurface_Hide;
	driver->move = FBSurface_Move;
	driver->resize = FBSurface_Resize;
	driver->update = FBSurface_Update;
	driver->present = FBSurface_Present;
	driver->setCaptionW = FBSurface_SetCaptionW;
	driver->setRenderMode = FBSurface_SetRenderMode;
	driver->setOpacity = FBSurface_SetOpacity;
	driver->getHandle = FBSurface_GetHandle;
	driver->beginPaint = FBSurface_BeginPaint;
	driver->endPaint = FBSurface_EndPaint;
	driver->bindEvent = FBDisplay_BindEvent;
	display.trigger = EventTrigger();
	display.active = TRUE;
	return driver;
}

void LCUI_DestroyLinuxFBDisplayDriver(LCUI_DisplayDriver driver)
{
	if (munmap(display.fb.mem, display.fb.mem_len) != 0) {
		perror("[display] framebuffer munmap failed");
	}
	switch (display.fb.var_info.bits_per_pixel) {
	case 8:
		ioctl(display.fb.dev_fd, FBIOPUTCMAP, &display.fb.cmap);
	default:
		Graph_Free(&display.surface.canvas);
		break;
	}
	EventTrigger_Destroy(display.trigger);
	close(display.fb.dev_fd);
	free(driver);
	display.fb.mem = NULL;
	display.fb.mem_len = 0;
	display.active = FALSE;
}

#endif
