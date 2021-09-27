/*
 * textedit.c -- textedit widget, used to allow user edit text.
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

#ifndef LCUI_TEXTBOX_H
#define LCUI_TEXTBOX_H

LCUI_BEGIN_HEADER

#define TextEdit_New() Widget_New("textedit")

/** Enable style tag parser  */
LCUI_API void TextEdit_EnableStyleTag(LCUI_Widget widget, LCUI_BOOL enable);

LCUI_API void TextEdit_EnableMultiline(LCUI_Widget widget, LCUI_BOOL enable);

LCUI_API void TextEdit_MoveCaret(LCUI_Widget widget, int row, int col);

/** 清空文本内容 */
LCUI_API void TextEdit_ClearText(LCUI_Widget widget);

/** 获取文本内容 */
LCUI_API size_t TextEdit_GetTextW(LCUI_Widget w, size_t start,
				      size_t max_len, wchar_t *buf);

/** 获取文本长度 */
LCUI_API size_t TextEdit_GetTextLength(LCUI_Widget w);

/** 设置文本编辑框内的光标，指定是否闪烁、闪烁时间间隔 */
LCUI_API void TextEdit_SetCaretBlink(LCUI_Widget w, LCUI_BOOL visible, int time);

LCUI_API LCUI_Object TextEdit_GetProperty(LCUI_Widget w, const char *name);

    /** 为文本框设置文本（宽字符版） */
LCUI_API int TextEdit_SetTextW(LCUI_Widget widget, const wchar_t *wstr);

LCUI_API int TextEdit_SetText(LCUI_Widget widget, const char *utf8_str);

/** 为文本框追加文本（宽字符版） */
LCUI_API int TextEdit_AppendTextW(LCUI_Widget widget, const wchar_t *wstr);

/** 为文本框插入文本（宽字符版） */
LCUI_API int TextEdit_InsertTextW(LCUI_Widget widget, const wchar_t *wstr);

/** 设置占位符，当文本编辑框内容为空时显示占位符 */
LCUI_API int TextEdit_SetPlaceHolderW(LCUI_Widget w, const wchar_t *wstr);

LCUI_API int TextEdit_SetPlaceHolder(LCUI_Widget w, const char *str);

/** 设置密码屏蔽符 */
LCUI_API void TextEdit_SetPasswordChar(LCUI_Widget w, wchar_t ch);

LCUI_API void LCUIWidget_AddTextEdit(void);

LCUI_END_HEADER

#endif
