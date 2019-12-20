/*
 * surface.h -- Graphic presentation layer.
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

#ifndef LCUI_SURFACE_H
#define LCUI_SURFACE_H

LCUI_BEGIN_HEADER

#define RENDER_MODE_BIT_BLT	0
#define RENDER_MODE_STRETCH_BLT 1

#ifdef LCUI_SURFACE_C
typedef struct LCUI_SurfaceRec_ * LCUI_Surface;
#else
typedef void* LCUI_Surface;
#endif

/** 关闭 surface */
LCUI_API void Surface_Close(LCUI_Surface surface);

/** 直接销毁 surface */
LCUI_API void Surface_Destroy(LCUI_Surface surface);

/** 新建一个 Surface */
LCUI_API LCUI_Surface Surface_New(void);

LCUI_API LCUI_BOOL Surface_IsReady(LCUI_Surface surface);

LCUI_API void Surface_Move(LCUI_Surface surface, int x, int y);

LCUI_API int Surface_GetWidth(LCUI_Surface surface);

LCUI_API int Surface_GetHeight(LCUI_Surface surface);

LCUI_API void Surface_Resize(LCUI_Surface surface, int w, int h);

LCUI_API void Surface_SetCaptionW(LCUI_Surface surface, const wchar_t *str);

LCUI_API void Surface_Show(LCUI_Surface surface);

LCUI_API void Surface_Hide(LCUI_Surface surface);

LCUI_API void *Surface_GetHandle(LCUI_Surface surface);

/** 设置 Surface 的渲染模式 */
LCUI_API void Surface_SetRenderMode(LCUI_Surface surface, int mode);

/** 更新 surface，应用缓存的变更 */
LCUI_API void Surface_Update(LCUI_Surface surface);

/**
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
LCUI_API LCUI_PaintContext Surface_BeginPaint(LCUI_Surface surface, LCUI_Rect *rect);

/**
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
LCUI_API void Surface_EndPaint(LCUI_Surface surface, LCUI_PaintContext paint);

/** 将帧缓存中的数据呈现至Surface的窗口内 */
LCUI_API void Surface_Present(LCUI_Surface surface);

LCUI_END_HEADER

#endif
