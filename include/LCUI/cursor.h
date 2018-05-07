/*
 * cursor.h -- Mouse cursor operation set.
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

#ifndef LCUI_CURSOR_H
#define LCUI_CURSOR_H

LCUI_BEGIN_HEADER

/* 初始化游标数据 */
LCUI_API void LCUI_InitCursor(void);

LCUI_API void LCUI_FreeCursor(void);

/* 获取鼠标游标的区域范围 */
LCUI_API void LCUICursor_GetRect(LCUI_Rect *rect);

/* 刷新鼠标游标在屏幕上显示的图形 */
LCUI_API void LCUICursor_Refresh(void);

/* 检测鼠标游标是否可见 */
LCUI_API LCUI_BOOL LCUICursor_IsVisible(void);

/* 显示鼠标游标 */
LCUI_API void LCUICursor_Show(void);

/* 隐藏鼠标游标 */
LCUI_API void LCUICursor_Hide(void);

/* 更新鼠标指针的位置 */
LCUI_API void LCUICursor_Update(void);

/* 设定游标的位置 */
LCUI_API void LCUICursor_SetPos(LCUI_Pos pos);

/** 设置游标的图形 */
LCUI_API int LCUICursor_SetGraph(LCUI_Graph *graph);

/* 获取鼠标指针当前的坐标 */
LCUI_API void LCUICursor_GetPos(LCUI_Pos *pos);

LCUI_API int LCUICursor_Paint(LCUI_PaintContext paint);

LCUI_END_HEADER

#endif
