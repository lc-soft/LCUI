/*
 * textview.h -- TextView widget for display text.
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

#ifndef LCUI_TEXTVIEW_H
#define LCUI_TEXTVIEW_H

LCUI_BEGIN_HEADER

/** 设定与标签关联的文本内容 */
LCUI_API int TextView_SetTextW(LCUI_Widget w, const wchar_t *text);

LCUI_API int TextView_SetText(LCUI_Widget w, const char *utf8_text);

LCUI_API void TextView_SetLineHeight(LCUI_Widget w, int height);

LCUI_API void TextView_SetTextAlign(LCUI_Widget w, int align);

LCUI_API void TextView_SetColor(LCUI_Widget w, LCUI_Color color);

LCUI_API void TextView_SetAutoWrap(LCUI_Widget w, LCUI_BOOL enable);

LCUI_API void TextView_SetMulitiline(LCUI_Widget w, LCUI_BOOL enable);

LCUI_API size_t LCUIWidget_RefreshTextView(void);

LCUI_API void LCUIWidget_AddTextView(void);

LCUI_API void LCUIWidget_FreeTextView(void);

LCUI_END_HEADER

#endif
