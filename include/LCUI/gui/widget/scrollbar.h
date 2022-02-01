﻿/*
 * scrollbar.c -- LCUI's scrollbar widget
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

#ifndef LCUI_SCROLLBAR_H
#define LCUI_SCROLLBAR_H

LCUI_BEGIN_HEADER

typedef enum LCUI_ScrollBarDirection {
	LCUI_SCROLLBAR_HORIZONTAL,
	LCUI_SCROLLBAR_VERTICAL
} LCUI_ScrollBarDirection;

/* TODO: remove these macro in next major version */

#define SBD_HORIZONTAL LCUI_SCROLLBAR_HORIZONTAL
#define SBD_VERTICAL LCUI_SCROLLBAR_VERTICAL

LCUI_API void ScrollBar_BindBox(ui_widget_t* w, ui_widget_t* box);

LCUI_API void ScrollBar_BindTarget(ui_widget_t* w, ui_widget_t* layer);

/** 获取滚动条的位置 */
LCUI_API float ScrollBar_GetPosition(ui_widget_t* w);

/** 将与滚动条绑定的内容滚动至指定位置 */
LCUI_API float ScrollBar_SetPosition(ui_widget_t* w, float pos);

/** 设置滚动条的方向 */
LCUI_API void ScrollBar_SetDirection(ui_widget_t* w,
				     LCUI_ScrollBarDirection direction);

LCUI_API void LCUIWidget_AddTScrollBar(void);

LCUI_END_HEADER

#endif
