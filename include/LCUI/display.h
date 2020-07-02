/*
 * display.h -- Graphic display control
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

#ifndef LCUI_DISPLAY_CONTROL_H
#define LCUI_DISPLAY_CONTROL_H

#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/surface.h>

LCUI_BEGIN_HEADER

/** 图形显示模式 */
typedef enum LCUI_DisplayMode {
	LCUI_DMODE_WINDOWED = 1, /**< 窗口化 */
	LCUI_DMODE_SEAMLESS,     /**< 与系统GUI无缝结合 */
	LCUI_DMODE_FULLSCREEN    /**< 全屏模式 */
} LCUI_DisplayMode;

#define LCUI_DMODE_DEFAULT LCUI_DMODE_WINDOWED

/** 显示驱动的事件类型 */
typedef enum LCUI_DisplayEventType {
	LCUI_DEVENT_NONE,
	LCUI_DEVENT_PAINT,
	LCUI_DEVENT_RESIZE,
	LCUI_DEVENT_MINMAXINFO,
	LCUI_DEVENT_READY
} LCUI_DisplayEventType;

typedef struct LCUI_MinMaxInfoRec_ {
	int min_width, min_height;
	int max_width, max_height;
} LCUI_MinMaxInfoRec, *LCUI_MinMaxInfo;

/** 显示驱动的事件数据结构 */
typedef struct LCUI_DisplayEventRec_ {
	int type;
	union {
		struct {
			LCUI_Rect rect;
		} paint;
		struct {
			int width, height;
		} resize;
		LCUI_MinMaxInfoRec minmaxinfo;
	};
	LCUI_Surface surface;
} LCUI_DisplayEventRec, *LCUI_DisplayEvent;

/** surface 的操作方法集 */
typedef struct LCUI_DisplayDriverRec_ {
	char name[256];
	int (*getWidth)(void);
	int (*getHeight)(void);
	LCUI_Surface (*create)(void);
	void (*destroy)(LCUI_Surface);
	void (*close)(LCUI_Surface);
	void (*resize)(LCUI_Surface, int, int);
	void (*move)(LCUI_Surface, int, int);
	void (*show)(LCUI_Surface);
	void (*hide)(LCUI_Surface);
	void (*update)(LCUI_Surface);
	void (*present)(LCUI_Surface);
	LCUI_BOOL (*isReady)(LCUI_Surface);
	LCUI_PaintContext (*beginPaint)(LCUI_Surface, LCUI_Rect *);
	void (*endPaint)(LCUI_Surface, LCUI_PaintContext);
	void (*setCaptionW)(LCUI_Surface, const wchar_t *);
	void (*setRenderMode)(LCUI_Surface, int);
	void *(*getHandle)(LCUI_Surface);
	int (*getSurfaceWidth)(LCUI_Surface);
	int (*getSurfaceHeight)(LCUI_Surface);
	void (*setOpacity)(LCUI_Surface, float);
	int (*bindEvent)(int, LCUI_EventFunc, void *, void (*)(void *));
} LCUI_DisplayDriverRec, *LCUI_DisplayDriver;

/* 设置呈现模式 */
LCUI_API int LCUIDisplay_SetMode(int mode);

/** 获取屏幕显示模式 */
LCUI_API int LCUIDisplay_GetMode(void);

/** 更新各种图形元素 */
LCUI_API void LCUIDisplay_Update(void);

/** 渲染内容 */
LCUI_API size_t LCUIDisplay_Render(void);

/** 呈现渲染后的内容 */
LCUI_API void LCUIDisplay_Present(void);

LCUI_API void LCUIDisplay_EnablePaintFlashing(LCUI_BOOL enable);

/** 设置显示区域的尺寸，仅在窗口化、全屏模式下有效 */
LCUI_API void LCUIDisplay_SetSize(int width, int height);

/** 获取屏幕宽度 */
LCUI_API int LCUIDisplay_GetWidth(void);

/** 获取屏幕高度 */
LCUI_API int LCUIDisplay_GetHeight(void);

/** 添加无效区域 */
LCUI_API void LCUIDisplay_InvalidateArea(LCUI_Rect *rect);

/** 获取当前部件所属的 surface */
LCUI_API LCUI_Surface LCUIDisplay_GetSurfaceOwner(LCUI_Widget w);

/** 根据 handle 获取 surface */
LCUI_Surface LCUIDisplay_GetSurfaceByHandle(void *handle);

/** 绑定 surface 触发的事件 */
LCUI_API int LCUIDisplay_BindEvent(int event_id, LCUI_EventFunc func, void *arg,
				   void *data, void (*destroy_data)(void *));

/** 初始化图形输出模块 */
LCUI_API int LCUI_InitDisplay(LCUI_DisplayDriver driver);

/** 停用图形输出模块 */
LCUI_API int LCUI_FreeDisplay(void);

LCUI_END_HEADER

#endif
